# N32WB031 模板工程

本项目基于`N32WB031_STB_V1.1`开发板

![](./assets/board.webp)

开发板相关资料见`assets`文件夹。

## 服务

包含以下蓝牙服务

- DIS(`0x180A`)
- BAS(`0x180F`)
- 自定义透传 (`0xCC00`)
  - Notify(`0xCC02`)
  - WoR(`0xCC01`)

## IO

- LED1(`PB0`)
- LED2(`PA6`)
- BUTTON1(`PB1`)
- BUTTON2(`PB2`)
- LOG: USART:115200 (`PB6/PB7`)

## Usage

由于此IC在`Sleep`状态无法访问`SWD`，所以上电后两灯交替闪烁，处于激活状态，方便烧录，按下`BUTTON1`或`BUTTON2`后才开始运行程序。

