#ifndef ESP8266_H_
#define ESP8266_H_

// Include STM32 Hardware

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_uart.h>
#include <stdint.h>

#include <string.h>

#include "../../Other/HelpFunction.h"

#define Timeout uint32_t(200);
typedef  UART_HandleTypeDef HardwareHandle;

class Esp8266
{
  public:
	enum WorkMode
	{
		Station = 0x01,
		Ap = 0x02,
		ApAndStation = 0x03,
		Error = 0x04,
	};
	enum EncryptMethod
	{
		Open = 0x00,
		Wpa = 0x02,
		Wpa2 = 0x03,
		WpaAndWpa = 0x04,
	};
	enum ConnectMode
	{
		Tcp,
		Udp,
	};
	struct ApInfo
	{
		char name[16];
		uint8 strength;
	};
	typedef IpAddress uint32_t typedef Port uint16_t public : explicit Esp8266(HardwareHandle *hardwareHandle = NULL)
	{
		mHardwareHandle = hardwareHandle;
		mMulitConnectHasOpen = false;
		mWorkMode = Station;
	}
	bool init(WorkMode mode = Station, Port port = 0);

	bool joinAp(const char *ssid, const char *pwd);
	bool quitAp();

	bool setAp(const char *ssid, const char *pwd, uint8_t chl, EncryptMethod method);

	bool setMulitConnect(bool y = false);
	bool connectServer(ConnectMode mode, IPAddress ip, Port port, uint8_t id = 0);

	bool sendDataPrepare(uint16_t length, uint8_t id = 0);
} bool sendDataEnd();

protected:
	bool sendCommand(const char *command);
	bool responseOk(const char *str);

protected:
//hardware send and read

virtual bool hardwareTransmit(const uint8_t *data, uint16_t length)
{
	return HAL_UART_Transmit_DMA(mHardwareHandle, data, length) == HAL_OK;
}
virtual bool hardwareReceive(uint8_t *data, uint16_t length)
{
	return HAL_UART_Receive(mHardwareHandle, data, length, Timeout) == HAL_OK;
}

private:
	HardwareHandle mHardwareHandle;
	bool mMulitConnectHasOpen;
	WorkMode mWorkMode;
	uint8_t mBuff[128];

};

#endif
