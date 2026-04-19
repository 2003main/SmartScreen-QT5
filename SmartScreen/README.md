# SmartScreen Qt Demo

基于 Qt5 的智慧屏项目，移植自 LVGL8 版本，支持 Ubuntu 本地调试和 全志T113 交叉编译部署。

## 功能
- 🍅 **番茄时钟** — 25分钟专注 + 5分钟休息，支持暂停/重置/自动循环
- 🌤 **天气显示** — HTTP请求 OpenWeatherMap，JSON解析，10分钟自动刷新
- ⚙ **系统设置** — 调整时长、城市、API Key
- ⌨ **快捷键** — F1/F2/F3切换页面，空格控制番茄钟，方向键物理按键支持

---

## Ubuntu 本地编译（调试用）

```bash
sudo apt install qt5-default qtbase5-dev cmake build-essential

mkdir build && cd build
cmake ..
make -j4
./SmartScreen
```

---

## T113 交叉编译

### 1. 准备工具链
```bash
# 安装ARM交叉编译工具链
sudo apt install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf

# 或使用全志SDK自带工具链（路径按实际修改）
export PATH=$PATH:/path/to/t113/toolchain/bin
```

### 2. 交叉编译 Qt5（一次性）
```bash
# 下载Qt5源码后配置（关键参数）
./configure \
  -prefix /opt/qt5-t113 \
  -xplatform linux-arm-gnueabihf-g++ \
  -no-opengl \
  -linuxfb \         # 使用framebuffer
  -no-xcb \
  -nomake examples \
  -nomake tests \
  -opensource -confirm-license

make -j4
make install
```

### 3. 编译本项目
```bash
mkdir build-t113 && cd build-t113
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-t113.cmake
make -j4
```

### 4. 部署到T113
```bash
# 拷贝到板子
scp SmartScreen root@192.168.1.x:/usr/local/bin/

# 板子上运行（framebuffer模式，无需X11）
export QT_QPA_PLATFORM=linuxfb:fb=/dev/fb0
export QT_QPA_FONTDIR=/usr/share/fonts
./SmartScreen
```

---

## 快捷键说明

| 按键 | 功能 |
|------|------|
| F1 | 切换到番茄时钟页 |
| F2 | 切换到天气页 |
| F3 | 切换到设置页 |
| 空格 | 开始/暂停番茄钟 |
| Esc | 重置番茄钟 |
| ← | 番茄钟页（物理按键） |
| → | 天气页（物理按键） |
| Enter | 开始/暂停（物理按键） |

---

## 天气API配置

1. 前往 https://openweathermap.org/api 注册免费账号
2. 获取 API Key（免费版支持60次/分钟）
3. 在设置页填入 API Key 和城市名（英文）
4. 不填 Key 时显示模拟数据，方便本地调试

---

## 与LVGL版本对比

| 功能 | LVGL版本 | Qt版本 |
|------|---------|--------|
| 番茄时钟 | ✅ | ✅ |
| 天气HTTP | ✅ Socket手写 | ✅ QNetworkAccessManager |
| 多页面切换 | ✅ lv_obj_clean | ✅ QStackedWidget |
| 快捷键 | GPIO中断 | QShortcut + keyPressEvent |
| 内存优化 | 手动管理 | Qt对象树自动管理 |
