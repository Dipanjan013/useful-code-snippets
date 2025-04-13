#include <stdio.h>
#include "sleep_manager.h"
#include "stm32f1xx_hal.h"

extern void SystemClock_Config(void);

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
