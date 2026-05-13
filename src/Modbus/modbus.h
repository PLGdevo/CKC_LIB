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

    uint32_t _serialConfig = SERIAL_8N1;

private:
    // ================= CRC16 =================
    uint16_t crc16(const uint8_t *data, size_t len)
    {
        uint16_t crc = 0xFFFF;

        for (size_t pos = 0; pos < len; pos++)
        {
            crc ^= (uint16_t)data[pos];

            for (uint8_t i = 0; i < 8; i++)
            {
                if (crc & 0x0001)
                {
                    crc >>= 1;
                    crc ^= 0xA001;
                }
                else
                {
                    crc >>= 1;
                }
            }
        }

        return crc;
    }

    // ================= RS485 TX =================
    void setTransmit()
    {
        if (_DE != -1)
        {
            digitalWrite(_DE, HIGH);
        }
    }

    // ================= RS485 RX =================
    void setReceive()
    {
        if (_DE != -1)
        {
            digitalWrite(_DE, LOW);
        }
    }

    // ================= CLEAR BUFFER =================
    void clearSerialBuffer()
    {
        while (serial->available())
        {
            serial->read();
        }
    }

    // ================= DEBUG HEX =================
    void printHex(uint8_t *data, int len)
    {
        for (int i = 0; i < len; i++)
        {
            Serial.printf("%02X ", data[i]);
        }

        Serial.println();
    }

public:
    // ================= BEGIN =================
    void beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx, uint32_t config = SERIAL_8N1);

    // ================= BEGIN RS485 =================
    void beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx, int de_pin, uint32_t config = SERIAL_8N1);

    // ================= TIMEOUT =================
    void setTimeout(uint32_t timeout);

    // ================= READ BYTES =================
    int readBytes(uint8_t *buf, size_t len);

    // ================= SEND BYTES =================
    void sendBytes(uint8_t *data, size_t len);

    // ================= BUILD FRAME =================
    void buildFrame(uint8_t slave, uint8_t function, uint16_t reg, uint16_t num, uint8_t *frame);

    // ================= CHECK RESPONSE =================
    int checkResponse(uint8_t slave, uint8_t function, uint16_t num, uint8_t *response, int len);

    // =========================================================
    // READ HOLDING REGISTER (0x03)
    // =========================================================
    int readHoldingRegister(uint8_t slave, uint16_t reg, uint16_t num, uint8_t *response);
    int readHoldingRegisterValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values);

    // =========================================================
    // READ INPUT REGISTER (0x04)
    // =========================================================
    int readInputRegisters(uint8_t slave, uint16_t reg, uint16_t num, uint8_t *response);
    int readInputRegistersValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values);

    // =========================================================
    // WRITE SINGLE REGISTER (0x06)
    // =========================================================
    bool writeSingleRegister(uint8_t slave, uint16_t reg, uint16_t value);
};

// ================= BEGIN =================
template <class Modbus>
void CKC_ModbusESP32<Modbus>::beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx, uint32_t config)
{
    serial = &ser;

    _baudrate = baud;
    _RX = rx;
    _TX = tx;
    _DE = -1;

    _serialConfig = config;

    serial->begin(_baudrate,
                  _serialConfig,
                  _RX,
                  _TX);
}

// ================= BEGIN RS485 =================
template <class Modbus>
void CKC_ModbusESP32<Modbus>::beginModbus(HardwareSerial &ser, uint32_t baud, int rx, int tx, int de_pin, uint32_t config)
{
    serial = &ser;

    _baudrate = baud;
    _RX = rx;
    _TX = tx;

    _DE = de_pin;

    _serialConfig = config;

    pinMode(_DE, OUTPUT);

    digitalWrite(_DE, LOW);

    serial->begin(_baudrate,
                  _serialConfig,
                  _RX,
                  _TX);
}

// ================= TIMEOUT =================
template <class Modbus>
void CKC_ModbusESP32<Modbus>::setTimeout(uint32_t timeout)
{
    _timeout = timeout;
}

// ================= READ BYTES =================
template <class Modbus>
int CKC_ModbusESP32<Modbus>::readBytes(uint8_t *buf, size_t len)
{
    if (serial == NULL)
        return 0;

    size_t index = 0;

    uint32_t startTime = millis();
    uint32_t lastByteTime = millis();

    while ((millis() - startTime) < _timeout)
    {
        while (serial->available())
        {
            buf[index++] = serial->read();

            lastByteTime = millis();

            // nhận đủ packet
            if (index >= len)
            {
                return index;
            }
        }

        // nếu đã nhận dữ liệu và im lặng > 20ms
        if (index > 0 && (millis() - lastByteTime) > 20)
        {
            return index;
        }

        delay(1);
    }

    return index;
}

// ================= SEND BYTES =================
template <class Modbus>
void CKC_ModbusESP32<Modbus>::sendBytes(uint8_t *data, size_t len)
{
    clearSerialBuffer();

    setTransmit();

    delayMicroseconds(100);

    serial->write(data, len);

    serial->flush();

    delayMicroseconds(100);

    setReceive();
}

// ================= BUILD FRAME =================
template <class Modbus>
void CKC_ModbusESP32<Modbus>::buildFrame(uint8_t slave, uint8_t function, uint16_t reg, uint16_t num, uint8_t *frame)
{
    frame[0] = slave;
    frame[1] = function;

    frame[2] = reg >> 8;
    frame[3] = reg & 0xFF;

    frame[4] = num >> 8;
    frame[5] = num & 0xFF;

    uint16_t crc = crc16(frame, 6);

    frame[6] = crc & 0xFF;
    frame[7] = crc >> 8;
}

