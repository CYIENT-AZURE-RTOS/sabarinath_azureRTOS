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
#include<stdio.h>
#include"main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern UART_HandleTypeDef huart1;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD ThreadOne;
TX_THREAD ThreadTwo;
TX_THREAD ThreadThree;
TX_THREAD ThreadFour;
TX_SEMAPHORE semaphore_one;
uint8_t greenflag=0;
uint8_t redflag=0;
uint8_t count =0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ThreadOne_Entry(ULONG thread_input);
void ThreadTwo_Entry(ULONG thread_input);
void ThreadThree_Entry(ULONG thread_input);
void ThreadFour_Entry(ULONG thread_input);

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
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  CHAR *pointer1;
  CHAR *pointer2;
  CHAR *pointer3;
  CHAR *pointer4;


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
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer3,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer4,
                       APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    ret = TX_POOL_ERROR;
  }




  if (tx_thread_create(&ThreadOne, "Thread One", ThreadOne_Entry, 0,
                       pointer1, APP_STACK_SIZE,
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       1, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  if (tx_thread_create(&ThreadTwo, "Thread Two", ThreadTwo_Entry, 0,
                       pointer2, APP_STACK_SIZE,
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       1, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }


  if (tx_thread_create(&ThreadThree, "Thread Three", ThreadThree_Entry, 0,
                       pointer3, APP_STACK_SIZE,
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       1, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  if (tx_thread_create(&ThreadFour, "Thread Four", ThreadFour_Entry, 0,
                       pointer4, APP_STACK_SIZE,
                       THREAD_ONE_PRIO, THREAD_ONE_PREEMPTION_THRESHOLD,
                       1, TX_AUTO_START) != TX_SUCCESS)
  {
    ret = TX_THREAD_ERROR;
  }

  tx_semaphore_create(&semaphore_one,"semaphore one",2);


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
	uint8_t pin=0;
	uint8_t t3_status = 0;
	uint8_t status;
	while(1)
	{

		status = tx_semaphore_get(&semaphore_one,TX_WAIT_FOREVER);
		if(TX_SUCCESS == status)
		{
			HAL_UART_Transmit(&huart1, "thread 1 aquired\n\r", 20, 1000);
			if(0==greenflag)
			{
				greenflag =1;
				pin = GREEN_LED_Pin;
			}
			else if(0==redflag)
			{
				redflag =1;
				pin = RED_LED_Pin;
			}
			for(uint8_t i=0;i<20;i++)
			{
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, pin);
				App_Delay(10);
			}
			if(pin==GREEN_LED_Pin)
			{
				greenflag =0;
			}
			else if(pin == RED_LED_Pin)
			{
				redflag =0;
			}
			count++;
			HAL_UART_Transmit(&huart1, "relaesing semaphore 1\n\r", 30, 100);
			tx_semaphore_put(&semaphore_one);
		}

	}

}






void ThreadTwo_Entry(ULONG thread_input)
{
	uint8_t pin=0;
	uint8_t t3_status = 0;
	uint8_t status;
	while(1)
	{
		status = tx_semaphore_get(&semaphore_one,TX_WAIT_FOREVER);
		if(TX_SUCCESS == status)
		{

			HAL_UART_Transmit(&huart1, "thread 2 aquired\n\r", 20, 1000);
			if(0==greenflag)
			{
				greenflag =1;
				pin = GREEN_LED_Pin;
			}
			else if(0==redflag)
			{
				redflag =1;
				pin = RED_LED_Pin;
			}
			for(uint8_t i=0;i<9;i++)
			{
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, pin);
				App_Delay(100);
			}
			if(pin==GREEN_LED_Pin)
			{
				greenflag =0;
			}
			else if(pin == RED_LED_Pin)
			{
				redflag =0;
			}
			if(count >= 3)
			{
				status = tx_thread_terminate(&ThreadOne);
				if(status==0)
				{
					HAL_UART_Transmit(&huart1, "suspending task *****\n\r", 30, 1000);
					status = tx_thread_delete(&ThreadOne);
					if(status ==0)
					{
						HAL_UART_Transmit(&huart1, "deleting task *****\n\r", 30, 1000);
					}
				}
			}
			HAL_UART_Transmit(&huart1, "relaesing semaphore 2\n\r", 30, 100);
			tx_semaphore_put(&semaphore_one);

		}

	}

}





void ThreadThree_Entry(ULONG thread_input)
{
	uint8_t pin=0;
	uint8_t t3_status = 0;
	uint8_t status;
	while(1)
	{

		status = tx_semaphore_get(&semaphore_one,TX_WAIT_FOREVER);
		if(TX_SUCCESS == status)
		{
			HAL_UART_Transmit(&huart1, "thread 3 aquired\n\r", 20, 1000);
			if(0==greenflag)
			{
				greenflag =1;
				pin = GREEN_LED_Pin;
			}
			else if(0==redflag)
			{
				redflag =1;
				pin = RED_LED_Pin;
			}
			for(uint8_t i=0;i<5;i++)
			{
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, pin);
				App_Delay(250);
			}
			if(pin==GREEN_LED_Pin)
			{
				greenflag =0;
			}
			else if(pin == RED_LED_Pin)
			{
				redflag =0;
			}
			HAL_UART_Transmit(&huart1, "relaesing semaphore 3\n\r", 30, 100);

			tx_semaphore_put(&semaphore_one);

		}

	}

}



void ThreadFour_Entry(ULONG thread_input)
{
	uint8_t pin=0;
	uint8_t t3_status = 0;
	uint8_t status;
	while(1)
	{

		status = tx_semaphore_get(&semaphore_one,TX_WAIT_FOREVER);
		if(TX_SUCCESS == status)
		{
			HAL_UART_Transmit(&huart1, "thread 4 aquired\n\r", 20, 1000);
			if(0==greenflag)
			{
				greenflag =1;
				pin = GREEN_LED_Pin;
			}
			else if(0==redflag)
			{
				redflag =1;
				pin = RED_LED_Pin;
			}
			for(uint8_t i=0;i<3;i++)
			{
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, pin);
				App_Delay(500);
			}
			if(pin==GREEN_LED_Pin)
			{
				greenflag =0;
			}
			else if(pin == RED_LED_Pin)
			{
				redflag =0;
			}
			HAL_UART_Transmit(&huart1, "relaesing semaphore 4 \n\r",30, 100);
			tx_semaphore_put(&semaphore_one);

		}


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
