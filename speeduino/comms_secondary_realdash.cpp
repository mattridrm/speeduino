#include "globals.h"
#include "comms_secondary.h"
#include "utilities.h"
#include "logger.h"

void serialSendRealDash(unsigned long canFrameId, const byte* rdFrameData)
{  
  static byte realdashHeader[4] = { 0x44, 0x33, 0x22, 0x11 };
  
  secondarySerial.write((const byte*) &realdashHeader, 4);
  secondarySerial.write((const byte*) &canFrameId, 4);
  secondarySerial.write(rdFrameData, 8);

}

void sendRealDashCan(void)
{ 
  static uint8_t realdashContrl = 0; 
  static uint32_t realDashBaseId = 0x3200;
  static byte realdashBuff[8];

  if (realdashContrl < LOG_ENTRY_SIZE)
  {
    
    for (uint8_t x = 0; x < 8; x++)
    { 
      realdashBuff[x] = getTSLogEntry(x + realdashContrl);
    }
  
    serialSendRealDash(realDashBaseId, realdashBuff);

    realDashBaseId++;
    realdashContrl = (realdashContrl + 8);

    if (realdashContrl >= LOG_ENTRY_SIZE)
    {
      realDashBaseId = 0x3200;
      realdashContrl = 0;
    }
  }
}