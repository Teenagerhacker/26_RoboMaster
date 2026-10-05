/**
  ******************************************************************************
  * @file    app.h
  * @brief   作业业务代码入口。main.c 只在这里声明、调用初始化，
  *          具体业务逻辑全部放在 Tasks 里。
  ******************************************************************************
  */
#ifndef __APP_H__
#define __APP_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 任务初始化：点亮板载 LED，并按题目需要启动定时器更新中断 */
void Tasks_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __APP_H__ */
