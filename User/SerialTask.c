#include "main.h"
#include "usart.h"
#include "cmsis_os.h"
#include "brv_i2c_oled.h"

uint8_t rx_buf[20];//接收缓冲区
uint8_t rx_index = 0;//接收位置，每次接收一字节都加一
uint8_t rx_char=0;//接收的单个字符

void StartSerialTask(void *argument)
{
  
	uint16_t Data_Control =0;
	uint8_t serial_angle=90;//串口模式默认角度为90
	
	HAL_UART_Receive_IT(&huart1 ,&rx_char ,1);//使能串口接收中断
	
  for(;;)
  {
		osSemaphoreAcquire(SerialSemaphoreHandle,osWaitForever);//等待信号量，避免占用资源
		
		if (strcmp((char*)rx_buf, "LIGHT") == 0)//strcmp要求收到命令完全匹配
		{
			CurrentMode=MODE_LIGHT;
		}
		else if (strncmp((char*)rx_buf, "ANGLE=", 6) == 0)//strncmp只需要前六个字符匹配
		{
			CurrentMode=MODE_SERIAL;
			int angle = atoi((char*)rx_buf + 6);//跳过前面的匹配字符，取角度
			if (angle >= 0 && angle <= 180)
			{
				serial_angle = (uint8_t)angle;//对角度进行解析，切换为串口模式
			}
			
			if (CurrentMode == MODE_SERIAL)
			{
				//将收到角度值映射到PWM占空比范围
				float m=(float)(serial_angle)/ANGLEMAX;
				Data_Control =(uint16_t)(PWMMIN+m*(PWMMAX-PWMMIN));
				
				//发送舵机控制数据
				osMessageQueuePut(Queue01Handle,&Data_Control,0,0);
			}
		}
  }
}
//串口中断回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  rx_buf[rx_index++] = rx_char;//串口每次中断收到一字节数据，存入缓冲区

	if (rx_char == 'h')//h为命令接收标志
    {
			rx_buf[rx_index-1 ] = '\0';  // 将标志位替换为字符串结束符
			rx_index = 0;//重置等待下次接收
			
			//释放二进制信号量，需放在数据处理完成后
			osSemaphoreRelease(SerialSemaphoreHandle);
    }

    HAL_UART_Receive_IT(&huart1, &rx_char, 1);//重新使能串口接收中断
	
}
