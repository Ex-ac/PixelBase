#include "PixelBase.h"

PixelBase *pixelBaseList[PixelBaseCounter] = {0x00};



#ifdef TransmitBySpi

inline static void TransmitHandle_Init(TransmitHandle *handle, SpiMaster *master, GPIO_TypeDef *port, uint32_t pin)
{
	SpiSlaver_Init(handle, master, port, pin);
}

inline static HAL_StatusTypeDef TransmitHandle_ReceiveByDMA(TransmitHandle *handle, uint8_t *data, uint32_t size)
{
	return SpiSlaver_ReceiveByDMA(handle, data, size);
}

inline static HAL_StatusTypeDef TransmitHandle_Receive(TransmitHandle *handle, uint8_t *data, uint32_t size, uint32_t ms)
{
	return SpiSlaver_Receive(handle, data, size, ms);
}

inline static HAL_StatusTypeDef TransmitHandle_TransmitByDMA(TransmitHandle *handle, const uint8_t *data, uint32_t size)
{
	return SpiSlaver_TransmitByDMA(handle, data, size);
}

inline static HAL_StatusTypeDef TransmitHandle_Transmit(TransmitHandle *handle, const uint8_t *data, uint32_t size, uint32_t ms)
{
	return SpiSlaver_Transmit(handle, data, size, ms);
}

inline static bool TransmitHandle_PrepareForTransmit(TransmitHandle *handle, uint8_t direction, uint32_t ms)
{
	return SpiSlaver_PrepareForTransmit(handle, direction, ms);
}

inline static bool TransmitHandle_EndTransmit(TransmitHandle *handle, uint8_t direction)
{
	return SpiSlaver_EndTransmit(handle, direction);
}

inline static bool TransmitHandle_WaitForTransmit(TransmitHandle *handle, uint8_t direction, uint32_t ms)
{
	return SpiSlaver_WaitForTransmit(handle, direction, ms);
}

#endif

#ifdef SendToPCByUartDriver

inline static HAL_StatusTypeDef SendToPCHandle_ReceiveByDMA(SendToPCHandle *handle, uint8_t *data, uint32_t size)
{
	return UartDriver_ReceiveByDMA(handle, data, size);
}

inline static HAL_StatusTypeDef SendToPCHandle_Receive(SendToPCHandle *handle, uint8_t *data, uint32_t size, uint32_t ms)
{
	return UartDriver_Receive(handle, data, size, ms);
}

inline static HAL_StatusTypeDef SendToPCHandle_TransmitByDMA(SendToPCHandle *handle, const uint8_t *data, uint32_t size)
{
	return UartDriver_TransmitByDMA(handle, data, size);
}

inline static HAL_StatusTypeDef SendToPCHandle_Transmit(SendToPCHandle *handle, const uint8_t *data, uint32_t size, uint32_t ms)
{
	return UartDriver_Transmit(handle, data, size, ms);
}

inline static bool SendToPCHandle_PrepareForTransmit(SendToPCHandle *handle, uint8_t direction, uint32_t ms)
{
	return UartDriver_PrepareForTransmit(handle, direction, ms);
}

inline static bool SendToPCHandle_EndTransmit(SendToPCHandle *handle, uint8_t direction)
{
	return UartDriver_EndTransmit(handle, direction);
}

inline static bool SendToPCHandle_WaitForTransmit(SendToPCHandle *handle, uint8_t direction, uint32_t ms)
{
	return UartDriver_WaitForTransmit(handle, direction, ms);
}

#endif

/*




*/
inline static HAL_StatusTypeDef PixelBase_GetFirst11Byte(PixelBase *pixelBase)
{
	return TransmitHandle_Receive(pixelBase->transmitHandle, pixelBase->receiveDataBuff, 11, HAL_TimeoutMs);
}
inline static HAL_StatusTypeDef PixelBase_GetLast2Byte(PixelBase *pixelBase)
{
	return TransmitHandle_Receive(pixelBase->transmitHandle, pixelBase->receiveDataBuff + 11, 2, HAL_TimeoutMs);
}
inline static HAL_StatusTypeDef PixelBase_GetLongByte(PixelBase *pixelBase, uint8_t *data, uint32_t size)
{
	if (pixelBase->transmitHandle->master->index == 0)
	{
		return TransmitHandle_Receive(pixelBase->transmitHandle, data, size, 10 * HAL_TimeoutMs);
	}
	return TransmitHandle_ReceiveByDMA(pixelBase->transmitHandle, data, size);
}

