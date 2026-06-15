#include "myjson.h"

CMyJson::CMyJson(void)
{
}

CMyJson::~CMyJson(void)
{
}


bool CMyJson::SaveJsonFile(const char* filename, const Json::Value& root)
{
	Json::FastWriter writer;
	std::string strOut = writer.write(root);
	ofstream ofs(filename, ios_base::out|ios_base::binary);
	if(!ofs)
		return false;

	ofs << strOut;
	ofs.close();
	return true;
}

bool CMyJson::LoadJsonFile(const char* filename, Json::Value& root OUT)
{
	Json::Reader reader;
	ifstream ifs(filename, ios_base::in | ios_base::binary);
	if(!ifs)
		return false;

	root.clear();
	return reader.parse(ifs, root);
}


JSON_RETURN_VAL CMyJson::ReadIntKey(const Json::Value& root, const string& keyName, int& outInt OUT)
{
	try
	{
		Json::Value key = root[keyName];
		if(key.isNull())
			return JSON_FAILED_INVALID;	//键无效

		if(key.isInt())
		{
			outInt = key.asInt();
			return JSON_SUCCESS;	//成功
		}
		return JSON_FAILED_TYPE;	//键值类型不正确
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
    }
}

JSON_RETURN_VAL CMyJson::ReadFloatKey(const Json::Value &root, const string &keyName, float &outFloat OUT)
{
    try
    {
        Json::Value key = root[keyName];
        if(key.isNull())
            return JSON_FAILED_INVALID;	//键无效

        if(key.isDouble())
        {
            outFloat = (float)key.asDouble();
            return JSON_SUCCESS;	//成功
        }
        return JSON_FAILED_TYPE;	//键值类型不正确
    }
    catch (...)
    {
        return JSON_FAILED_ABORT;
    }
}

JSON_RETURN_VAL CMyJson::WriteIntKey(const string& keyName, int intData, Json::Value& root OUT)
{
	try
	{
		root[keyName] = intData;
		return JSON_SUCCESS;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
    }
}

JSON_RETURN_VAL CMyJson::WriteFloatKey(const string &keyName, float fData, Json::Value &root OUT)
{
    try
    {
        root[keyName] = fData;
        return JSON_SUCCESS;
    }
    catch (...)
    {
        return JSON_FAILED_ABORT;
    }
}

JSON_RETURN_VAL CMyJson::ReadStringKey(const Json::Value& root, const string& keyName, string& outStr OUT)
{
	try
	{
		Json::Value key = root[keyName];
		if(key.isNull())
			return JSON_FAILED_INVALID;

		if(key.isString())
		{
			outStr.clear();
			outStr = key.asString();
			return JSON_SUCCESS;
		}
		return JSON_FAILED_TYPE;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
	}
}

JSON_RETURN_VAL CMyJson::WriteStringKey(const string& keyName, const string& strData, Json::Value& root OUT)
{
	try
	{
		root[keyName] = strData;
		return JSON_SUCCESS;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
	}
}

JSON_RETURN_VAL CMyJson::ReadIntArrayKey(const Json::Value& root, const string& keyName, const vector<int>& inDimensions, int* outBuffer OUT, INT_ARRAY_KEY& outArray OUT)
{
	try
	{
		outArray.Dimensions.clear();
		outArray.Datas.clear();
		Json::Value key = root[keyName];
		if(key.isNull())
			return JSON_FAILED_INVALID;

		if(key.isArray())
		{
			int DimensionCount = _RecursionReadArrayDimension(key, outArray.Dimensions);
			int data_total = 1;
			while (DimensionCount--)
				data_total *= outArray.Dimensions[DimensionCount];

			if(_ArrayDimensionIsEqual(inDimensions, outArray.Dimensions))
			{
				_RecursionReadArrayData(key, data_total, outBuffer);
				return JSON_SUCCESS;
			}
			else
			{
				int* pIntData = new int[data_total];
				_RecursionReadArrayData(key, data_total, pIntData);
				for(int i = 0; i < data_total; i++)
					outArray.Datas.push_back(*(pIntData + i));
				delete[] pIntData;
				return JSON_FAILED_ARRAY_NUM;		//将读取到的数组数据全部放在outArray中
			}
		}
		return JSON_FAILED_TYPE;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
	}
}

