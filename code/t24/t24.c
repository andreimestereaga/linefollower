#include <avr/io.h>

#define F_CPU 8000000UL

#include <util/delay.h>

#include "dbg_putchar.h"
#include "mcal_at24.h"
#include "thomson_codes.h"

#define IDLE 		0x0
#define CHECK_SIG	0x3
#define START_RCV	0x1
#define DECODE_MSG	0x2

#define STOP_CMD_1 			0x7030
#define STOP_CMD_2 			0x1C0F
#define TURBINE_UP_1		0x610C
#define TURBINE_UP_2		0x1843
#define TURBINE_DOWN_1 		0x103
#define TURBINE_DOWN_2 		0x40C
#define START_1 			0xF0C 
#define START_2 			0x3C3 

#define PID_P_DEC_1 		0x4030
#define PID_P_DEC_2 		0x100F
#define PID_P_INC_1			0xC3
#define PID_P_INC_2			0x30C	

#define PID_D_DEC_1			0x3
#define PID_D_DEC_2			0xC
#define PID_D_INC_1			0x300C
#define PID_D_INC_2			0xC03


void init_turbine();

void _sys_init(){

	dbg_tx_init();
	init_turbine();

	SET_SENSOR_INPUT;
	SET_LED;

	LED_ON;
	_delay_ms(50);
	LED_OFF;
	_delay_ms(50);
	LED_ON;
	_delay_ms(100);
	LED_OFF;

};




uint8_t signal_periods[75];
uint32_t signal_code = 0;
uint8_t  signal_shifter = 0;
uint16_t turbine_lvl = 500;

//volatile uint8_t sig_idx = 0;
volatile uint8_t dec_state = IDLE;
volatile uint16_t hold_counter = 0;

/*volatile uint8_t msg_counter = 0;
volatile uint8_t msg_end_qalif = 0;*/

volatile uint8_t sig_lvl = 0;
volatile uint8_t sig_lvl_prev = 1;
volatile uint8_t idx = 0;


#define MS_1	100
#define MS_1_8	180


void init_turbine(){

	SET_TIMER_REG1;
	SET_TIMER_REG2;


	SET_TOP_LVL_ICR;

	TURBINE_SET_PORT;

	SET_TOP_LVL_OCR;
	_delay_ms(1000);


	TIMER_COMPARE_REG = 0;
	_delay_ms(1000);

	
	TIMER_COMPARE_REG = 0;
	turbine_lvl = 500;
}

uint8_t set_turbine(uint16_t value){
	
	uint8_t ret_val = 0;
	if( (value >= 500) && (value <= 1000)){
		TIMER_COMPARE_REG = value;
		ret_val = 1;
	}

	return ret_val;
}

void stop_turbine(){
	TIMER_COMPARE_REG = 0x0000;
	turbine_lvl = 500;
}

void max_turbine(){
	TIMER_COMPARE_REG = 700;
	turbine_lvl = 700;

	_delay_ms(1000);

	TIMER_COMPARE_REG = 800;
	turbine_lvl = 800;

	_delay_ms(1000);


	TIMER_COMPARE_REG = 900;
	turbine_lvl = 900;

	_delay_ms(500);


	TIMER_COMPARE_REG = 950;
	turbine_lvl = 950;

	_delay_ms(500);

	TIMER_COMPARE_REG = 1000;
	turbine_lvl = 1000;



}



int main(){
	
	_sys_init();	

	


	while(1){


	
	
	sig_lvl = GET_DEC_LVL;

	

	switch(dec_state){
		
		case IDLE:
		{
			

			if( (hold_counter > MS_1) && (sig_lvl != sig_lvl_prev)){
				hold_counter = 0;
				signal_shifter = 0;
				signal_code = 0;
				idx = 0;
				dec_state = CHECK_SIG;
			}
			
			if(sig_lvl == 0){
				hold_counter++;
			}
			else{
				hold_counter = 0;
			}
		
			
			
		
		}
		break;

		case CHECK_SIG:
		{

			if((sig_lvl != sig_lvl_prev)){
				
				signal_periods[idx] = (uint8_t)hold_counter;
				hold_counter = 0;
				idx++;
			}
			else if(hold_counter > 200){
				idx++;
			}

			if(idx == 74){
				dec_state = START_RCV;
				hold_counter = 0;
				idx = 0;
			}
	
			if(hold_counter == 255){

			}
			else
			{
				hold_counter++;
			}

		}	
		break;

		case START_RCV:
		{
			for(idx = 0; idx < 75; idx++){
			
				if(signal_periods[idx] != 0){
					if( (signal_periods[idx] > 85) && (signal_periods[idx] < 110)){
						
						signal_code |=  (1 << signal_shifter);

					}
					else if( (signal_periods[idx] > 40) && (signal_periods[idx] < 60)){

					}
						signal_shifter++;

						if(signal_shifter == 32){
							break;
						}
					
				}
				else{
					break;
				}
			}
			for(idx = 0; idx < 75; idx++){
				signal_periods[idx] = 0;
			}
			dec_state = DECODE_MSG;

		}
		break;

		case DECODE_MSG:
		{
		
			if(signal_code == STOP_CMD_1 || signal_code == STOP_CMD_2){
				dbg_putchar(0x55);
				dbg_putchar(0xAA);
				stop_turbine();
			}
			else if(TURBINE_UP_1 == signal_code || TURBINE_UP_2 == signal_code){
				
				if(set_turbine( (uint16_t)(turbine_lvl + 50)) ){
				turbine_lvl += 50;
				}
				_delay_ms(100);
			}
			else if(TURBINE_DOWN_1 == signal_code || TURBINE_DOWN_2 == signal_code){
				if(set_turbine( (uint16_t)(turbine_lvl - 50)) ){
				turbine_lvl -= 50;
			}
				_delay_ms(100);
			}
			else if(START_1 == signal_code || START_2 == signal_code){
				dbg_putchar(0x55);
				dbg_putchar(0xFF);
				_delay_ms(100);
			}
			

			else if(PID_P_INC_1 == signal_code || PID_P_INC_2 == signal_code){
				dbg_putchar(0x55);
				dbg_putchar(0xF1);
				dbg_putchar(0xAA);
				_delay_ms(100);
			}
			else if(PID_P_DEC_1 == signal_code || PID_P_DEC_2 == signal_code){
				dbg_putchar(0x55);
				dbg_putchar(0xF2);
				dbg_putchar(0xAA);
				_delay_ms(100);
			}
			else if(PID_D_DEC_1 == signal_code || PID_D_DEC_2 == signal_code){
				dbg_putchar(0x55);
				dbg_putchar(0xD2);
				dbg_putchar(0xAA);
				_delay_ms(100);
			}
			else if(PID_D_INC_1 == signal_code || PID_D_INC_2 == signal_code){
				dbg_putchar(0x55);
				dbg_putchar(0xD1);
				dbg_putchar(0xAA);
				_delay_ms(100);
			}
			
			
		
		
		
			/*
			dbg_putchar( (uint8_t)signal_code);
			dbg_putchar( (uint8_t)(signal_code >> 8));
			dbg_putchar( (uint8_t)(signal_code >> 16));
			dbg_putchar( (uint8_t)(signal_code >> 24));
			*/
			
			
			
			
			

			dec_state = IDLE;
		}
		break;
		default:
		{
		}
		break;
	}

	sig_lvl_prev = sig_lvl;
	_delay_us(10);

	}

}
