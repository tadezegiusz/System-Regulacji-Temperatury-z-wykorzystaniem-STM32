/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "eth.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LiquidCrystal.h"
#include "bmp280.h"
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
BMP280_HandleTypedef bmp280;
uint16_t size;
float temperatura, cisnienie, wilgotnosc;
uint8_t dane[50];

uint8_t temp[2];
float y[2] = {0, 0};
float u[2] = {0, 0};
float tempz = 35;
float uchyb;


bool t[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int c[2]={0,0};
int j=0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_I2C1_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  LiquidCrystal(GPIOC, RS_Pin, RW_Pin, ENABLE_Pin, D0_Pin, D1_Pin, D2_Pin, D3_Pin);

  bmp280_init_default_params(&bmp280.params);
  bmp280.addr = BMP280_I2C_ADDRESS_0;
  bmp280.i2c = &hi2c1;
  bmp280_init(&bmp280, &bmp280.params);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 2500);

  int wypelnienie;
  char* tekst;



  HAL_UART_Receive_IT(&huart3, &temp, 2);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	HAL_Delay(500);

	while(!bmp280_read_float(&bmp280, &temperatura, &cisnienie, &wilgotnosc)){
	size = sprintf((char *)dane, "czujnik wybuch\n");
	HAL_UART_Transmit(&huart3, dane, size, 1000);
	}
	size = sprintf((char *)dane, "Obecna temperatura: %.2f \r\n", temperatura);

	HAL_UART_Transmit(&huart3, dane, size, 1000);

	size = sprintf((char *)dane, "Zadana temperatura: %.2f \r\n", tempz);

	HAL_UART_Transmit(&huart3, dane, size, 1000);

	size = sprintf((char *)dane, "Sygnal sterujacy: %.2f \r\n", y[0]);

	HAL_UART_Transmit(&huart3, dane, size, 1000);


	u[1] = u[0];

	y[1] = y[0];

	uchyb=tempz-temperatura;
	u[0] = uchyb;
	y[0] = 0.212*u[0]-0.21*u[1]+y[1];
	if(y[0]>12) y[0]=12;
	if(y[0]<0) y[0]=-1;
	wypelnienie=(5000*y[0])/12;
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, wypelnienie);

	sprintf(*tekst, "%.2f", temperatura);
	setCursor(0, 0);
	print(*tekst);
	sprintf(*tekst, "%.2f", y[0]);
	setCursor(0, 1);
	print(*tekst);
	sprintf(*tekst, "%.2f", tempz);
	setCursor(6, 0);
	print(*tekst);

	setCursor(5, 0);
	print(" ");




	//klawiatura wybucha
	HAL_GPIO_WritePin(k0_GPIO_Port, k0_Pin, 1);
	t[0]=HAL_GPIO_ReadPin(w0_GPIO_Port, w0_Pin);
	t[3]=HAL_GPIO_ReadPin(w1_GPIO_Port, w1_Pin);
	t[6]=HAL_GPIO_ReadPin(w2_GPIO_Port, w2_Pin);
	t[9]=HAL_GPIO_ReadPin(w3_GPIO_Port, w3_Pin);
	HAL_GPIO_WritePin(k0_GPIO_Port, k0_Pin, 0);

	HAL_GPIO_WritePin(k1_GPIO_Port, k1_Pin, 1);
	t[1]=HAL_GPIO_ReadPin(w0_GPIO_Port, w0_Pin);
	t[4]=HAL_GPIO_ReadPin(w1_GPIO_Port, w1_Pin);
	t[7]=HAL_GPIO_ReadPin(w2_GPIO_Port, w2_Pin);
	t[10]=HAL_GPIO_ReadPin(w3_GPIO_Port, w3_Pin);
	HAL_GPIO_WritePin(k1_GPIO_Port, k1_Pin, 0);

	HAL_GPIO_WritePin(k2_GPIO_Port, k2_Pin, 1);
	t[2]=HAL_GPIO_ReadPin(w0_GPIO_Port, w0_Pin);
	t[5]=HAL_GPIO_ReadPin(w1_GPIO_Port, w1_Pin);
	t[8]=HAL_GPIO_ReadPin(w2_GPIO_Port, w2_Pin);
	t[11]=HAL_GPIO_ReadPin(w3_GPIO_Port, w3_Pin);
	HAL_GPIO_WritePin(k2_GPIO_Port, k2_Pin, 0);

	if(t[11]==1){
		setCursor(0, 0);
		print("wybuch");
	}

	for(int i=0; i<10; i++){
		if(t[i] == 1 ){
			c[j]=i;
			j=1;
		}
	}

	if(t[10]==1){
		int z=c[0]*10 + c[1];
		if(z>20 && z<60){
			sprintf(z, "%.2f");
			tempz=z;
		}
		c[0]=0;
		c[1]=0;
		j=0;
	}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	int ed;

	if(huart->Instance == USART3)
	{
		ed=10*(temp[0]-48)+(temp[1]-48);
		if(ed>20 && ed<60){
			tempz = ed;
		}
	}

	HAL_UART_Receive_IT(&huart3, &temp, 2);
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
