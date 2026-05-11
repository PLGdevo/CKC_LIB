#ifndef ModbusMaster_h
#define ModbusMaster_h

#define __MODBUSMASTER_DEBUG__ (0)
#define __MODBUSMASTER_DEBUG_PIN_A__ 4
#define __MODBUSMASTER_DEBUG_PIN_B__ 5

#include "Arduino.h"

static uint16_t crc16_update(uint16_t crc, uint8_t a)
{
  int i;

  crc ^= a;
  for (i = 0; i < 8; ++i)
  {
    if (crc & 1)
      crc = (crc >> 1) ^ 0xA001;
    else
      crc = (crc >> 1);
  }

  return crc;
}

static inline uint16_t lowWord(uint32_t ww)
{
  return (uint16_t)((ww) & 0xFFFF);
}

static inline uint16_t highWord(uint32_t ww)
{
  return (uint16_t)((ww) >> 16);
}

class ModbusMaster
{
public:
  ModbusMaster();

  void begin(uint8_t, Stream &serial);
  void idle(void (*)());
  void preTransmission(void (*)());
  void postTransmission(void (*)());

  static const uint8_t IllegalFunction = 0x01;

  static const uint8_t IllegalDataAddress = 0x02;

  static const uint8_t IllegalDataValue = 0x03;
  
  static const uint8_t SlaveDeviceFailure = 0x04;
  
  static const uint8_t Success = 0x00;

  static const uint8_t InvalidSlaveID = 0xE0;
  
  static const uint8_t InvalidFunction = 0xE1;

  static const uint8_t ResponseTimedOut = 0xE2;

  static const uint8_t InvalidCRC = 0xE3;

  uint16_t getResponseBuffer(uint8_t);
  void clearResponseBuffer();
  uint8_t setTransmitBuffer(uint8_t, uint16_t);
  void clearTransmitBuffer();

  void beginTransmission(uint16_t);
  uint8_t requestFrom(uint16_t, uint16_t);
  void sendBit(bool);
  void send(uint8_t);
  void send(uint16_t);
  void send(uint32_t);
  uint8_t available(void);
  uint16_t receive(void);

  uint8_t readCoils(uint16_t, uint16_t);
  uint8_t readDiscreteInputs(uint16_t, uint16_t);
  uint8_t readHoldingRegisters(uint16_t, uint16_t);
  uint8_t readInputRegisters(uint16_t, uint8_t);
  uint8_t writeSingleCoil(uint16_t, uint8_t);
  uint8_t writeSingleRegister(uint16_t, uint16_t);
  uint8_t writeMultipleCoils(uint16_t, uint16_t);
  uint8_t writeMultipleCoils();
  uint8_t writeMultipleRegisters(uint16_t, uint16_t);
  uint8_t writeMultipleRegisters();
  uint8_t maskWriteRegister(uint16_t, uint16_t, uint16_t);
  uint8_t readWriteMultipleRegisters(uint16_t, uint16_t, uint16_t, uint16_t);
  uint8_t readWriteMultipleRegisters(uint16_t, uint16_t);

private:
  Stream *_serial;                            
  uint8_t _u8MBSlave;                         ///< Modbus slave (1..255) initialized in begin()
  static const uint8_t MaxBufferSize = 64;    ///< size of response/transmit buffers
  uint16_t _u16ReadAddress;                   ///< slave register from which to read
  uint16_t _u16ReadQty;                       ///< quantity of words to read
  uint16_t _u16ResponseBuffer[MaxBufferSize]; ///< buffer to store Modbus slave response; read via GetResponseBuffer()
  uint16_t _u16WriteAddress;                  ///< slave register to which to write
  uint16_t _u16WriteQty;                      ///< quantity of words to write
  uint16_t _u16TransmitBuffer[MaxBufferSize]; ///< buffer containing data to transmit to Modbus slave; set via SetTransmitBuffer()
  uint16_t *txBuffer;                         // from Wire.h -- need to clean this up Rx
  uint8_t _u8TransmitBufferIndex;
  uint16_t u16TransmitBufferLength;
  uint16_t *rxBuffer; // from Wire.h -- need to clean this up Rx
  uint8_t _u8ResponseBufferIndex;
  uint8_t _u8ResponseBufferLength;

