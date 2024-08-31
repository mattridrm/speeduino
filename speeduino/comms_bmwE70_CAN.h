#ifndef COMMS_BMWE70_CAN_H
#define COMMS_BMWE70_CAN_H

#if defined(NATIVE_CAN_AVAILABLE)

int CAN_read1(void);
void initBmwE70CAN(void);
void readBmwE70Can(void);

extern struct bmwE70Can_t bmwE70CanValues;
extern struct bmwE70_t bmwE70;

struct bmwE70_t {
/***
* INMOT	0x1881	unsigned int	-	rpm	-	0.5	-	0	Engine speed
***/
  struct RPM {
    uint16_t request = 0x1881;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    /* Return Can 2 byte response in Big endian format scaled to the BMW documentation*/
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.5F; };
  } rpm;


/***
* IMEIA	0x0500	unsigned int	-	mg/hub	-	0.003052	-	-100	Injection quantity - current value
***/
  struct INJECTION_Q {
    uint16_t request = 0x0500;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    /* Return Can 2 byte response in Big endian format scaled to the BMW documentation*/
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.003052F - 100; };
  } injection_q;

/***
* 0x083C	unsigned int	-	mg/Hub	-	0.003891	-	0	Desired quantity main injection
***/
  struct INJECTION_SP {
    uint16_t request = 0x083C;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    /* Return Can 2 byte response in Big endian format scaled to the BMW documentation*/
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.003891F; };
  } injection_sp;

/***
* ILMKG	0x0708	unsigned int	-	kg/h	-	0.1	-	0	Whole HFM-air mass flow
***/
  struct AIR_MASS {
    uint16_t request = 0x0708;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    /* Return Can 2 byte response in Big endian format scaled to the BMW documentation*/
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.1F; };
  } air_mass;

/***
* ILMMG	0x0709	unsigned int	-	mg/hub	-	0.024414	-	0	Air mass per cylinder
***/
  struct AIR_MASS_PER_C {
    uint16_t request = 0x0709;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    /* Return Can 2 byte response in Big endian format scaled to the BMW documentation*/
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.024414F; };
  } air_mass_per_c;

/***
* SMOMO	0x07D0	unsigned int	-	Nm	-	0.114443	-	-2500	News back gerechnetes inner engine torque
***/
  struct TQ_T {
    uint16_t request = 0x07D0;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    /* Return Can 2 byte response in Big endian format scaled to the BMW documentation*/
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.114443F - 2500; };
  } tq_t;

/***
* IMOAK	0x07D1	unsigned int	-	Nm	-	0.114443	-	-2500	current engine torque
***/
  struct TQ {
    uint16_t request = 0x07D1;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    /* Return Can 2 byte response in Big endian format scaled to the BMW documentation*/
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.114443F - 2500; };
  } tq;

/***
* RailP_pCurr_mp	0x0674	unsigned int	-	bar	-	0.045777	-	0	Rail pressure - Actual
***/
  struct FUEL_P {
    uint16_t request = 0x0674;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.045777F; };
  } fuel_p;

/***
* ITAVO	0x041E	unsigned int	-	degC	-	0.031281	-	-50	Exhaust gas temperature before catalytic converter - corrected value
***/
  struct EGT {
    uint16_t request = 0x041E;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    /* Return Can 2 byte response in Big endian format scaled to the BMW documentation*/
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.031281F - 50; };
  } egt;

/***
* IFPWG	0x015E	unsigned int	-	%	-	0.001526	-	0	filtered pedal sensor signal
  IUPW1	0x015F	unsigned int	-	mV	-	0.076295	-	0	Pedal sensor signal 1 - raw voltage
  IUPW2	0x0160	unsigned int	-	mV	-	0.076295	-	0	Pedal sensor signal 2 - Voltage raw
  APP_r	0x0162	unsigned int	-	%	-	0.012207	-	0	Pedal sensor position
***/
  struct TPS {
    uint16_t request = 0x0162;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t response = (response_hb << 8) | response_lb ;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.012207F; };
  } tps;

