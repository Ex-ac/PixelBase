#ifndef PIXELBASE_H_
#define PIXELBASE_H_

#ifdef __cplusplus
extern "C"
{
#endif



#include "../../Fatfs/FatFsApi.h"

#include "../../PortingLayer/GlobalDefine.h"


#define LongCommandBuffSize 13
#define ShortCommandBuffSize 7
#define MaxSizeOfBuffByte 1024

#ifndef HAL_TimeoutMs

#define HAL_TimeoutMs 1UL

#endif



#define USE_FATFS
#define USE_SPI
#define USE_SendToPc

#ifdef SendToPCByUartDriver

#include "../SpiDriver/SpiDriver.h"
typedef SpiSlaver TransmitHandle;

#define TransmitHandle_Init(slaver, master, port, pin) (SpiSlaver_Init((slaver), (master), (port), (pin)))

#define TransmitHandle_ReceiveByDMA(handle, data, size) (SpiSlaver_ReceiveByDMA(handle, (data), (size)))

#define TransmitHandle_Receive(handle, data, size, timeout) (SpiSlaver_Receive((handle), (data), (size), (timeout)))
	
#define TransmitHandle_TransmitByDMA(handle, data, size) (SpiSlaver_TransmitByDMA((handle), (data), (size)))
	
#define TransmitHandle_Transmit(handle, data, size, timeout) (SpiSlaver_Transmit((handle), (data), (size), (timeout)))

#define TransmitHandle_PrepareForTransmit(handle, direction, ms) (SpiSlaver_PrepareForTransmit((handle), (direction), (ms)))
	
#define TransmitHandle_EndTransmit(handle, direction) (SpiSlaver_EndTransmit((handle), (direction)))

#define TransmitHandle_WaitForTransmit(handle, direction, ms) (SpiSlaver_WaitForTransmit((handle), (direction), (ms)))


#endif


void PixelBase_SetSaveDirPath(char *dirPath);




#ifdef SendToPCByUartDriver
#include "../UartDriver/UartDriver.h"
typedef  UartDriver SendToPCHandle;

#define SendToPCHandle_PrepareForTransmit(handle, direction, ms) (UartDriver_PrepareForTransmit((handle), (direction), (ms)))

#define SendToPCHandle_EndTransmit(handle, direction) (UartDriver_EndTransmit((handle), (direction)))

#define SendToPCHandle_WaitForTransmit(handle, direction, ms) (UartDriver_WaitForTransmit((handle), (direction), (ms)))


#define SendToPCHandle_ReceiveByDMA(handle, data, size) (UartDriver_ReceiveByDMA(handle, (data), (size)))

#define SendToPCHandle_Receive(handle, data, size, timeout) (UartDriver_Receive((handle), (data), (size), (timeout)))
	
#define SendToPCHandle_TransmitByDMA(handle, data, size) (UartDriver_TransmitByDMA((handle), (data), (size)))
	
#define SendToPCHandle_Transmit(handle, data, size, timeout) (UartDriver_Transmit((handle), (data), (size), (timeout)))

#endif



#ifndef TimeoutMs 
#define TimeoutMs 1
#endif

#ifdef USE_RTOS

#define delayMs(ms) (vTaskDelay(pdMS_TO_TICKS(ms)))

#else

#define delayMs(ms) (HAL_Delay((ms)))

#endif

typedef uint16_t Zoom;

typedef enum TagFlag
{
	StartFlag = 0x8e,
	EndFlag = 0xe8
}Flag;

typedef enum TagRequestCommand
{
	RequestCommand_Inquiry = 0x01,
	RequestCommand_Focus = 0x09,
	RequestCommand_TakePicture = 0x05,
	RequestCommand_GetPicturePack = 0x07
}RequestCommand;

typedef enum TagAnswerCommand
{
	AnswerCommand_Inquiry = 0x02,
	AnswerCommand_Focus = 0x0a,
	AnswerCommand_TakePicture = 0x06,
	AnswerCommand_GetPicturePack = 0x08
}AnswerCommand;

typedef enum TagErrorCode
{
	ErrorCode_NoneError = 0x00,
	ErrorCode_CommandError = 0x01,
	ErrorCode_DriverFaliureError = 0x02,
	ErrorCode_BusyError = 0x03,
	ErrorCode_CheckError = 0x04,
	ErrorCode_DataError = 0x05,
	ErrorCode_UnkownError = 0x06,
	ErrorCode_NeedDataBuff,
}ErrorCode;

typedef enum TagBaudRate
{
	BaudRate9600 = 1,
	BaudRate19200,
	BaudRate38400,
	BaudRate57600,
	BaudRate115200,
	BaudRate230400,
	BaudRate460800,
	BaudRate921600
}BaudRate;

typedef enum TagFocusRequest
{
	FocusRequest_AutoFocus = 0x01,
	FocusRequest_GetZoom = 0x02,
	FocusRequest_SetZoom = 0x03
}FocusRequest;

typedef enum TagResolution
{
	Resolution_W320 = 1,
	Resolution_W640,
	Resolution_W800,
	Resolution_W1024,
	Resolution_W1280,
	Resolution_W1600,
	Resolution_W2048,
	Resolution_W2592,
}Resolution;

typedef enum TagQuality
{
	Quality_VeryHigh = 1,
	Quality_High,
	Quality_Medium,
	Quality_Low
}Quality;

typedef enum TagImageFormat
{
	ImageFormat_JPG = 1,
	ImageFormat_RGB565,
	ImageFormat_YUV422
}ImageFormat;

typedef enum TagExposure
{
	AutoExposure = 0x00,
	Exposure0 = 0x80,
	Exposure1 = 0x81,
	Exposure2 = 0x82,
	Exposure3 = 0x83,
	Exposure4 = 0x84,
	Exposure5 = 0x85,
	Exposure6,
	Exposure7,
	Exposure8,
	Exposure9,
	Exposure10,
	Exposure11,
	Exposure12,
	Exposure13,
	Exposure14,
	Exposure15
}Exposeure;


typedef struct TagDriverInfo
{
	bool hasFirmware;
	bool isSpi;
	uint8_t baudRate;
	uint8_t softwareAndHardware;
	Zoom zoom;
}DriverInfo;

typedef struct TagTakePictureInfo
{
	bool isColor;
	uint8_t resolution;
	uint8_t quality;
	uint8_t imageFormat;
	uint8_t exposure;
	
}TakePictureInfo;

typedef struct TagPicturePackInfo
{
	uint16_t sizeOfPack;
	uint32_t sizeOfByte;
	
}PicturePackInfo;

typedef struct TagPackData
{
	uint16_t numberOfPack;
	uint16_t sizeOfByte;
	uint16_t sizeOfPack;
	const uint8_t *data;
}PackData;

typedef enum TagSaveWay
{
	SaveToSD = 0x01,
	SendToPC = 0x02,
	SendDataPackToPC = 0x04,

	SDSaveFinished = 0x08,
	PCSendFinished = 0x10,
}SaveWay;

typedef enum TagPixelBaseStatus
{
	PixelBase_Free = 0x01 << 0,
	PixelBase_NeedAnswer = 0x01 << 1,
	PixelBase_NeedGetAnswer = 0x01 << 2,
	PixelBase_NeedDeal = 0x01 << 3,
	
	PixelBase_AutoGetNextDataPack = 0x01 << 4,
}PixelBaseStatus;

typedef struct TagPixelBase
{

  uint8_t id;
	uint8_t status;
	uint8_t saveWay;
	uint8_t lastErrorCode;
	
	uint8_t receiveDataBuff[LongCommandBuffSize];
	uint8_t sendDataBuff[LongCommandBuffSize];

	Zoom zoom;
	
	TakePictureInfo takePictureInfo;
	PicturePackInfo picturePackInfo;
	
	DriverInfo driverInfo;
	PackData packData;

    //pixelbase
    TransmitHandle *transmitHandle;

	GpioPin irqPin;

	uint32_t tickUse;

	SendToPCHandle *sendToPCHandle;

}PixelBase;


extern PixelBase *pixelBaseList[PixelBaseCounter];
//Protocol
bool PixelBase_SendRequestCommand(PixelBase *pixelBase, const uint8_t *data);
#define PixelBase_ReSendRequestCommand(pixelBase) (PixelBase_SendRequestCommand((pixelBase), (pixelBase)->sendDataBuff))

uint8_t PixelBase_GetAnswer(PixelBase *pixelBase, uint8_t *data);

#define PixelBase_GetFirst11Byte(pixelBase) (TransmitHandle_Receive((pixelBase)->transmitHandle, (pixelBase)->receiveDataBuff, 11, HAL_TimeoutMs))
#define PixelBase_GetLast2Byte(pixelBase) (TransmitHandle_Receive((pixelBase)->transmitHandle, (pixelBase)->receiveDataBuff + 11, 2, HAL_TimeoutMs))
#define PixelBase_GetLongByte(pixelBase, data, size) (TransmitHandle_ReceiveByDMA((pixelBase)->transmitHandle, (data), (size)))


uint8_t PixelBase_DriverId(PixelBase *pixelBase);


#define PixelBase_DriverInfo(pixelBase) ((const DeiverInfo *)(&((pixelBase)->driverInfo)))
#define PixelBase_PicturePackInfo(pixelBase) ((const PicturePackInfo *)(&((pixelBase)->picturePackInfo)))
#define PixelBase_Zoom(pixelBase) ((pixelBase)->zoom)


#define PixelBase_TakePictureInfo(pixelBase) ((const TakePictureInfo *)(&((pixelBase)->takePictureInfo)))
#define PixelBase_LastPackData(pixelBase) ((const PackData *)(&((pixelBase)->packData)))


#define PixelBase_PackDataAnswerCommand(pixelBase) (*((pixelBase)->receiveDataBuff + 4))

#define PixelBase_PackSize(pixelBase) (((uint16_t)(*((pixelBase)->receiveDataBuff + 1)) << 8) + (uint16_t)(*((pixelBase)->receiveDataBuff + 2)))

uint8_t PixelBase_InquiryAnswer(PixelBase *pixelBase);
uint8_t PixelBase_TakePictureAnswer(PixelBase *pixelBase);
uint8_t PixelBase_FoucsAnswer(PixelBase *pixelBase);
uint8_t PixelBase_GetPicturePackAnswer(PixelBase *pixelBase, const uint8_t *data);

uint8_t PixelBase_CheckSum(const uint8_t *data, const uint16_t count);

void PixelBase_InquiryRequest(uint8_t *data);
void PixelBase_FocusRequest(uint8_t *data, uint8_t focus, const Zoom zoom);
void PixelBase_TakePictureRequest(uint8_t *data, TakePictureInfo *p_info);
void PixelBase_GetPicturePackRequest(uint8_t *data, uint16_t numberOfPack, uint16_t totalSizeOfPack);



//Pixlebase
#ifdef USE_SPI
void PixelBase_Init(PixelBase *pixelBase, uint8_t id, SpiMaster *master, GPIO_TypeDef *csnPort, uint16_t csnPin, GPIO_TypeDef *irqPort, uint16_t irqPin, SendToPCHandle *pcHnadle);
#endif

#define PixeBase_SetSaveWay(pixelBase, saveWay) ((pixelBase)->saveWay |= (saveWay))
#define PixelBase_SaveWay(pixelBase) (uint8_t((pixelBase)->saveWay))

//deal data pack
void PixelBase_DealAnswer(PixelBase *pixelBase);
#define PixelBase_DealPackDataFinished(pixelBase) (((pixelBase)->saveWay & uint8_t(SDSaveFinished)) > 0 && ((pixelBase)->saveWay & uint8_t(PCSendFinished))) > 0)

//send data pack to pc
#define PixelBase_SetSendPackDataFinished(pixelBase) ((pixelBase->saveWay |= uint8_t(PCSendFinished)))
bool PixelBase_SendPackData(PixelBase *pixelBase);

//save data pack to sd
#define PixelBase_SetSavePackDataFinished(pixelBase) ((pixelBase)->saveWay |= uint8_t(SDSaveFinished))
bool PixelBase_SavePackData(PixelBase *pixelBase);

//auto get pack
#define PixelBase_AutoGetNextDataPack(pixelBase) (((pixelBase)->status & PixelBase_AutoGetNextDataPack) > 0)
#define PixelBase_SetAutoGetDataPack(pixelBase, y) (y ? (PixelBase)->status |= PixelBase_AutoGetNextDataPack : (PixelBase)->status &= (~PixelBase_AutoGetNextDataPack))

bool PixelBase_NextRequestCommand(PixelBase *pixelBase, uint8_t *data);

//
#define PixeBase_CurrentDataPackNumber(pixelBase) ((pixelBase)->packData.numberOfPack)


// request
#define PixelBase_Status(pxielBase) ((pixelBase)->status & 0x0f)
#define PixelBase_NeedAnswer(pixelBase) (((pixelBase)->status & PixelBase_NeedAnswer) > 0)
#define PixelBase_NeedGetAnswer(pixelBase) (((pixelBase)->status & PixelBase_NeedGetAnswer) > 0)
#define PixelBase_NeedDeal(pixelBase) (((pixelBase)->status & PixelBase_NeedDeal) > 0)


#define PixleBase_HasAnswerIn(pixelBase) (GpioPin_IsInputHight(&((pixelBase)->irqPin)) ? false : PixelBase_SetNeedAnswer((pixelBase), false), true)

#define PixelBase_SetNeedAnswer(pixelBase, y) ((y) ? ((pixelBase)->status = (pixelBase)->status & 0xf0) + PixelBase_NeedAnswer : ((pixelBase)->status & 0xf0) + PixelBase_NeedGetAnswer)
#define PixelBase_SetNeedGetAnswer(pixelBase, y) ((y) ? ((pixelBase)->status = (pixelBase)->status & 0xf0) + PixelBase_NeedGetAnswer : ((pixelBase)->status & 0xf0) + PixelBase_NeedDeal)
#define PixelBase_SetNeedDeal(pixelBase, y) (y ? (pixelBase)->status = ((pixelBase)->status & 0xf0) + PixelBase_NeedDeal : (pixelBase)->status = ((pixelBase)->status & 0xf0) + PixelBase_Free)

#define PixelBase_SetStatusFree(pixelBase, y) ((y) ? (pixelBase)->status = ((pixelBase)->status & 0xf0) + PixelBase_Free : (pixelBase)->status)
#ifdef USE_RTOS
#define PixelBase_Timeout(pixelBase) ((pixelBase)->tickUse > 3)
#endif


#ifdef __cplusplus
}
#endif

#endif
