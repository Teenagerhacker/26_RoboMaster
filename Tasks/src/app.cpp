/**
  ******************************************************************************
  * @file    app.cpp
  * @brief   电控第一次作业：三题的业务代码（GPIO / 定时器 / 看门狗）。
  *
  *   题目1  GPIO   ：点亮板载 LED（F103 最小系统板 PC13，低电平点亮）。
  *   题目2  定时器 ：TIM2 更新中断周期 1ms，tick 自增，并在回调里喂狗。
  *   题目3  看门狗 ：TIM2 仍 1ms 自增 tick，但回调里不喂狗，芯片反复复位。
  *
  *   三题共用本文件，通过 HOMEWORK_TASK 宏切换，见下方注释。
  ******************************************************************************
  */
#include "app.h"
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "iwdg.h"

/* ==================== 作业题目切换 ====================
 *  1 = GPIO   点灯（不启动定时器、不喂狗 → 芯片每 ~2s 复位）
 *  2 = 定时器 （1ms 中断 tick 自增 + 喂狗 → tick 持续增长）
 *  3 = 看门狗 （1ms 中断 tick 自增、不喂狗 → tick 涨到 ~2000 后归零）
 * ====================================================== */
#define HOMEWORK_TASK 2

/* 全局 tick：在 1ms 定时器中断里自增，定义成 volatile uint32_t 供 Ozone 观察 */
volatile uint32_t tick = 0;

/**
  * @brief  任务初始化。
  *         - 点亮板载 LED（PC13 拉低）
  *         - 题目2/3 需要启动 TIM2 更新中断（周期 1ms）
  */
void Tasks_Init(void)
{
  /* GPIO：PC13 拉低，点亮板载 LED */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

#if HOMEWORK_TASK != 1
  /* 启动 TIM2 更新中断，周期 1ms */
  HAL_TIM_Base_Start_IT(&htim2);
#endif
}

/**
  * @brief  定时器更新中断回调（全工程只此一份，放在 Tasks 里）。
  * @note   用 C++ 写必须加 extern "C"，否则与 HAL 里的弱符号链接不上。
  */
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    tick = tick + 1;

#if HOMEWORK_TASK == 2
    /* 题目2：喂狗；题目3 去掉这行（不喂狗 → 超时复位，tick 被清零） */
    HAL_IWDG_Refresh(&hiwdg);
#endif
  }
}