bool PixelBase_SendRequestCommand(PixelBase *pixelBase, const uint8_t *data)
{
	uint8_t *tData = pixelBase->sendDataBuff;
	uint32_t i = 0;
	bool y = false;

	*(tData + 0) = StartFlag;
	*(tData + 1) = 0x00;
	*(tData + 2) = 0x08;
	*(tData + 3) = pixelBase->id;
	*(tData + LongCommandBuffSize - 1) = EndFlag;

	for (; i < ShortCommandBuffSize; ++i)
	{
		*(tData + i + 4) = *(data + i);
	}
	*(tData + LongCommandBuffSize - 2) = PixelBase_CheckSum((tData + 1), LongCommandBuffSize - 3);

	while (!TransmitHandle_PrepareForTransmit(pixelBase->transmitHandle, Transmit, TimeoutMs))
	{
		delayMs(TimeoutMs);
	}

	if (TransmitHandle_Transmit(pixelBase->transmitHandle, pixelBase->sendDataBuff, LongCommandBuffSize, HAL_TimeoutMs) == HAL_OK)
	{
		if (!TransmitHandle_WaitForTransmit(pixelBase->transmitHandle, Transmit, TimeoutMs))
		{
			DebugBreak();
		}
		y = true;
		PixelBase_SetNeedAnswer(pixelBase, true);
		pixelBase->tickUse = 0;
	}
	TransmitHandle_EndTransmit(pixelBase->transmitHandle, Transmit);
	return y;
}

bool PixelBase_ReSendRequestCommand(PixelBase *pixelBase)
{
	bool y = false;

	while (!TransmitHandle_PrepareForTransmit(pixelBase->transmitHandle, Transmit, TimeoutMs))
	{
		delayMs(TimeoutMs);
	}

	if (TransmitHandle_Transmit(pixelBase->transmitHandle, pixelBase->sendDataBuff, LongCommandBuffSize, HAL_TimeoutMs) == HAL_OK)
	{
		if (!TransmitHandle_WaitForTransmit(pixelBase->transmitHandle, Transmit, TimeoutMs))
		{
			DebugBreak();
		}
		y = true;
		PixelBase_SetNeedAnswer(pixelBase, true);
		pixelBase->tickUse = 0;
	}
	TransmitHandle_EndTransmit(pixelBase->transmitHandle, Transmit);
	return y;
}

uint8_t PixelBase_GetAnswer(PixelBase *pixelBase, uint8_t *data)
{
	uint8_t sum = 0;
	HAL_StatusTypeDef temp;

	while (!TransmitHandle_PrepareForTransmit(pixelBase->transmitHandle, Receive, TimeoutMs))
	{
		delayMs(TimeoutMs);
	}

	temp = PixelBase_GetFirst11Byte(pixelBase);

	if (temp != HAL_OK)
	{
//		DebugBreak();
		TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
		return pixelBase->lastErrorCode = ErrorCode_DriverFaliureError;
	}

	if (PixelBase_PackDataAnswerCommand(pixelBase) == AnswerCommand_GetPicturePack)
	{
		if (data == 0)
		{
//			DebugBreak();
			TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
			return pixelBase->lastErrorCode = ErrorCode_NeedDataBuff;
		}
		else
		{
			temp = PixelBase_GetLongByte(pixelBase, data, PixelBase_PackSize(pixelBase) - 8);
			if (temp != HAL_OK)
			{
//				DebugBreak();
				TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
				return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_DriverFaliureError);
			}
			if (!TransmitHandle_WaitForTransmit(pixelBase->transmitHandle, Receive, 10 * TimeoutMs))
			{
//				DebugBreak();
			}
			sum += PixelBase_CheckSum(data, PixelBase_PackSize(pixelBase) - 8);
		}
	}

	temp = PixelBase_GetLast2Byte(pixelBase);
	if (temp != HAL_OK)
	{
//		DebugBreak();
		TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
		return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_DriverFaliureError);
	}
	TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
	sum += PixelBase_CheckSum(pixelBase->receiveDataBuff + 1, LongCommandBuffSize - 3);

	if (sum != *(pixelBase->receiveDataBuff + LongCommandBuffSize - 2))
	{
//		DebugBreak();
		return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_CheckError);
	}
	PixelBase_SetNeedGetAnswer(pixelBase, false);
	return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_NoneError);
}

