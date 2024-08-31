#include "globals.h"

#if defined(NATIVE_CAN_AVAILABLE)
#include "comms_bmwE70_CAN.h"
#include "utilities.h"

#if HAL_CAN_MODULE_ENABLED
//This activates CAN1 interface on STM32 as an secondary Can to the Can0 interface.
STM32_CAN Can1 (CAN1, ALT_2, RX_SIZE_128, TX_SIZE_64);
/*
These CAN interfaces and pins are available for use, depending on the chip/package:
Default CAN1 pins are PA11 and PA12. Alternative (ALT) pins are PB8 & PB9 and ALT_2 pins are PD0 & PD1.
Default CAN2 pins are PB12 & PB13. Alternative (ALT) pins are PB5 & PB6.
Default CAN3 pins are PA8 & PA15. Alternative (ALT) pins are PB3 & PB4.
*/
#endif

bmwE70_t bmwE70;
bmwE70Can_t bmwE70CanValues;

static CAN_message_t inMsg2;
static CAN_message_t outMsg2;

void sendBmwMessage(void);
void updateE70CanIn(void);
void bmwE70UpdateCanValues(void);

/*
  Array to send messages to Canbus: 0-ACK, 1-Continue, 2-MSG 1,
  3-MSG 2, 4-MSG 3, 5-MSG 4, 6-MSG 5, 7-MSG 6, 8-MSG 7, 9-MSG 8
*/
 const byte bmwMessages[10][8] = {
  { 0x12, 0x30, 0x0F, 0x02, 0x00, 0x00, 0x00, 0x00 },
  
  { 0x12, 0x02, 0x2C, 0x10, 0x00, 0x00, 0x00, 0x00 },
  

  { 0x12, 0x10, 0x2E, 0x2C, 0x10, highByte(bmwE70.rpm.request), lowByte(bmwE70.rpm.request), highByte(bmwE70.injection_sp.request)},
  
  { 0x12, 0x21, lowByte(bmwE70.injection_sp.request), highByte(bmwE70.injection_q.request), lowByte(bmwE70.injection_q.request), 
                highByte(bmwE70.air_mass.request), lowByte(bmwE70.air_mass.request), highByte(bmwE70.air_mass_per_c.request)},
  
  { 0x12, 0x22, lowByte(bmwE70.air_mass_per_c.request), highByte(bmwE70.tq_t.request), lowByte(bmwE70.tq_t.request), 
                highByte(bmwE70.tq.request), lowByte(bmwE70.tq.request), highByte(bmwE70.fuel_p.request)},
  
  { 0x12, 0x23, lowByte(bmwE70.fuel_p.request), highByte(bmwE70.egt.request), lowByte(bmwE70.egt.request), 
                highByte(bmwE70.tps.request), lowByte(bmwE70.tps.request), highByte(bmwE70.ambiant_p.request)},
  
  { 0x12, 0x24, lowByte(bmwE70.ambiant_p.request), highByte(bmwE70.boostp_a.request), lowByte(bmwE70.boostp_a.request), 
                highByte(bmwE70.afr.request), lowByte(bmwE70.afr.request), highByte(bmwE70.charge_air_t.request)},
  
  { 0x12, 0x25, lowByte(bmwE70.charge_air_t.request), highByte(bmwE70.air_intake_t.request), lowByte(bmwE70.air_intake_t.request), 
                highByte(bmwE70.coolant_t.request), lowByte(bmwE70.coolant_t.request), highByte(bmwE70.motor_t.request)},
  
  { 0x12, 0x26, lowByte(bmwE70.motor_t.request), highByte(bmwE70.oil_t.request), lowByte(bmwE70.oil_t.request), 
                highByte(bmwE70.vss.request), lowByte(bmwE70.vss.request), highByte(bmwE70.gear.request)},
  
  { 0x12, 0x27, lowByte(bmwE70.gear.request), highByte(bmwE70.gear_m.request), lowByte(bmwE70.gear_m.request), 
                highByte(bmwE70.trans_t.request), lowByte(bmwE70.trans_t.request), 0xff },
  
};


int CAN_read1()
{
  return Can1.read(inMsg2);
}

/*
  0-ACK, 1-Continue, 2-MSG 1,
  3-MSG 2, 4-MSG 3, 5-MSG 4, 6-MSG 5, 7-MSG 6, 8-MSG 7, 9-MSG 8
*/
void sendBmwMessage(int bmwMessage){
    outMsg2.id = 0x6F1;
    outMsg2.seq = 1; 
    outMsg2.len = 8;
    
    for ( int i = 0; i < 8; i++ ){
      outMsg2.buf[i] = bmwMessages[(byte) bmwMessage][(byte) i];
    }
    Can1.write(outMsg2);
}

/* Setup of Canbus port to E70 diagnostic port. */
void initBmwE70CAN()
{
      Can1.begin();
      Can1.setBaudRate(500000);
      Can1.enableFIFO();
      sendBmwMessage(2);
}