JSON_RETURN_VAL CMyJson::ReadFloatArrayKey(const Json::Value &root, const string &keyName,const vector<int>& inDimensions, float *outBuffer OUT,Float_ARRAY_KEY& outArray OUT)
{
    try
    {
        outArray.Dimensions.clear();
        outArray.Datas.clear();
        Json::Value key = root[keyName];
        if(key.isNull())
            return JSON_FAILED_INVALID;

        if(key.isArray())
        {
            int DimensionCount = _RecursionReadArrayDimension(key, outArray.Dimensions);
            int data_total = 1;
            while (DimensionCount--)
                data_total *= outArray.Dimensions[DimensionCount];

            if(_ArrayDimensionIsEqual(inDimensions, outArray.Dimensions))
            {
                _RecursionReadFloatArrayData(key, data_total, outBuffer);
                return JSON_SUCCESS;
            }
            else
            {
                float* pfloatData = new float[data_total];
                _RecursionReadFloatArrayData(key, data_total, pfloatData);
                for(int i = 0; i < data_total; i++)
                    outArray.Datas.push_back(*(pfloatData + i));
                delete[] pfloatData;
                return JSON_FAILED_ARRAY_NUM;		//将读取到的数组数据全部放在outArray中
            }
        }
        return JSON_FAILED_TYPE;

    }
    catch (...)
    {
        return JSON_FAILED_ABORT;
    }
}

JSON_RETURN_VAL CMyJson::ReadStringArrayKey(const Json::Value& root, const string& keyName, vector<string>& outArray OUT)
{
	try
	{
		outArray.clear();
		Json::Value key = root[keyName];
		if(key.isNull())
			return JSON_FAILED_INVALID;
		
        int number = GetArraySize(key);
		if(number > 0)
		{
			for(int i = 0; i < number; i++)
			{
				if(key[i].isString())
					outArray.push_back(key[i].asString());
				else
				{
					outArray.clear();
					return JSON_FAILED_TYPE;
				}
			}
			return JSON_SUCCESS;
		}
		return JSON_FAILED_TYPE;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
	}
}

/*
JSON_RETURN_VAL CMyJson::ReadWStringArrayKey(const Json::Value& root, const string& keyName, vector<wstring>& outArray OUT)
{
	try
	{
		outArray.clear();
		Json::Value key = root[keyName];
		if(key.isNull())
			return JSON_FAILED_INVALID;

        int number = GetArraySize(key);
		if(number > 0)
		{
			wstring wStr;
			for(int i = 0; i < number; i++)
			{
				if(key[i].isString())
				{
					StringToWstring(key[i].asString(), wStr);
					outArray.push_back(wStr);
				}
				else
				{
					outArray.clear();
					return JSON_FAILED_TYPE;
				}
			}
			return JSON_SUCCESS;
		}
		return JSON_FAILED_TYPE;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
	}
}
*/

JSON_RETURN_VAL CMyJson::WriteIntArrayKey(const string& keyName, const vector<int>& inDimensions, const int* bufData, Json::Value& root OUT)
{
	try
	{
		root.removeMember(keyName);

		Json::Value key;
		_RecursionWriteArrayData(inDimensions, bufData, key);
		root[keyName] = key;
		return JSON_SUCCESS;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
	}
}

JSON_RETURN_VAL CMyJson::WriteFloatArrayKey(const string &keyName, const vector<int> &inDimensions, const float *bufData, Json::Value &root)
{
    try
    {
        root.removeMember(keyName);
        Json::Value key;
        _RecursionWriteFloatArrayData(inDimensions, bufData, key);
        root[keyName] = key;
        return JSON_SUCCESS;
    }
    catch (...)
    {
        return JSON_FAILED_ABORT;
    }
}

