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
#include "i2c.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "SX1278.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {_TRANSMITER, _RECEIVER}
_mode;

typedef enum {_MAIN, _SELECT_MODE, _MENU, _SEND_CMD, _OPTIONS}
_screen;

typedef enum {_LEFT_BUTTON, _RIGHT_BUTTON}
_button;

SX1278_hw_t SX1278_hw;
SX1278_t SX1278;

int master;
int ret;

char buffer[512];

int message;
int message_length;

bool leftButtonPressed = false;
bool rightButtonPressed = false;
bool newMsgCome = false;

bool txInitialized = false, rxInitialized = false;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
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

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	rightButtonPressed = (GPIO_Pin == RIGHT_BUTTON_Pin);
	leftButtonPressed = (GPIO_Pin == LEFT_BUTTON_Pin);
	newMsgCome = (GPIO_Pin == DIO0_Pin);
}



void assembleAndPutTextIntoDisplay(_mode loraMode, char * txt1, char * txt2, char * txt3){

	ssd1306_Fill(Black);

	uint8_t fontHeight = 10, lineNo, initX = 5, initY = 5;

	lineNo = 0;
	ssd1306_SetCursor(initX, initY + (fontHeight + 1)* lineNo);
	if(loraMode == _TRANSMITER){
		ssd1306_WriteString("M: TRANSMITER", Font_7x10, White);
	}
	else if(loraMode == _RECEIVER){
		ssd1306_WriteString("M: RECEIVER", Font_7x10, White);
	}

	lineNo = 1;
	ssd1306_SetCursor(initX, initY + (fontHeight + 1)* lineNo);
	ssd1306_WriteString(txt1, Font_7x10, White);

	lineNo = 2;
	ssd1306_SetCursor(initX, initY + (fontHeight + 1)* lineNo);
	ssd1306_WriteString(txt2, Font_7x10, White);

	ssd1306_UpdateScreen();
}

void writeLineByLine(char * txt, char * txt1, char * txt2, char * txt3){

	ssd1306_Fill(Black);

	uint8_t fontHeight = 10, lineNo, initX = 5, initY = 5;

	lineNo = 0;
	ssd1306_SetCursor(initX, initY + (fontHeight + 1)* lineNo);
	ssd1306_WriteString(txt, Font_7x10, White);

	lineNo = 1;
	ssd1306_SetCursor(initX, initY + (fontHeight + 1)* lineNo);
	ssd1306_WriteString(txt1, Font_7x10, White);

	lineNo = 2;
	ssd1306_SetCursor(initX, initY + (fontHeight + 1)* lineNo);
	ssd1306_WriteString(txt2, Font_7x10, White);

	ssd1306_UpdateScreen();
}

bool isButtonPressed(_button b){

	uint32_t debounceTime_ms = 50;
	uint32_t pressingTime_ms = 100;
	switch(b){
	case _LEFT_BUTTON:
		if(HAL_GPIO_ReadPin(LEFT_BUTTON_GPIO_Port, LEFT_BUTTON_Pin) == GPIO_PIN_SET){
			HAL_Delay(debounceTime_ms);
			if(HAL_GPIO_ReadPin(LEFT_BUTTON_GPIO_Port, LEFT_BUTTON_Pin) == GPIO_PIN_SET){
				HAL_Delay(pressingTime_ms);
				return true;
			}
		}
		break;
	case _RIGHT_BUTTON:
		if(HAL_GPIO_ReadPin(RIGHT_BUTTON_GPIO_Port, RIGHT_BUTTON_Pin) == GPIO_PIN_SET){
			HAL_Delay(debounceTime_ms);
			if(HAL_GPIO_ReadPin(RIGHT_BUTTON_GPIO_Port, RIGHT_BUTTON_Pin) == GPIO_PIN_SET){
				HAL_Delay(pressingTime_ms);
				return true;
			}
		}
		break;
	}
	return false;
}

void sendMsg_Basic(char * msg){
	if(!txInitialized){
		ret = SX1278_LoRaEntryTx(&SX1278, 16, 2000);
		txInitialized = true;
		rxInitialized = false;
	}

	message_length = sprintf(buffer, msg);
	ret = SX1278_LoRaEntryTx(&SX1278, message_length, 2000);

	ret = SX1278_LoRaTxPacket(&SX1278, (uint8_t*) buffer,
			message_length, 2000);
}

bool readMsg(){
	if(!rxInitialized){
		ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000);
		rxInitialized = true;
		txInitialized = false;
	}

	ret = SX1278_LoRaRxPacket(&SX1278);
	if (ret > 0) {
		SX1278_read(&SX1278, (uint8_t*) buffer, ret);
		return true;
	}
	return false;
}

