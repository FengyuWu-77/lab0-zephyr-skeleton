# Lab 0 Zephyr 实验设计

## 目标

在一个 GitHub 仓库中完成 ESE 5180 Lab 0 的 Zephyr/Nordic 实验，覆盖三块 MCU 板的 Blinky、West 构建流程、Kconfig、Device Tree、printk/Logger、Ztest，以及 nRF7002 DK 上的 BME280 I²C 读取，并准备课程要求的 README、截图、视频、GitHub 和 Gradescope 交付物。

## 用户请求与课程要求的边界

用户请求是“带我一步步完成 Lab 0”。课程 PDF 中的章节要求、板卡目标、命令、截图、视频、提交位置和 checkoff 属于实验原文；本设计将这些要求拆成可验证的阶段。每一阶段只在上一阶段的构建或硬件验证通过后继续。

## 工具链与目标板

- upstream Zephyr workspace：用于 vanilla Zephyr 的通用构建和 QEMU 测试。
- Nordic nRF Connect SDK v3.4.x：用于 nRF7002 DK 的 `ns` 目标、Nordic 示例和 BME280 硬件实验。
- XIAO nRF52840 Sense 的目标名：`xiao_ble/nrf52840/sense`。
- STM32 Nucleo F411RE 的目标名：`nucleo_f411re`。
- nRF7002 DK 的课程目标名：`nrf7002dk/nrf5340/cpuapp/ns`。
- QEMU 目标：`qemu_cortex_m3`，用于主机侧 Ztest/Twister 验证。

已有的 upstream Zephyr workspace 和 Nordic NCS 必须分开核对版本，避免使用 upstream Zephyr 的 board target 替代课程指定的 NCS `ns` target。

## 仓库结构

仓库根目录为 `/Users/ericcc/Desktop/ESE 5180/lab0-zephyr-skeleton`。实验应用和测试代码放在仓库内；构建目录、生成文件和本地工具链缓存不提交。README 记录每个章节的命令、板卡、验证结果、截图索引和提交信息。

后续代码结构按实验需要逐步形成：

```text
lab0-zephyr-skeleton/
├── src/
├── boards/
├── sum_printk/
├── sum_log/
├── tests/
├── prj.conf
├── Kconfig
├── CMakeLists.txt
├── testcase.yaml
└── README.md
```

## 分阶段设计

### 阶段 0：环境基线

确认 Python 3.12、West、CMake、Ninja、QEMU、Zephyr workspace、NCS v3.4.x、VS Code 扩展以及 Git 状态。记录版本和路径，不修改用户已有的 Zephyr workspace。

验收标准：每个工具都能报告版本；课程仓库处于可追踪的 Git 分支；NCS 能识别课程指定的 nRF7002 DK target。

### 阶段 1：Vanilla Zephyr Blinky

从 Zephyr Blinky sample 开始，为 XIAO、Nucleo 和 nRF7002 DK 分别建立 build。先验证构建，再在对应硬件上烧录和观察 LED。为视频保留完整的构建/烧录终端历史。

验收标准：三个 target 的 build 成功；有硬件连接时三个板都能运行 Blinky；README 记录 target、build 命令、flash 命令和观察结果。

### 阶段 2：Nordic Blinky 修改

在 Nordic NCS 应用中把 Blinky 的睡眠时间改为 2 秒。先保存原始行为的证据，再重新构建、烧录并记录修改后的行为。

验收标准：nRF7002 DK 能展示原始速率和 2 秒速率的差异；源代码和构建结果提交到仓库；视频文件名符合课程要求。

### 阶段 3：West 工作流

使用 `west build`、`west flash` 和必要的帮助命令复现构建/烧录，不依赖 GUI 完成核心流程。README 逐项解释位置参数和 flags，并嵌入终端截图。

验收标准：命令可在正确的 Zephyr/NCS 环境中复现；README 能解释 `west init`、`west update`、`west build`、`west flash` 的作用。

### 阶段 4：Kconfig 与求和实现

将求和实现拆为 `sum_printk` 和 `sum_log` 两个模块。顶层 Kconfig 定义互斥 choice，默认启用 `CONFIG_SUM_PRINT`；CMake 根据 Kconfig 只编译被选择的模块。`prj.conf` 配置 console、UART、Logger 和默认日志等级。

