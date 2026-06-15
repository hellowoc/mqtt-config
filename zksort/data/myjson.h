#ifndef JSON_MYSJON_H_INCLUDED
#define JSON_MYSJON_H_INCLUDED


#include <fstream>
#include <vector>
#include <string>

#include "json.h"
using namespace std;

#define		OUT

struct INT_ARRAY_KEY
{
    vector<int> Dimensions;		//数组的各维数据数量，如int num[2][3]，则该参数有两个元素，第一个元素为2，第二个为3
    vector<int> Datas;			//数组数据，如int num[2][3]，则该参数有6个元素，每个元素表示num数组内的数值，
                                //其中第1个~3个对应num[0][0]~num[0][2]，第4~6个对应num[1][0]~num[1][2]
};

struct Float_ARRAY_KEY
{
    vector<int> Dimensions;		//数组的各维数据数量，如int num[2][3]，则该参数有两个元素，第一个元素为2，第二个为3
    vector<float> Datas;			//数组数据，如int num[2][3]，则该参数有6个元素，每个元素表示num数组内的数值，
                                //其中第1个~3个对应num[0][0]~num[0][2]，第4~6个对应num[1][0]~num[1][2]
};

enum JSON_RETURN_VAL
{
	JSON_SUCCESS			= 0,		//操作成功

	JSON_FAILED_INVALID		= 1,		//无效的键名
	JSON_FAILED_TYPE		= 2,		//键值类型错误
	JSON_FAILED_ABORT		= 3,		//操作被异常中断
	JSON_FAILED_ARRAY_NUM	= 4			//读取到的数组数据数量与指定的数组数量不一致，无法赋值
};

class CMyJson
{
public:
	CMyJson(void);
	~CMyJson(void);

public:

	static bool LoadJsonFile(const char* filename, Json::Value& root OUT);
	static bool SaveJsonFile(const char* filename, const Json::Value& root);
    static int GetArraySize(const Json::Value& json_val);

	//从root中读取键对应的值，返回成功将键值赋给最后一个参数，失败不赋值
	static JSON_RETURN_VAL ReadIntKey(const Json::Value& root, const string& keyName, int& outInt OUT);
    static JSON_RETURN_VAL ReadFloatKey(const Json::Value& root, const string& keyName, float& outFloat OUT);
	static JSON_RETURN_VAL ReadStringKey(const Json::Value& root, const string& keyName, string& outStr OUT);

	//从root中读取键对应的数组值：
	//返回成功，将数组的数据全部赋值给outBuffer，outArray没有数据
	//返回JSON_FAILED_ARRAY_NUM时，将读取到的数组数量和数据放在outArray中，outBuffer没有数据
	//返回其他值时，outBuffer与outArray都没有数据
	static JSON_RETURN_VAL ReadIntArrayKey(const Json::Value& root, const string& keyName, const vector<int>& inDimensions, int* outBuffer OUT, INT_ARRAY_KEY& outArray OUT);
    static JSON_RETURN_VAL ReadFloatArrayKey(const Json::Value& root, const string& keyName,const vector<int>& inDimensions,float* outBuffer OUT, Float_ARRAY_KEY& outArray OUT);
	//从root中读取键对应的数组字符串：（仅支持一维数组）
	//返回成功时，将读取到的数组数据放在outArray中
	//返回其他值时，outArray没有数据
	static JSON_RETURN_VAL ReadStringArrayKey(const Json::Value& root, const string& keyName, vector<string>& outArray OUT);


	static JSON_RETURN_VAL WriteIntKey(const string& keyName, int intData, Json::Value& root OUT);
    static JSON_RETURN_VAL WriteFloatKey(const string& keyName, float fData, Json::Value& root OUT);
	static JSON_RETURN_VAL WriteStringKey(const string& keyName, const string& strData, Json::Value& root OUT);

	static JSON_RETURN_VAL WriteIntArrayKey(const string& keyName, const vector<int>& inDimensions, const int* bufData, Json::Value& root OUT);
    static JSON_RETURN_VAL WriteFloatArrayKey(const string& keyName, const vector<int>& inDimensions, const float* bufData, Json::Value& root OUT);

	static JSON_RETURN_VAL WriteStringArrayKey(const string& keyName, const vector<string>& inArray, Json::Value& root OUT);

private:
	static int _RecursionReadArrayDimension(const Json::Value& key, vector<int>& ArrayDimension OUT);
	static void _RecursionReadArrayData(const Json::Value& key, int data_total, int* outData OUT);
	static bool _ArrayDimensionIsEqual(const vector<int>& number1, const vector<int>& number2);
	static void _RecursionWriteArrayData(const vector<int>& Dimensions, const int* bufData, Json::Value& key OUT);

    static void _RecursionReadFloatArrayData(const Json::Value& key, int data_total, float* outData OUT);
    static void _RecursionWriteFloatArrayData(const vector<int>& inDimensions, const float* bufData, Json::Value& key OUT);
};


#endif
