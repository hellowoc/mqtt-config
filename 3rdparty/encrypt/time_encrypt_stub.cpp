/* x86_64 stub for time_encrypt - no actual encryption, just decrypts to "2099-12-31" */
#include "time_encrypt.h"
#include <string.h>

int DecryptCode(const char* code, unsigned short* wYear, unsigned short* wMonth, unsigned short* wDay, char* onlyID)
{
    /* Stub: always return far-future date, decryption always succeeds */
    if (wYear)  *wYear  = 2099;
    if (wMonth) *wMonth = 12;
    if (wDay)   *wDay   = 31;
    if (onlyID) strcpy(onlyID, "X86STUB");
    (void)code;
    return 0; /* decrypt_succeed */
}

bool WhetherCodesMatch(const char* code1, const char* code2)
{
    (void)code1;
    (void)code2;
    return true; /* always match on x86 */
}
