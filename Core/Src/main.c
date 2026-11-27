/* USER CODE BEGIN Header */
/**
  ********************************----------------------------------------------
  * @file           : main.c
  * @brief          : Main program body
  ********************************----------------------------------------------
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ********************************----------------------------------------------
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "i2c-lcd.h"
#include "tmp102.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    MODE_HOME = 0,
    MODE_AWAY = 1
} SystemMode_t;

typedef enum {
    MODE_OPEN = 0,
    MODE_CLOSED = 1
} Etat_fenetre;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_Pin GPIO_PIN_5
#define LED_Port GPIOA

// PIN Boutons
#define BT_MODE_Pin GPIO_PIN_13
#define BT_MODE_Port GPIOC
#define BT_MODE2_Pin GPIO_PIN_12
#define BT_MODE2_Port GPIOC
#define BT_FENETRE_Pin GPIO_PIN_11
#define BT_FENETRE_Port GPIOC

// PIN Capteurs
#define PIR_Pin GPIO_PIN_0
#define PIR_Port GPIOA
#define GAZ_DETECTOR_Pin GPIO_PIN_0
#define GAZ_DETECTOR_Port GPIOB
#define SENSOR_Pin GPIO_PIN_7
#define SENSOR_Port GPIOA

// PIN Alerte
#define LED_INTRUSION_Pin GPIO_PIN_1
#define LED_INTRUSION_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_4
#define BUZZER_GPIO_Port GPIOA

// Variable seuil
#define GAS_THRESHOLD 3500
#define GAS_WARNING_THRESHOLD 3200
#define HUMIDITE_THRESHOLD 3500
#define ADC_READ_INTERVAL 100
#define TEMP_READ_INTERVAL 200
#define TEMP_WARNING_HIGH 26.0f
#define TEMP_CRITICAL_THRESHOLD 23.0f
#define TEMP_HOT_THRESHOLD 28.0f
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile SystemMode_t currentMode = MODE_HOME;
volatile uint32_t last_interrupt_time = 0;
uint8_t led_r;
volatile Etat_fenetre currentEtat = MODE_CLOSED;
volatile uint32_t last_interrupt_time_2 = 0;


volatile uint8_t alarm_active = 0;
volatile uint8_t gaz_active = 0;
volatile uint8_t fenetre_active = 0;
volatile uint8_t update_lcd_flag = 1;
volatile uint8_t hum_active = 0;

volatile uint8_t timer_acquisition_flag = 0;
volatile uint8_t uart_msg_id = 0;

uint32_t last_led_toggle = 0;
uint32_t last_buzzer_toggle = 0;
uint32_t last_adc_read = 0;
uint32_t last_temp_read = 0;

uint32_t adc_gas_val = 0;
uint32_t adc_humidite_val = 0;

float current_temp = 0.0f;


extern ADC_HandleTypeDef hadc1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


uint32_t Read_ADC(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    uint32_t val = 0;
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        val = HAL_ADC_GetValue(&hadc1);
    }
    HAL_ADC_Stop(&hadc1);
    return val;
}

void Set_Servo_Angle(uint16_t pulse_width)
{
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse_width);
}

void Send_Message_All(char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
    lcd_clear();
    lcd_put_cur(0, 0);
    lcd_send_string(msg);
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
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(100);

  lcd_init();
  lcd_clear();
  HAL_Delay(50);

  lcd_put_cur(0, 0);
  lcd_send_string("Bonjour Lisa");
  HAL_Delay(1000);
  lcd_clear();
  lcd_put_cur(0, 0);
  lcd_send_string("Bienvenue");
  HAL_Delay(1000);

  char welcome_msg[] = " Bienvenue \r\n Mode: HOME , Fenetre fermee \r\n";
  HAL_UART_Transmit(&huart2, (uint8_t*)welcome_msg, strlen(welcome_msg), HAL_MAX_DELAY);

  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  Set_Servo_Angle(750);

  if (TMP102_Init() != HAL_OK)
  {
      char e[] = "TMP102 absent !\r\n";
      HAL_UART_Transmit(&huart2, (uint8_t*)e, strlen(e), 100);
  }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

        //Envoi des messages UART demandés par les interruptions (sans bloquer l'ISR)
        if (uart_msg_id != 0)
        {
            if (uart_msg_id == 1) {
                char msg[] = "Mode : HOME \r\n";
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
            } else if (uart_msg_id == 2) {
                char msg[] = "Mode : AWAY \r\n";
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
            } else if (uart_msg_id == 3) {
                char msg[] = "[ALERTE] Intrusion detectee!\r\n";
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
            } else if (uart_msg_id == 4) {
                char msg[] = "Fenetre ouverte \r\n";
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
            } else if (uart_msg_id == 5) {
                char msg[] = "Fenetre fermee \r\n";
                HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
            }
            uart_msg_id = 0;
        }

        //Mise à jour de l'écran LCD
        if (update_lcd_flag == 1)
        {
            update_lcd_flag = 0;
            lcd_clear();
            HAL_Delay(2);


            lcd_put_cur(0, 0);
            char line0[17];
            if (currentMode == MODE_HOME) {
                snprintf(line0, sizeof(line0), "MODE:HOME T:%.1fC", current_temp);
            } else {
                snprintf(line0, sizeof(line0), "MODE:AWAY T:%.1fC", current_temp);
            }
            lcd_send_string(line0);

            // État des alertes ou de la fenêtre
            lcd_put_cur(1, 0);
            char line1[17] = "                ";
            if (alarm_active == 1) {
                snprintf(line1, sizeof(line1), "ALERTE INTRUS.  ");
            }
            else if (gaz_active == 1) {
                snprintf(line1, sizeof(line1), "ALERTE GAZ      ");
            }
            else if (hum_active == 1) {
                snprintf(line1, sizeof(line1), "ALERTE HUMIDITE ");
            }
            else if (currentEtat == MODE_OPEN) {
                snprintf(line1, sizeof(line1), "Fenetre ouverte ");
            }
            else {
                snprintf(line1, sizeof(line1), "Fenetre fermee  ");
            }
            lcd_send_string(line1);
        }

        //Acquisition de TOUS les capteurs cadencée par le Timer matériel (TIM2)
        if (timer_acquisition_flag == 1)
        {
            timer_acquisition_flag = 0;

            //Lecture de l'humidité
            adc_humidite_val = Read_ADC(ADC_CHANNEL_7);
            if (adc_humidite_val > HUMIDITE_THRESHOLD)
            {
                if (hum_active == 0)
                {
                    hum_active = 1;
                    fenetre_active = 0;
                    currentEtat = MODE_CLOSED;
                    update_lcd_flag = 1;

                    char hum_msg[60];
                    sprintf(hum_msg, "[ALERTE] humidite detectee! Valeur: %lu\r\n", adc_humidite_val);
                    HAL_UART_Transmit(&huart2, (uint8_t*)hum_msg, strlen(hum_msg), 100);
                }
            }
            else if (hum_active == 1)
            {
                hum_active = 0;
                update_lcd_flag = 1;
            }

            // Lecture du gaz
            adc_gas_val = Read_ADC(ADC_CHANNEL_8);
            uint8_t previous_gaz_state = gaz_active;
            if (adc_gas_val > GAS_THRESHOLD)
            {
                gaz_active = 1;
                update_lcd_flag = 1;

                if (previous_gaz_state == 0)
                {
                    char gas_msg[50];
                    sprintf(gas_msg, "[ALERTE] Gaz detecte! Valeur: %lu\r\n", adc_gas_val);
                    HAL_UART_Transmit(&huart2, (uint8_t*)gas_msg, strlen(gas_msg), 100);
                }
            }
            else if (adc_gas_val > GAS_WARNING_THRESHOLD && adc_gas_val <= GAS_THRESHOLD)
            {
                char attention_gas_msg[80];
                sprintf(attention_gas_msg, "[ALERTE] Attention Gaz bientot atteint! Valeur: %lu\r\n", adc_gas_val);
                HAL_UART_Transmit(&huart2, (uint8_t*)attention_gas_msg, strlen(attention_gas_msg), 100);
                gaz_active = 0;
            }
            else
            {
                gaz_active = 0;
            }

            //Lecture de la température (TMP102)
            int16_t raw_temp = 0;
            if (TMP102_Measure(&raw_temp) == HAL_OK)
            {
                current_temp = TMP102_RawToCelsius(raw_temp);

                // 1. Gestion automatique de la fenêtre selon la température
                if (current_temp > TEMP_HOT_THRESHOLD) // > 28°C : Il fait trop chaud, on ouvre la fenêtre
                {
                    if (currentMode == MODE_HOME && fenetre_active == 0)
                    {
                        fenetre_active = 1;
                        currentEtat = MODE_OPEN;
                        char hot_msg[] = "[INFO] Il fait trop chaud (> 28C) - Fenetre ouverte\r\n";
                        HAL_UART_Transmit(&huart2, (uint8_t*)hot_msg, strlen(hot_msg), 100);
                    }
                }
                else if (current_temp <= TEMP_CRITICAL_THRESHOLD) // <= 23°C : Il fait trop froid, on ferme la fenêtre
                {
                    if (currentMode == MODE_HOME && fenetre_active == 1)
                    {
                        fenetre_active = 0;
                        currentEtat = MODE_CLOSED;
                        char f_msg[] = "[ALERTE] Temperature critique <= 23C - Fenetre fermee\r\n";
                        HAL_UART_Transmit(&huart2, (uint8_t*)f_msg, strlen(f_msg), 100);
                    }
                }
                else if (current_temp > TEMP_CRITICAL_THRESHOLD && current_temp <= TEMP_WARNING_HIGH) // Entre 23°C et 26°C
                {
                    char temp_warn_msg[60];
                    sprintf(temp_warn_msg, "[ATTENTION] Temperature fraiche (zone d'alerte) : %.1f C\r\n", current_temp);
                    HAL_UART_Transmit(&huart2, (uint8_t*)temp_warn_msg, strlen(temp_warn_msg), 100);
                }
                // Note : Entre 26°C et 28°C, c'est la zone "confort" libre, le bouton manuel garde le contrôle.

                update_lcd_flag = 1;

                // 2. Formatage et envoi de la température mesurée sur l'UART
                char temp_msg[50];
                int32_t mc  = TMP102_RawToMilliC(raw_temp);
                int32_t ent = mc / 1000;
                int32_t dec = (mc < 0 ? -mc : mc) % 1000 / 10;

                sprintf(temp_msg, "Temperature : %s%ld.%02ld C\r\n",
                        (mc < 0 && ent == 0) ? "-" : "", ent, dec);
                HAL_UART_Transmit(&huart2, (uint8_t*)temp_msg, strlen(temp_msg), 100);
            }
            else
            {
                char err_msg[] = "[ERREUR] Lecture TMP102 echouee\r\n";
                HAL_UART_Transmit(&huart2, (uint8_t*)err_msg, strlen(err_msg), 100);
            }
        }

        //Gestion du servomoteur
        if (fenetre_active == 1)
        {
            Set_Servo_Angle(1900);
        }
        else
        {
            Set_Servo_Angle(750);

        }

        //Gestion du clignotement des buzzers et LEDs d'alerte
        if (alarm_active == 1 || gaz_active == 1)
        {
            uint32_t current_tick = HAL_GetTick();

            if (current_tick - last_buzzer_toggle >= 2)
            {
                last_buzzer_toggle = current_tick;
                HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
            }
            if (current_tick - last_led_toggle >= 250)
            {
                last_led_toggle = current_tick;
                HAL_GPIO_TogglePin(LED_INTRUSION_GPIO_Port, LED_INTRUSION_Pin);
            }
        }
        else
        {
            HAL_GPIO_WritePin(LED_INTRUSION_GPIO_Port, LED_INTRUSION_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if (htim->Instance == TIM2)
 {

   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
   led_r = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5);


   timer_acquisition_flag = 1;
 }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  // Bouton de changement de mode
  if (GPIO_Pin == BT_MODE_Pin || GPIO_Pin == BT_MODE2_Pin)
  {
    uint32_t current_time = HAL_GetTick();
    if ((current_time - last_interrupt_time) > 200)
    {
      last_interrupt_time = current_time;

      if (currentMode == MODE_HOME)
      {
        currentMode = MODE_AWAY;
        currentEtat = MODE_CLOSED;
        fenetre_active = 0;
        uart_msg_id = 2;
      }
      else
      {
        currentMode = MODE_HOME;
        alarm_active = 0;
        currentEtat = MODE_CLOSED;
        uart_msg_id = 1;
      }
      update_lcd_flag = 1;
    }
  }

  //Capteur PIR
  else if (GPIO_Pin == PIR_Pin)
  {
    if (currentMode == MODE_AWAY)
    {
      alarm_active = 1;
      uart_msg_id = 3;
      update_lcd_flag = 1;
    }
  }
  //Bouton Fenêtre / Servomoteur
  else if (GPIO_Pin == BT_FENETRE_Pin)
  {
    uint32_t current_time_2 = HAL_GetTick();
    if ((current_time_2 - last_interrupt_time_2) > 500)
    {
      last_interrupt_time_2 = current_time_2;

      if (currentMode == MODE_HOME)
      {
          if (currentEtat == MODE_CLOSED)
          {
              currentEtat = MODE_OPEN;
              fenetre_active = 1;
              uart_msg_id = 4;
          }
          else
          {
              currentEtat = MODE_CLOSED;
              fenetre_active = 0;
              uart_msg_id = 5;
          }
          update_lcd_flag = 1;
      }
      else
      {
          currentEtat = MODE_CLOSED;
          fenetre_active = 0;
      }
    }
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
