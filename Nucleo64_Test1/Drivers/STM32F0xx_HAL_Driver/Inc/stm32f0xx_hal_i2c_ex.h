/**
  ******************************************************************************
  * @file    stm32f0xx_hal_i2c_ex.h
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    27-May-2016
  * @brief   Header file of I2C HAL Extended module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F0xx_HAL_I2C_EX_H
#define __STM32F0xx_HAL_I2C_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal_def.h"  

/** @addtogroup STM32F0xx_HAL_Driver
  * @{
  */

/** @addtogroup I2CEx
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/

/** @defgroup I2CEx_Exported_Constants I2C Extended Exported Constants
  * @{
  */

/** @defgroup I2CEx_Analog_Filter I2C Extended Analog Filter
  * @{
  */
#define I2C_ANALOGFILTER_ENABLE         (0x00000000U)
#define I2C_ANALOGFILTER_DISABLE        I2C_CR1_ANFOFF
/**
  * @}
  */

/** @defgroup I2CEx_FastModePlus I2CEx Fast Mode Plus
  * @{
  */
#if defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F042x6) || defined(STM32F048xx)
#define I2C_FASTMODEPLUS_PA9            SYSCFG_CFGR1_I2C_FMP_PA9  /*!< Enable Fast Mode Plus on PA9       */
#define I2C_FASTMODEPLUS_PA10           SYSCFG_CFGR1_I2C_FMP_PA10 /*!< Enable Fast Mode Plus on PA10      */
#define I2C_FASTMODEPLUS_PB6            SYSCFG_CFGR1_I2C_FMP_PB6  /*!< Enable Fast Mode Plus on PB6       */
#define I2C_FASTMODEPLUS_PB7            SYSCFG_CFGR1_I2C_FMP_PB7  /*!< Enable Fast Mode Plus on PB7       */
#define I2C_FASTMODEPLUS_PB8            SYSCFG_CFGR1_I2C_FMP_PB8  /*!< Enable Fast Mode Plus on PB8       */
#define I2C_FASTMODEPLUS_PB9            SYSCFG_CFGR1_I2C_FMP_PB9  /*!< Enable Fast Mode Plus on PB9       */
#define I2C_FASTMODEPLUS_I2C1           SYSCFG_CFGR1_I2C_FMP_I2C1 /*!< Enable Fast Mode Plus on I2C1 pins */
#define I2C_FASTMODEPLUS_I2C2           SYSCFG_CFGR1_I2C_FMP_I2C2 /*!< Enable Fast Mode Plus on I2C2 pins */
#elif defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)
#define I2C_FASTMODEPLUS_PB6            SYSCFG_CFGR1_I2C_FMP_PB6  /*!< Enable Fast Mode Plus on PB6       */
#define I2C_FASTMODEPLUS_PB7            SYSCFG_CFGR1_I2C_FMP_PB7  /*!< Enable Fast Mode Plus on PB7       */
#define I2C_FASTMODEPLUS_PB8            SYSCFG_CFGR1_I2C_FMP_PB8  /*!< Enable Fast Mode Plus on PB8       */
#define I2C_FASTMODEPLUS_PB9            SYSCFG_CFGR1_I2C_FMP_PB9  /*!< Enable Fast Mode Plus on PB9       */
#define I2C_FASTMODEPLUS_I2C1           SYSCFG_CFGR1_I2C_FMP_I2C1 /*!< Enable Fast Mode Plus on I2C1 pins */
#define I2C_FASTMODEPLUS_I2C2           SYSCFG_CFGR1_I2C_FMP_I2C2 /*!< Enable Fast Mode Plus on I2C2 pins */
#elif defined(STM32F030x6) || defined(STM32F031x6) || defined(STM32F038xx) || defined(STM32F070x6) || defined(STM32F030xC)
#define I2C_FASTMODEPLUS_PA9            SYSCFG_CFGR1_I2C_FMP_PA9  /*!< Enable Fast Mode Plus on PA9       */
#define I2C_FASTMODEPLUS_PA10           SYSCFG_CFGR1_I2C_FMP_PA10 /*!< Enable Fast Mode Plus on PA10      */
#define I2C_FASTMODEPLUS_PB6            SYSCFG_CFGR1_I2C_FMP_PB6  /*!< Enable Fast Mode Plus on PB6       */
#define I2C_FASTMODEPLUS_PB7            SYSCFG_CFGR1_I2C_FMP_PB7  /*!< Enable Fast Mode Plus on PB7       */
#define I2C_FASTMODEPLUS_PB8            SYSCFG_CFGR1_I2C_FMP_PB8  /*!< Enable Fast Mode Plus on PB8       */
#define I2C_FASTMODEPLUS_PB9            SYSCFG_CFGR1_I2C_FMP_PB9  /*!< Enable Fast Mode Plus on PB9       */
#define I2C_FASTMODEPLUS_I2C1           SYSCFG_CFGR1_I2C_FMP_I2C1 /*!< Enable Fast Mode Plus on I2C1 pins */
#elif defined(STM32F070xB)
#define I2C_FASTMODEPLUS_PB6            SYSCFG_CFGR1_I2C_FMP_PB6  /*!< Enable Fast Mode Plus on PB6       */
#define I2C_FASTMODEPLUS_PB7            SYSCFG_CFGR1_I2C_FMP_PB7  /*!< Enable Fast Mode Plus on PB7       */
#define I2C_FASTMODEPLUS_PB8            SYSCFG_CFGR1_I2C_FMP_PB8  /*!< Enable Fast Mode Plus on PB8       */
#define I2C_FASTMODEPLUS_PB9            SYSCFG_CFGR1_I2C_FMP_PB9  /*!< Enable Fast Mode Plus on PB9       */
#define I2C_FASTMODEPLUS_I2C1           SYSCFG_CFGR1_I2C_FMP_I2C1 /*!< Enable Fast Mode Plus on I2C1 pins */
#else /* defined(STM32F030x8) || defined(STM32F051xx) || defined(STM32F058xx) */
#define I2C_FASTMODEPLUS_PB6            SYSCFG_CFGR1_I2C_FMP_PB6  /*!< Enable Fast Mode Plus on PB6       */
#define I2C_FASTMODEPLUS_PB7            SYSCFG_CFGR1_I2C_FMP_PB7  /*!< Enable Fast Mode Plus on PB7       */
#define I2C_FASTMODEPLUS_PB8            SYSCFG_CFGR1_I2C_FMP_PB8  /*!< Enable Fast Mode Plus on PB8       */
#define I2C_FASTMODEPLUS_PB9            SYSCFG_CFGR1_I2C_FMP_PB9  /*!< Enable Fast Mode Plus on PB9       */
#endif
/**
  * @}
  */
    
