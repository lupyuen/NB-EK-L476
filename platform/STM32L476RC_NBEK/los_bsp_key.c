//******************************************************************************
//                                www.ghostyu.com
//
//                 Copyright (c) 2017-2018, WUXI Ghostyu Co.,Ltd.
//                                All rights reserved.
//
//  FileName : los_bsp_key.c
//  Date :     2018-1-09 10:40
//  Version :  V0001
//  History :  初始创建版本
//******************************************************************************
#include "los_task.h"
#include "los_event.h"

#include "los_inspect_entry.h"
#include "los_config.h"

#ifdef LOSCFG_LIB_LIBC
#include "string.h"
#include <stdio.h>
#endif

#include "los_bsp_key.h"
#include "gpio.h"

#ifdef LOS_STM32L476xx
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"
#endif

//------------------------------------------------------------------------------
//Define and const area
#define KEY_UP_EVENT_ID     0x00000001
#define KEY_RIGHT_EVENT_ID  0x00000002
#define KEY_DOWN_EVENT_ID   0x00000004
#define KEY_LEFT_EVENT_ID   0x00000008
#define KEY_EVENT_MASK      0x0000000F

const PIN_HWAttrs  KEY_HWAtrrs[BUTTONn] = {
  {
    BTN_UP_PIN,
    BTN_UP_GPIO_PORT,
    GPIO_MODE_IT_FALLING,
    GPIO_PULLUP,
    0,
    GPIO_PIN_RESET,
    BTN_UP_EXTI_IRQn,
    EXTI3_IRQHandler,
    0x0F
  },
  {
    BTN_RIGHT_PIN,
    BTN_RIGHT_GPIO_PORT,
    GPIO_MODE_IT_FALLING,
    GPIO_PULLUP,
    0,
    GPIO_PIN_RESET,
    BTN_RIGHT_EXTI_IRQn,
    EXTI2_IRQHandler,
    0x0F
  },
  {
    BTN_DOWN_PIN,
    BTN_DOWN_GPIO_PORT,
    GPIO_MODE_IT_FALLING,
    GPIO_PULLUP,
    0,
    GPIO_PIN_RESET,
    BTN_DOWN_EXTI_IRQn,
    EXTI1_IRQHandler,
    0x0F
  },
  {
    BTN_LEFT_PIN,
    BTN_LEFT_GPIO_PORT,
    GPIO_MODE_IT_FALLING,
    GPIO_PULLUP,
    0,
    GPIO_PIN_RESET,
    BTN_LEFT_EXTI_IRQn,
    EXTI0_IRQHandler,
    0x0F
  },
};

const PIN_Config keyCfg = {
  .num = BUTTONn,
  .hwAttrs = KEY_HWAtrrs
};
//------------------------------------------------------------------------------
//Global variable 
//
static key_press_t key_check_press = {BTN_NONE,0};
/*任务PID*/
static UINT32 g_KeyTaskID;
/*事件控制结构体*/
static EVENT_CB_S  key_event;
//------------------------------------------------------------------------------
/*创建按钮任务*/
static void Key_TaskInit(void);
/*任务处理函数*/
static void KeyTskfunc(void);
/*返回指定按钮引脚状态*/
static uint32_t BSP_PB_GetState(Button_TypeDef Button);
//------------------------------------------------------------------------------
//******************************************************************************
// fn : LOS_EvbKeyInit
//
// brief : Init the button
//
// param : none
//
// return : none
void LOS_EvbKeyInit(void)
{

#ifdef LOS_STM32L476xx
	/* Configure PC0,PC1,PC2,PC3 pin as input floating */
  Board_GPIO_Init(&keyCfg);
	
	Key_TaskInit();
#endif

}
//******************************************************************************
// fn : LOS_EvbKeyDeInit
//
// brief : Deinit the button
//
// param : none
//
// return : none
void LOS_EvbKeyDeInit(void)
{
#ifdef LOS_STM32L476xx

	for(uint8_t i = 0 ; i < BUTTONn ; i++)
	{
    LOS_HwiDelete(KEY_HWAtrrs[i].irq);
    Board_GPIO_DeInit(KEY_HWAtrrs[i].Port, KEY_HWAtrrs[i].pin);
	}
  
#endif
}
#ifdef LOS_STM32L476xx
//******************************************************************************
// fn : LOS_EvbGetKey
//
// brief : Get the key 
//
// param : tickout
//
// return : BTN_UP,BTN_RIGHT,BTN_DOWN,BTN_LEFT
extern Button_TypeDef LOS_EvbGetKey(uint8_t tickout)
{
#ifdef LOS_STM32L476xx
	Button_TypeDef  tmpBtn = BTN_NONE;
	UINT32   uwEvent = 0;
	
	//限制读超时时间
	if(tickout > 10)
	{
		tickout = 10;
	}
	
	if(g_KeyTaskID)
	{
		uwEvent = LOS_EventRead(&key_event, KEY_EVENT_MASK, LOS_WAITMODE_OR|LOS_WAITMODE_CLR,tickout);
		if(uwEvent)
		{
			if((uwEvent & KEY_UP_EVENT_ID))
			{
				tmpBtn = BTN_UP;
			}
			else if((uwEvent & KEY_RIGHT_EVENT_ID))
			{
				tmpBtn = BTN_RIGHT;
			}
			else if((uwEvent & KEY_DOWN_EVENT_ID))
			{
				tmpBtn = BTN_DOWN;
			}
			else if((uwEvent & KEY_LEFT_EVENT_ID))
			{
				tmpBtn = BTN_LEFT;
			}
		}
	}
	return tmpBtn;
	
#else
	return BTN_NONE;
#endif
	
}
//******************************************************************************
// fn : HAL_GPIO_EXTI_Callback
//
// brief : EXTI line detection callbacks
//
// param : GPIO_Pin -> Specifies the pins connected EXTI line
//
// return : none
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case BTN_UP_PIN:
		{
			key_check_press.key = BTN_UP;
			break;
		}
		case BTN_RIGHT_PIN:
		{
			key_check_press.key = BTN_RIGHT;
			break;
		}
		case BTN_DOWN_PIN:
		{
			key_check_press.key = BTN_DOWN;
			break;
		}
		case BTN_LEFT_PIN:
		{
			key_check_press.key = BTN_LEFT;
			break;
		}		
		default:
			break;
	}
}

