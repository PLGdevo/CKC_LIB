#ifndef INC_CKC_MODBUSESP32_HPP_
#define INC_CKC_MODBUSESP32_HPP_

#include <Arduino.h>

template <class Modbus>
class CKC_ModbusESP32
{
private:
    HardwareSerial *serial = NULL;
    uint32_t _baudrate = 9600;
    int _RX = -1;
    int _TX = -1;
    int _DE = -1;
    uint32_t _timeout = 1000;

    uint16_t crc16(uint8_t *data, uint8_t len)
    {
        uint16_t crc = 0xFFFF;
        for (int pos = 0; pos < len; pos++)
        {
            crc ^= data[pos];
            for (int i = 0; i < 8; i++)
            {
                if (crc & 1)
                {
                    crc >>= 1;
                    crc ^= 0xA001;
                }
                else
                    crc >>= 1;
            }
        }
        return crc;
    }

    void setTransmit()
    {
        if (_DE != -1)
            digitalWrite(_DE, HIGH);
    }

    void setReceive()
    {
        if (_DE != -1)
            digitalWrite(_DE, LOW);
    }

public:
    void beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx)
    {
        serial = &ser;
        _baudrate = baud;
        _RX = rx;
        _TX = tx;
        _DE = -1;
        serial->begin(_baudrate, SERIAL_8N1, _RX, _TX);
    }

    void beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx, int de_pin)
    {
        serial = &ser;
        _baudrate = baud;
        _RX = rx;
        _TX = tx;
        _DE = de_pin;

        pinMode(_DE, OUTPUT);
        digitalWrite(_DE, LOW); // RX mode

        serial->begin(_baudrate, SERIAL_8N1, _RX, _TX);
    }

    int readBytes(uint8_t *buf, size_t len)
    {
        if (serial == NULL)
            return 0;

        size_t index = 0;
        unsigned long start = millis();

        while ((millis() - start) < _timeout)
        {
            if (serial->available())
            {
                buf[index++] = serial->read();
                if (index >= len)
                    break;
            }
        }
        return index;
    }

    void sendBytes(uint8_t *data, size_t len)
    {
        setTransmit();
        delayMicroseconds(100);

        serial->write(data, len);
        serial->flush();

        delayMicroseconds(100);
        setReceive();
    }

    // ================= READ HOLDING REGISTER =================
    int readHoldingRegister(uint8_t slave, uint16_t reg, uint16_t num, uint8_t *response)
    {
        uint8_t frame[8];

        frame[0] = slave;
        frame[1] = 0x04; // FIX CHÍNH: dùng 0x03
        frame[2] = reg >> 8;
        frame[3] = reg & 0xFF;
        frame[4] = num >> 8;
        frame[5] = num & 0xFF;

        uint16_t crc = crc16(frame, 6);
        frame[6] = crc & 0xFF;
        frame[7] = crc >> 8;

        sendBytes(frame, 8);

        return readBytes(response, 5 + 2 * num);
    }

    int readHoldingRegisterValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values)
    {
        uint8_t buffer[128];

        int len = readHoldingRegister(slave, reg, num, buffer);

        if (len < (5 + 2 * num))
        {
            Serial.println("ERROR: Not enough data");
            return -1;
        }

        // Check function code
        if (buffer[1] != 0x04)
        {
            Serial.println("ERROR: Wrong function response");
            return -2;
        }

        // Check byte count
        if (buffer[2] != num * 2)
        {
            Serial.println("ERROR: Byte count mismatch");
            return -3;
        }

        for (int i = 0; i < num; i++)
        {
            values[i] = (buffer[3 + i * 2] << 8) | buffer[4 + i * 2];
        }

        return num;
    }

    // ================= WRITE SINGLE =================
    bool writeSingleRegister(uint8_t slave, uint16_t reg, uint16_t value)
    {
        uint8_t frame[8];

        frame[0] = slave;
        frame[1] = 0x06;
        frame[2] = reg >> 8;
        frame[3] = reg & 0xFF;
        frame[4] = value >> 8;
        frame[5] = value & 0xFF;

        uint16_t crc = crc16(frame, 6);
        frame[6] = crc & 0xFF;
        frame[7] = crc >> 8;

        sendBytes(frame, 8);

        uint8_t resp[8];
        int len = readBytes(resp, 8);

        if (len < 8)
            return false;

        return true;
    }
};

CKC_ModbusESP32<int> CKCModbus;

#endif