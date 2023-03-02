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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/*
 * Note to self:
 *  added the lib files I created for the SPI
 *  need to add custom functions to spi.c (and spi.h)
 */

//#include "usbd_cdc_if.h"
#include "l3gd20.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* I2C variables */
__IO uint32_t     Xfer_Direction = 0;
__IO uint32_t     Xfer_Complete = 0;
// Buffer used for transmission
uint8_t aTxBuffer[4];
// Buffer used for reception
uint8_t aRxBuffer[2];

/* Gyroscope variables */
uint8_t _newData = 0x00;
uint8_t spi_Buffer[6];
// ID of L3GD20
uint8_t ui8gyroID;
// test buffer
uint8_t testBuf[1];


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer))
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void set_led(uint8_t counter);
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

  //The following code initialises the receive and transmit buffers being used.
  aRxBuffer[0]=0x00;
  aRxBuffer[1]=0x00;
  aRxBuffer[2]=0x00;
  aRxBuffer[3]=0x00;

  aTxBuffer[0]=0x00;
  aTxBuffer[1]=0x00;

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  // Initialise the gyro driver
  GYRO_Init();
  ui8gyroID = L3GD20_ReadID();
  GYRO_IO_Read(testBuf, L3GD20_CTRL_REG3_ADDR, sizeof(testBuf));

  /* Enable the Listen mode with Interrupts for the I2C:
   * The Listen Mode will wait for an I2C Event to occur
   * and will be treated in the Interrupt Service Routine of the I2C. */
  if(HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK)
  {
	  /* Transfer error in reception process */
	  Error_Handler();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	// check to see if new

	if(Xfer_Complete)
	{
		HAL_Delay(1);

		/* Put I2C peripheral in listen mode process */
		if(HAL_I2C_EnableListen_IT(&hi2c1) != HAL_OK)
		{
			/* Transfer error in reception process */
			Error_Handler();
		}
		Xfer_Complete = 0;
	}

    // get new data from gyro
	L3GD20_ReadXYZAngRate_Raw(spi_Buffer);
    /* USER CODE END WHILE */

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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_TIM1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

// callback function to reset the I2C if time-out occurred
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim == &htim1){
		// reset timer
		HAL_TIM_Base_Stop_IT(&htim1);
		Xfer_Complete = 1;
	}
}

/**
  * @brief  Set BSP LEDs.
  * @param  uint_8: counter - which LED should be switched on.
  * @note   This is a simple helper function to turn on a single LED, it requires a number to select an LED,
  * 		starting at 0 with the LED in the 12oclock position and increasing clockwise (up to 7 for the 8th LED)
  * @retval None
  */

void set_led(uint8_t counter){
	// set all user LEDs low
	HAL_GPIO_WritePin(GPIOE, LD4_Pin|LD3_Pin|LD5_Pin|LD7_Pin
							|LD9_Pin|LD10_Pin|LD8_Pin|LD6_Pin,
							GPIO_PIN_RESET);
	  switch (counter) {
	    case 0:
	    	HAL_GPIO_WritePin(GPIOE, LD3_Pin, GPIO_PIN_SET);
	    	break;
	    case 1:
	    	HAL_GPIO_WritePin(GPIOE, LD5_Pin, GPIO_PIN_SET);
	    	break;
	    case 2:
	    	HAL_GPIO_WritePin(GPIOE, LD7_Pin, GPIO_PIN_SET);
	    	break;
	    case 3:
	    	HAL_GPIO_WritePin(GPIOE, LD9_Pin, GPIO_PIN_SET);
	    	break;
	    case 4:
	    	HAL_GPIO_WritePin(GPIOE, LD10_Pin, GPIO_PIN_SET);
	    	break;
	    case 5:
	    	HAL_GPIO_WritePin(GPIOE, LD8_Pin, GPIO_PIN_SET);
	    	break;
	    case 6:
	    	HAL_GPIO_WritePin(GPIOE, LD6_Pin, GPIO_PIN_SET);
	    	break;
	    case 7:
	    	HAL_GPIO_WritePin(GPIOE, LD4_Pin, GPIO_PIN_SET);
	    	break;
	    default:
//	    	HAL_GPIO_WritePin(GPIOE, LD3_Pin, GPIO_PIN_SET); // set the first led
			break;
	  }

}

/**
  * @brief  Tx Transfer completed callback.
  * @param  I2cHandle: I2C handle.
  * @note   This example shows a simple way to report end of IT Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
	HAL_TIM_Base_Stop_IT(&htim1); // stop time-out counter

	Xfer_Complete = 1;
}


/**
  * @brief  Rx Transfer completed callback.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{

	HAL_TIM_Base_Stop_IT(&htim1); // stop timeout counter
//  aRxBuffer[0]; // dummy byte - no idea, something wrong in smbus2 lib on python side
//  aRxBuffer[1]; // command - set_LED = 0x01
//  aRxBuffer[2]; // value of LED

	switch (aRxBuffer[1]){
	case 0x01:
		set_led(aRxBuffer[2]);
		break;
	case 0x02:
		// set tx buffer to Xval
		  aTxBuffer[0]=spi_Buffer[0];
		  aTxBuffer[1]=spi_Buffer[1];
		  break;
	case 0x03:
		// get Yval
		  aTxBuffer[0]=spi_Buffer[2];
		  aTxBuffer[1]=spi_Buffer[3];
		  break;
	default:
		// do nothing
		break;
	}
	Xfer_Complete = 1;
}


/**
  * @brief  Slave Address Match callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  TransferDirection: Master request Transfer Direction (Write/Read), value of @ref I2C_XferOptions_definition
  * @param  AddrMatchCode: Address Match Code
  * @retval None
  */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	Xfer_Direction = TransferDirection;
	HAL_TIM_Base_Start_IT(&htim1); // start timeout timer
	if (Xfer_Direction != 0)
	{
		/* Start the transmission process */
		/* While the I2C in reception process, user can transmit data through "aTxBuffer" buffer */
		if (HAL_I2C_Slave_Seq_Transmit_IT(&hi2c1, (uint8_t *)aTxBuffer, TXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK)
		{
			/* Transfer error in transmission process */
			Error_Handler();
		}
	}
	else
	{
		/* Put I2C peripheral in reception process */
		if (HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, (uint8_t *)aRxBuffer, RXBUFFERSIZE, I2C_FIRST_AND_LAST_FRAME) != HAL_OK)
		{
			/* Transfer error in reception process */
			Error_Handler();
		}
	}

}

/**
  * @brief  Listen Complete callback.
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval None
  */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
}

/**
  * @brief  I2C error callbacks.
  * @param  I2cHandle: I2C handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
  /** Error_Handler() function is called when error occurs.
    * 1- When Slave doesn't acknowledge its address, Master restarts communication.
    * 2- When Master doesn't acknowledge the last data transferred, Slave doesn't care in this example.
    */
  if (HAL_I2C_GetError(I2cHandle) != HAL_I2C_ERROR_AF)
  {
    Error_Handler();
  }
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
