#include <avr/io.h>
#include <stdint.h>
#define F_CPU 20000000UL
#include <util/delay.h>
#include <avr/interrupt.h>



#include "pwm.h"
#include "QTR.h"
#include "uart.h"
#include "scheduler.h"
#include "pid.h"
#include "communication_handler.h"
#include "common.h"

void calculate_proportional();

uint8_t temp = 0;
int16_t offset = 0;
uint16_t sensor_value = 0;
uint8_t last_value = 0;

int8_t last_offset = 4;
int16_t output_pid = 0;

uint8_t left_speed = 0;
uint8_t right_speed = 0;

int16_t speed = 90;

int16_t calcSpeed = 0;

int16_t weight_val = 0;
int16_t sens_val = 0;

int8_t wValues[10] = {-50, -40, -30,  -20, -10, 10, 20, 30, 40, 50};
uint16_t sensor_values[10];


void alg()
{

	
	sensor_value = QTR_reading(&sensor_values[0]);


	sens_val = 0;
	weight_val = 0;
	
	for(uint8_t idx = 0; idx < 10; idx++){
			sens_val += sensor_values[idx];
			weight_val += wValues[idx] * sensor_values[idx];
			
	}

	if( sensor_value == 0){
		offset = (int8_t)(weight_val / sens_val);
		last_offset = offset;
	}
	else{
		offset = last_offset;
	}
	
 	offset *= -1;
	output_pid = pid_calculate(offset);


	//pwm_setMotorsSigned(speed - output_pid, speed);

	if(output_pid > 0) // increase left motor speed
	{
//		if(output_pid > 120){
//			pwm_setMotorsSigned(-output_pid, speed+output_pid);
//		}
//		else{
			pwm_setMotorsSigned(speed- output_pid, speed+output_pid);
//		}
		
	}
	else if(output_pid < 0) //right motor
	{
	//f(output_pid < 120){
//	pwm_setMotorsSigned(speed-output_pid, output_pid);
	//
	//lse{
			pwm_setMotorsSigned(speed-output_pid,speed + output_pid);
	//
		
	}
	else{
		pwm_setMotors(MOTOR_FORWARD,speed,MOTOR_FORWARD,speed);
	}
}


int main()
{

	pwm_init();
	uart_init(255);
	QTR_init();
	sch_init();
	pid_init();
	PID_readFromEepromP_D();




	INIT_STATUS_LED;
	STATUS_LED_OFF;
	DDRB |= 0x80;



	sch_setTask(alg,0,3);
	sch_setTask(communication_handler_task,1,20);
	sch_setTaskStatus(TASK_2,ENABLE_TASK);
	pid_setpoint(0);

	if( PID_EepromValidValues() == PID_EEPROM_OK){
		PID_useEepromValues();
		STATUS_LED_ON;

	}
	else{
		pid_set_consts(2000,0,1000);
	}
	

	
	sei(); 	
	

	while(1)
	{

		sch_main();

		
	}


}