void readBmwE70Can()
{
  switch ( inMsg2.buf[1] ) {  
      case 0x10:       
        bmwE70.rpm.response_hb = inMsg2.buf[5];
        bmwE70.rpm.response_lb = inMsg2.buf[6];
        bmwE70.injection_sp.response_hb = inMsg2.buf[7];
        sendBmwMessage(0);
       break;
      
      case 0x21:
       bmwE70.injection_sp.response_lb = inMsg2.buf[2];
       bmwE70.injection_q.response_hb = inMsg2.buf[3];
       bmwE70.injection_q.response_lb = inMsg2.buf[4];
       bmwE70.air_mass.response_hb = inMsg2.buf[5];
       bmwE70.air_mass.response_lb = inMsg2.buf[6];
       bmwE70.air_mass_per_c.response_hb = inMsg2.buf[7];
       break;

      case 0x22:  
       bmwE70.air_mass_per_c.response_lb = inMsg2.buf[2];
       bmwE70.tq_t.response_hb = inMsg2.buf[3];
       bmwE70.tq_t.response_lb = inMsg2.buf[4];
       bmwE70.tq.response_hb = inMsg2.buf[5];
       bmwE70.tq.response_lb = inMsg2.buf[6];
       bmwE70.fuel_p.response_hb = inMsg2.buf[7];
       break;

      case 0x23:
       bmwE70.fuel_p.response_lb = inMsg2.buf[2];
       bmwE70.egt.response_hb = inMsg2.buf[3];
       bmwE70.egt.response_lb = inMsg2.buf[4];
       bmwE70.tps.response_hb = inMsg2.buf[5];
       bmwE70.tps.response_lb = inMsg2.buf[6];
       bmwE70.ambiant_p.response_hb = inMsg2.buf[7];
       break; 

      case 0x24:
       bmwE70.ambiant_p.response_lb = inMsg2.buf[2];
       bmwE70.boostp_a.response_hb = inMsg2.buf[3];
       bmwE70.boostp_a.response_lb = inMsg2.buf[4];
       bmwE70.afr.response_hb = inMsg2.buf[5];
       bmwE70.afr.response_lb = inMsg2.buf[6];
       bmwE70.charge_air_t.response_hb = inMsg2.buf[7];
       break;

     case 0x25:
      bmwE70.charge_air_t.response_lb = inMsg2.buf[2];
      bmwE70.air_intake_t.response_hb = inMsg2.buf[3];
      bmwE70.air_intake_t.response_lb = inMsg2.buf[4];
      bmwE70.coolant_t.response_hb = inMsg2.buf[5];
      bmwE70.coolant_t.response_lb = inMsg2.buf[6];
      bmwE70.motor_t.response_hb = inMsg2.buf[7];
      break;

     case 0x26:
      bmwE70.motor_t.response_lb = inMsg2.buf[2];
      bmwE70.oil_t.response_hb = inMsg2.buf[3];
      bmwE70.oil_t.response_lb = inMsg2.buf[4];
      bmwE70.vss.response_hb = inMsg2.buf[5];
      bmwE70.vss.response_lb = inMsg2.buf[6];
      bmwE70.gear.response = inMsg2.buf[7];
      break;      

      case 0x27:
       bmwE70.gear_m.response = inMsg2.buf[2];
       bmwE70.trans_t.response = inMsg2.buf[3];
       bmwE70UpdateCanValues();
       sendBmwMessage(1);
       break;

      case 0x30:
        sendBmwMessage(3);
        sendBmwMessage(4);
        sendBmwMessage(5);
        sendBmwMessage(6);
        sendBmwMessage(7);
        sendBmwMessage(8);
        sendBmwMessage(9);                                           
       break;
     
     default: break;     
  }
}

/* Return Scaled E70 Can Data Valuesfrom can poll. */
void bmwE70UpdateCanValues()
  { 
    bmwE70CanValues.rpm =           bmwE70.rpm.scaled_response();
    bmwE70CanValues.injection_q =   bmwE70.injection_q.scaled_response();
    bmwE70CanValues.injection_sp =  bmwE70.injection_sp.scaled_response();
    bmwE70CanValues.air_mass =      bmwE70.air_mass.scaled_response();
    bmwE70CanValues.air_mass_per_c= bmwE70.air_mass_per_c.scaled_response();
    bmwE70CanValues.tq_t  =         bmwE70.tq_t.scaled_response();  
    bmwE70CanValues.tq    =         bmwE70.tq.scaled_response();
    bmwE70CanValues.fuel_p    =     bmwE70.fuel_p.scaled_response();
    bmwE70CanValues.egt    =        bmwE70.egt.scaled_response();
    bmwE70CanValues.tps    =        bmwE70.tps.scaled_response();
    bmwE70CanValues.ambiant_p  =    bmwE70.ambiant_p.scaled_response();
    bmwE70CanValues.boostp_a   =    bmwE70.boostp_a.scaled_response();
    bmwE70CanValues.afr    =        bmwE70.afr.scaled_response();
    bmwE70CanValues.charge_air_t =  bmwE70.charge_air_t.scaled_response();
    bmwE70CanValues.air_intake_t =  bmwE70.air_intake_t.scaled_response();
    bmwE70CanValues.coolant_t    =  bmwE70.coolant_t.scaled_response();
    bmwE70CanValues.motor_t    =    bmwE70.motor_t.scaled_response();
    bmwE70CanValues.oil_t    =      bmwE70.oil_t.scaled_response();
    bmwE70CanValues.vss    =        bmwE70.vss.scaled_response();
    bmwE70CanValues.gear   =        bmwE70.gear.scaled_response();
    bmwE70CanValues.gear_m  =       bmwE70.gear_m.scaled_response();
    bmwE70CanValues.trans_t    =    bmwE70.trans_t.scaled_response();       
  
    currentStatus.canin[0] =  bmwE70CanValues.egt * 1.8F + 32;   // EGT Temp in F
    currentStatus.canin[1] =  bmwE70CanValues.fuel_p * 14.504F + 0.5F;  // Fuel Pressure in PSI
    currentStatus.canin[2] =  bmwE70CanValues.afr;  // Converted lamda
    currentStatus.canin[3] =  bmwE70CanValues.injection_sp; // mg per hub
    currentStatus.canin[4] =  bmwE70CanValues.injection_q; // mg per hub
    currentStatus.canin[5] =  bmwE70CanValues.air_mass;   // Kg per hour
    currentStatus.canin[6] =  bmwE70CanValues.air_mass_per_c;   // Kg per hour
  }
#endif