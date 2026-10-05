# 电控第一次作业（STM32F103）

RoboMaster 电控组第一次作业，基于 STM32F103C8Tx 最小系统板，CubeMX + CMake + Ninja + Ozone。

## 题目

| 题号 | 内容 | 关键点 |
|------|------|--------|
| 1 | GPIO | PC13 推挽输出，低电平点亮板载 LED |
| 2 | 定时器 | TIM2 更新中断周期 1ms，`tick` 自增，回调里喂狗 |
| 3 | 看门狗 | IWDG 超时约 2s，回调里不喂狗，芯片反复复位 |

三题共用一套代码，通过 `Tasks/src/app.cpp` 里的 `HOMEWORK_TASK` 宏切换（`1`/`2`/`3`）。

## 目录结构

```
├── CMakeLists.txt            # 发放的 CMake 模板（user_folders="Tasks"）
├── CMakePresets.json         # Debug/Release 构建预设
├── cube_practice.ioc         # CubeMX 工程配置
├── cmake/
│   ├── gcc-arm-none-eabi.cmake
│   └── stm32cubemx/CMakeLists.txt
├── Core/                     # CubeMX 生成的外设初始化
├── Drivers/                  # STM32F1xx HAL + CMSIS
├── startup_stm32f103xb.s
├── STM32F103xx_FLASH.ld      # 链接脚本（64KB Flash / 20KB RAM）
└── Tasks/                    # 业务代码（题目逻辑都在这里）
    ├── inc/app.h
    └── src/app.cpp
```

## 构建

工具链：CMake ≥ 3.22 + `arm-none-eabi-gcc` + Ninja。

```shell
cmake --preset Debug
cmake --build --preset Debug
```

产物：`build/cube_practice.elf`（用 Ozone 打开调试）。

## 关键配置

- 时钟：HSE 8MHz × PLL9 = 72MHz；APB1 = 36MHz，APB1 定时器时钟 = 72MHz。
- 定时器：TIM2，`PSC = 72-1`，`ARR = 1000-1` → 1ms 更新中断。
- 看门狗：IWDG，分频 64，`Reload = 1249`，LSI ≈ 40kHz → 超时约 2s。

## 三题现象

1. **GPIO**：LED 常亮（不喂狗，芯片每 ~2s 复位，LED 会短暂熄灭后重新点亮）。
2. **定时器**：`tick` 每秒约 +1000，持续增长（喂狗）。
3. **看门狗**：`tick` 从 0 涨到约 2000 后归零，反复循环（不喂狗复位）。

## 预编译固件

`firmware/` 目录下是三题各自编译好的 `.elf` 文件，可直接用 Ozone 烧录，无需重新编译：

| 文件 | 题目 | 现象 |
|------|------|------|
| `task1_gpio.elf` | 1 GPIO | LED 亮 |
| `task2_timer.elf` | 2 定时器 | tick 每秒约 +1000 |
| `task3_watchdog.elf` | 3 看门狗 | tick 涨到约 2000 归零 |

> 若修改了源码，重新 `cmake --build --preset Debug` 编译后再替换对应的 `.elf`。
