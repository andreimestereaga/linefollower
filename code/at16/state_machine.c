/*
 * state_machine.c
 *
 * Created: 14.05.2015 22:56:17
 *  Author: Andrei
 */ 


#include <state_machine.h>
#include <task_config.h>
#include <menu.h>
#include <snake.h>
#include <nokia5110.h>

#define STATE_STARTING	 0x1
#define STATE_MENU		 0x2
#define STATE_PLAY		 0x3
#define STATE_EXIT		 0x5
#define STATE_PAUSE		 0x6



uint8_t current_state;
uint8_t toDo_Task;

uint16_t game_overCounter = 0;

void sm_init()
{
	current_state = STATE_STARTING;
	toDo_Task = 0x1;
}

uint8_t sm_getTasks()
{
	return toDo_Task;
}

void sm_main()
{
	
	switch(current_state)
	{
		case STATE_STARTING:
		{
			
			current_state = STATE_MENU;
			toDo_Task |= LCD_REFRESH_WORK | PWM_RUN_WORK | MENU_UPDATE_WORK; 
			break;	
		}
		case STATE_MENU:
		{
		
			
			if(menu_getCurrentSelection() == PLAY_SELECT)
			{
				toDo_Task = 0x0;
				toDo_Task |= 0x1  | SNAKE_WORK;
				current_state = STATE_PLAY;
			}
			
			
			break;
		}
		case STATE_PLAY:
		{
			if(getGameStatus())
			{
				toDo_Task = 0x0;
				toDo_Task |= 0x1 | PWM_RUN_WORK | LCD_REFRESH_WORK;
				current_state = STATE_EXIT;
			}
			
			break;	
		}
		case STATE_EXIT:
		{
			
			menu_gameOver();
			
			if(game_overCounter == 200)
			{
				snake_resetGameOver();
				game_overCounter = 0;
				current_state = STATE_MENU;
				nokia_clearDisplay();
				toDo_Task = 0x0;
				toDo_Task |= 0x1 | LCD_REFRESH_WORK | PWM_RUN_WORK | MENU_UPDATE_WORK;
			}
			else
			{
				
				game_overCounter++;
			}
			
			break;
		}
		case STATE_PAUSE:
		{
			
			break;
		}
		default:
		{
			
			break;
		}
		
		
	}
	
}