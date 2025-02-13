# Atlas Generator

LayaAir Atlas Generator 可以在Linux下编译
Frok from https://gitlab.com/hyzmm/atlas-generator

## 简介
这是一个基于Qt的图集生成器，可以将多个图片打包成一个图集（Atlas/Sprite Sheet），并生成相应的数据文件。该工具支持多种矩形打包算法，可以自动选择最优的打包方案。

## 功能特点

- 支持多种矩形打包算法
- 自动裁切图片空白区域
- 支持边缘扩展
- 支持导出JSON格式的数据文件
- 支持批量处理多个目录

## 依赖要求

- CMake (>= 3.7)
- Qt5 (Core和Gui模块)
- C++14兼容的编译器

## 在Linux下安装依赖

### Ubuntu/Debian系统：

```bash
# 安装编译工具
sudo apt-get update
sudo apt-get install build-essential cmake

# 安装Qt5依赖
sudo apt-get install qt5-default qtbase5-dev
```

### CentOS/RHEL系统：

```bash
# 安装编译工具
sudo yum groupinstall "Development Tools"
sudo yum install cmake

# 安装Qt5依赖
sudo yum install qt5-default
sudo yum install qt5-qtbase-devel
```

## 编译步骤

1. 克隆项目代码：
```bash
git clone <repository_url>
cd atlas-generator-linux
```

2. 创建并进入构建目录：
```bash
mkdir build
cd build
```

3. 配置项目：
```bash
cmake ..
```

4. 编译项目：
```bash
make
```

编译完成后，在build目录下会生成`atlas_generator`可执行文件。

## 使用方法

### 命令行参数

```bash
./atlas_generator [选项] <输入目录>
```

### 配置文件

你也可以通过配置文件来设置打包参数，参考项目中的`atlasConfig`文件。

## 工作流程

1. 从命令行或配置文件读取配置
2. 读取输入目录下的所有目录，检测修改
3. 根据设置裁切空白和扩展边缘
4. 测试并选择最优的打包算法
5. 拼接图片
6. 导出图片和数据文件

## 贡献

欢迎提交Issue和Pull Request。详细信息请参考`CONTRIBUTING.md`文件。