inline uint8_t PixelBase_DriverId(PixelBase *pixelBase)
{
	return pixelBase->id;
}

inline const DriverInfo *PixelBase_DriverInfo(PixelBase *pixelBase)
{
	return &(pixelBase->driverInfo);
}
inline const PicturePackInfo *PixelBase_PicturePackInfo(PixelBase *pixelBase)
{
	return &(pixelBase->picturePackInfo);
}

inline Zoom PixelBase_Zoom(PixelBase *pixelBase)
{
	return pixelBase->zoom;
}

inline const TakePictureInfo *PixelBase_TakePictureInfo(PixelBase *pixelBase)
{
	return &(pixelBase->takePictureInfo);
}
inline const PackData *PixelBase_LastPackData(PixelBase *pixelBase)
{
	return &(pixelBase->packData);
}

inline uint8_t PixelBase_PackDataAnswerCommand(PixelBase *pixelBase)
{
	return *(pixelBase->receiveDataBuff + 4);
}

inline uint16_t PixelBase_PackSize(PixelBase *pixelBase)
{
	return (uint16_t)((*(pixelBase->receiveDataBuff + 1)) << 8) + (uint16_t)(*(pixelBase->receiveDataBuff + 2));
}

inline uint8_t PixelBase_InquiryAnswer(PixelBase *pixelBase)
{
	uint8_t *tData = pixelBase->receiveDataBuff;

	if (*(tData + 6) != 0 || *(tData + 7) != 3)
	{
		return pixelBase->lastErrorCode = ErrorCode_DataError;
	}

	pixelBase->driverInfo.baudRate = *(tData + 5);
	pixelBase->driverInfo.hasFirmware = *(tData + 9) == 0;
	pixelBase->driverInfo.isSpi = ((*(tData + 10)) & 0x80) > 0;
	pixelBase->driverInfo.softwareAndHardware = *(tData + 10) & 0x7f;
	return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_NoneError);
}

inline uint8_t PixelBase_TakePictureAnswer(PixelBase *pixelBase)
{
	uint8_t *tData = pixelBase->receiveDataBuff;
	if (*(tData + 5) == 0x00)
	{
		pixelBase->picturePackInfo.sizeOfByte = ((uint32_t)(*(tData + 6) << 16)) + ((uint32_t)(*(tData + 7)) << 8) + (uint32_t)(*(tData + 8));
		pixelBase->picturePackInfo.sizeOfPack = ((uint16_t)(*(tData + 9)) << 8) + (uint16_t)(*(tData + 10));
#ifdef USE_FatfsThread
		while (!FatfsThread_AddCreatePixelBaseDirCommand(pixelBase->id, 1))
		{
			delayMs(1);
		}
#endif
		return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_NoneError);
	}
	else
	{
		return pixelBase->lastErrorCode = *(tData + 5);
	}
}

inline uint8_t PixelBase_FoucsAnswer(PixelBase *pixelBase)
{
	uint8_t *tData = pixelBase->receiveDataBuff;
	if (*(tData + 5) == 0x00)
	{
		pixelBase->zoom = (uint16_t)(*(tData + 6) << 8) + (*(tData + 7));

		return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_NoneError);
	}
	else
	{
		return pixelBase->lastErrorCode = *(tData + 5);
	}
}

