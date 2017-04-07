#include <avr/interrupt.h>
#include <avr/io.h>
#include "communication_handler.h"
#include "pid.h"

#include "scheduler.h"
#include "pwm.h"
#include "common.h"

#define ROBOT_IDLE 		0
#define ROBOT_RUNNING	1


uint8_t uart_buffer[UART_RX_MAX];
uint8_t robot_state = ROBOT_IDLE;
uint16_t temp_var = 0;

#define AN_COMMON			0
#define AN_SELECT_CMD		1

#define AN_INC_P			2
#define AN_DEC_P			3

#define AN_INC_D			4
#define AN_DEC_D			5

void communication_init();
void _analyse_cmd(uint8_t cmd_char);


void communication_handler_init()
{


	
}

uint8_t an_state = AN_COMMON;
void _analyse_cmd(uint8_t cmd_char)
{



	switch(an_state)
	{
		case AN_COMMON:
		{
			if(cmd_char == 0x55){
				an_state = AN_SELECT_CMD;
			}
			else{
				an_state = AN_COMMON;
			}
		}
		break;
		case AN_SELECT_CMD:
		{
			/*QUICK STOP*/
			if(cmd_char == 0xAA){
				sch_setTaskStatus(TASK_1,DISABLE_TASK);
				pwm_setMotors(MOTOR_FORWARD,0,MOTOR_FORWARD,0);
				robot_state = ROBOT_IDLE;
				STATUS_LED_ON;
				an_state = AN_COMMON;
			}
			/*QUICK START*/
			else if(cmd_char == 0xFF){
				sch_setTaskStatus(TASK_1,ENABLE_TASK);
				robot_state = ROBOT_RUNNING;
				STATUS_LED_ON;
				an_state = AN_COMMON;
			}
			else if(cmd_char == 0xF1){
				an_state = AN_INC_P;
			}
			else if(cmd_char == 0xF2){
				an_state = AN_DEC_P;
			}
			else if(cmd_char == 0xD1){
				an_state = AN_INC_D;
			}
			else if(cmd_char == 0xD2){
				an_state = AN_DEC_D;
			}
			else{
				an_state = AN_COMMON;
			}

		}
		break;
		case AN_INC_P:
		{
			if(cmd_char == 0xAA){
				temp_var = PID_getP();
				if( (int32_t) ((int32_t)temp_var + (int32_t)P_UNIT) < 65000){
					temp_var += P_UNIT;
					PID_writeToEepromP(temp_var);
					pid_setP(temp_var);
					STATUS_LED_ON;
				}
			
				an_state = AN_COMMON;
			}
			else{
				an_state = AN_COMMON;
			}
		}
		break;
		case AN_DEC_P:
		{
			if(cmd_char == 0xAA){
				temp_var = PID_getP();

				if( (int32_t) ((int32_t)temp_var - (int32_t)P_UNIT) > 0){
					temp_var -= P_UNIT;
					PID_writeToEepromP(temp_var);
					pid_setP(temp_var);
					STATUS_LED_ON;
				}
			
				an_state = AN_COMMON;
			}
			else{
				an_state = AN_COMMON;
			}
			
		}
		break;
		case AN_INC_D:
		{
			if(cmd_char == 0xAA){
				temp_var = PID_getD();
			
				if( (int32_t) ((int32_t)temp_var + (int32_t)D_UNIT) < 65000){
					temp_var += D_UNIT;
					PID_writeToEepromD(temp_var);
					pid_setD(temp_var);
					STATUS_LED_ON;
				}
			
				an_state = AN_COMMON;
			}
			else{
				an_state = AN_COMMON;
			}			
		}
		break;
		case AN_DEC_D:
		{
			if(cmd_char == 0xAA){
				temp_var = PID_getD();
				if( (int32_t) ((int32_t)temp_var - (int32_t)D_UNIT) > 0){
					temp_var -= D_UNIT;
					PID_writeToEepromD(temp_var);
					pid_setD(temp_var);
					STATUS_LED_ON;
				}

				an_state = AN_COMMON;
			}
			else{
				an_state = AN_COMMON;
			}			
		}
		break;
		default:
		{

		}
		break;
	}

}




void communication_handler_task()
{	

	STATUS_LED_OFF;
	//get all rx commands
	uint8_t available_data = 0;
	available_data = uart_rx_flush(&uart_buffer[0]);

	for(uint8_t idx = 0; idx < available_data; idx++)
	{
		_analyse_cmd(uart_buffer[idx]);
		uart_buffer[idx] = 0;
	}

	
	
}