验收标准：`CONFIG_SUM_PRINT=y` 构建只使用 printk 实现；`CONFIG_SUM_LOG=y` 构建只使用 Logger 实现，并输出不同严重级别和输入值 hexdump；两种结果都有截图。

### 阶段 5：Device Tree alias 与按键

在板级 overlay 中定义 `LED5180` alias，使 `main.c` 只通过 alias 访问 LED2；随后定义 button alias，使程序轮询按键并切换 LED 状态。overlay 文件名与课程 target 的 board 名一致，并放在应用的 `boards/` 目录。

验收标准：C 代码不硬编码 LED/button 的具体 GPIO；overlay 被实际加载；LED2 和按键行为在 nRF7002 DK 上验证。

### 阶段 6：Logger 与输出验证

对比 `printk()` 和 Logger 的阻塞/过滤/后端特性。为 Logger 实现加入日志等级和 hexdump；使用串口终端记录两种 Kconfig 选择的输出，并制作课程要求的视频。

验收标准：两个配置均能独立构建和运行；Logger 输出包含 hexdump；README 解释 Logger 与 printk 的主要差异以及 deferred logging 的适用性。

### 阶段 7：Ztest/Twister

在 `tests/SUM_UNIT_TEST` 中加入 Ztest suite，覆盖正数、负数和零等基本求和场景。使用 QEMU 运行单个测试和 Twister，必要时再加入 nRF7002 DK 作为允许平台。

验收标准：测试源码、`prj.conf`、`testcase.yaml` 和 CMake 完整；QEMU/Twister 输出通过；README 解释 Ztest 的启动机制以及 `west build` 与 `west twister` 的适用场景。

### 阶段 8：BME280 I²C

在 overlay 中打开 `i2c1`，使用 nRF7002 DK 的 P1.14/P1.15 配置 SCL/SDA，创建地址为 `0x77` 的 I²C 节点。启用 `CONFIG_SENSOR` 和 `CONFIG_I2C`，通过 Zephyr I²C API 直接写入 `CTRL_MEAS`，再从温度寄存器读取原始数据并按 BME280 校准参数换算。

硬件连接固定为：BME280 VCC→nRF7002 DK VDD（1.8 V）、GND→GND、SDA→P1.15、SCL→P1.14。连接前确认传感器模块支持 1.8 V 供电和逻辑电平。

验收标准：串口日志持续输出合理的温度值；完成一次硬件日志截图；代码提交到仓库；另有不依赖硬件的 Device Tree sanity Ztest，使用 mock 或 FFF 验证节点和基本逻辑。

### 阶段 9：交付与 checkoff

逐条核对课程绿色高亮要求：GitHub commit、README 截图、1.1/2.1/6.2 视频、Gradescope、BME280 输出、Ztest 输出和 Teaching Team checkoff。最终 README 保留命令、板卡、配置、日志和截图的对应关系。

## 错误处理与安全边界

- 如果某个 target 在配置阶段失败，先确认实际使用的 Zephyr/NCS workspace 和 board target，再检查 Kconfig/Device Tree，不直接修改生成文件。
- 如果烧录失败，先确认数据线、板卡供电、调试器连接和串口设备；硬件问题与软件问题分开记录。
- BME280 连接前必须确认 1.8 V 电源和逻辑电平，不能把 3.3 V 信号直接接到课程指定的 1.8 V 接口。
- 所有构建目录和生成文件默认放在仓库外或 `.gitignore` 中；只提交源码、配置、README 和课程要求的证据。

## 分阶段提交策略

每个主要阶段完成后单独提交：

```text
01 vanilla blinky
02 nordic blinky
03 west workflow
04 kconfig sum
05 devicetree aliases
06 logging
07 ztest
08 bme280
```

每次提交前检查 `git diff`、`git status`、构建日志和 README 证据是否一致；未通过验证的阶段不标记为完成。

## 不在本阶段范围内的内容

- 不修改实验 PDF、课程服务器或 Gradescope 内容。
- 不把课程要求的旧版示例扩展成额外功能。
- 不替换课程指定的 nRF7002 DK `ns` target。
- 不在没有硬件电气确认时尝试 BME280 接线。