inline uint8_t PixelBase_GetPicturePackAnswer(PixelBase *pixelBase, const uint8_t *data)
{
	pixelBase->packData.data = data;

	if (*(pixelBase->receiveDataBuff + 5) == 0x01)
	{
		pixelBase->packData.numberOfPack = (uint16_t)(*(pixelBase->receiveDataBuff + 6) << 8) + (*(pixelBase->receiveDataBuff + 7));
		pixelBase->packData.sizeOfByte = PixelBase_PackSize(pixelBase) - 8;
		pixelBase->packData.sizeOfPack = pixelBase->picturePackInfo.sizeOfPack;

		return pixelBase->lastErrorCode = ErrorCode_NoneError;
	}
	else
	{
		return pixelBase->lastErrorCode = *(data + 5);
	}
}

inline uint8_t PixelBase_CheckSum(const uint8_t *data, const uint16_t count)
{
	uint8_t sum = 0;
	uint16_t i = 0;
	for (; i < count; ++i)
	{
		sum += *(data + i);
	}
	return sum;
}

inline void PixelBase_InquiryRequest(uint8_t *data)
{
	*(data + 0) = (uint8_t)(RequestCommand_Inquiry);
}

inline void PixelBase_FocusRequest(uint8_t *data, uint8_t focus, const Zoom zoom)
{
	*data = RequestCommand_Focus;
	*(data + 1) = focus;
	if (focus == FocusRequest_SetZoom)
	{
		*(data + 2) = zoom / 256;
		*(data + 3) = zoom % 256;
	}
}

inline void PixelBase_TakePictureRequest(uint8_t *data, TakePictureInfo *p_info)
{
	*data = RequestCommand_TakePicture;
	*(data + 1) = p_info->resolution;
	*(data + 2) = p_info->isColor ? 0x00 : 0x01 | p_info->quality;
	*(data + 3) = p_info->imageFormat;
	*(data + 4) = p_info->exposure;
}

inline void PixelBase_GetPicturePackRequest(uint8_t *data, uint16_t numberOfPack, uint16_t totalSizeOfPack)
{
	if (numberOfPack > totalSizeOfPack)
	{
		return;
	}
	*data = RequestCommand_GetPicturePack;
	*(data + 1) = 0x01;
	*(data + 2) = numberOfPack / 256;
	*(data + 3) = numberOfPack % 256;
	*(data + 4) = totalSizeOfPack / 256;
	*(data + 5) = totalSizeOfPack % 256;
	*(data + 6) = 0;
}

//Pixlebase
#ifdef TransmitBySpi
void PixelBase_Init(PixelBase *pixelBase, uint8_t id, SpiMaster *master, GPIO_TypeDef *csnPort, uint32_t csnPin, GPIO_TypeDef *irqPort, uint32_t irqPin, SendToPCHandle *pcHnadle)
{
	GPIO_InitTypeDef irqInitData;

	pixelBase->sendToPCHandle = pcHnadle;
	pixelBase->saveWay = 0x00;
	pixelBase->saveWay |= (uint8_t)(SendToPC);
#ifdef PixelBase_SaveToSD
	pixelBase->saveWay |= (uint8_t)(SaveToSD);
#endif
	pixelBase->id = id;
	pixelBase->status |= PixelBaseStatus_AutoGetNextDataPack;
#ifdef USE_RTOS
	pixelBase->transmitHandle = (TransmitHandle *)(pvPortMalloc(sizeof(TransmitHandle)));
#else
	pixelBase->transmitHandle = (TransmitHandle *)malloc(sizeof(TransmitHandle));
#endif

	TransmitHandle_Init(pixelBase->transmitHandle, master, csnPort, csnPin);

	irqInitData.Pin = irqPin;
	irqInitData.Mode = GPIO_MODE_INPUT;
	irqInitData.Pull = GPIO_PULLUP;

	pixelBase->irqPin.pin = irqPin;
	pixelBase->irqPin.port = irqPort;
	pixelBase->tickUse = 0;
	GpioPin_Init(&(pixelBase->irqPin), &irqInitData);
	pixelBaseList[id] = pixelBase;
}
#endif

inline void PixeBase_SetSaveWay(PixelBase *pixelBase, uint8_t saveWay)
{
	pixelBase->saveWay |= saveWay;
}
inline uint8_t PixelBase_SaveWay(PixelBase *pixelBase)
{
	return pixelBase->saveWay;
}

