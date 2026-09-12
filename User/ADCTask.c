#include "adc.h"
#include "cmsis_os.h"
#include "main.h"
#include "usart.h"
#include "brv_i2c_oled.h"

void StartADCTask(void *argument)
{
	uint16_t Data_Control =0;
	uint16_t ADC_Value = 0;

	 for(;;)
  {

		if (CurrentMode==MODE_LIGHT)
		{

			HAL_ADC_Start(&hadc1);//使能ADC
			
			//读取ADC值
			if (HAL_ADC_PollForConversion(&hadc1,500)==HAL_OK)
				{
					ADC_Value = HAL_ADC_GetValue(&hadc1);//读取ADC值
				}
				
			//限制ADC值
			if(ADC_Value>ADCMAX) ADC_Value=ADCMAX;
			if(ADC_Value<ADCMIN) ADC_Value=ADCMIN;
			
			//将ADC值映射到PWM占空比范围
			float p =(float)(ADC_Value-ADCMIN)/(ADCMAX-ADCMIN);
			Data_Control =(uint16_t)(PWMMIN+p*(PWMMAX-PWMMIN));
				
				osMessageQueuePut(Queue01Handle,&Data_Control,0,0);//将控制数据发送给舵机任务
				osMessageQueuePut(Queue02Handle,&ADC_Value,0,0);//将显示数据发送到OLED任务
		}
		osDelay(500);
  }

}
