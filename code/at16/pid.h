#ifndef _PID_H
#define _PID_H


#include "stdint.h"

#define P_UNIT 100
#define D_UNIT 100

#define PID_EEPROM_OK	1
#define PID_EEPROM_NOK	2


void pid_init();
extern uint8_t PID_EepromValidValues();
extern void PID_writeToEepromP(uint16_t P);
extern void PID_writeToEepromD(uint16_t D);
extern uint16_t PID_getP();
extern uint16_t PID_getD();
extern void PID_useEepromValues();
extern void PID_readFromEepromP_D();
extern void PID_writeToEepromP_D(uint16_t P, uint16_t D);
extern void pid_setpoint(int16_t point);
extern int16_t pid_calculate(int16_t value);
extern void pid_set_consts(int16_t p, int16_t i, int16_t d);
extern void pid_setD(uint16_t d);
extern void pid_setP(uint16_t p);


#endif // _PID_H
