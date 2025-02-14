#include "REG_SDM120.h"
#include <ModbusMaster.h>
//----------tes
ModbusMaster node;
//01 04 00 00 00 02 71 3F // Test 30001
//------------------------------------------------
// Convent 32bit to float
//------------------------------------------------
float HexTofloat(uint32_t x) {
  return (*(float*)&x);
}

uint32_t FloatTohex(float x) {
  return (*(uint32_t*)&x);
}
//------------------------------------------------

float Read_Meter_float(char addr , uint16_t  REG) {
  float i = 0;
  uint8_t j, result;
  uint16_t data[2];
  uint32_t value = 0;
  node.begin(Serial);
  result = node.readInputRegisters (REG, 2); ///< Modbus function 0x04 Read Input Registers
  delay(500);
  if (result == node.ku8MBSuccess) {
    for (j = 0; j < 2; j++)
    {
      data[j] = node.getResponseBuffer(j);
    }
    value = data[0];
    value = value << 16;
    value = value + data[1];
    i = HexTofloat(value);
    //Serial.println("Connec modbus Ok.");
    return i;
  } else {
    Serial.print("Connec modbus fail. REG >>> "); Serial.println(REG, HEX); // Debug
    delay(1000); 
    return 0;
  }
}

void GET_METER() {     // Update read all data
  delay(1000);                              // เคลียบัสว่าง 
    for (char i = 0; i < Total_of_Reg ; i++){
      DATA_METER [i] = Read_Meter_float(ID_meter, Reg_addr[i]);//แสกนหลายตัวตามค่า ID_METER_ALL=X
    } 
}

//**************************************************************************************************************
void setup() {
  Serial.begin(19200);
  Serial.println();
  Serial.println();
  Serial.println(F("****************RS485 RTU SDM120*******************"));
}

void loop() {
  //float x = Read_Meter_float(ID_meter,Reg_Volt);
  GET_METER();
  Serial.println();
  Serial.print("Voltage = "); Serial.print(DATA_METER[0]);Serial.println(" VAC");
  Serial.print("Current = "); Serial.print(DATA_METER[1]);Serial.println(" Amps");
  Serial.print("Active Power= "); Serial.print(DATA_METER[2]);Serial.println(" Watts");
  Serial.print("Power Factor = "); Serial.println(DATA_METER[3]);
  Serial.print("Frequency = "); Serial.print(DATA_METER[4]);Serial.println(" Hz");
  Serial.print("Total Active Energy = "); Serial.print(DATA_METER[5]);Serial.println(" kWh");
  delay(5000);
}
