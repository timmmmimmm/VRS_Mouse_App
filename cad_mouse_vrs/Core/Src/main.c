/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "stm32f3xx_it.h"
#include "tlv493c/tlv493.h"
#include "W25Q32.h"
#include "movingAverageFIlter.h"
#include "mouseAxisPostProcessing.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool change_in_conf = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void create_message(uint8_t *message, uint8_t *len, int16_t rot_x, int16_t rot_y, int16_t rot_z);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  W25Q32_REGISTER_CALLBACKS(SPI_Receive,SPI_Send,GPIO_SPI_CS_LOW,GPIO_SPI_CS_HIGH,SPI_Delay);
  USART2_RegisterCallback(proccesDmaData);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM15_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  W25Q32_RESET();
  W25Q32_GET_ID();

//  W25Q32_WRITE_ACTION_BUTTON_0(5);
//  W25Q32_WRITE_ACTION_BUTTON_1(2);
  W25Q32_WRITE_DPI(1);
  (void)tlv493_init(i2c_master_read, i2c_master_write, HAL_Delay);
  initFilter(tlv493_getX, tlv493_getY, tlv493_getZ, tlv493_update_data, HAL_Delay);
  uint16_t dpi = 1;
	dpi = W25Q32_READ_DPI();
	if(dpi > 20 || dpi < 0)
		dpi = 1;
	dpi *= 50;
	dpi = (uint16_t)(1000/(1050-dpi)); //spytat sa tima ci bude problem nastavit sesitivitu 1-20
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  uint8_t message[256];
  uint8_t length;
  MouseAxisInfo *mouseAxisInfo = 0;

  while (1)
  {
    /* USER CODE END WHILE */
	  //pride vzdy predosla hodnota a ty ju prepocitas napr predosla je 60deg dalsie 61 tak neotoci ju o 61 dlasich ale len o 1 ?
	  //akoze neviem ci to dava zmysel alebo pokiaal drzi do strany tak zeby to posuvalo ?
	  //idk mozme dat discusion
	  //vo funkci si zavolaj najprv tlv493_update_data(); a potom getX()...
//	  calc_rot(&rotX,&rotY,&rotZ,&zoom);
	  if(change_in_conf){
		    dpi = W25Q32_READ_DPI();
			if(dpi > 20 || dpi < 0)
				dpi = 1;
			dpi *= 50;
			dpi = (uint16_t)(1000/(1050-dpi));
			change_in_conf = false;
	  }
	  mouseAxisInfo = MA_filterData();

	  mouseAxisInfo = PP_ProcessData(mouseAxisInfo,dpi);

	  create_message(message, &length, mouseAxisInfo->x, mouseAxisInfo->y, mouseAxisInfo->z);
	  USART2_PutBuffer(message, length);
	  HAL_Delay(1);

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_0)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_SetSystemCoreClock(8000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}

/* USER CODE BEGIN 4 */
void proccesDmaData(const uint8_t* data, uint8_t len)
{
	//this function parses 3 numbers representing DPI, ACTION BUTTON 0 and ACTION BUTTON 1 and writes them to the flash
	uint16_t numbers[3];
	static uint8_t k=0;
	static uint8_t counter = 0;
	for (uint8_t i= 0;i<len;i++){
		if ((data[i]<='9')&&(data[i]>='0')){
			if (counter==0){
				numbers[k]=data[i]-'0'; //first digit
			}
			else{
				numbers[k]=numbers[k]*10+(data[i]-'0');
			}
			counter++;
		}
		else if (data[i]==','){
			k++;
			counter=0;
		}
		else if (data[i]=='$'){
			uint16_t a=W25Q32_READ_DPI();
			uint8_t b=W25Q32_READ_ACTION_BUTTON_0();
			uint8_t c=W25Q32_READ_ACTION_BUTTON_1();
			char tx_buffer[128];
			uint16_t len = sprintf(tx_buffer, "{\"dpi\":%d,\"button0\":%d,\"button1\":%d}\r\n",a,b,c);
			USART2_PutBuffer((uint8_t*)tx_buffer, len);
			k=0;
			break;
		}
	}
	if (k>=2){  //if there were 3 numbers sent
		change_in_conf = true;
		W25Q32_WRITE_DPI(numbers[0]);
		W25Q32_WRITE_ACTION_BUTTON_0(numbers[1]);
		W25Q32_WRITE_ACTION_BUTTON_1(numbers[2]);
		k=0;
	}

}
void create_message(uint8_t *message, uint8_t *len, int16_t rot_x, int16_t rot_y, int16_t rot_z){
	uint8_t but0 = 0, but1 = 0;
  /**
   * @brief button actions
   * 0 - nothing
   * 1 - HOME (F6)
   * 2 - Cancel (ESC) 
   * 3 - Previous (F5)  
   * 4 - Zoom ALL (HOME)
   *
  */
//
//	if(get_button(0)){
//		but0 = W25Q32_READ_ACTION_BUTTON_0();
//		reset_button(0);
//	}
//	if(get_button(1)){
//		but1 = W25Q32_READ_ACTION_BUTTON_1();
//		reset_button(1);
//	}
//


	but0 = get_button(0);
	but1 = get_button(1);

	if(but0 || but1)
	{
		///////////////////////////////////////////  ARDUINO DEBUG  ///////////////////////////////////////////
//		*len = (uint8_t)sprintf((char*)message,
//				"RotX:%d,RotY:%d,Zoom:%d,Btn1:%d,Btn2:%d\r\n",
//				rot_x,rot_y,rot_z,but0,but1);

		///////////////////////////////////////////  Production  ///////////////////////////////////////////
		*len = (uint8_t)sprintf((char*)message,
					"{\"RotX\":%d,\"RotY\":%d,\"Zoom\":%d,\"Button0\":%d,\"Button1\":%d}\r\n",
					rot_x,rot_y,rot_z,but0,but1);

		reset_button(0);
		reset_button(1);
		return;
	}

	///////////////////////////////////////////  ARDUINO DEBUG  ///////////////////////////////////////////
//	*len = (uint8_t)sprintf((char*)message,
//					"RotX:%d,RotY:%d,Zoom:%d,Btn1:%d,Btn2:%d\r\n",
//					rot_x,rot_y,rot_z,0,0);

	///////////////////////////////////////////  Production  ///////////////////////////////////////////
	*len = (uint8_t)sprintf((char*)message,
						"{\"RotX\":%d,\"RotY\":%d,\"Zoom\":%d,\"Button0\":%d,\"Button1\":%d}\r\n",
						rot_x,rot_y,rot_z,but0,but1);

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
