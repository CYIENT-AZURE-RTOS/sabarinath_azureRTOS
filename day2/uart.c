/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
TX_THREAD ThreadOne;
TX_THREAD ThreadTwo;
TX_MUTEX mutexOne;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern UART_HandleTypeDef huart1;
uint8_t str1[1000] = {"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh\n\r"};
uint8_t str2[1000] = {"qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\n\r"};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void App_Delay(uint32_t Delay);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

   /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  //(void)byte_pool;
  CHAR *pointer1;
  CHAR *pointer2;

  if (tx_byte_allocate(byte_pool, (VOID **) &pointer1,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }

  if (tx_byte_allocate(byte_pool, (VOID **) &pointer2,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }


  if (tx_thread_create(&ThreadOne, "uart_task", ThreadOne_Entry, 0,
                       pointer1, APP_STACK_SIZE,
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       3000, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  if (tx_thread_create(&ThreadTwo, "uart_task2", ThreadTwo_Entry, 0,
                       pointer2, APP_STACK_SIZE,
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       1, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

   tx_mutex_create(&mutexOne,"mutex_one",TX_NO_INHERIT);

  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

void ThreadOne_Entry(ULONG thread_input)
{
  uint8_t t1_count=0;
  while(1)									/* Infinite loop */
  {
	if(tx_mutex_get(&mutexOne,1000)==0)
	{
		HAL_UART_Transmit(&huart1, str1, strlen(str1), 1000);	/* Toggle Green LED */
		tx_mutex_put(&mutexOne);
	}
	//tx_thread_sleep(100);							/* Delay for 100=1000ms */

  }
}


void ThreadTwo_Entry(ULONG thread_input)
{
  uint8_t t1_count=0;
  while(1)									/* Infinite loop */
  {
	  if(tx_mutex_get(&mutexOne,0)==0)
	{
		  HAL_UART_Transmit(&huart1,str2, strlen(str2), 1000);	/* Toggle Green LED */
		  tx_mutex_put(&mutexOne);
	}
	//tx_thread_sleep(100);						/* Delay for 100=1000ms */
   HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
   App_Delay(100);
  }
}

/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(uint32_t Delay)
{
  UINT initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}


/* USER CODE END 1 */