JSON_RETURN_VAL CMyJson::WriteStringArrayKey( const string& keyName, const vector<string>& inArray, Json::Value& root OUT )
{
	try
	{
		root.removeMember(keyName);

        for(int i = 0; i < (int)inArray.size(); i++)
			root[keyName].append(inArray[i]);
		return JSON_SUCCESS;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
	}
}

/*
JSON_RETURN_VAL CMyJson::WriteWStringArrayKey( const string& keyName, const vector<wstring>& inArray, Json::Value& root OUT )
{
	try
	{
		root.removeMember(keyName);

		string Str;
		for(int i = 0; i < inArray.size(); i++)
		{
			WstringToString(inArray[i], Str);
			root[keyName].append(Str);
		}
		return JSON_SUCCESS;
	}
	catch (...)
	{
		return JSON_FAILED_ABORT;
	}
}
*/

int CMyJson::GetArraySize(const Json::Value& json_val)
{
    return (json_val.isArray() ? json_val.size() : 0);
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CMyJson::_RecursionReadArrayDimension(const Json::Value& key, vector<int>& ArrayDimension OUT)
{
    int number = GetArraySize(key);
	if(number > 0)
	{
		ArrayDimension.push_back(number);
		return _RecursionReadArrayDimension(key[0], ArrayDimension);
	}
	else	return ArrayDimension.size();
}

void CMyJson::_RecursionReadArrayData(const Json::Value& key, int data_total, int* outData OUT)
{
    int number = GetArraySize(key);
	if(number > 0)
	{
		bool bret = key[0].isArray();
		for(int i = 0; i < number; i++)
		{
			if(bret)
			{
				_RecursionReadArrayData(key[i], data_total/number, outData + i*data_total/number);
			}
			else	*(outData + i) = key[i].asInt();
		}
	}
}


void CMyJson::_RecursionReadFloatArrayData(const Json::Value& key, int data_total, float* outData OUT)
{
    int number = GetArraySize(key);
    if(number > 0)
    {
        bool bret = key[0].isArray();
        for(int i = 0; i < number; i++)
        {
            if(bret)
            {
                _RecursionReadFloatArrayData(key[i], data_total/number, outData + i*data_total/number);
            }
            else	*(outData + i) = key[i].asFloat();
        }
    }
}

bool CMyJson::_ArrayDimensionIsEqual(const vector<int>& number1, const vector<int>& number2)
{
	if(number1.size() != number2.size())
		return false;

    for(int i = 0; i < (int)number1.size(); i++)
		if(number1[i] != number2[i])
			return false;
	return true;
}

void CMyJson::_RecursionWriteArrayData(const vector<int>& inDimensions, const int* bufData, Json::Value& key OUT)
{
	int DimensionCount = inDimensions.size();
	int data_total = 1;
	for(int i = 0; i < DimensionCount; i++)
		data_total *= inDimensions[i];

	int number = inDimensions[0];
	for(int i = 0; i < number; i++)
	{
		if(1 != DimensionCount)
		{
			vector<int> vec(inDimensions.begin()+1, inDimensions.end());
			_RecursionWriteArrayData(vec, bufData + i*data_total/number, key[i]);
		}
		else	key.append(*(bufData + i));
	}
}

void CMyJson::_RecursionWriteFloatArrayData(const vector<int>& inDimensions, const float* bufData, Json::Value& key OUT)
{
    int DimensionCount = inDimensions.size();
    int data_total = 1;
    for(int i = 0; i < DimensionCount; i++)
        data_total *= inDimensions[i];

    int number = inDimensions[0];
    for(int i = 0; i < number; i++)
    {
        if(1 != DimensionCount)
        {
            vector<int> vec(inDimensions.begin()+1, inDimensions.end());
            _RecursionWriteFloatArrayData(vec, bufData + i*data_total/number, key[i]);
        }
        else	key.append(*(bufData + i));
    }
}
