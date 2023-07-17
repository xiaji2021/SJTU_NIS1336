### NIS1336大作业说明文档

> 夏季521051910099
>
> 周祎晨 521021911080
>
> 闫家硕518021910532 

### 请查看Github仓库：https://github.com/xiaji2021/SJTU_NIS1336

#### 1 使用前准备

在此软件中，使用了c++的第三方库libbcrypt和libssh，因而在使用前需要进行下载和安装。

##### 1.1 下载并安装libbcrypt库

###### 1.1.1 克隆libbcrypt库:

`git clone https://github.com/trusch/libbcrypt`

###### 1.1.2 进入克隆的bcrypt文件夹:

`cd bcrypt`

###### 1.1.3 使用Makefile构建并且安装库：

`make`

`sudo make install`

###### 1.1.4 安装完成后，即可在代码中使用bcrypt库，如：

`#include <bcrypt/bcrypt.hpp>`

##### 1.2 下载并安装libssh

`sudo apt-get install libssh-dev`

#### 2 使用方法

进入文件夹，在命令行中使用Makefile进行编译：

`make`

编译完成后即可在命令行中使用运行程序。

如果修改了代码，可以先使用`make clean`清理之前编译的文件，再使用`make`重新编译。