  // Modbus function codes for bit access
  static const uint8_t ReadCoils = 0x01;          ///< Modbus function 0x01 Read Coils
  static const uint8_t ReadDiscreteInputs = 0x02; ///< Modbus function 0x02 Read Discrete Inputs
  static const uint8_t WriteSingleCoil = 0x05;    ///< Modbus function 0x05 Write Single Coil
  static const uint8_t WriteMultipleCoils = 0x0F; ///< Modbus function 0x0F Write Multiple Coils

  // Modbus function codes for 16 bit access
  static const uint8_t ReadHoldingRegisters = 0x03;       ///< Modbus function 0x03 Read Holding Registers
  static const uint8_t ReadInputRegisters = 0x04;         ///< Modbus function 0x04 Read Input Registers
  static const uint8_t WriteSingleRegister = 0x06;        ///< Modbus function 0x06 Write Single Register
  static const uint8_t WriteMultipleRegisters = 0x10;     ///< Modbus function 0x10 Write Multiple Registers
  static const uint8_t MaskWriteRegister = 0x16;          ///< Modbus function 0x16 Mask Write Register
  static const uint8_t ReadWriteMultipleRegisters = 0x17; ///< Modbus function 0x17 Read Write Multiple Registers
  
  static const uint16_t ResponseTimeout = 2000; ///< Modbus timeout [milliseconds]
 
  uint8_t ModbusMasterTransaction(uint8_t u8MBFunction);
  
  void (*_idle)();
  
  void (*_preTransmission)();
  
  void (*_postTransmission)();
};

ModbusMaster::ModbusMaster(void)
{
  _idle = 0;
  _preTransmission = 0;
  _postTransmission = 0;
}

void ModbusMaster::begin(uint8_t slave, Stream &serial)
{
//  txBuffer = (uint16_t*) calloc(ku8MaxBufferSize, sizeof(uint16_t));
  _u8MBSlave = slave;
  _serial = &serial;
  _u8TransmitBufferIndex = 0;
  u16TransmitBufferLength = 0;
  
#if __MODBUSMASTER_DEBUG__
  pinMode(__MODBUSMASTER_DEBUG_PIN_A__, OUTPUT);
  pinMode(__MODBUSMASTER_DEBUG_PIN_B__, OUTPUT);
#endif
}


void ModbusMaster::beginTransmission(uint16_t u16Address)
{
  _u16WriteAddress = u16Address;
  _u8TransmitBufferIndex = 0;
  u16TransmitBufferLength = 0;
}

// eliminate this function in favor of using existing MB request functions
// uint8_t ModbusMaster::requestFrom(uint16_t address, uint16_t quantity)
// {
//   uint8_t read;
//   // clamp to buffer length
//   if (quantity > MaxBufferSize)
//   {
//     quantity = MaxBufferSize;
//   }
//   // set rx buffer iterator vars
//   _u8ResponseBufferIndex = 0;
//   _u8ResponseBufferLength = read;

//   return read;
// }
uint8_t ModbusMaster::requestFrom(uint16_t address, uint16_t quantity)
{
  // clamp to buffer length
  if (quantity > MaxBufferSize)
  {
    quantity = MaxBufferSize;
  }

  _u16ReadAddress = address;
  _u16ReadQty = quantity;

  // reset buffer index
  _u8ResponseBufferIndex = 0;
  _u8ResponseBufferLength = 0;

  // return số lượng request hợp lệ (ép về uint8_t nếu library cũ)
  return (uint8_t)quantity;
}


void ModbusMaster::sendBit(bool data)
{
  uint8_t txBitIndex = u16TransmitBufferLength % 16;
  if ((u16TransmitBufferLength >> 4) < MaxBufferSize)
  {
    if (0 == txBitIndex)
    {
      _u16TransmitBuffer[_u8TransmitBufferIndex] = 0;
    }
    bitWrite(_u16TransmitBuffer[_u8TransmitBufferIndex], txBitIndex, data);
    u16TransmitBufferLength++;
    _u8TransmitBufferIndex = u16TransmitBufferLength >> 4;
  }
}


void ModbusMaster::send(uint16_t data)
{
  if (_u8TransmitBufferIndex < MaxBufferSize)
  {
    _u16TransmitBuffer[_u8TransmitBufferIndex++] = data;
    u16TransmitBufferLength = _u8TransmitBufferIndex << 4;
  }
}


void ModbusMaster::send(uint32_t data)
{
  send(lowWord(data));
  send(highWord(data));
}


void ModbusMaster::send(uint8_t data)
{
  send(word(data));
}

uint8_t ModbusMaster::available(void)
{
  return _u8ResponseBufferLength - _u8ResponseBufferIndex;
}