// ================= CHECK RESPONSE =================
template <class Modbus>
int CKC_ModbusESP32<Modbus>::checkResponse(uint8_t slave, uint8_t function, uint16_t num, uint8_t *response, int len)
{
    int expected = 5 + (num * 2);

    if (len < expected)
    {
        MODBUS_LOG_ERROR("MODBUS", "ERROR LEN: %d / %d",
                         len,
                         expected);

        return -1;
    }

    // DEBUG RX
    MODBUS_CHECK_DEBUG("MODBUS", "RX: ");
    printHex(response, len);

    // SLAVE
    if (response[0] != slave)
    {
        MODBUS_LOG_ERROR("MODBUS", "ERROR: WRONG SLAVE");

        return -2;
    }

    // EXCEPTION
    if (response[1] == (function | 0x80))
    {
        MODBUS_LOG_ERROR("MODBUS", "MODBUS EXCEPTION: %02X",
                         response[2]);

        return -3;
    }

    // FUNCTION
    if (response[1] != function)
    {
        MODBUS_LOG_ERROR("MODBUS", "ERROR: WRONG FUNCTION");

        return -4;
    }

    // BYTE COUNT
    if (response[2] != (num * 2))
    {
        MODBUS_LOG_ERROR("MODBUS", "ERROR: BYTE COUNT");

        return -5;
    }

    // CRC
    uint16_t crcCalc =
        crc16(response, expected - 2);

    uint16_t crcRecv =
        response[expected - 2] |
        (response[expected - 1] << 8);

    if (crcCalc != crcRecv)
    {
        MODBUS_LOG_WARN("MODBUS", "CRC FAIL: %04X / %04X",
                        crcCalc,
                        crcRecv);

        return -6;
    }

    return 1;
}

// =========================================================
// READ HOLDING REGISTER (0x03)
// =========================================================
template <class Modbus>
int CKC_ModbusESP32<Modbus>::readHoldingRegister(uint8_t slave, uint16_t reg, uint16_t num, uint8_t *response)
{
    uint8_t frame[8];

    buildFrame(slave,
               0x03,
               reg,
               num,
               frame);

    MODBUS_CHECK_DEBUG("MODBUS", "TX: ");
    printHex(frame, 8);

    sendBytes(frame, 8);

    int expected = 5 + (num * 2);

    int len = readBytes(response,
                        expected);

    return checkResponse(slave, 0x03, num, response, len);
}

template <class Modbus>
int CKC_ModbusESP32<Modbus>::readHoldingRegisterValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values)
{
    uint8_t buffer[256];

    int rs = readHoldingRegister(slave,
                                 reg,
                                 num,
                                 buffer);

    if (rs < 0)
        return rs;

    for (int i = 0; i < num; i++)
    {
        values[i] =
            ((uint16_t)buffer[3 + (i * 2)] << 8) |
            buffer[4 + (i * 2)];

        MODBUS_LOG_DEBUG("MODBUS", "REG[%d] = %u",
                         reg + i,
                         values[i]);
    }
    delay(10);
    return num;
}

// =========================================================
// READ INPUT REGISTER (0x04)
// =========================================================
template <class Modbus>
int CKC_ModbusESP32<Modbus>::readInputRegisters(uint8_t slave, uint16_t reg, uint16_t num, uint8_t *response)
{
    uint8_t frame[8];

    buildFrame(slave,
               0x04,
               reg,
               num,
               frame);

    MODBUS_CHECK_DEBUG("MODBUS", "TX: ");
    printHex(frame, 8);

    sendBytes(frame, 8);

    int expected = 5 + (num * 2);

    int len = readBytes(response,
                        expected);

    return checkResponse(slave,
                         0x04,
                         num,
                         response,
                         len);
}

template <class Modbus>
int CKC_ModbusESP32<Modbus>::readInputRegistersValue(uint8_t slave, uint16_t reg, uint16_t num, uint16_t *values)
{
    uint8_t buffer[256];

    int rs = readInputRegisters(slave, reg, num, buffer);

    if (rs < 0)
        return rs;

    for (int i = 0; i < num; i++)
    {
        values[i] =
            ((uint16_t)buffer[3 + (i * 2)] << 8) |
            buffer[4 + (i * 2)];

        MODBUS_LOG_DEBUG("MODBUS", "INPUT[%d] = %u",
                         reg + i,
                         values[i]);
    }
    delay(10);

    return num;
}

// =========================================================
// WRITE SINGLE REGISTER (0x06)
// =========================================================
template <class Modbus>
bool CKC_ModbusESP32<Modbus>::writeSingleRegister(uint8_t slave, uint16_t reg, uint16_t value)
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

    MODBUS_CHECK_DEBUG("MODBUS", "TX: ");
    printHex(frame, 8);

    sendBytes(frame, 8);

    uint8_t response[8];

    int len = readBytes(response, 8);

    if (len != 8)
    {
        MODBUS_LOG_ERROR("MODBUS", "WRITE FAIL");

        return false;
    }

    MODBUS_CHECK_DEBUG("MODBUS", "RX: ");
    printHex(response, len);

    // CRC CHECK
    uint16_t crcCalc =
        crc16(response, 6);

    uint16_t crcRecv =
        response[6] |
        (response[7] << 8);

    if (crcCalc != crcRecv)
    {
        MODBUS_LOG_ERROR("MODBUS", "WRITE CRC FAIL");

        return false;
    }
    delay(10);
    return true;
}

CKC_ModbusESP32<int> CKCModbus;

#endif
