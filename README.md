# ADC 多通道采集与 PWM 波形输出系统

基于 STC89C52RC 单片机的 ADC 数据采集、串口通信与 PWM 方波输出系统。

## 功能

- 读取 XPT2046 四通道 ADC 数据（VBAT / AUX / XP / YP）
- 通过串口以 JustFloat 协议发送至上位机（VOFA+），实时显示波形
- 中断接收串口命令，动态修改 PWM 参数（频率 / 占空比）
- P1.0 引脚输出频率和占空比可调的方波，可用于验证 ADC 功能

## 硬件环境

| 项目 | 型号 |
|------|------|
| MCU | STC89C52RC |
| 晶振 | 11.0592MHz |
| ADC | XPT2046（SPI，P3.4-P3.7） |
| 串口 | 9600bps，P3.0(RXD) / P3.1(TXD) |
| PWM 输出 | P1.0 |

## 项目结构

```
├── main.c              # 主程序
├── Src/
│   ├── XPT2046.c       # ADC 驱动（位Bang SPI）
│   ├── uart.c          # 串口驱动（中断接收）
│   ├── PWM.c           # PWM 输出（Timer0）
│   └── strutils.c      # 字符串工具
├── Inc/
│   ├── XPT2046.h
│   ├── uart.h
│   ├── PWM.h
│   └── strutils.h
├── STARTUP.A51         # 8051 启动文件
└── upper_computer.py   # Python 上位机（备用）
```

## 通信协议

### 下行（MCU → PC）

JustFloat 二进制格式，4 通道 float + 帧尾，共 20 字节：

```
[ch0:float][ch1:float][ch2:float][ch3:float][0x00][0x00][0x80][0x7F]
```

### 上行（PC → MCU）

ASCII 文本命令，以 `\n` 结尾：

| 命令 | 示例 | 说明 |
|------|------|------|
| `F=<频率>\n` | `F=1000\n` | 设置 PWM 频率（Hz） |
| `D=<占空比>\n` | `D=75\n` | 设置 PWM 占空比（%） |

## 使用方法

1. Keil C51 或 EIDE 编译烧录
2. VOFA+ 连接串口，协议选 **JustFloat**，波特率 9600
3. 添加 4 个 float 通道即可看到波形
4. 发送 `F=500\n`、`D=25\n` 等命令控制 PWM 输出

## 开发工具

- Keil C51 V9.60
- EIDE (VS Code 插件)
- VOFA+ 上位机
