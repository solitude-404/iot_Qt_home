# 智能家居监控系统

基于 Qt 框架开发的智能家居监控系统，支持温湿度监测、设备控制与历史数据管理。

## 功能特性

### 温湿度监测
- 实时温湿度显示（通过串口获取 DHT11 传感器数据）
- 温湿度曲线图表展示
- 温度阈值设置（上限/下限）
- 继电器自动控制（加热/冷却）

### 设备控制
- **空调控制**：开关、温度调节
- **灯光控制**：总开关 + 分区控制、亮度调节
- **窗帘控制**：开关
- **门禁控制**：开门/关门

### 模式切换
- 舒适模式：26°C，灯光50%，窗帘打开
- 睡眠模式：24°C，灯光关闭，窗帘关闭
- 观影模式：25°C，灯光关闭，窗帘关闭
- 手动模式：用户自定义

### 历史数据
- 温湿度历史记录（支持日期筛选、分页查询）
- 操作日志记录（支持设备名、日期筛选）

## 技术栈

- **框架**：Qt 6.5.3
- **编译器**：MinGW 64-bit
- **数据库**：SQLite
- **通信**：串口 UART (9600 波特率)

## 项目结构

```
Qt/
├── main.cpp              # 程序入口
├── mainwindow.cpp/h      # 主窗口（温湿度监测界面）
├── homepage.cpp/h        # 主页控制面板
├── camerawidget.cpp/h   # 摄像头界面
├── dbhelper.cpp/h        # 数据库操作
├── gaugewidget.cpp/h    # 仪表盘组件
├── res.qrc               # 资源文件（图标）
└── build/                # 编译输出目录
```

## 串口通信协议

单片机发送数据格式：
```
T:25.0C H:60%TH:31,TL:29    # 温度、湿度、阈值
Heater ON                   # 加热器开启
Cooler ON                   # 冷却器开启
Temperature normal          # 温度正常
Relays OFF                  # 继电器关闭
Alarm ON / Alarm OFF        # 警报状态
```

## 编译运行

1. 使用 Qt Creator 打开 `.pro` 文件
2. 选择 Kits > Desktop Qt 6.5.3 MinGW 64-bit
3. 点击构建 > 运行

或使用命令行：
```bash
qmake . -spec win32-g++ CONFIG+=debug
mingw32-make
```

## 数据库

- 位置：`build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/debug/smart_home.db`
- 表：
  - `device_status`：设备当前状态
  - `temp_humidity_history`：温湿度历史记录
  - `operation_log`：操作日志

## 注意事项

- 串口波特率需设置为 9600
- 资源文件（SVG图标）需正确加载
- 首次运行会自动初始化数据库
