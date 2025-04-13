/*
 * sleep_manager.h
 *
 *  Created on: Mar 31, 2025
 *      Author: Dipanjan
 */

#ifndef INC_SLEEP_MANAGER_H_
#define INC_SLEEP_MANAGER_H_

/*!
 * @def Use 1 if required to re-enter sleep upon IRQ execution complete.
 */
#define SLEEP_MANAGER_REENTER_SLEEP 0

/*!
 * @fn Function to enter sleep mode.
 * It internally applies the sleep_manager_PreSleepOps and after wake-up it calls sleep_manager_PostSleepOps which are weak function
 * @return none
 */
void sleep_manager_SleepStart(void);

/*!
 * @fn This is weak function. Can be used in app layer to apply config before sleep
 * @brief Example Deinit() of peripherals
 * @return none
 */
void sleep_manager_PreSleepOps(void);

/*!
 * @fn This is weak function. Can be used in app layer to apply config after sleep
 * @brief Example Init() of peripherals
 * @return none
 */
void sleep_manager_PostSleepOps(void);

#endif /* INC_SLEEP_MANAGER_H_ */
