#ifndef MODEL_SCRIPT_PARSER_H
#define MODEL_SCRIPT_PARSER_H


namespace GothOgre
{
	/** This helper class can parse model script file formats:
	*.mds, *.mms, *.sky. The class cannot process these model scripts,
	it can read a file and split it into tokens for the following processing. */
	class ModelScriptParser
	{
	public:
		// Token's type
		enum Type
		{
			// C++ identifier, containing letters, digits, and underscore,
			// the first character is non-digit
			IDENTIFIER,

			// String, in quotes
			STRING,

			// Integer number
			INTEGER,

			// Real number
			REAL,

			// Operator - a single char of the following: +-*/:()[]{}
			OPERATOR,

			// End-of-file
			END_OF_FILE
		};

	public:
		ModelScriptParser();
		~ModelScriptParser();

		// Loads a specified script file.
		// File is loaded into a temporary memory buffer,
		// so there is no need in the source data stream while 
		// model script is being actually parsing with function "peek*" and "get*".
		void loadScript(const DataStreamExPtr& _dataStream);

		// Clear the state of the parser.
		void clear();

		/** Peek the next(=current) token's type without moving the file position. */
		Type          peekNextType() const;

		/** Check if the end of the file reached.
		eof() returns the same result as (peekNextType() == END_OF_FILE). */
		bool          eof() const;

		/** Returns the next(=current) token without moving the file position. 
		The function throws an exception if the end-of-file reached. */
		const String& peekNextToken() const;

		/** Returns the next(=current) upper-cased identifier without moving the file position.
		The function throws an exception if the next token isn't an identifier. */
		const String& peekNextIdentifier() const;

		/** Returns the next(=current) string without moving the file position.
		A returned value doesn't contain quotes.
		The function throws an exception if the next token isn't a string. */
		const String& peekNextString() const;

		/** Returns the next(=current) operator without moving the file position.
		The function throws an exception if the next token isn't an operator. */
		const String& peekNextOperator() const;

		/** Returns the next(=current) real number without moving the file position.
		The function throws an exception if the next token isn't a real number. */
		Real          peekNextReal() const;

		/** Returns the next(=current) integer number without moving the file position.
		The function throws an exception if the next token isn't an integer number. */
		int           peekNextInteger() const;

		/** Returns the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the end-of-file reached. */
		const String& getNextToken();

		/** Returns the next(=current) upper-cased identifier, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an identifier. */
		const String& getNextIdentifier();

		/** Returns the next(=current) string, and moves the file position further to the next token. 
		A returned value doesn't contain quotes.
		The function throws an exception if the next token isn't a string. */
		const String& getNextString();

		/** Returns the next(=current) operator, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an operator. */
		const String& getNextOperator();

		/** Returns the next(=current) real number, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't a real number. */
		Real          getNextReal();

		/** Returns the next(=current) integer number, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an integer number. */
		int           getNextInteger();

		/** Skips the next(=current) token, and moves the file position further to the next token.  
		The function throws an exception if the end-of-file reached. */
		void skipNextToken();

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't equal to a specified string. */
		void skipNextToken(const String& _token);

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an identifier. */
		void skipNextIdentifier();

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an identifier or if it isn't equal to a specified value. */
		void skipNextIdentifier(const String& _identifier);

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't a string in quotes. */
		void skipNextString();

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't a string in quotes or if it isn't equal to a specified value (without quotes). */
		void skipNextString(const String& _str);

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an operator. */
		void skipNextOperator();

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an operator or if it isn't equal to a specified value. */
		void skipNextOperator(const String& _op);

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't a real number. */
		void skipNextReal();

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't a real number or if it isn't equal to a specified value. */
		void skipNextReal(Real _real);

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an integer number. */
		void skipNextInteger();

		/** Skips the next(=current) token, and moves the file position further to the next token. 
		The function throws an exception if the next token isn't an integer number or if it isn't equal to a specified value. */
		void skipNextInteger(int _int);

		/** Throws an exception with error message containg 
		file name, line index (starting from 1) and column index (starting from 1). 
		An optional parameters means number of calls of the "get*" and "skip*" functions
		after (and including) the call which caused the error. */
		void throwException(const String& _message, 
			size_t _numGotTokensStartingFromError = 0) const;

	private:
		struct TokenInfo
		{
			Type   type;
			size_t filePos;
			size_t lineIndex;
			size_t columnIndex;
			String token;
			String str;
			Real   realNumber;
			int    intNumber;
		};
		typedef vector<TokenInfo*>::type TokenInfoList;

		void prepareNextToken();
		void advanceCurrent();

	private:
		DataStreamExPtr          mDataStream;
		String                   mText;
		size_t                   mCurrentLine;
		size_t                   mCurrentColumn;

		String                   mFileName;
		TokenInfoList            mTokenInfoList;
		TokenInfoList::iterator  mCurrent;
		static const size_t      TOKEN_INFO_LIST_MAX_SIZE = 10;
	};
}

#endif // MODEL_SCRIPT_PARSER_H