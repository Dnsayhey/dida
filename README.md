# DIDA

一个基于 `AirM2M Core ESP32-C3 + Arduino + LVGL + ST7789` 的小屏天气设备项目。

主流程已经可以编译、烧录并在实机上显示中文天气页面。项目面向无触摸屏的小屏设备，交互围绕单个实体按钮设计。

## 项目概览

- 硬件平台：`AirM2M Core ESP32-C3`
- 显示方案：`240x320 ST7789 TFT`
- 输入方式：无触摸屏，单个外接实体按钮
- UI 框架：`LVGL 9`
- 构建工具：`PlatformIO`
- 网络能力：连接 Wi-Fi 后获取天气和时间

## 当前功能

- 启动后初始化显示、LVGL、页面系统、按钮和背光
- 读取本地 NVS 配置
- 无 Wi-Fi 配置时启动 SoftAP 配网页
- 自动连接已保存的 Wi-Fi
- 通过和风天气 API 获取实时天气和 7 日天气
- 通过 NTP 同步时间
- 单按键完成天气页、亮度与主题页、恢复出厂页之间的切换
- 根据光敏传感器自动调节背光
- 支持手动切换背光档位
- 支持浅色/深色主题切换
- 支持长按恢复出厂设置
- 使用项目专用中文 UI 字库显示中文页面文案

## 硬件连接

### TFT 显示屏引脚

- `GPIO 2`：`TFT_SCLK`
- `GPIO 3`：`TFT_MOSI`
- `GPIO 4`：`TFT_DC`
- `GPIO 5`：`TFT_RST`
- `GPIO 7`：`TFT_CS`

### 其他功能引脚

- `GPIO 0`：光线传感器输入
- `GPIO 6`：TFT 背光 PWM 输出
- `GPIO 8`：按钮输入，项目外接按键，不是开发板板载 BOOT/RST
- `GPIO 12`：开发板指示灯

### 显示屏规格

- 分辨率：`240x320`
- 驱动芯片：`ST7789`
- 颜色格式：`TFT_BGR`
- 触摸能力：无触摸

## 软件架构

项目当前按职责分成几层：

1. 入口层
   [src/main.cpp](/Users/yanlei/Projects/c/dida/src/main.cpp:1) 只负责串口初始化、启动 `AppRuntime` 和创建两个 FreeRTOS 任务。
2. 应用层
   [src/app](/Users/yanlei/Projects/c/dida/src/app) 负责编排运行时、启动流程、应用状态、按钮命令和页面展示数据。
3. 固定硬件配置
   [src/board/board_config.h](/Users/yanlei/Projects/c/dida/src/board/board_config.h:1) 集中保存开发板引脚、Wi-Fi 超时和周期任务间隔等常量。
4. 硬件能力层
   [src/hardware](/Users/yanlei/Projects/c/dida/src/hardware) 封装显示设备、实体按钮、背光 PWM、光敏读取和光敏背光策略。
5. 页面系统
   [src/pages](/Users/yanlei/Projects/c/dida/src/pages) 负责页面创建、显示隐藏和当前页面内部事件处理。
6. 底层服务
   [src/config](/Users/yanlei/Projects/c/dida/src/config)、[src/net](/Users/yanlei/Projects/c/dida/src/net)、[src/weather](/Users/yanlei/Projects/c/dida/src/weather)、[src/system](/Users/yanlei/Projects/c/dida/src/system) 分别负责 NVS、网络/HTTP、天气接口和周期任务。

## 启动流程

上电后主要流程如下：

1. `setup()` 初始化串口并调用 `AppRuntime::begin()`
2. `AppRuntime::begin()` 初始化应用状态和显示设备；如果启用 `DEV_MODE`，会额外注入本地开发默认配置
3. 创建 `lvgl_task`
   负责页面初始化、按钮轮询、背光任务和 LVGL timer
4. 创建 `init_and_api_task`
   负责 SoftAP 配网、Wi-Fi 连接、城市 ID 确认、天气/NTP 周期任务注册和启动后页面切换
5. 周期任务立即执行一次天气同步，然后按配置间隔继续运行

更详细的真实状态见 [docs/当前实现状态.md](/Users/yanlei/Projects/c/dida/docs/当前实现状态.md:1)。

## 页面说明

当前定义了以下页面：

- `PAGE_INIT`：初始化页面
- `PAGE_NETWORK_SETUP`：SoftAP 配网页，显示热点和访问地址
- `PAGE_REAL_TIME_WEATHER`：实时天气页
- `PAGE_FUTURE_WEATHER`：未来天气页
- `PAGE_THEME_SETTINGS`：亮度与主题页
- `PAGE_FACTORY_RESET`：恢复出厂页

### 按键交互

按钮事件先由 `AppController` 转成 `AppCommand`，再由 `AppCommandExecutor` 落到 `PageManager`。

- 单击：
  在实时天气页和未来天气页之间切换；设置页中用于切换亮度模式。
- 双击：
  天气页进入亮度与主题页，亮度与主题页进入恢复出厂页，恢复出厂页返回实时天气页。
- 长按：
  亮度与主题页切换浅色/深色主题；恢复出厂页执行恢复出厂设置。

