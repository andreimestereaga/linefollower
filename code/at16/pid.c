#include "pid.h"
#include <avr/eeprom.h>

uint16_t EEMEM ePid_P;
uint16_t EEMEM ePid_D;

uint16_t RAM_ePID_P = 0;
uint16_t RAM_ePID_D = 0;






#define SET_POINT (int16_t)0
#define D_LATCH ((int8_t)20)

volatile int16_t set_point = 0;
volatile int16_t error;
volatile int16_t prv_error;
volatile uint16_t _i = 0;//10;
volatile uint16_t _d = 0;
volatile uint16_t _p = 0;

volatile int32_t temp;
//volatile int16_t integrate = 0;
//volatile int16_t integrate_count = 0;
volatile int16_t derivate;
volatile int16_t output;

volatile int8_t dLatchCounter = 10;

void pid_init()
{
	dLatchCounter = 0;
}

void PID_readFromEepromP_D(){
	while( !eeprom_is_ready	());
	RAM_ePID_P = eeprom_read_word(&ePid_P);
	RAM_ePID_D = eeprom_read_word(&ePid_D);
}

void PID_writeToEepromP_D(uint16_t P, uint16_t D){
	while( !eeprom_is_ready	());
	eeprom_write_word(&ePid_P,P);
	eeprom_write_word(&ePid_D,D);
}
void PID_writeToEepromP(uint16_t P){
	while( !eeprom_is_ready	());
	eeprom_write_word(&ePid_P,P);
}
void PID_writeToEepromD(uint16_t D){
	while( !eeprom_is_ready	());
	eeprom_write_word(&ePid_D,D);
}

uint16_t PID_getP(){
	return _p;
}
uint16_t PID_getD(){
	return _d;
}	

uint8_t PID_EepromValidValues(){
	uint8_t ret_val = PID_EEPROM_NOK;
	if( (RAM_ePID_P != 0xFFFF) && ( RAM_ePID_D != 0xFFFF) ){
		ret_val = PID_EEPROM_OK;
	}

	return ret_val;
}


void PID_useEepromValues(){
	_i = RAM_ePID_P;
	_d = RAM_ePID_D;
}


void pid_setpoint(int16_t point)
{
	set_point = point;
}

void pid_set_consts(int16_t p, int16_t i, int16_t d)
{
	_p = p;
	_i = i;
	_d = d;
}

void pid_setD(uint16_t d){
	_d = d;
}

void pid_setP(uint16_t p){
	_p = p;
}

int16_t pid_calculate(int16_t value)
{
	error = set_point - (int16_t)value;
	
//	if(integrate_count == 5)
//	{
//		integrate += error;
//		integrate_count = 0;
//	}
//	integrate_count++;

//	if (error != prv_error)
//	{
//		derivate = (error - prv_error);
//		dLatchCounter = D_LATCH;
//	}
//	else
//	{
//		if (dLatchCounter > 0)
//		{
//			dLatchCounter--;
//		}
//		else
//		{
			derivate = (error - prv_error);
//		}
//	}

	temp = (int32_t)((int32_t)((int32_t)_p * (int32_t)error) + (int32_t)_d * (int32_t)derivate); //+ (int32_t)_i * (int32_t)integrate); 
	if(temp > (int32_t)32767)
	{
		temp = 32767;
	}
	else if(temp < (int32_t) -32767)
	{
		temp = -32767;
	}


	output = (int8_t)(((uint32_t)temp * (uint32_t)127) / (uint32_t)(32767) );
	
	

	prv_error = error;
	return output;
}
