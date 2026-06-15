#ifndef TIME_ENCRYPT
#define TIME_ENCRYPT

enum DECRYPT_STATUS
{
    decrypt_succeed = 0,//解密成功
    decrypt_error_code_format,//密码格式错误
};

const unsigned int MAX_CODE_LENGTH = 27;
const unsigned int MIN_CODE_LENGTH = 3;

/*输入加密码，输出到期时间与唯一码*/
int DecryptCode(const char* code, unsigned short* wYear, unsigned short* wMonth, unsigned short* wDay, char* onlyID);

/*两个加密码是否匹配，唯一码相同的匹配，返回true*/
bool WhetherCodesMatch(const char* code1, const char* code2);

#endif // TIME_ENCRYPT




