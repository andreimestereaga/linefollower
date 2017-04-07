#include "pwm.h"

void pwm_init(){

	MOTORS_SET_TOP;
	MOTORS_INIT_TCCR1A;
	MOTORS_INIT_TCCR1B;

	MOTOR_DDR_SETUP;	

}


void pwm_setMotors(uint8_t right_dir,uint8_t right_val, uint8_t left_dir, uint8_t left_val){
	
	if(right_dir == MOTOR_FORWARD){
		SET_RIGHT_FORWARD;
		SET_RIGHT_FW_PWM(right_val);
	}
	else{
		SET_RIGHT_BACKWARD;
		SET_RIGHT_BK_PWM(right_val);
	}

	if(left_dir == MOTOR_FORWARD){
		SET_LEFT_FORWARD;
		SET_LEFT_FW_PWM(left_val);
	}
	else{
		SET_LEFT_BACKWARD;
		SET_LEFT_BK_PWM(left_val);
	}

}
void pwm_setMotorsSigned(int16_t right_val, int16_t left_val)
{
	if (right_val > 0)
	{
		SET_RIGHT_FORWARD;
		SET_RIGHT_FW_PWM(right_val);
	}
	else
	{
		SET_RIGHT_BACKWARD;
		SET_RIGHT_BK_PWM(-right_val);
	}
	if (left_val > 0)
	{
		SET_LEFT_FORWARD;
		SET_LEFT_FW_PWM(left_val);
	}
	else
	{
		SET_LEFT_BACKWARD;
		SET_LEFT_BK_PWM(-left_val);
	}

}
