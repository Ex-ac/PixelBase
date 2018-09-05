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
#include <task.h>
#include 
#endif


void FatFsApi_Init(void);

void FatFsApi_AddTask(PixelBase *pxielBase);


bool FatFsApi_Prepare(uint32_t ms);
void FatFsApi_End(void);

bool FatFsApi_Error(uint8_t ret);

FRESULT FatFsApi_Open(FIL* fp, const TCHAR* path, BYTE mode);
FRESULT FatFsApi_Lseek(FIL* fp, FSIZE_t ofs);
FRESULT FatFsApi_Write(FIL* fp, const void *buff, UINT btw, UINT* bw);
FRESULT FatFsApi_Close(FIL* fp);

#ifdef __cplusplus
}
#endif

#endif
