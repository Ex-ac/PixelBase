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

#ifdef __cplusplus
}
#endif

#endif
