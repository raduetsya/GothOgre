	bool ZenArchive::readParam(ParamData& _paramData)
	{
		const ParamDesc* paramDesc = _paramData.getDesc();
		ParamType::Type paramType = paramDesc->getType();
		const String& paramName = paramDesc->getName();
		bool ret = true;
		switch(paramType)
		{
			case ParamType::NIL:
				break;
			
			case ParamType::BOOL:
			{
				bool value; 
				ret = readBool( paramName, value ); 
				if(ret) _paramData.setBool(value); 
				break;
			}
			case ParamType::CHAR:
			{
				uchar value;
				ret = readByte( paramName, value );
				if(ret) _paramData.setChar( (char) value );
				break;
			}
			case ParamType::SHORT:
			{
				ushort value;
				ret = readWord( paramName, value );
				if(ret) _paramData.setShort( (short) value );
				break;
			}
			case ParamType::LONG:
			{
				int value;
				ret = readInt( paramName, value );
				if(ret) _paramData.setLong( (long) value );
				break;
			}
			case ParamType::UCHAR:
			{
				uchar value;
				ret = readByte( paramName, value );
				if(ret) _paramData.setUChar( value );
				break;
			}
			case ParamType::USHORT:
			{
				ushort value;
				ret = readWord(paramName, value);
				if(ret) _paramData.setUShort( value );
				break;
			}
			case ParamType::ULONG:   
			{
				int value;
				ret = readInt( paramName, value );
				if(ret) _paramData.setULong( (ulong) value );
				break;
			}
			case ParamType::REAL:
			{
				Real value;
				ret = readReal( paramName, value );
				if(ret) _paramData.setReal( value );
				break;
			}
			case ParamType::ENUM:
			{
				int value;
				ret = readEnum( paramName, value );
				if(ret) _paramData.setEnum( value );
				break;
			}
			case ParamType::STRING:  
			{
				String value;
				ret = readString( paramName, value );
				if(ret) _paramData.setString( value );
				break;
			}
			case ParamType::COLOR:
			{
				ColourValue value;
				ret = readColor( paramName, value );
				if(ret) _paramData.setColor( value );
				break;
			}
			case ParamType::VECTOR3:
			{
				Vector3 value;
				ret = readVec3( paramName, value );
				if(ret) _paramData.setVector3( value );
				break;
			}
			case ParamType::VECTOR4:
			{
				Real data[4];
				ret = readRawReal( paramName, data, sizeof(data) );
				if(ret)
				{
					Vector4 vec4( data[0], data[1], data[2], data[3]);
					_paramData.setVector4( vec4 );
				}
				break;
			}
			case ParamType::MATRIX3: 
			{
				Real data[9];
				ret = readRawReal( paramName, data, sizeof(data) );
				if(ret)
				{
					Matrix3 mat3( data[0], data[1], data[2], 
								  data[3], data[4], data[5], 
								  data[6], data[7], data[8] );
					_paramData.setMatrix3( mat3 );
				}
				break;
			}
			case ParamType::MATRIX4: 
			{
				Real data[16];
				ret = readRawReal( paramName, data, sizeof(data) );
				if(ret)
				{
				  Matrix4 mat4( data[0],  data[1],  data[2],  data[3], 
								data[4],  data[5],  data[6],  data[7],
								data[8],  data[9],  data[10], data[11],
								data[12], data[13], data[14], data[15] );
				  _paramData.setMatrix4( mat4 );
				}
				break;
			}
			case ParamType::CHAR_ARRAY:
			{
				ret = readRaw( paramName, const_cast<char*>(_paramData.getCharArray()), _paramData.getArraySize() * sizeof(char) );
				break;
			}
			case ParamType::SHORT_ARRAY:
			{
				ret = readRaw( paramName, const_cast<short*>(_paramData.getShortArray()), _paramData.getArraySize() * sizeof(short) );
				break;
			}
			case ParamType::LONG_ARRAY:
			{
				ret = readRaw( paramName, const_cast<long*>(_paramData.getLongArray()), _paramData.getArraySize() * sizeof(long) );
				break;
			}
			case ParamType::UCHAR_ARRAY:
			{
				ret = readRaw( paramName, const_cast<uchar*>(_paramData.getUCharArray()), _paramData.getArraySize() * sizeof(uchar) );
				break;
			}
			case ParamType::USHORT_ARRAY:
			{
				ret = readRaw( paramName, const_cast<ushort*>(_paramData.getUShortArray()), _paramData.getArraySize() * sizeof(ushort) );
				break;
			}
			case ParamType::ULONG_ARRAY:
			{
				ret = readRaw( paramName, const_cast<ulong*>(_paramData.getULongArray()), _paramData.getArraySize() * sizeof(ulong) );
				break;
			}
			case ParamType::REAL_ARRAY:
			{
				ret = readRawReal( paramName, const_cast<Real*>(_paramData.getRealArray()), _paramData.getArraySize() * sizeof(Real) );
				break;
			}
			case ParamType::VOID_PTR:
				break;
		}
		return ret;
	}

	bool ZenArchive::readParam(ParamContainer* _paramContainer, long _id)
	{
		return readParam(_paramContainer->getData(_id));
	}
	