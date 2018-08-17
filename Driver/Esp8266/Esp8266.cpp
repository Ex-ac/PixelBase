#include "Esp8266.h"

bool Esp8266::init(WorkMode mode = Station, Port port)
{
    mWorkMode = mode;
    switch (work)
    {
    case ApAndStation:
        sendCommand("CWMODE=3");
        break;

    case Station:
        sendCommand("CWMODE=1");
        break;

    default:
        sendCommand("CWMODE=2");
    }


    if (responseOk("OK"))
    {
        if (reset())
        {
            return;
        }
    }

    mWorkMode = Error;
}
bool Esp8266::reset()
{
    sendCommand("RST");
    if (responseOk("OK"))
    {
        HAL_Delay(2000);
        return true;
    }
    return false;
}

bool Esp8266::joinAp(const char *ssid, const char *pwd)
{
    sprintf(mBuff, "CWJAP=\"%s\",\"%s\"",ssid, pwd);
    if (!sendCommand(mBuff))
    {
        return false;
    }

    return responseOk("OK");
}
bool Esp8266::quitAp()
{
    if (sendCommand("CWQAP"))
    {
        return responseOk("OK");
    }

    return false;
}

bool Esp8266::setAp(const char *ssid, const char *pwd, uint8_t chl, EncryptMethod method)
{
    sprintf(mBuff, "CWSAP=\"%s\",\"%s\",%d, %d", ssid, pwd, chl, uint8_t(method));
     if (!sendCommand(mBuff))
    {
        return false;
    }

    return responseOk("OK");

}

bool Esp8266::setMulitConnect(bool y = false)
{
    if (y)
    {
        if (!sendCommand("CIPMUX=0"))
        {
            return false;
        }
    }
    else
    {
        if (!sendCommand("CIPMUX=1"))
        {
            return false;
        }
    }

    if (responseOk("OK"))
    {
        mMulitConnectHasOpen = y;
        return true;
    }
    return false;
}
bool Esp8266::connectServer(ConnectMode mode, IPAddress ip, Port port, uint8_t id)
{
    if (!hardwareTransmit("AT+CIPSTART=", 12))
    {
        return false;
    }

    if (mMulitConnectHasOpen)
    {
        if (!hardwareTransmit(&('0' + id), 1))
        {
            return false;
        }

        if (!hardwareTransmit(",", 1))
        {
            return false;
        }
    }

    if (mode == Udp)
    {
        if (!hardwareTransmit("\"UDP\"", 5))
        {
            return false;
        }
    }
    else
    {
        if (!hardwareTransmit("\"TCP\"", 5))
        {
            return false;
        }
    }

    if (!hardwareTransmit(",", 1))
    {
        return false;
    }

    if (!hardwareTransmit(ip, std::strlen(ip)))
    {
        return false;
    }

    if (!hardwareTransmit(",", 1))
    {
        return false;
    }
    char temp[8] = "";
    HelpFunction::uint32ToString(port, temp);
    if (!hardwareTransmit(temp, std::strlen(temp)))
    {
        return false;
    }

    if (!hardwareTransmit("\n", 1))
    {
        return false;
    }

    return responseOk("OK");
}

bool Esp8266::sendDataPrepare(uint16_t length, uint8_t id)
{
    if (!hardwareTransmit("AT+CIPSEND=", 11))
    {
        return false;
    }

    if (mMulitConnectHasOpen)
    {
        if (!hardwareTransmit(&('0' + id), 1))
        {
            return false;
        }
    }

    char temp[8];
    HelpFunction::uint32ToString(length, temp);

    if (!hardwareTransmit(temp, std::strlen(temp)))
    {
        return false;
    }

    if (!hardwareTransmit("\n", 1))
    {
        return false;
    }

    return responseOk(">");
}
void Esp8266::sendData(const uint8_t *data, uint8_t length)
{
    if (!hardwareTransmit(data, length))
    {
        return false;
    }
    //穿透

    //
}
bool Esp8266::sendDataEnd()
{
    return responseOk("SEND OK");
}

bool Esp8266::sendCommand(const char *command)
{
    if (!hardwareTransmit("AT+", 3))
    {
        return false;
    }

    if (!hardwareTransmit(command, std::strlen(command)))
    {
        return false;
    }

    if (!hardwareTransmit("\n", 1))
    {
        return false;
    }
    return true;
}
bool Esp8266::responseOk(const char *str)
{
    uint8_t response[16] = {};
    hardwareReceive(data, 16);

    for (uint8_t i = 0; i < 16 && data[i] != '\0' && *(str + i) != '\0'; ++i)
    {
        if (data[i] != *(str + i))
        {
            return false;
        }
    }
    return true;
}