/**
  * @}
  */ 
  
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @addtogroup I2CEx_Exported_Functions I2C Extended Exported Functions
  * @{
  */

/** @addtogroup I2CEx_Exported_Functions_Group1 Extended features functions
  * @brief    Extended features functions
  * @{
  */
  
/* Peripheral Control functions  ************************************************/
HAL_StatusTypeDef HAL_I2CEx_ConfigAnalogFilter(I2C_HandleTypeDef *hi2c, uint32_t AnalogFilter);
HAL_StatusTypeDef HAL_I2CEx_ConfigDigitalFilter(I2C_HandleTypeDef *hi2c, uint32_t DigitalFilter);
#if !defined(STM32F030x6) && !defined(STM32F030x8) && !defined(STM32F070x6) && !defined(STM32F070xB) && !defined(STM32F030xC) 
HAL_StatusTypeDef HAL_I2CEx_EnableWakeUp (I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2CEx_DisableWakeUp (I2C_HandleTypeDef *hi2c);
#endif /* !(STM32F030x6) && !(STM32F030x8) && !(STM32F070x6) && !(STM32F070xB) && !(STM32F030xC)  */
void HAL_I2CEx_EnableFastModePlus(uint32_t ConfigFastModePlus);
void HAL_I2CEx_DisableFastModePlus(uint32_t ConfigFastModePlus);

/* Private constants ---------------------------------------------------------*/
/** @defgroup I2C_Private_Constants I2C Private Constants
  * @{
  */

/**
  * @}
  */ 

/* Private macros ------------------------------------------------------------*/
/** @defgroup I2C_Private_Macro I2C Private Macros
  * @{
  */
#define IS_I2C_ANALOG_FILTER(FILTER)    (((FILTER) == I2C_ANALOGFILTER_ENABLE) || \
                                         ((FILTER) == I2C_ANALOGFILTER_DISABLE))

#define IS_I2C_DIGITAL_FILTER(FILTER)   ((FILTER) <= 0x0000000FU)

#if defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F042x6) || defined(STM32F048xx)
#define IS_I2C_FASTMODEPLUS(__CONFIG__) ((((__CONFIG__) & (I2C_FASTMODEPLUS_PA9)) == I2C_FASTMODEPLUS_PA9)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PA10)) == I2C_FASTMODEPLUS_PA10) || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB6)) == I2C_FASTMODEPLUS_PB6)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB7)) == I2C_FASTMODEPLUS_PB7)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB8)) == I2C_FASTMODEPLUS_PB8)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB9)) == I2C_FASTMODEPLUS_PB9)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_I2C1)) == I2C_FASTMODEPLUS_I2C1) || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_I2C2)) == I2C_FASTMODEPLUS_I2C2))
#elif defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)
#define IS_I2C_FASTMODEPLUS(__CONFIG__) ((((__CONFIG__) & (I2C_FASTMODEPLUS_PB6)) == I2C_FASTMODEPLUS_PB6)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB7)) == I2C_FASTMODEPLUS_PB7)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB8)) == I2C_FASTMODEPLUS_PB8)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB9)) == I2C_FASTMODEPLUS_PB9)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_I2C1)) == I2C_FASTMODEPLUS_I2C1) || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_I2C2)) == I2C_FASTMODEPLUS_I2C2))
#elif defined(STM32F030x6) || defined(STM32F031x6) || defined(STM32F038xx) || defined(STM32F070x6) || defined(STM32F030xC)
#define IS_I2C_FASTMODEPLUS(__CONFIG__) ((((__CONFIG__) & (I2C_FASTMODEPLUS_PA9)) == I2C_FASTMODEPLUS_PA9)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PA10)) == I2C_FASTMODEPLUS_PA10) || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB6)) == I2C_FASTMODEPLUS_PB6)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB7)) == I2C_FASTMODEPLUS_PB7)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB8)) == I2C_FASTMODEPLUS_PB8)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB9)) == I2C_FASTMODEPLUS_PB9)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_I2C1)) == I2C_FASTMODEPLUS_I2C1))
#elif defined(STM32F070xB)
#define IS_I2C_FASTMODEPLUS(__CONFIG__) ((((__CONFIG__) & (I2C_FASTMODEPLUS_PB6)) == I2C_FASTMODEPLUS_PB6)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB7)) == I2C_FASTMODEPLUS_PB7)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB8)) == I2C_FASTMODEPLUS_PB8)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB9)) == I2C_FASTMODEPLUS_PB9)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_I2C1)) == I2C_FASTMODEPLUS_I2C1))
#else /* defined(STM32F030x8) || defined(STM32F051xx) || defined(STM32F058xx) */
#define IS_I2C_FASTMODEPLUS(__CONFIG__) ((((__CONFIG__) & (I2C_FASTMODEPLUS_PB6)) == I2C_FASTMODEPLUS_PB6)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB7)) == I2C_FASTMODEPLUS_PB7)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB8)) == I2C_FASTMODEPLUS_PB8)   || \
                                         (((__CONFIG__) & (I2C_FASTMODEPLUS_PB9)) == I2C_FASTMODEPLUS_PB9))
#endif
/**
  * @}
  */
  
/* Private Functions ---------------------------------------------------------*/
/** @defgroup I2C_Private_Functions I2C Private Functions
  * @{
  */ 
/* Private functions are defined in stm32f0xx_hal_i2c_ex.c file */
/**
  * @}
  */   

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
}
#endif

#endif /* __STM32F0xx_HAL_I2C_EX_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

