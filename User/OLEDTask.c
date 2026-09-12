#include "main.h"
#include "cmsis_os.h"
#include "brv_i2c_oled.h"

void StartOLEDTask(void *argument)
{
	{
		uint16_t ADC_Value;
		char display[10];//用于展示adc值处理后的光照强度
		for(;;)
		{
			if (CurrentMode==MODE_LIGHT)
			{
				osMessageQueueGet(Queue02Handle,&ADC_Value,NULL,100);
				OLED_CLS();//清除数据
				
				//使用OLED显示字符与汉字
				OLED_ShowChinese_F16X16(1,1,8);
				OLED_ShowChinese_F16X16(1,2,9);
				OLED_ShowString_F8X16(1,6,(uint8_t *)":");
				sprintf(display,"%d",ADCMAX-ADC_Value);//将需要显示的光照强度写入变量
				OLED_ShowString_F8X16(1,7,(uint8_t *)display);
				OLED_ShowChinese_F16X16(3,1,14);
				OLED_ShowChinese_F16X16(3,2,15);
				OLED_ShowString_F8X16(3,6,(uint8_t *)":");
				OLED_ShowChinese_F16X16(3,4,8);
				OLED_ShowChinese_F16X16(3,5,16);
			}
			else {
				OLED_CLS();//清除数据
				
				//使用OLED显示字符与汉字
				OLED_ShowChinese_F16X16(2,1,14);
				OLED_ShowChinese_F16X16(2,2,15);
				OLED_ShowString_F8X16(2,6,(uint8_t *)":");
				OLED_ShowChinese_F16X16(2,4,17);
				OLED_ShowChinese_F16X16(2,5,18);
			}
		
			osDelay(500);
		}
  }
}