/***
* IPUMG	0x0C1C	unsigned int	-	hPa	-	0.030518	-	0	Ambient pressure - *10 to convert to Kpa
***/
    struct AMBIANT_P {
    uint16_t request = 0x0C1C;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.030518F * 10; };
  } ambiant_p;

/***
* IPLAD	0x076D	unsigned int	-	hPa	-	0.091554	-	0	Boost pressure - Actual - *10 to convert to Kpa
***/
  struct BOOSTP_A {
    uint16_t request = 0x076D;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.091554F * 10; };
  } boostp_a;

/***
* ILMBD1	0x0A2D	unsigned int	-	-	-	0.001	-	0	The measured from LSU lambda value 1
***/
  struct AFR {
    uint16_t request = 0x0A2D;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.001F; };
  } afr;

/***
* ITLAL	0x076F	unsigned int	-	degC	-	0.01	-	-100	Charge air temperature after intercooler
***/
  struct CHARGE_AIR_T {
    uint16_t request = 0x076F;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.01F - 100; };
  } charge_air_t;


/***
* ITANS	0x0772	unsigned int	-	degC	-	0.1	-	-273.14	Air temperature at the HFM-position
***/
  struct AIR_INTAKE_T {
    uint16_t request = 0x0772;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.1F - 273.14F; };
  } air_intake_t;

/***
* ITKUM	0x0547	unsigned int	-	degC	-	0.01	-	-100	Coolant temperature
***/
  struct COOLANT_T {
    uint16_t request = 0x0547;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.01F - 100; };
  } coolant_t;

/***
* ITMOT	0x0AF1	unsigned int	-	degC	-	0.1	-	-273.14	Motor temperature
***/
   struct MOTOR_T {
    uint16_t request = 0x0AF1;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.1F - 273.14F; };
  } motor_t;

/***
* ITOEL	0x0458	unsigned int	-	degC	-	0.01	-	-100	Filtered oil temperature
***/
  struct OIL_T {
    uint16_t request = 0x0458;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.01F - 100; };
  } oil_t;

/***
* IVKMH	0x0B40	unsigned int	-	km/h	-	0.004578	-	0	Vehicle speed
***/
  struct VSS {
    uint16_t request = 0x0B40;
    uint8_t response_hb = 0x00;
    uint8_t response_lb = 0x00;
    uint16_t scaled_response(){ return ((response_hb << 8) | response_lb) * 0.004578F; };
  } vss;

/* ---------------------uint8_t ----------------------------*/

/***
* Gangi	0x09F6	unsigned char	-	-	-	1	-	0	Current gear internally
***/
   struct GEAR {
    uint16_t request = 0x09F6;
    uint8_t response = 0x00;
    uint8_t scaled_response (){ uint8_t scaled_response = response; return scaled_response; };
  } gear;

/***
* IGETT	0x09F8	unsigned char	-	-	-	1	-	0	Current Transmission Type
***/
  struct GEAR_M {
    uint16_t request = 0x09F8;
    uint8_t response = 0x00;
    uint8_t scaled_response (){ uint8_t scaled_response = response; return scaled_response; };
  } gear_m;

/***
* Tra_tOil	0x0EA7	unsigned char	-	degC	-	1	-	-40	Transmission Fluid Temperature
***/
  struct TRANS_T {
    uint16_t request = 0x0EA7;
    uint8_t response = 0x00;
    uint8_t scaled_response (){ uint8_t scaled_response = response - 40; return scaled_response; };
  } trans_t;

};

struct bmwE70Can_t {
    uint16_t rpm;
    uint16_t injection_q;
    uint16_t injection_sp;
    uint16_t air_mass;
    uint16_t air_mass_per_c;
    uint16_t tq_t; 
    uint16_t tq;
    uint16_t fuel_p;
    uint16_t egt;    
    uint16_t tps ;    
    uint16_t ambiant_p;    
    uint16_t boostp_a;   
    uint16_t afr;    
    uint16_t charge_air_t;    
    uint16_t air_intake_t;
    uint16_t coolant_t;    
    uint16_t motor_t;    
    uint16_t oil_t;    
    uint16_t vss;    
    uint8_t gear;
    uint8_t gear_m;    
    uint8_t trans_t;     
};

#endif
#endif // COMMS_CAN_H