uint16_t ModbusMaster::receive(void)
{
  if (_u8ResponseBufferIndex < _u8ResponseBufferLength)
  {
    return _u16ResponseBuffer[_u8ResponseBufferIndex++];
  }
  else
  {
    return 0xFFFF;
  }
}

void ModbusMaster::idle(void (*idle)())
{
  _idle = idle;
}

void ModbusMaster::preTransmission(void (*preTransmission)())
{
  _preTransmission = preTransmission;
}

void ModbusMaster::postTransmission(void (*postTransmission)())
{
  _postTransmission = postTransmission;
}

uint16_t ModbusMaster::getResponseBuffer(uint8_t u8Index)
{
  if (u8Index < MaxBufferSize)
  {
    return _u16ResponseBuffer[u8Index];
  }
  else
  {
    return 0xFFFF;
  }
}

void ModbusMaster::clearResponseBuffer()
{
  uint8_t i;
  
  for (i = 0; i < MaxBufferSize; i++)
  {
    _u16ResponseBuffer[i] = 0;
  }
}

uint8_t ModbusMaster::setTransmitBuffer(uint8_t u8Index, uint16_t u16Value)
{
  if (u8Index < MaxBufferSize)
  {
    _u16TransmitBuffer[u8Index] = u16Value;
    return Success;
  }
  else
  {
    return IllegalDataAddress;
  }
}

void ModbusMaster::clearTransmitBuffer()
{
  uint8_t i;
  
  for (i = 0; i < MaxBufferSize; i++)
  {
    _u16TransmitBuffer[i] = 0;
  }
}



uint8_t ModbusMaster::readCoils(uint16_t u16ReadAddress, uint16_t u16BitQty)
{
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty = u16BitQty;
  return ModbusMasterTransaction(ReadCoils);
}

uint8_t ModbusMaster::readDiscreteInputs(uint16_t u16ReadAddress,
  uint16_t u16BitQty)
{
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty = u16BitQty;
  return ModbusMasterTransaction(ReadDiscreteInputs);
}

uint8_t ModbusMaster::readHoldingRegisters(uint16_t u16ReadAddress,
  uint16_t u16ReadQty)
{
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty = u16ReadQty;
  return ModbusMasterTransaction(ReadHoldingRegisters);
}

uint8_t ModbusMaster::readInputRegisters(uint16_t u16ReadAddress,
  uint8_t u16ReadQty)
{
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty = u16ReadQty;
  return ModbusMasterTransaction(ReadInputRegisters);
}

uint8_t ModbusMaster::writeSingleCoil(uint16_t u16WriteAddress, uint8_t u8State)
{
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = (u8State ? 0xFF00 : 0x0000);
  return ModbusMasterTransaction(WriteSingleCoil);
}

uint8_t ModbusMaster::writeSingleRegister(uint16_t u16WriteAddress,
  uint16_t u16WriteValue)
{
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = 0;
  _u16TransmitBuffer[0] = u16WriteValue;
  return ModbusMasterTransaction(WriteSingleRegister);
}

uint8_t ModbusMaster::writeMultipleCoils(uint16_t u16WriteAddress,
  uint16_t u16BitQty)
{
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = u16BitQty;
  return ModbusMasterTransaction(WriteMultipleCoils);
}
uint8_t ModbusMaster::writeMultipleCoils()
{
  _u16WriteQty = u16TransmitBufferLength;
  return ModbusMasterTransaction(WriteMultipleCoils);
}

uint8_t ModbusMaster::writeMultipleRegisters(uint16_t u16WriteAddress,
  uint16_t u16WriteQty)
{
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = u16WriteQty;
  return ModbusMasterTransaction(WriteMultipleRegisters);
}

// new version based on Wire.h
uint8_t ModbusMaster::writeMultipleRegisters()
{
  _u16WriteQty = _u8TransmitBufferIndex;
  return ModbusMasterTransaction(WriteMultipleRegisters);
}

uint8_t ModbusMaster::maskWriteRegister(uint16_t u16WriteAddress,
  uint16_t u16AndMask, uint16_t u16OrMask)
{
  _u16WriteAddress = u16WriteAddress;
  _u16TransmitBuffer[0] = u16AndMask;
  _u16TransmitBuffer[1] = u16OrMask;
  return ModbusMasterTransaction(MaskWriteRegister);
}