//deal data pack
void PixelBase_DealAnswer(PixelBase *pixelBase)
{
	if (pixelBase->saveWay & SendToPC)
	{
		PixelBase_SendPackData(pixelBase);
	}
	if (pixelBase->saveWay & SaveToSD && PixelBase_PackDataAnswerCommand(pixelBase) == AnswerCommand_GetPicturePack)
	{
		PixelBase_SavePackData(pixelBase);
	}

	PixelBase_SetStatusFree(pixelBase, true);
}

inline bool PixelBase_DealPackDataFinished(PixelBase *pixelBase)
{
	return (pixelBase->saveWay & ((uint8_t)(SDSaveFinished) + (uint8_t)(PCSendFinished))) == ((uint8_t)(SDSaveFinished) + (uint8_t)(PCSendFinished));
}

inline void PixelBase_SetSendPackDataFinished(PixelBase *pixelBase)
{
	pixelBase->saveWay |= (uint8_t)(PCSendFinished);
}

bool PixelBase_SendPackData(PixelBase *pixelBase)
{
	uint8_t *tData = pixelBase->receiveDataBuff;

	uint8_t str[64];
	bool ok = true;

	*(tData + 0) = 0x7e;
	*(tData + 1) = 0;
	*(tData + 2) = LongCommandBuffSize;
	*(tData + 3) = pixelBase->id;
	*(tData + 11) = PixelBase_CheckSum(tData + 1, LongCommandBuffSize - 3);

	if (PixelBase_PackDataAnswerCommand(pixelBase) == (uint8_t)(AnswerCommand_TakePicture))
	{
		sprintf(str, "%2d take photo success! Total pack: %8d.\n", pixelBase->id, pixelBase->picturePackInfo.sizeOfPack);
	}
	else if (PixelBase_PackDataAnswerCommand(pixelBase) == (uint8_t)(AnswerCommand_GetPicturePack))
	{
		if (pixelBase->packData.numberOfPack == pixelBase->packData.sizeOfPack)
		{
			sprintf(str, "\n\n%2d pack number: %8d, Total pack: %8d.\n\n\n", pixelBase->id, pixelBase->packData.numberOfPack, pixelBase->packData.sizeOfPack);
		}

		else
		{
			// ok = false;
			sprintf(str, "%2d pack number: %8d, Total pack: %8d.\n", pixelBase->id, pixelBase->packData.numberOfPack, pixelBase->packData.sizeOfPack);
		}
	}
	else if (PixelBase_PackDataAnswerCommand(pixelBase) == (uint8_t)(AnswerCommand_Focus))
	{
		sprintf(str, "%2d focus is: %d\n", pixelBase->id, pixelBase->zoom);
	}
	if (ok)
	{
		do
		{
			if (ok && SendToPCHandle_PrepareForTransmit(pixelBase->sendToPCHandle, Transmit, 1))
			{
				if (ok && SendToPCHandle_Transmit(pixelBase->sendToPCHandle, str, strlen(str), 10 * HAL_TimeoutMs) != HAL_OK)
				{

//					DebugBreak();
					SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
					ok = false;
				}

				if (ok && !SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, TimeoutMs))
				{
//					DebugBreak();
					SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
					ok = false;
				}

				if (ok)
				{
					SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
				}
			}
			else
			{
				delayMs(TimeoutMs);
			}
		} while (!ok);
	}
	// if (PixelBase_PackDataAnswerCommand(pixelBase) != (uint8_t)(AnswerCommand_GetPicturePack) || !(pixelBase->saveWay & (uint8_t)(SendDataPackToPC)))
	// {
	// 	if (!SendToPCHandle_PrepareForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
	// 	{
	// 		return false;
	// 	}
	// 	//		SendToPCHandle_TransmitByDMA(pixelBase->sendToPCHandle, tData, LongCommandBuffSize);

	// 	SendToPCHandle_Transmit(pixelBase->sendToPCHandle, tData, LongCommandBuffSize, 10 * HAL_TimeoutMs);
	// 	if (!SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
	// 	{
	// 		SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
	// 		return false;
	// 	}
	// 	SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);

	// 	do
	// 	{

	// 		if (ok && SendToPCHandle_PrepareForTransmit(pixelBase->sendToPCHandle, Transmit, TimeoutMs))
	// 		{

	// 		}
	// 		else
	// 		{
	// 			vTaskDelay(pdMS_TO_TICKS(TimeoutMs));
	// 		}
	// 	} while(!ok)
	// }
	// else
	// {
	// 	*(tData + 1) = (uint8_t)((pixelBase->packData.sizeOfByte + LongCommandBuffSize) >> 8);
	// 	*(tData + 2) = (uint8_t)((pixelBase->packData.sizeOfByte + LongCommandBuffSize) & 0xff);
	// 	*(tData + 11) += PixelBase_CheckSum(pixelBase->packData.data, pixelBase->packData.sizeOfByte);

	// 	if (!SendToPCHandle_PrepareForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
	// 	{
	// 		return false;
	// 	}
	// 	SendToPCHandle_TransmitByDMA(pixelBase->sendToPCHandle, tData, LongCommandBuffSize - 2);

	// 	if (!SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
	// 	{
	// 		SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
	// 		return false;
	// 	}

	// 	SendToPCHandle_TransmitByDMA(pixelBase->sendToPCHandle, pixelBase->packData.data, pixelBase->packData.sizeOfByte);

	// 	if (!SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
	// 	{
	// 		SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
	// 		return false;
	// 	}

	// 	SendToPCHandle_TransmitByDMA(pixelBase->sendToPCHandle, tData + LongCommandBuffSize - 2, 2);

	// 	if (!SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
	// 	{
	// 		SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
	// 		return false;
	// 	}

	// 	SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
	// }
	return true;
}
//save data pack to sd
inline void PixelBase_SetSavePackDataFinished(PixelBase *pixelBase)
{
	pixelBase->saveWay |= (uint8_t)(SDSaveFinished);
}

