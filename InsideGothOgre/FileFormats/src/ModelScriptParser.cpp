#include "Precompiled.h"
#include "ModelScriptParser.h"


namespace GothOgre
{
	//----------------------------------------------------------------------------
	ModelScriptParser::ModelScriptParser()
	{
		mCurrentLine = 0;
		mCurrentColumn = 0;
		mCurrent = mTokenInfoList.end();
	}
	//----------------------------------------------------------------------------
	ModelScriptParser::~ModelScriptParser()
	{
		for(TokenInfoList::iterator it = mTokenInfoList.end(); 
			it != mTokenInfoList.end(); ++it)
		{
			delete *it;
		}
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::clear()
	{
		mFileName.clear();
		mDataStream.setNull();

		// Don't clear the list actually, just set the iterator
		mCurrent = mTokenInfoList.end();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::loadScript(const DataStreamExPtr& _dataStream)
	{
		clear();
		
		// Process null data stream - just clearing.
		if(_dataStream.isNull())
			return;

		// Copy file name and setup encoding
		mFileName = _dataStream->getName();
		EncodingSettings::getSingleton().setupStreamEncoding(_dataStream);

		// Prebufferize for fast parsing
		mDataStream = (DataStreamExPtr) (MemoryDataStreamPtr) 
			new MemoryDataStream(_dataStream, true, true);
		mDataStream->setEncoding(_dataStream->getEncoding());

		// The file's beginning
		mCurrentLine = -1;
		mCurrentColumn = 0;
		mText.clear();

		// Parse the first token
		prepareNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::prepareNextToken()
	{
		if(mCurrent != mTokenInfoList.end())
			return;

		// Add a single token info into the list of them
		if(mTokenInfoList.size() < TOKEN_INFO_LIST_MAX_SIZE)
		{
			mTokenInfoList.push_back(new TokenInfo);
		}
		else
		{
			// Or reuse the old token info again
			memmove(&mTokenInfoList[0], &mTokenInfoList[1], 
				sizeof(mTokenInfoList[0]) * (mTokenInfoList.size() - 1));
		}
		mCurrent = mTokenInfoList.end();
		--mCurrent;
		TokenInfo& tokenInfo = **mCurrent;

		bool multilineComment = false;

		// Cycle until a token has not been read
		while(true)
		{
			// Read one more line
			while(mCurrentColumn >= mText.size())
			{
				if(mDataStream->eof())
				{
					tokenInfo.type = END_OF_FILE;
					tokenInfo.filePos = mDataStream->size();
					tokenInfo.lineIndex = mCurrentLine + 1;
					tokenInfo.columnIndex = mCurrentColumn + 1;
					return;
				}
				mDataStream->readLine(mText, EOL::CRLF);
				mCurrentColumn = 1;
				mCurrentLine++;
			}

			// Search the end of the current multiline comment
			if(multilineComment)
			{
				size_t i = mText.find("*/", mCurrentColumn);
				if(i == String::npos)
				{
					mCurrentColumn = mText.size();
				}
				else
				{
					mCurrentColumn = i + 2;
					multilineComment = false;
				}
				continue;
			}
			
			// Check if a comment found
			int ch = mText[mCurrentColumn];
			if(ch == '/' && mCurrentColumn + 1 < mText.size())
			{
				int ch2 = mText[mCurrentColumn + 1];
				if(ch2 == '/')
				{
					// It's a single-line comment.
					mCurrentColumn = mText.size();
					continue;
				}
				if(ch2 == '*')
				{
					// It's a beginning of a multi-line comment.
					mCurrentColumn += 2;
					multilineComment = true;
					continue;
				}
			}
				
			// Skip whitespaces
			if(ch == ' ' || ch == '\t')
			{
				++mCurrentColumn;
				continue;
			}

			// Check if an identifier found
			tokenInfo.filePos = mDataStream->tellg();
			tokenInfo.lineIndex = mCurrentLine + 1;
			tokenInfo.columnIndex = mCurrentColumn + 1;
			size_t start = mCurrentColumn;
			if(isalpha(ch) || ch == '_')
			{
				++mCurrentColumn;
				while(mCurrentColumn < mText.size())
				{
					ch = mText[mCurrentColumn];
					if(!isalnum(ch) && ch != '_')
						break;
					++mCurrentColumn;
				}
				tokenInfo.type = IDENTIFIER;
				tokenInfo.token = StrUtil::toUpperCase(mText.substr(start, mCurrentColumn - start));
				return;
			}

			// Check if a number found
			if(isdigit(ch) || ch == '+' || ch == '-' || ch == '.')
			{
				tokenInfo.token = mText.substr(start, mCurrentColumn - start);
				StrStream ss(tokenInfo.token, StrStream::DONT_COPY);
				Real realNumber;
				ss >> realNumber;
				if(!ss.fail())
				{
					mCurrentColumn += ss.tellg();
					int intNumber = (int) realNumber;
					if( (Real) intNumber == realNumber )
					{
						tokenInfo.type = INTEGER;
						tokenInfo.intNumber = intNumber;
					}
					else
					{
						tokenInfo.type = REAL;
						tokenInfo.realNumber = realNumber;
					}
					return;
				}
			}

			// Check if a string found
			if(ch == '\"')
			{
				size_t i = mText.find('\"', mCurrentColumn + 1);
				if(i == String::npos)
					mCurrentColumn = mText.size();
				else
					mCurrentColumn = i + 1;

				tokenInfo.token = mText.substr(start, mCurrentColumn - start);
				tokenInfo.str = tokenInfo.token.substr(1, tokenInfo.token.length() - 2);
				tokenInfo.type = STRING;
			}

			// Check if an operator found
			static const String& operatorChars = "+-*/:()[]{}";
			if(operatorChars.find(ch) != String.npos)
			{
				++mCurrentColumn;
				tokenInfo.token = mText.substr(start, mCurrentColumn - start);
				tokenInfo.type = OPERATOR;
				return;
			}

			// Invalid character
			{
				StrStream ss;
				ss << mDataStream->getName() << 
					" (line " << mCurrentLine << ", column " << mCurrentColumn << "): " <<
					"Unexpected character '";
				if(32 <= ch && ch <= 127)
					ss << ch;
				else
					ss << "\\x" << std::hex << std::setw(2) << std::setfill('0') << ch;
				GOTHOGRE_EXCEPT( ss.str() );
			}
		}
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::advanceCurrent()
	{
		if( (*mCurrent)->type != END_OF_FILE )
		{
			++mCurrent;
			prepareNextToken();
		}
	}
	//----------------------------------------------------------------------------
	ModelScriptParser::Type ModelScriptParser::peekNextType() const
	{
		return (*mCurrent)->type;
	}
	//----------------------------------------------------------------------------
	bool ModelScriptParser::eof() const
	{
		return (*mCurrent)->type == END_OF_FILE;
	}
	//----------------------------------------------------------------------------
	const String& ModelScriptParser::peekNextToken() const
	{
		if( (*mCurrent)->type == END_OF_FILE )
			throwException("Unexpected end of file.");

		return (*mCurrent)->token;
	}
	//----------------------------------------------------------------------------
	const String& ModelScriptParser::peekNextIdentifier() const
	{
		if( (*mCurrent)->type != IDENTIFIER )
			throwException("Expected an identifier.");

		return (*mCurrent)->token;
	}
	//----------------------------------------------------------------------------
	const String& ModelScriptParser::peekNextString() const
	{
		if( (*mCurrent)->type != STRING )
			throwException("Expected a string.");

		return (*mCurrent)->str;
	}
	//----------------------------------------------------------------------------
	const String& ModelScriptParser::peekNextOperator() const
	{
		if( (*mCurrent)->type != OPERATOR )
			throwException("Expected an operator.");

		return (*mCurrent)->token;
	}
	//----------------------------------------------------------------------------
	Real ModelScriptParser::peekNextReal() const
	{
		if( (*mCurrent)->type != REAL )
			throwException("Expected a real number.");

		return (*mCurrent)->realNumber;
	}
	//----------------------------------------------------------------------------
	int ModelScriptParser::peekNextInteger() const
	{
		if( (*mCurrent)->type != INTEGER )
			throwException("Expected an integer number.");

		return (*mCurrent)->intNumber;
	}
	//----------------------------------------------------------------------------
	const String& ModelScriptParser::getNextToken()
	{
		const String& r = peekNextToken();
		advanceCurrent();
		return r;
	}
	//----------------------------------------------------------------------------
	const String& ModelScriptParser::getNextIdentifier()
	{
		const String& r = peekNextIdentifier();
		advanceCurrent();
		return r;
	}
	//----------------------------------------------------------------------------
	const String& ModelScriptParser::getNextString()
	{
		const String& r = peekNextString();
		advanceCurrent();
		return r;
	}
	//----------------------------------------------------------------------------
	const String& ModelScriptParser::getNextOperator()
	{
		const String& r = peekNextOperator();
		advanceCurrent();
		return r;
	}
	//----------------------------------------------------------------------------
	Real ModelScriptParser::getNextReal()
	{
		Real r = peekNextReal();
		advanceCurrent();
		return r;
	}
	//----------------------------------------------------------------------------
	int ModelScriptParser::getNextInteger()
	{
		int r = peekNextInteger();
		advanceCurrent();
		return r;
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextToken()
	{
		advanceCurrent();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextToken(const String& _token)
	{
		if(peekNextToken() != _token)
			throwException("Expected '" + _token + "'");
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextIdentifier()
	{
		peekNextIdentifier();
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextIdentifier(const String& _identifier)
	{
		if(!StrUtil::equalsNoCase(peekNextIdentifier(), _identifier))
			throwException("Expected '" + _identifier + "'");
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextString()
	{
		peekNextString();
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextString(const String& _str)
	{
		if(peekNextString() != _str)
			throwException("Expected \"" + _str + "\"");
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextOperator()
	{
		peekNextOperator();
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextOperator(const String& _op)
	{
		if(peekNextOperator() != _op)
			throwException("Expected \"" + _op + "\"");
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextReal()
	{
		peekNextReal();
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextReal(Real _real)
	{
		if(peekNextReal() != _real)
		{
			throwException("Expected \"" + StrConv::toString(_real) + "\"");
		}
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextInteger()
	{
		peekNextInteger();
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::skipNextInteger(int _int)
	{
		if(peekNextInteger() != _int)
			throwException("Expected \"" + StrConv::toString(_int) + "\"");
		skipNextToken();
	}
	//----------------------------------------------------------------------------
	void ModelScriptParser::throwException(const String& _message, size_t _numGotTokensStartingFromError) const
	{
		size_t lineIndex = -1;
		size_t columnIndex = -1;

		TokenInfoList::iterator it = mCurrent;
		while(it != mTokenInfoList.begin())
		{
			if(!_numGotTokensStartingFromError)
			{
				lineIndex = (*it)->lineIndex;
				columnIndex = (*it)->columnIndex;
				break;
			}
			--it;
			--_numGotTokensStartingFromError;
		}

		GOTHOGRE_EXCEPT(mFileName << 
			" (line " << lineIndex << ", column " << columnIndex << "): " <<
			_message);
	}
}