完整按钮行为见 [docs/当前实现状态.md](/Users/yanlei/Projects/c/dida/docs/当前实现状态.md:1)。

## 目录结构

```text
dida/
├── include/                # 屏幕与 LVGL 配置头文件
├── src/
│   ├── app/                # 应用编排、状态、配置语义、天气同步和展示数据
│   ├── board/              # 固定硬件和周期常量
│   ├── config/             # NVS 配置读写
│   ├── hardware/           # 显示、按钮、背光、光敏等硬件能力封装
│   ├── net/                # Wi-Fi、网络状态和 HTTP 请求
│   ├── pages/              # 页面实现
│   ├── sources/font/       # 字体资源
│   ├── system/             # 周期任务管理
│   ├── weather/            # 天气服务、响应解析和数据结构
│   └── main.cpp            # Arduino 入口
├── docs/                   # 项目说明文档
├── platformio.ini          # PlatformIO 配置
└── README.md
```

## 构建与烧录

建议安装：

- [PlatformIO Core](https://docs.platformio.org/)
- 或 VS Code + PlatformIO 插件

常用命令：

```bash
pio run
pio run -t upload
pio device monitor
```

当前默认环境定义在 [platformio.ini](/Users/yanlei/Projects/c/dida/platformio.ini:1)：

- `platform = espressif32@6.6.0`
- `board = airm2m_core_esp32c3`
- `framework = arduino`

## 配置说明

### NVS 配置项

项目会把以下内容保存在 NVS 中：

- Wi-Fi SSID
- Wi-Fi 密码
- 地区信息 `adm`
- 位置名称 `location`
- 和风天气位置 ID `location_id`
- 背光模式
- 主题模式

底层读写入口是 [src/config/config_manager.h](/Users/yanlei/Projects/c/dida/src/config/config_manager.h:1)。应用层代码优先通过 `app/DeviceConfigProvider` 和 `app/DeviceSettingsStore` 访问这些配置语义。

### 开发模式

[platformio.ini](/Users/yanlei/Projects/c/dida/platformio.ini:1) 默认没有启用 `DEV_MODE`。如果需要在开发烧录时自动补充默认 Wi-Fi 和城市配置，可以临时取消下面这行注释：

```ini
-D DEV_MODE=1
```

启用后，启动时会通过 `DeviceConfigProvider` 应用开发默认配置。当前策略是只为缺失项补默认值，不无条件覆盖已存在的设备配置。

开发默认 Wi-Fi 和城市可复制 [include/dev_config_local.example.h](/Users/yanlei/Projects/c/dida/include/dev_config_local.example.h:1) 为被 Git 忽略的 `include/dev_config_local.h`。如果本地未配置这些值，`DEV_MODE` 会跳过空默认值，设备会进入 SoftAP 配网页。

首次配网时，设备会显示 `DIDA-xxxx` 热点名、热点密码 `12345678` 和 `192.168.4.1`。手机连接该热点后打开浏览器，填写 Wi-Fi、密码和天气位置即可保存配置。

### 天气接口

天气数据来自和风天气 API，相关逻辑位于 [src/weather/weather_service.cpp](/Users/yanlei/Projects/c/dida/src/weather/weather_service.cpp:1)。

当前实现里：

- API Base URL 和 API Key 通过 `QWEATHER_API_BASE_URL` / `QWEATHER_API_KEY` 配置
- 本地开发可复制 [include/weather_config_local.example.h](/Users/yanlei/Projects/c/dida/include/weather_config_local.example.h:1) 为被 Git 忽略的 `include/weather_config_local.h`
- 返回内容会先解压，再用 `ArduinoJson` 解析

如果没有配置天气接口，固件仍可编译，天气同步会进入失败状态并在串口输出配置缺失提示。

## 字体说明

LVGL 默认 Montserrat 字体不覆盖中文。项目当前使用 [src/sources/font/ui_zh_20.c](/Users/yanlei/Projects/c/dida/src/sources/font/ui_zh_20.c:1) 作为中文 UI 小字库。

新增静态中文文案时，需要同步更新字库字符表并重新生成字体。生成命令记录在 [src/sources/font/font_convert.md](/Users/yanlei/Projects/c/dida/src/sources/font/font_convert.md:1)。

## 当前状态

当前已经可用：

- 页面样式已经统一成适配 240x320 无触摸屏的中文卡片风格
- SoftAP 配网、天气同步、中文显示、按钮切页、亮度模式和明暗主题已经可用
- 恢复出厂页已支持长按清理 NVS

仍需继续产品化：

- 天气同步状态还不能区分网络失败、解析失败和无数据
- 配网失败后的设备端反馈和重试入口还可以继续优化
- 当前还没有自动测试用例

## 相关文档

建议先看文档导航：

- [docs/README.md](/Users/yanlei/Projects/c/dida/docs/README.md:1)
- [docs/硬件适配说明.md](/Users/yanlei/Projects/c/dida/docs/硬件适配说明.md:1)
- [docs/当前实现状态.md](/Users/yanlei/Projects/c/dida/docs/当前实现状态.md:1)

当前实际行为仍应以 `src/` 下代码为准。