bool PixelBase_SavePackData(PixelBase *pixelBase)
{
#ifdef USE_FatfsThread
	while (!FatfsThread_AddSavePixelBaseDataCommand(pixelBase->id, 1))
	{
		delayMs(1);
	}
#endif
	// uint8_t fileName[22] = {0x00};
	// uint32_t count;
	// bool ok = true;

	// sprintf((char *)(fileName), "%2d.jpg", pixelBase->id);

	// for (uint32_t i = 0; i < 22; ++i)
	// {
	// 	if (fileName[i] == ' ')
	// 	{
	// 		fileName[i] = '0';
	// 	}
	// }
	// do
	// {
	// 	ok = true;
	// 	if (ok && FatfsApi_Prepare(TimeoutMs))
	// 	{
	// 		if (ok && FatfsApi_Open(&SDFile, (const char *)(fileName), FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	// 		{
	// 			ok = false;
	// 		}

	// 		if (ok && pixelBase->packData.numberOfPack == 1)
	// 		{
	// 			if (FatfsApi_Lseek(&SDFile, pixelBase->picturePackInfo.sizeOfByte) != FR_OK)
	// 			{
	// 				ok = false;
	// 			}
	// 		}
	// 		if (ok && FatfsApi_Lseek(&SDFile, (pixelBase->packData.numberOfPack - 1) * MaxSizeOfBuffByte) != FR_OK)
	// 		{
	// 			ok = false;
	// 		}

	// 		if (ok && FatfsApi_Write(&SDFile, pixelBase->packData.data, pixelBase->packData.sizeOfByte, &count) != FR_OK)
	// 		{
	// 			ok = false;
	// 		}
			
	// 		if (FatfsApi_Close(&SDFile) != FR_OK)
	// 		{
	// 			ok = false;
	// 		}

	// 		FatfsApi_End();

	// 	}
	// 	else
	// 	{
	// 		ok = false;
	// 	}
		
	// 	if (!ok)
	// 	{
	// 		delayMs(1);
	// 	}
	// } while (!ok);

	return true;
}

//auto get pack

