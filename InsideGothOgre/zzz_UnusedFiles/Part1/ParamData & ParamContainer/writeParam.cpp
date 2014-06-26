	void ZenArchive::writeParam(const ParamData& _paramData)
	{
		const ParamDesc* paramDesc = _paramData.getDesc();
		ParamType::Type paramType = paramDesc->getType();
		const String& paramName = paramDesc->getName();
		switch(paramType)
		{
			case ParamType::NIL:     break;
			case ParamType::BOOL:    writeBool(paramName, _paramData.getBool()); break;
			case ParamType::CHAR:    writeByte(paramName, (uchar) _paramData.getChar()); break;
			case ParamType::SHORT:   writeWord(paramName, (ushort) _paramData.getShort()); break;
			case ParamType::LONG:    writeInt(paramName, _paramData.getLong()); break;
			case ParamType::UCHAR:   writeByte(paramName, _paramData.getUChar()); break;
			case ParamType::USHORT:  writeWord(paramName, _paramData.getUShort()); break;
			case ParamType::ULONG:   writeInt(paramName, (long) _paramData.getULong()); break;
			case ParamType::REAL:    writeReal(paramName, _paramData.getReal()); break;
			case ParamType::ENUM:    writeEnum(paramName, _paramData.getEnum()); break;
			case ParamType::STRING:  writeString(paramName, _paramData.getString()); break;
			case ParamType::COLOR:   writeColor(paramName, _paramData.getColor()); break;
			case ParamType::VECTOR3: writeVec3(paramName, _paramData.getVector3()); break;
			case ParamType::VECTOR4: 
			{
				const Vector4& vec4 = _paramData.getVector4();
				Real data[] = {vec4.x, vec4.y, vec4.z, vec4.w};
				writeRawReal(paramName, data, sizeof(data)); 
				break;
			}
			case ParamType::MATRIX3: 
			{
				const Matrix3& mat3 = _paramData.getMatrix3();
				Real data[] = { mat3[0][0], mat3[0][1], mat3[0][2],
								mat3[1][0], mat3[1][1], mat3[1][2],
								mat3[2][0], mat3[2][1], mat3[2][2] };
				writeRawReal(paramName, data, sizeof(data)); 
				break;
			}
			case ParamType::MATRIX4:
			{
				const Matrix4& mat4 = _paramData.getMatrix4();
				Real data[] = { mat4[0][0], mat4[0][1], mat4[0][2], mat4[0][3],
				 		        mat4[1][0], mat4[1][1], mat4[1][2], mat4[1][3],
								mat4[2][0], mat4[2][1], mat4[2][2], mat4[2][3],
								mat4[3][0], mat4[3][1], mat4[3][2], mat4[3][3] };
				writeRawReal(paramName, data, sizeof(data)); 
				break;
			}
			case ParamType::CHAR_ARRAY:   writeRaw(paramName, _paramData.getCharArray(), _paramData.getArraySize() * sizeof(char)); break;
			case ParamType::SHORT_ARRAY:  writeRaw(paramName, _paramData.getShortArray(), _paramData.getArraySize() * sizeof(short)); break;
			case ParamType::LONG_ARRAY:   writeRaw(paramName, _paramData.getLongArray(), _paramData.getArraySize() * sizeof(long)); break;
			case ParamType::UCHAR_ARRAY:  writeRaw(paramName, _paramData.getUCharArray(), _paramData.getArraySize() * sizeof(uchar)); break;
			case ParamType::USHORT_ARRAY: writeRaw(paramName, _paramData.getUShortArray(), _paramData.getArraySize() * sizeof(ushort)); break;
			case ParamType::ULONG_ARRAY:  writeRaw(paramName, _paramData.getULongArray(), _paramData.getArraySize() * sizeof(ulong)); break;
			case ParamType::REAL_ARRAY:   writeRawReal(paramName, _paramData.getRealArray(), _paramData.getArraySize() * sizeof(Real)); break;
			case ParamType::VOID_PTR:     break;
		}
	}

	void ZenArchive::writeParam(const ParamContainer* _paramContainer, long _id)
	{
		writeParam(_paramContainer->getData(_id));
	}
	