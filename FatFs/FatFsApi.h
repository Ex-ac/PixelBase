#ifndef FATFSAPI_H_
#define FATFSAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <fatfs.h>
#include <stdbool.h>

#include "../PortingLayer/GlobalDefine.h"

#ifdef USE_RTOS
#include <FreeRTOS.h>
#include <semphr.h>
#endif

void FatFsApi_Init(void);

bool FatFsApi_Prepare(uint32_t ms);
void FatFsApi_End(void);

bool FatFsApi_Error(uint8_t ret);

FRESULT FatFsApi_open(FIL* fp, const TCHAR* path, BYTE mode);
FRESULT FatFsApi_lseek(FIL* fp, DWORD ofs);
FRESULT FatFsApi_write(FIL* fp, const void *buff, UINT btw, UINT* bw);
FRESULT FatFsApi_close(FIL* fp);

#ifdef __cplusplus
}
#endif

#endif