uint8_t ModbusMaster::readWriteMultipleRegisters(uint16_t u16ReadAddress,
  uint16_t u16ReadQty, uint16_t u16WriteAddress, uint16_t u16WriteQty)
{
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty = u16ReadQty;
  _u16WriteAddress = u16WriteAddress;
  _u16WriteQty = u16WriteQty;
  return ModbusMasterTransaction(ReadWriteMultipleRegisters);
}
uint8_t ModbusMaster::readWriteMultipleRegisters(uint16_t u16ReadAddress,
  uint16_t u16ReadQty)
{
  _u16ReadAddress = u16ReadAddress;
  _u16ReadQty = u16ReadQty;
  _u16WriteQty = _u8TransmitBufferIndex;
  return ModbusMasterTransaction(ReadWriteMultipleRegisters);
}

/* _____PRIVATE FUNCTIONS____________________________________________________ */

uint8_t ModbusMaster::ModbusMasterTransaction(uint8_t u8MBFunction)
{
  uint8_t u8ModbusADU[256];
  uint8_t u8ModbusADUSize = 0;
  uint8_t i, u8Qty;
  uint16_t u16CRC;
  uint32_t u32StartTime;
  uint8_t u8BytesLeft = 8;
  uint8_t u8MBStatus = Success;
  
  // assemble Modbus Request Application Data Unit
  u8ModbusADU[u8ModbusADUSize++] = _u8MBSlave;
  u8ModbusADU[u8ModbusADUSize++] = u8MBFunction;
  
  switch(u8MBFunction)
  {
    case ReadCoils:
    case ReadDiscreteInputs:
    case ReadInputRegisters:
    case ReadHoldingRegisters:
    case ReadWriteMultipleRegisters:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16ReadAddress);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16ReadAddress);
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16ReadQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16ReadQty);
      break;
  }
  
  switch(u8MBFunction)
  {
    case WriteSingleCoil:
    case MaskWriteRegister:
    case WriteMultipleCoils:
    case WriteSingleRegister:
    case WriteMultipleRegisters:
    case ReadWriteMultipleRegisters:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteAddress);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteAddress);
      break;
  }
  
  switch(u8MBFunction)
  {
    case WriteSingleCoil:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
      break;
      
    case WriteSingleRegister:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[0]);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[0]);
      break;
      
    case WriteMultipleCoils:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
      u8Qty = (_u16WriteQty % 8) ? ((_u16WriteQty >> 3) + 1) : (_u16WriteQty >> 3);
      u8ModbusADU[u8ModbusADUSize++] = u8Qty;
      for (i = 0; i < u8Qty; i++)
      {
        switch(i % 2)
        {
          case 0: // i is even
            u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[i >> 1]);
            break;
            
          case 1: // i is odd
            u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[i >> 1]);
            break;
        }
      }
      break;
      
    case WriteMultipleRegisters:
    case ReadWriteMultipleRegisters:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16WriteQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16WriteQty << 1);
      
      for (i = 0; i < lowByte(_u16WriteQty); i++)
      {
        u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[i]);
        u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[i]);
      }
      break;
      
    case MaskWriteRegister:
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[0]);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[0]);
      u8ModbusADU[u8ModbusADUSize++] = highByte(_u16TransmitBuffer[1]);
      u8ModbusADU[u8ModbusADUSize++] = lowByte(_u16TransmitBuffer[1]);
      break;
  }
  
  // append CRC
  u16CRC = 0xFFFF;
  for (i = 0; i < u8ModbusADUSize; i++)
  {
    u16CRC = crc16_update(u16CRC, u8ModbusADU[i]);
  }
  u8ModbusADU[u8ModbusADUSize++] = lowByte(u16CRC);
  u8ModbusADU[u8ModbusADUSize++] = highByte(u16CRC);
  u8ModbusADU[u8ModbusADUSize] = 0;

  // flush receive buffer before transmitting request
  while (_serial->read() != -1);

  // transmit request
  if (_preTransmission)
  {
    _preTransmission();
  }
  for (i = 0; i < u8ModbusADUSize; i++)
  {
    _serial->write(u8ModbusADU[i]);
  }
  
  u8ModbusADUSize = 0;
  _serial->flush();    // flush transmit buffer
  if (_postTransmission)
  {
    _postTransmission();
  }
  
  // loop until we run out of time or bytes, or an error occurs
  u32StartTime = millis();
  while (u8BytesLeft && !u8MBStatus)
  {
    if (_serial->available())
    {
#if __MODBUSMASTER_DEBUG__
      digitalWrite(__MODBUSMASTER_DEBUG_PIN_A__, true);
#endif
      u8ModbusADU[u8ModbusADUSize++] = _serial->read();
      u8BytesLeft--;
#if __MODBUSMASTER_DEBUG__
      digitalWrite(__MODBUSMASTER_DEBUG_PIN_A__, false);
#endif
    }
    else
    {
#if __MODBUSMASTER_DEBUG__
      digitalWrite(__MODBUSMASTER_DEBUG_PIN_B__, true);
#endif
      if (_idle)
      {
        _idle();
      }
#if __MODBUSMASTER_DEBUG__
      digitalWrite(__MODBUSMASTER_DEBUG_PIN_B__, false);
#endif
    }
    
    // evaluate slave ID, function code once enough bytes have been read
    if (u8ModbusADUSize == 5)
    {
      // verify response is for correct Modbus slave
      if (u8ModbusADU[0] != _u8MBSlave)
      {
        u8MBStatus = InvalidSlaveID;
        break;
      }
      
      // verify response is for correct Modbus function code (mask exception bit 7)
      if ((u8ModbusADU[1] & 0x7F) != u8MBFunction)
      {
        u8MBStatus = InvalidFunction;
        break;
      }
      
      // check whether Modbus exception occurred; return Modbus Exception Code
      if (bitRead(u8ModbusADU[1], 7))
      {
        u8MBStatus = u8ModbusADU[2];
        break;
      }
      
      // evaluate returned Modbus function code
      switch(u8ModbusADU[1])
      {
        case ReadCoils:
        case ReadDiscreteInputs:
        case ReadInputRegisters:
        case ReadHoldingRegisters:
        case ReadWriteMultipleRegisters:
          u8BytesLeft = u8ModbusADU[2];
          break;
          
        case WriteSingleCoil:
        case WriteMultipleCoils:
        case WriteSingleRegister:
        case WriteMultipleRegisters:
          u8BytesLeft = 3;
          break;
          
        case MaskWriteRegister:
          u8BytesLeft = 5;
          break;
      }
    }
    if ((millis() - u32StartTime) > ResponseTimeout)
    {
      u8MBStatus = ResponseTimedOut;
    }
  }
  
  // verify response is large enough to inspect further
  if (!u8MBStatus && u8ModbusADUSize >= 5)
  {
    // calculate CRC
    u16CRC = 0xFFFF;
    for (i = 0; i < (u8ModbusADUSize - 2); i++)
    {
      u16CRC = crc16_update(u16CRC, u8ModbusADU[i]);
    }
    
    // verify CRC
    if (!u8MBStatus && (lowByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 2] ||
      highByte(u16CRC) != u8ModbusADU[u8ModbusADUSize - 1]))
    {
      u8MBStatus = InvalidCRC;
    }
  }

  // disassemble ADU into words
  if (!u8MBStatus)
  {
    // evaluate returned Modbus function code
    switch(u8ModbusADU[1])
    {
      case ReadCoils:
      case ReadDiscreteInputs:
        // load bytes into word; response bytes are ordered L, H, L, H, ...
        for (i = 0; i < (u8ModbusADU[2] >> 1); i++)
        {
          if (i < MaxBufferSize)
          {
            _u16ResponseBuffer[i] = word(u8ModbusADU[2 * i + 4], u8ModbusADU[2 * i + 3]);
          }
          
          _u8ResponseBufferLength = i;
        }
        
        // in the event of an odd number of bytes, load last byte into zero-padded word
        if (u8ModbusADU[2] % 2)
        {
          if (i < MaxBufferSize)
          {
            _u16ResponseBuffer[i] = word(0, u8ModbusADU[2 * i + 3]);
          }
          
          _u8ResponseBufferLength = i + 1;
        }
        break;
        
      case ReadInputRegisters:
      case ReadHoldingRegisters:
      case ReadWriteMultipleRegisters:
        // load bytes into word; response bytes are ordered H, L, H, L, ...
        for (i = 0; i < (u8ModbusADU[2] >> 1); i++)
        {
          if (i < MaxBufferSize)
          {
            _u16ResponseBuffer[i] = word(u8ModbusADU[2 * i + 3], u8ModbusADU[2 * i + 4]);
          }
          
          _u8ResponseBufferLength = i;
        }
        break;
    }
  }
  
  _u8TransmitBufferIndex = 0;
  u16TransmitBufferLength = 0;
  _u8ResponseBufferIndex = 0;
  return u8MBStatus;
}
#endif