void displayMainScreen(){

}
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
	MX_I2C1_Init();
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */

	_mode loraMode;
	_screen currentScreen = _MAIN;
	uint8_t option = 0, cmdNo = 0;
	bool newMsg = false;
	bool sendMsg = false;

	ssd1306_Init();

	ssd1306_Fill(Black);

	ssd1306_SetCursor(45, 20);
	ssd1306_WriteString("HELLO", Font_7x10, White);
	ssd1306_UpdateScreen();

	if(HAL_GPIO_ReadPin(MODE_GPIO_Port, MODE_Pin) == GPIO_PIN_SET){
		loraMode = _TRANSMITER;
	}
	else{
		loraMode = _RECEIVER;
	}

	uint32_t newMsgDispTime, currTime;
	newMsgDispTime = currTime = HAL_GetTick();

	//initialize LoRa module
	SX1278_hw.dio0.port = DIO0_GPIO_Port;
	SX1278_hw.dio0.pin = DIO0_Pin;
	SX1278_hw.nss.port = NSS_GPIO_Port;
	SX1278_hw.nss.pin = NSS_Pin;
	SX1278_hw.reset.port = RESET_GPIO_Port;
	SX1278_hw.reset.pin = RESET_Pin;
	SX1278_hw.spi = &hspi1;

	SX1278.hw = &SX1278_hw;

	SX1278_init(&SX1278, 434000000, SX1278_POWER_17DBM, SX1278_LORA_SF_7,
			SX1278_LORA_BW_125KHZ, SX1278_LORA_CR_4_5, SX1278_LORA_CRC_EN, 10);

	if (loraMode == _TRANSMITER) {
		ret = SX1278_LoRaEntryTx(&SX1278, 16, 2000);
		txInitialized = true;
	} else {
		ret = SX1278_LoRaEntryRx(&SX1278, 16, 2000);
		rxInitialized = true;
	}

	HAL_Delay(1000);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		currTime = HAL_GetTick();

		switch(loraMode){

		case _RECEIVER:
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_RESET);

			if(readMsg()){
				newMsgDispTime = HAL_GetTick();
				if(strcmp(buffer, "LED ON") == 0){
					HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_SET);
				}
				else if(strcmp(buffer, "LED OFF") == 0){
					HAL_GPIO_WritePin(RED_LED_GPIO_Port, RED_LED_Pin, GPIO_PIN_RESET);
				}
			}

			break;

		case _TRANSMITER:
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(BLUE_LED_GPIO_Port, BLUE_LED_Pin, GPIO_PIN_SET);

			if(sendMsg){
				sendMsg_Basic(buffer);
				sendMsg = false;
				HAL_Delay(100);
			}
			break;
		}


		switch(currentScreen){
		case _MAIN:
			if(currTime - newMsgDispTime < 1000){
				assembleAndPutTextIntoDisplay(loraMode, "New msg:", buffer, "");
			}
			else{
				assembleAndPutTextIntoDisplay(loraMode, "","","");
			}
			if(isButtonPressed(_LEFT_BUTTON)){
				currentScreen = _MENU;
			}
			break;

		case _MENU:
			if(option == 0){
				writeLineByLine("->Select mode", "Send cmd", "Options", "");
				if(isButtonPressed(_LEFT_BUTTON)){
					currentScreen = _SELECT_MODE;
					option = 0;
				}
			}
			else if(option == 1){
				writeLineByLine("Select mode", "->Send cmd", "Options", "");
				if(isButtonPressed(_LEFT_BUTTON)){
					currentScreen = _SEND_CMD;
					option = 0;
				}
			}
			else if(option == 2){
				writeLineByLine("Select mode", "Send cmd", "->Options", "");
				if(isButtonPressed(_LEFT_BUTTON)){
					currentScreen = _OPTIONS;
					option = 0;
				}
			}

			if(isButtonPressed(_RIGHT_BUTTON)){
				option = (option + 1)%3;
			}
			break;

		case _SELECT_MODE:
			if(option == 0){
				writeLineByLine("->RECEIVER", "TRANSMITER", "", "");
				if(isButtonPressed(_LEFT_BUTTON)){
					loraMode = _RECEIVER;
					currentScreen = _MAIN;
					option = 0;
				}
			}
			else if(option == 1){
				writeLineByLine("RECEIVER", "->TRANSMITER", "", "");
				if(isButtonPressed(_LEFT_BUTTON)){
					loraMode = _TRANSMITER;
					currentScreen = _MAIN;
					option = 0;
				}
			}

			if(isButtonPressed(_RIGHT_BUTTON)){
				option = (option + 1)%2;
			}

			break;

		case _SEND_CMD:
			if(option == 0){
				writeLineByLine("->LED ON", "LED OFF", "", "");
				if(isButtonPressed(_LEFT_BUTTON)){
					sprintf(buffer, "LED ON");
					sendMsg = true;
					currentScreen = _MAIN;
					option = 0;

					writeLineByLine("", "Msg sent!", "(Perharps)", "");
					HAL_Delay(1000);
					currentScreen = _MAIN;
				}
			}
			else if(option == 1){
				writeLineByLine("LED ON", "->LED OFF", "", "");
				if(isButtonPressed(_LEFT_BUTTON)){
					sprintf(buffer, "LED OFF");
					sendMsg = true;
					currentScreen = _MAIN;
					option = 0;

					writeLineByLine("", "Msg sent!", "(Perharps)", "");
					HAL_Delay(1000);
					currentScreen = _MAIN;
				}
			}

			if(isButtonPressed(_RIGHT_BUTTON)){
				option = (option + 1)%2;
			}
			break;

		case _OPTIONS:
			writeLineByLine("Nothing is", "written here", "yet :)", "");
			HAL_Delay(1000);
			currentScreen = _MAIN;
			break;

		}

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

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

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
