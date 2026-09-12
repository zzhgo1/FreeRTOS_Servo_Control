#include "main.h"
#include "cmsis_os.h"
#include "tim.h"



void StartServoTask(void *argument)
{

	uint16_t pwmval=0;
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//使能pwm输出
	
  for(;;)
  {
		osMessageQueueGet(Queue01Handle,&pwmval,NULL,100);//接收ADC或串口传来的数据
		
		__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,pwmval);//修改pwmval的值改变占空比来控制舵机角度
		
    osDelay(500);
  }
}

