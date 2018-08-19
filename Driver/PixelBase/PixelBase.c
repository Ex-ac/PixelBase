#include "PixelBase.h"


PixelBase *pixelBaseList[PixelBaseCounter];

static char saveDirPath[11];
inline void PixelBase_SetSaveDirPath(char *dirPath)
{
	int32_t i = 0;
	for (; i < 11; ++i)
	{
		saveDirPath[i] = *(dirPath + i);
	}
}




bool PixelBase_SendRequestCommand(PixelBase *pixelBase, const uint8_t *data)
{
	uint8_t *tData = pixelBase->sendDataBuff;
	bool y = false;

	

	*(tData + 0) = StartFlag;
	*(tData + 1) = 0x00;
	*(tData + 2) = 0x08;
	*(tData + 3) = pixelBase->id;
	*(tData + LongCommandBuffSize - 1) = EndFlag;

	for (uint8_t i = 0; i < ShortCommandBuffSize; ++i)
	{
		*(tData + i + 4) = *(data + i);
	}
	*(tData + LongCommandBuffSize - 2) = PixelBase_CheckSum((tData + 1), LongCommandBuffSize - 2);
	
	while (!TransmitHandle_PrepareForTransmit(pixelBase->transmitHandle, Tranmist, TimeoutMs))
	{
	}
	
	if (TransmitHandle_Transmit(pixelBase->transmitHandle, tData, LongCommandBuffSize, HAL_TimeoutMs) == HAL_OK)
	{
		while (!TransmitHandle_WaitForTransmit(pixelBase->transmitHandle, Transmit, TimeoutMs))
		{
		}
		y = true;
		PixelBase_SetNeedAnswer(pixelBase, true);
		pixelBase->tickUse = 0;
	}

	TransmitHandle_EndTransmit(pixelBase->transmitHandle, uint8_t(Direction_Tranmist));
	return y;
}

uint8_t PixelBase_GetAnswer(PixelBase *pixelBase, uint8_t *data)
{
	uint8_t sum = 0;
	HAL_StatusTypeDef temp;
	while (!TransmitHandle_PrepareForTransmit(pixelBase->transmitHandle, Receive, TimeoutMs))
	{
	}
	temp = PixelBase_GetFirst11Byte(pixelBase);

	if (temp != HAL_OK)
	{
		TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
		return pixelBase->lastErrorCode = ErrorCode_DriverFaliureError;
	}

	if (PixelBase_PackDataAnswerCommand(pixelBase) == AnswerCommand_GetPicturePack)
	{
		if (data == 0)
		{
			TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
			return pixelBase->lastErrorCode = ErrorCode_NeedDataBuff;
		}
		else
		{
			temp = TransmitHandle_ReceiveByDMA(pixelBase->transmitHandle, data, PixelBase_PackSize(pixelBase) - 8);
			if (temp != HAL_OK)
			{
				TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
				return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_DriverFaliureError);
			}
			sum += PixelBase_CheckSum(data, PixelBase_PackSize(pixelBase) - 8);
		}

	}

	temp = PixelBase_GetLast2Byte(pixelBase);
	if (temp != HAL_OK)
	{
		TransmitHandle_EndTransmit(pixelBase->transmitHandle, Receive);
		return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_DriverFaliureError);
	}


	sum += PixelBase_CheckSum(pixelBase->receiveDataBuff, LongCommandBuffSize - 3);

	if (sum != *(pixelBase->receiveDataBuff + LongCommandBuffSize - 2))
	{
		return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_CheckError);
	}
	PixelBase_SetNeedGetAnswer(pixelBase, false);
	return pixelBase->lastErrorCode = (uint8_t)(ErrorCode_NoneError);
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
		pixelBase->picturePackInfo.sizeOfByte = ((uint32_t)(*(tData + 6) << 16)) + ((uint32_t)(*(tData + 7)) << 8) + (uint32_t)(*(tData + 10));

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

	if (*(data + 5) == 0x00)
	{
		pixelBase->packData.numberOfPack = (uint16_t)(*(data + 6) << 8) + (*(data + 7));
		pixelBase->packData.sizeOfByte = (uint16_t)(*(data + 1) << 8) + *(data + 2);
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

    for (uint16_t i = 0; i < count; ++i)
    {
        sum += *(data + i);
    }
    return sum;
}