//******************************************************************************
// fn : Key_TaskInit
//
// brief : Init the key task
//
// param : none
//
// return : none
void Key_TaskInit(void)
{
	TSK_INIT_PARAM_S stTask1;
	UINT32 uwRet = LOS_OK;
	
	/*创建任务*/
	memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
	stTask1.pfnTaskEntry = (TSK_ENTRY_FUNC)KeyTskfunc;
	stTask1.pcName       = "KeyTask";
	stTask1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	stTask1.usTaskPrio   = LOS_TASK_PRIORITY_LOWEST -1;
	uwRet = LOS_TaskCreate(&g_KeyTaskID, &stTask1);
	if(uwRet != LOS_OK)
	{
			return ;
	}
}
//******************************************************************************
// fn : KeyTskfunc
//
// brief : KeyTask func
//
// param : none
//
// return : none
static void KeyTskfunc(void)
{
	UINT32 uwRet;
	uwRet = LOS_EventInit(&key_event);
	if(uwRet != LOS_OK)
	{
		return ;
	}
	for(;;)
	{
		//检查按钮中断是否正的触发
		if(key_check_press.key)
		{
			//延时20ticks,作用消抖
			LOS_TaskDelay(KEY_DELAY_TICK);
			uwRet = 0;
			switch(key_check_press.key)
			{
				case BTN_UP:
				{
					if(!BSP_PB_GetState(BTN_UP))
					{
						uwRet = KEY_UP_EVENT_ID;
            //printf("key_up\n");
					}
					break;
				}
				case BTN_RIGHT:
				{
					if(!BSP_PB_GetState(BTN_RIGHT))
					{
						uwRet = KEY_RIGHT_EVENT_ID;
            //printf("key_right\n");
					}
					break;
				}
				case BTN_DOWN:
				{
					if(!BSP_PB_GetState(BTN_DOWN))
					{
						uwRet = KEY_DOWN_EVENT_ID;
            //printf("key_down\n");
					}
					break;
				}
				case BTN_LEFT:
				{
					if(!BSP_PB_GetState(BTN_LEFT))
					{
						uwRet = KEY_LEFT_EVENT_ID;
            //printf("key_left\n");
					}
					break;
				}		
				default:
					break;
			}
			if(uwRet)
			{
				//不关心的事件成功与否
				LOS_EventWrite(&key_event, uwRet);
			}
			key_check_press.key = BTN_NONE;
		}
		else
		{
			LOS_TaskYield();
		}
		
	}
}

//******************************************************************************************
// fn : BSP_PB_GetState
//
// brief : Return the selected Button state.
//
// param : Button -> the key
//
// return : the state of key pin
uint32_t BSP_PB_GetState(Button_TypeDef Button)
{
  if(Button == BTN_NONE)
  {
    return 0xFF;
  }
  return Board_GPIO_ReadPin(&KEY_HWAtrrs[Button - 1]);
}
#endif
