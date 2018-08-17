//定义全局变量
#ifndef GLOBALVARIABLE_H_
#define GLOBALVARIABLE_H_


#include "../PixelBase/PixelBase.h"
#include "../SpiDriver/SpiSlaver.h"
#include "../SpiDriver/SpiMaster.h"
#include "../TaskThreadPool/TaskThreadPool.h"
#include "../FileSystem/FileSystem.h"
#include "../TaskThreadPool/TaskThreadPool.h"
#include "../UsartDriver/Usart.h"


#define CameraCount 14

extern PixelBase *cameraList[CameraCount];

void createPixelBase();


#endif