inline void PixelBase_InquiryRequest(uint8_t *data)
{
	*data = RequestCommand_Inquiry;
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
}

//Pixlebase
#ifdef USE_SPI
void  PixelBase_Init(PixelBase *pixelBase, uint8_t id, SpiMaster *master, GPIO_TypeDef *csnPort, uint16_t csnPin, GPIO_TypeDef *irqPort, uint16_t irqPin, SendToPCHandle *pcHnadle)
{
	GPIO_InitTypeDef irqInitData;
	
	pixelBase->sendToPcHandle = pcHnadle;
	
	pixelBase->id = id;
	
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



bool PixelBase_SendPackData(PixelBase *pixelBase)
{
	uint8_t *tData = pixelBase.receiveDataBuff;
	
	*(tData+0)= 0x7e;
	*(tData+1)= 0;
	*(tData+2)= LongCommandBuffSize;
	*(tData+3)= id;
	*(tData+11) = checksum(tData + 1, LongCommandBuffSize - 3);

	if (PixelBase_PackDataAnswerCommand(pixelBase) != uint8_t(AnswerCommand_GetPicturePack) || !(pixelBase->saveWay & uint8_t(SendDataPackToPC)))
	{
		if (!SendToPCHandle_PrepareForTransmit(pixleBase->sendToPCHandle, Transmit, 100))
		{
			return false;
		}
		SendToPCHandle_TransmitByDMA(pixelBase->sendToPCHandle, tData, LongCommandBuffSize);

		if (!SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
		{
			SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
			return false;
		}
		SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
	}
	else
	{
		*(tData+1)= uint16_t(pixelBase->packData.sizeOfByte + LongCommandBuffSize) >> 8;
		*(tData+2)= uint16_t(pixelBase->packData.sizeOfByte + LongCommandBuffSize) & 0xff;
		*(tData+11) += checksum(pixelBase->packData.data, pixelBase->packData.sizeOfByte);

		if (!SendToPCHandle_PrepareForTransmit(pixleBase->sendToPCHandle, Transmit, 100))
		{
			return false;
		}
		SendToPCHandle_TransmitByDMA(pixelBase->sendToPCHandle, tData, LongCommandBuffSize - 2);

		if (!SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
		{
			SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
			return false;
		}

		SendToPCHandle_TransmitByDMA(pixelBase->sendToPCHandle, pixelBase->packData.data, pixelBase->packData.sizeOfByte);


		if (!SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
		{
			SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
			return false;
		}

		SendToPCHandle_TransmitByDMA(pixelBase->sendToPCHandle, tData + LongCommandBuffSize - 2,  2);

		if (!SendToPCHandle_WaitForTransmit(pixelBase->sendToPCHandle, Transmit, 100))
		{
			SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
			return false;
		}

		SendToPCHandle_EndTransmit(pixelBase->sendToPCHandle, Transmit);
	}
	return true;
}
//save data pack to sd

bool PixelBase_SavePackData(PixelBase *pixelBase)
{
	uint8_t fileName[17] = {0x00};
	uint32_t count;
	sprintf((char *)(fileName), "%s/%2d.jpg", saveDirPath, pixelBase->id);
	
	while (!FatFsApi_Prepare(1))
	{
		
	}
	
	if (f_open(&SDFile, (const char *)(fileName), FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		__breakpoint(0);
		FatFsApi_End();
		return false;
	}

	if (pixelBase->packData.numberOfPack == 1)
	{
		f_lseek(&SDFile, pixelBase->picturePackInfo.sizeOfByte);
	}

	f_lseek(&SDFile, (pixelBase->packData.numberOfPack - 1) * MaxSizeOfBuffByte);

	f_write(&SDFile, pixelBase->packData.data, pixelBase->packData.sizeOfByte, &count);

	f_close(&SDFile);
	
	FatFsApi_End();
	
	return true;
}

//auto get pack

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

