#ifndef FATFSAPI_H_
#define FATFSAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <fatfs.h>
#include <stdbool.h>
#include <stdio.h>

#include "../PortingLayer/GlobalDefine.h"

#include <rtc.h>

#define DirPathWidth 17

#ifdef USE_RTOS
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#endif

#ifdef USE_FatfsThread

#include "../Driver/PixelBase/PixelBase.h"

#ifndef FatfsThreadTaskPriority
#define FatfsThreadTaskPriority 3
#endif

#ifndef FatfsThreadTaskStackDepth
#define FatfsThreadTaskStackDepth 1024 * 2
#endif

    typedef struct TagFatfsThreadQueueItem
    {
        uint8_t data[10];
    } FatfsThreadQueueItem;

    void FatfsThread_Init(void);

    bool FatfsThread_AddCreateSaveDirCommand(const RTC_TimeTypeDef *time, const RTC_DateTypeDef *date, uint32_t ms);
    bool FatfsThread_AddCreatePixelBaseDirCommand(uint8_t id, uint32_t ms);
    bool FatfsThread_AddSavePixelBaseDataCommand(uint8_t id, uint32_t ms);

    bool FatfsThread_IsCreatePixelBaseDirCommand(const FatfsThreadQueueItem *item);
    bool FatfsThread_IsSavePixelBaseDataCommand(const FatfsThreadQueueItem *item);

    void FatfsThread_TaskFunction(void *arg);
    // #ifndef MemaryPoolSize
    // #define MemaryPoolSize 50
    // #endif

    // typedef enum TagMemaryPoolFlage
    // {
    //     MemaryPoolFlage_Free = 0x01,
    //     MemaryPoolFlage_Used = 0x02,
    // }MemaryPoolFlage;

    // bool MemaryPool_Create();
    // uint8_t *MemaryPool_Request();
    // void MemaryPool_Release(uint8_t *data);

#else

void FatfsApi_Init(void);

void FatfsApi_AddTask(PixelBase *pxielBase);

bool FatfsApi_Prepare(uint32_t ms);
void FatfsApi_End(void);

#endif

    bool FatfsApi_Open(FIL *fp, const TCHAR *path, BYTE mode);
    bool FatfsApi_Lseek(FIL *fp, FSIZE_t ofs);
    bool FatfsApi_Write(FIL *fp, const void *buff, UINT btw, UINT *bw);
    bool FatfsApi_Close(FIL *fp);
    bool FatfsApi_MkDir(const char *dirPath);

#ifdef __cplusplus
}
#endif

#endif