inline bool PixelBase_AutoGetNextDataPack(PixelBase *pixelBase)
{
	return (pixelBase->status & (uint8_t)(PixelBaseStatus_AutoGetNextDataPack)) > 0;
}
inline void PixelBase_SetAutoGetDataPack(PixelBase *pixelBase, bool y)
{
	if (y)
	{
		pixelBase->status |= (uint8_t)(PixelBaseStatus_AutoGetNextDataPack);
	}
	else
	{
		pixelBase->status &= (~((uint8_t)(PixelBaseStatus_AutoGetNextDataPack)));
	}
}

bool PixelBase_NextRequestCommand(PixelBase *pixelBase, uint8_t *data)
{
	if (PixelBase_PackDataAnswerCommand(pixelBase) == AnswerCommand_TakePicture)
	{
		PixelBase_GetPicturePackRequest(data, 1, pixelBase->picturePackInfo.sizeOfPack);
		return true;
	}
	else if (PixelBase_PackDataAnswerCommand(pixelBase) == AnswerCommand_GetPicturePack)
	{
		if (pixelBase->packData.numberOfPack < pixelBase->picturePackInfo.sizeOfPack)
		{
			PixelBase_GetPicturePackRequest(data, pixelBase->packData.numberOfPack + 1, pixelBase->picturePackInfo.sizeOfPack);
			return true;
		}
	}
	return false;
}

inline uint16_t PixeBase_CurrentDataPackNumber(PixelBase *pixelBase)
{
	return pixelBase->packData.numberOfPack;
}

// request
inline uint8_t PixelBase_Status(PixelBase *pixelBase)
{
	return pixelBase->status & 0x0f;
}

inline bool PixelBase_NeedAnswer(PixelBase *pixelBase)
{

	return (pixelBase->status & (uint8_t)(PixelBaseStatus_NeedAnswer)) > 0;
}

inline bool PixelBase_NeedGetAnswer(PixelBase *pixelBase)
{
	return (pixelBase->status & (uint8_t)(PixelBaseStatus_NeedGetAnswer)) > 0;
}

inline bool PixelBase_NeedDeal(PixelBase *pixelBase)
{
	return (pixelBase->status & (uint8_t)(PixelBaseStatus_NeedDeal)) > 0;
}

inline bool PixleBase_HasAnswerIn(PixelBase *pixelBase)
{
	if (GpioPin_IsInputHight(&(pixelBase->irqPin)))
	{
		return false;
	}
	else
	{
		PixelBase_SetNeedAnswer(pixelBase, false);
		return true;
	}
}

inline void PixelBase_SetNeedAnswer(PixelBase *pixelBase, bool y)
{
	if (y)
	{
		pixelBase->status = (pixelBase->status & 0xf0) + (uint8_t)(PixelBaseStatus_NeedAnswer);
	}
	else
	{
		pixelBase->status = (pixelBase->status & 0xf0) + (uint8_t)(PixelBaseStatus_NeedGetAnswer);
	}
}

inline void PixelBase_SetNeedGetAnswer(PixelBase *pixelBase, bool y)
{
	if (y)
	{
		pixelBase->status = (pixelBase->status & 0xf0) + (uint8_t)(PixelBaseStatus_NeedGetAnswer);
	}
	else
	{
		pixelBase->status = (pixelBase->status & 0xf0) + (uint8_t)(PixelBaseStatus_NeedDeal);
	}
}

inline void PixelBase_SetNeedDeal(PixelBase *pixelBase, bool y)
{
	if (y)
	{
		pixelBase->status = (pixelBase->status & 0xf0) + (uint8_t)(PixelBaseStatus_NeedDeal);
	}
	else
	{
		pixelBase->status = (pixelBase->status & 0xf0) + (uint8_t)(PixelBaseStatus_Free);
	}
}

inline void PixelBase_SetStatusFree(PixelBase *pixelBase, bool y)
{
	if (y)
	{
		pixelBase->status = (pixelBase->status & 0xf0) + (uint8_t)(PixelBaseStatus_Free);
	}
}

bool PixelBase_Timeout(PixelBase *pixelBase)
{
#ifdef USE_RTOS
	return pixelBase->tickUse > TicksTimeout;
#else
	return false;
#endif
}
