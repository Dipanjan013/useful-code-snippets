#include <stdio.h>
#include "sleep_manager.h"
#include "stm32f1xx_hal.h"

extern void SystemClock_Config(void);


/* example code for pre sleep and post sleep operation
void sleep_manager_PreSleepOps(void)
{
  //Put the BG95 to sleep
  wis_bg95_handle_MyBg95PowerOff();
  if(wis_handle_adxl345_GetIrqFlag()){
      wis_handle_adxl345_SetIrqFlag(0);
      wis_handle_adxl345_ClearIrq();
  }

//  service_w25_PowerCtrl(SERVICE_W25_POWER_OFF);
//  wis_bh1750_handle_powerdown();

  printf("Going to sleep\r\n");
  HAL_UART_DeInit(&huart1);
  HAL_UART_DeInit(&huart2);

  __HAL_RCC_PWR_CLK_ENABLE();
  //  HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
  //  HAL_NVIC_ClearPendingIRQ(EXTI0_1_IRQn);

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  //ADXL INT PIN2
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void sleep_manager_PostSleepOps(void)
{
  __HAL_I2C_DISABLE(&hi2c1);
  __HAL_I2C_ENABLE(&hi2c1);
  HAL_I2C_MspInit(&hi2c1);
  MX_GPIO_Init();
  MX_TIM2_Init();
  //  MX_IWDG_Init();
  MX_RTC_Init();
  MX_SPI2_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  if(wis_handle_adxl345_GetIrqFlag()){
    wis_handle_adxl345_SetIrqFlag(0);
    wis_handle_adxl345_ClearIrq();
    printf("WAKEUP on ACTIVITY\r\n");
    uint8_t id = 0;
    wis_handle_adxl345_GetDeviceId(&id);
    printf("adxl Device id : 0x%02X\r\n",id);
  }
  printf("PIN STATE : %u\r\n", HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4));
  printf("Wake-up\r\n");
  uint16_t w25ChipID = 0;
  service_w25_ReadID(&w25ChipID);
  printf("W25Q128 Chip ID %u\r\n", w25ChipID);
//  wis_bg95_handle_MyBg95PowerOn();
  wis_uart_handle_init(BG95_UART_BASEADDR, wis_bg95_handle_receivedata_callback);
  wis_bg95_handle_set_power(1);
}*/

__weak void sleep_manager_PreSleepOps(void)
{
  ;
  //recommended to set all GPIOs to analog mode and disable IRQ if any for more power optimization
}

__weak void sleep_manager_PostSleepOps(void)
{
  ;
}

void sleep_manager_SleepStart(void)
{

  //Apply pre-sleep operation if any
  sleep_manager_PreSleepOps();

  HAL_SuspendTick();
#if (SLEEP_MANAGER_REENTER_SLEEP)
  HAL_PWR_EnableSleepOnExit();	//Re enter sleep mode after interrupt execution is over
#endif
  /* Enter Stop Mode */
  HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
  /** wake- up*/
#if (SLEEP_MANAGER_REENTER_SLEEP)
  HAL_PWR_DisableSleepOnExit();		//use this anywhere else (in IRQ callback) as per application
#endif
  SystemClock_Config();
  //Apply post-sleep operation if any
  sleep_manager_PostSleepOps();
}
