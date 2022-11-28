# Scout2导航平台项目开发书

版本：V1_0

## 硬件

### 结构

![1](.\img\项目开发书-88.png)



注意：惯导和两个机械式雷达的出线口都是朝车后；惯导的X轴要朝向小车运动方向；固态雷达的出线口是朝下；这样摆放下惯导与雷达的旋转矩阵是已经确定了的。

### 电路和模块

#### 电气连接

![2](.\img\项目开发书-2.jpg)

1. scout2航空插头电源连接24V分电板（考虑线长，电源正极需要连接开关）

2. scout2航空插头can总线USB接口连接工控机（考虑线长）

![电气连接-1](.\img\电气连接-1.jpg)

3. 24V分电板分电口连接工控机供电（考虑线长）

4. 工控机USB接口连接相机和惯导（暂时不考虑换线，供电和数据直接由USB提供）

SBG惯导

![电气连接-4](.\img\电气连接-4.jpg)

5. 24V分电板分电口接24V-12V模块

6. 24V-12V模块输出接12V分电板

7. 12V分电板分电口连接两款雷达电源（考虑空中接插件连接）

8. 两款雷达网口连接工控机（考虑空中接插件连接）

速腾雷达

![电气连接-2](.\img\电气连接-2.jpg)

livox雷达

![电气连接-3](.\img\电气连接-3.jpg)

![电气连接-5](.\img\电气连接-5.jpg)

##### RTK的安装

三合一串口模块：

![电气-1](.\img\电气-1.png)

![电气-2](.\img\电气-2.png)

SBG惯导连线：

![电气-3](.\img\电气-3.png)

4G模块

![电气-4](.\img\电气-4.png)



#### 模块调试

##### SBG-Ellipse-N惯导

###### 磁力计校准

![3](.\img\项目开发书-3.jpg)

![4](.\img\项目开发书-4.jpg)

![5](.\img\项目开发书-5.jpg)

Window上位机连接上模块，

![6](.\img\项目开发书-6.jpg)

在室外点击Calibrate Magnetometers，然后按手册进行校准，让传感器尽可能在多的不同方向旋转。校准结束，按下“Calibrate”按钮，点击OK，上位机会有参数值示意，达到1左右就可以了。

###### Windows上位机配置和Ubuntu –ROS配置和环境安装

Yaml文件和上位机配置对应：

![7](.\img\项目开发书-7.png)

这里的frequency是节点频率，不作更改。

![项目开发书-74](.\img\项目开发书-74.png)

![8](.\img\项目开发书-8.png)

![9](.\img\项目开发书-9.png)

![10](.\img\项目开发书-10.png)

![11](.\img\项目开发书-11.png)

![12](.\img\项目开发书-12.png)

![13](.\img\项目开发书-13.png)

![14](.\img\项目开发书-14.png)

![15](.\img\项目开发书-15.png)

GNSS配置

![16](.\img\项目开发书-16.png)

![17](.\img\项目开发书-17.png)

![18](.\img\项目开发书-18.png)

改成true则为ROS的imu消息格式

![19](.\img\项目开发书-19.png)

![20](.\img\项目开发书-20.png)

![21](.\img\项目开发书-21.png)

![22](.\img\项目开发书-22.png)

![23](.\img\项目开发书-23.png)

![24](.\img\项目开发书-24.png)

![25](.\img\项目开发书-25.png)

![26](.\img\项目开发书-26.png)

时间戳     

![27](.\img\项目开发书-27.png)

授时输出

![28](.\img\项目开发书-28.png)

后处理数据输出

###### 惯导ROS驱动包yaml文件修改

惯导ROS驱动包yaml文件可以修改传感器参数，但是不建议。建议的做法是在上位机进行修改，修改后yaml文件也同步进行修改。比如：
1)	需要找到传感器的串口号，然后更改portName: "/dev/ttyUSB0"
2)	更改波特率： baudRate: 921600
3)	更改运动模型： motionProfile: 2  （注：车载模型）
4)	更改输出：改成ros时间：time_reference: "ros"    
5)	更改输出：改成ROS的IMU消息格式 ：    ros_standard: true
6)	更改输出：更改为ENU坐标系，    use_enu: true
7)	更改输出：更改frame_id ：        frame_id: "imu_link"   
8)	更改输出：更改系统状态刷新频率：log_status: 200         
9)	更改输出：更改IMU数据刷新频率：  log_imu_data: 1   
10)	更改输出：更改UTC时间刷新频率：  log_utc_time: 200
11)	更改输出：更改磁力计数据刷新频率：    log_mag: 4

###### 编译和运行

```
cd catkin_ws/src
git clone https://github.com/SBG-Systems/sbg_ros_driver.git
cd ../
catkin_make
roslaunch sbg_driver sbg_device.launch
```

注意：在运行前要给设备串口给权限。

临时更改权限：

```
sudo chmod 777 /dev/ttyUSB0
```

永久打开串口权限：

首先查看用户组：

```
ls -l /dev/ttyUSB0
```

终端输出：

```
crw-rw-rw- 1 root dialout 188, 0 12月 19 14:47 /dev/ttyUSB0
```

可以看到用户 root ，所属用户组为 dialout, 因此一种方法是把我们的当前用户名 加入到这个用户组。

```
$ whoami
ausim
```

```
sudo usermod -aG dialout ausim
```

最后，重启再登入系统，就可以使用该串口了 。

##### 工控

###### 安装系统

1. 网上下载ubuntu16.04镜像；

2. 使用UltraISO软件制作系统启动盘；

3. 插入系统启动盘，连接好屏幕、鼠标和键盘；

4. 工控机接通电源，按F2进入bios；

5. 选择启动项为系统启动盘，enter进入系统进行安装，直接将系统全部安装到一个硬盘中，即选择擦除硬盘安装；

6. 安装后进入系统（这里如果系统断电重启后不能自动找到启动项进入系统，则需要在https://sourceforge.net/projects/boot-repair-cd/ 下载boot-repair-disk镜像，同样制作系统启动盘，进入启动盘里的boot-repair系统，进行boot修复。如下图点击上面选项）

![29](.\img\项目开发书-29.jpg)

7. 连接外网wifi以便下载，或者连接以太网；

###### 安装远程服务

1. 首先找好合适的服务器，并更新软件：

```
sudo apt-get update
sudo apt-get upgrade
```

2. 下载远程ssh服务端，输入sudo apt-get install openssh-server

或者 sudo apt-get install ssh*

3. 下载远程VNC服务端，sudo apt-get install xrdp vnc4server xbase-clients

或者sudo apt-get install vnc*

4. 安装dconf-tools，sudo apt-get install dconf-tools

5. 搜索打开桌面共享软件（desktop sharing）

![30](.\img\项目开发书-30.jpg)

配置如下（避免每次都要在工控机即服务端点击确认）：

注：允许其他用户控制您的桌面也要勾选

![31](F:\file\实验室\项目\无人车平台感知模块\多传感器融合SLAM\图片\项目开发书-31.jpg)

6. 安装dconf-editor，sudo apt-get install dconf-editor

7. 搜索打开dconf系统配置编辑器：

![32](.\img\项目开发书-32.jpg)

依次展开org->gnome->desktop->remote-access，然后取消 “requlre-encryption”的勾选即可，如下：

![33](.\img\项目开发书-33.jpg)

8. 远程访问搭建方式有两种：第一种是工控机和笔记本电脑的wifi都连同一个路由器；第二种方式是工控机设置热点模式，发出wifi，然后笔记本连接此wifi。第一种方式需要在同个wifi环境下，可以连接外网下载。第二种方式方便室外调试。两种方式在ubuntu下查询ip地址：输入ifconfig，获取wifi地址，ip名称一般是w开头的，如下：

![34](.\img\项目开发书-34.jpg)

第二种方式配置：点击edit connections

![35](.\img\项目开发书-35.jpg)

点击add

![36](.\img\项目开发书-36.jpg)

选择WIFI

![37](.\img\项目开发书-37.jpg)

选择热点hotspot，并改名字，一个是工控机里面的名字，一个是wifi热点名字。

![38](.\img\项目开发书-38.jpg)

设置密码后保存，然后工控机连接上这个wifi

![39](.\img\项目开发书-39.jpg)

如下：

![40](.\img\项目开发书-40.jpg)

9. 测试远程ssh，笔记本连接上wifi，下载xshell或者直接用win10自带powershell，ubuntu环境则直接用自带命令行终端，输入：ssh 服务端name@ip ，例：ssh [ausim@10.42.01](mailto:ausim@10.42.01) ，第一次连接需要输入yes确认，密码是1。

![41](.\img\项目开发书-41.jpg)

10. 测试远程VNC，笔记本或者手机、平板下载vncviewer，点击new connection，输入ip地址和服务端名称

![42](.\img\项目开发书-42.jpg)

![43](.\img\项目开发书-43.jpg)

11. 测试远程传输文件，下载FileZilla：

![44](.\img\项目开发书-44.jpg)

###### 安装驱动

1. sudo apt-get install minicom （安装串口助手）

2. 测试惯导，输入以下：

   ![45](.\img\项目开发书-45.jpg)

发现惯导串口为USB0，打开minicom：sudo minicom –s ，点击第三项，设置串口名称和波特率，如下：

![46](.\img\项目开发书-46.jpg)

设置完成后保存，具体看网上minicom的教程。

退出后打开minicom会有数据，乱码因为是16进制文件。这里就说明惯导是没问题的，具体可以运行惯导驱动查看话题数据。

![47](.\img\项目开发书-47.jpg)

后面需要给惯导串口权限，sudo chmod 777 /dev/ttyUSB0 ，但这个权限每次重启都要给，可以写在自启脚本上，每次启动就自动给权限。

3. 测试雷达：

![48](.\img\项目开发书-48.jpg)

如上：eno1对应的是LAN1，enp1s0对应的是LAN2。

选择一个网口配置本机ip为192.168.1.102（这是速腾雷达，livox雷达是192.168.1.50），如下：

选择以太网：

![49](.\img\项目开发书-49.jpg)

选择设备和改名称：

![50](.\img\项目开发书-50.jpg)

改成手动，点击add增加ip，掩码为255.255.255.0 ，DNS（Gateway）不需要：

![51](.\img\项目开发书-51.jpg)

雷达进行通电，网口插上LAN1，连接上以太网，然后ping 192.168.1.200，能ping通则说明雷达连接没问题，后面可以运行雷达驱动，这里就不说明了。

也可以ifconfig看下是否改好，这里发现eno1是改好了。

![52](.\img\项目开发书-52.jpg)

4. 测试相机，相机后面再具体说明。海康相机直接插usb，有上位机软件和SDK，网上也有ros的包可以开发。

5. 其他问题:建议将工控机桌面密码取消，不然远程VNC连接会让你先输入密码（但是这样就需要连接屏幕和键盘，远程VNC就没有意义了）。

###### 安装环境

**googlepinyin的安装：**

安装汉语语言包：

```shell
sudo apt-get install language-pack-zh-hans
```

安装谷歌拼音输入法：

```shell
sudo apt-get install fcitx-googlepinyin
```

打开SystemSettings-->Language Support，系统会进行语言包搜索，照系统提示安装语言包。
如图所示，选择“键盘输入方式系统”为：fcitx。

![53](.\img\项目开发书-53.jpg)

重启电脑或者注销用户。进入界面后可以看到fcitx输入法框架已经运行，系统状态栏的输入法图标已经改变。

打开终端，执行“fcitx-configtool”命令，在如图界面进行输入法配置。

![54](.\img\项目开发书-54.jpg)

如图增加谷歌拼音输入法：

![55](.\img\项目开发书-55.png)



**安装ROS：**

```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```

```shell
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
```

or

```shell
curl-sSL 'http://keyserver.ubuntu.com/pks/lookup?op=get&search=0xC1CF6E31E6BADE8868B172B4F42ED6FBAB17C654' | sudo apt-key add -
```

```shell
sudo apt-get update
sudo apt-get install ros-kinetic-desktop-full
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
sudo apt install python-rosdep python-rosinstall python-rosinstall-generator python-wstool build-essential
sudo rosdep init
rosdep update
```

或者直接参考ros-wiki安装。

sudo rosdep init 出现 

```
ERROR: cannot download default sources list from:
```

第一步：

首先进入https://github.com/ros/rosdistro

去把这个包下载下来。

第二步：

修改这个包中rosdep/source.list.d/下的文件20-default.list，将这个文件中指向raw.githubusercontent.com的url地址全部修改为指向本地文件的地址，也就是该下载好的包的地址，以下是改好的样例：

```html
# os-specific listings first

yaml file:///home/xxx/rosdistro/rosdep/osx-homebrew.yaml osx

\# generic

yaml file:///home/xxx/rosdistro/rosdep/base.yaml

yaml file:///home/xxx/rosdistro/rosdep/python.yaml

yaml file:///home/xxx/rosdistro/rosdep/ruby.yaml

gbpdistro file:///home/xxx/rosdistro/releases/fuerte.yaml fuerte

\# newer distributions (Groovy, Hydro, ...) must not be listed anymore, they are being fetched from the rosdistro index.yaml instead
```

 

***注意：***在py语言中：url本地文件地址格式是：file://+文件地址，后面更改其他文件中地址的时候也一样。

第三步：

修改/usr/lib/python2.7/dist-packages/rosdep2该文件夹下面的sources_list.py文件，如下：

 

```
# default file to download with 'init' command in order to bootstrap

\# rosdep

DEFAULT_SOURCES_LIST_URL = 'file:///home/xxx/rosdistro/rosdep/sources.list.d/20-default.list'

 

\# seconds to wait before aborting download of rosdep data
```

 

第四步

修改以下两个文件里面的代码：

/usr/lib/python2.7/dist-packages/rosdep2/rep3.py

/usr/lib/python2.7/dist-packages/rosdistro/__init__.py

下面是分别修改后的样例：

/usr/lib/python2.7/dist-packages/rosdep2/rep3.py文件：

 

```
# location of targets file for processing gbpdistro files

REP3_TARGETS_URL = 'file:///home/xxx/rosdistro/releases/targets.yaml'

 

\# seconds to wait before aborting download of gbpdistro data
```

 

/usr/lib/python2.7/dist-packages/rosdistro/__init__.py的文件：

 

```
# index information


DEFAULT_INDEX_URL = 'file:///home/xxx/rosdistro/index-v4.yaml'
 

def get_index_url():
```

 

然后进行sudo rosdep init :

![56](.\img\项目开发书-56.jpg)

**安装各类包：**

第一步：安装雷达驱动环境：
安装pcap：

```
sudo apt-get install -y libpcap-dev
```

安装Yaml：

```
sudo apt-get install -y libyaml-cpp-dev （若已安装ROS desktop-full, 可跳过）
```

github下载rslidar_sdk（速腾雷达驱动）
修改CMakeLists.txt和package.xml：
（1） 将文件顶部的set(COMPILE_METHOD ORIGINAL)改为set(COMPILE_METHOD CATKIN)
（2） 将set(POINT_TYPE XYZI) 改为set(POINT_TYPE XYZIRT)
（3） 将rslidar_sdk工程目录下的package_ros1.xml文件重命名为package.xml。
（4） 可能需要更改launch文件中雷达型号为RS16

编译：建立ros工作空间，在工作空间编译。

第二步：rs雷达话题转velodyne雷达话题
GitHub下载：https://github.com/HViktorTsoi/rs_to_velodyne.git 
并编译，可放在同个工作空间编译。

第三步：安装A-LOAM
安装Ceres Solver
官网有安装的教程: https://link.zhihu.com/?target=http%3A//ceres-solver.org/installation.html 

安装PCL,使用ROS自带即可。

编译A-LOAM，即下载源码建立ROS工作空间并编译。

```shell
cd ~/catkin_ws/src
git clone
https://link.zhihu.com/?target=https%3A//github.com/HKUST-Aerial-Robotics/A-LOAM.git 
cd ../
catkin_make
source ~/catkin_ws/devel/setup.bash
```

第四步：安装LeGO-LOAM
安装gtsam：

```shell
wget -O ~/Downloads/gtsam.zip https://github.com/borglab/gtsam/archive/4.0.0-alpha2.zip
cd ~/Downloads/ && unzip gtsam.zip -d ~/Downloads/
cd ~/Downloads/gtsam-4.0.0-alpha2/
mkdir build && cd build
```

在CMakeLists.txt文件中if（GTSAM_USE_SYSTEM_EIGEN）
判断的前边添加如下代码：

```
set(GTSAM_USE_SYSTEM_EIGEN ON)
```

```shell
cmake ..
sudo make install
```

下载编译LeGO-LOAM

```shell
cd ~/catkin_ws/src
git clone https://github.com/RobustFieldAutonomyLab/LeGO-LOAM.git
cd ..
catkin_make -j1
```

修改LeGO-LOAM的launch文件：
将这句话的true改成false

```html
<param name="/use_sim_time" value="false"/>
```

第五步：安装Livox雷达驱动
雷达SDK下载和安装，GitHub下载最新源码。

```shell
git clone https://github.com/Livox-SDK/Livox-SDK.git
cd Livox-SDK
cd build && cmake ..
make
sudo make install
```

安装ros驱动
新建文件夹ws_livox/src

```shell
git clone https://github.com/Livox-SDK/livox_ros_driver.git ws_livox/src
cd ws_livox
catkin_make
source ./devel/setup.sh
```

第六步：安装Livox-mapping

```shell
cd ~/catkin_ws/src
git clone https://github.com/Livox-SDK/livox_mapping.git
cd ..
catkin_make
source ~/catkin_ws/devel/setup.bash
```

第七步：安装SBG惯导ROS驱动

第八步：安装LIO-SAM

**注意**

ubuntu18.04下显示屏有问题，不能正常显示。

##### 雷达

##### 相机

##### RTK

###### 4g模块

千寻账号：

![调试-1](.\img\调试-1.png)

首次使用的配置：

![调试-2](.\img\调试-2.png)

进行测试：

![调试-3](.\img\调试-3.png)

发送：

$GPGGA,000001,3112.518576,N,12127.901251,E,1,8,1,0,M,-32,M,3,0*4B

###### 惯导

![调试-4](.\img\调试-4.png)

设置portE为115200/rs232

![调试-5](.\img\调试-5.png)

确认开启磁力计

![调试-6](.\img\调试-6.png)

设置Rtcm为portE

GPGGA设置为1hz

确定状态

![调试-7](.\img\调试-7.png)

现象：

![调试-8](.\img\调试-8.png)

##### Scout2底盘

###### ubuntu自启脚本

```shell
cd /etc/init.d
sudo touch scout_robot.sh
sudo chmod 777 scout_robot.sh
sudo gedit scout_robot.sh
```

写下：

```shell
#!/bin/bash
### BEGIN INIT INFO
# Provides:          bbzhh.com
# Required-Start:    $local_fs $network
# Required-Stop:     $local_fs
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: tomcat service
# Description:       tomcat service daemon
### END INIT INFO

sudo modprobe gs_usb
sudo ip link set can0 up type can bitrate 500000

exit 0
```

```shell
sudo update-rc.d scout_robot.sh defaults 100
```

## 移动硬盘环境安装

### 安装系统

1.	备份移动硬盘数据然后进行格式化，或者拿一个全新的移动硬盘。
2.	使用DiskGenius软件将移动硬盘更改为GPT格式。
3.	如果主板启动方式是legacy，则磁盘分区成四个区，其中分别容量为512MB（存放引导）、200G(安装ubuntu系统，容量自定义)、32G（swap交换区，一般是内存的2倍），剩余空间为移动硬盘，可存文件。如果是UEFI启动的主板，移动硬盘需要有ESP分区，512MB的分区不需要了。
4.	下载ubuntu系统镜像，用UltraISO制作系统启动盘（要用一个U盘）。

![57](.\img\项目开发书-57.jpg)

5. 将移动硬盘和U盘插入电脑，开启电脑，进入bios，修改bios启动项为U盘。

6. 安装ubuntu系统，选择something else。

如果是legacy，刚刚分好的512MB的空间选择为efi分区，如果是UEFI启动方式，则不需要选择efi分区，自动选择好了；32G空间选择为swap分区；200G选择为EXT4文件格式，挂载直接为根目录，即/，最后要注意ubuntu引导安装的盘要选择你的移动硬盘。

7. 安装完成后就可以起飞了，后面就是安装各个环境了。

### 安装环境

安装终端：

```shell
sudo apt install terminator
```

安装filezilla

```
sudo apt-get install filezilla
```

**googlepinyin的安装：**

安装汉语语言包：

```shell
sudo apt-get install language-pack-zh-hans
```

安装谷歌拼音输入法：

```shell
sudo apt-get install fcitx-googlepinyin
```

打开SystemSettings-->Language Support，系统会进行语言包搜索，照系统提示安装语言包。
如图所示，选择“键盘输入方式系统”为：fcitx。

![58](.\img\项目开发书-53.jpg)

重启电脑或者注销用户。进入界面后可以看到fcitx输入法框架已经运行，系统状态栏的输入法图标已经改变。

打开终端，执行“fcitx-configtool”命令，在如图界面进行输入法配置。

![59](.\img\项目开发书-54.jpg)

如图增加谷歌拼音输入法：

![60](.\img\项目开发书-55.png)

**官网下载谷歌浏览器并安装**

https://www.google.cn/chrome/ 

**科学上网**

https://ikuuu.co/auth/login
这里注册帐号，白嫖或者冲钱。

下载v2ray

https://github.com/Qv2ray/Qv2ray/releases/download/v1.99.6/Qv2ray-refs.tags.v1.99.6-linux.AppImage
或者打开网站：https://github.com/Qv2ray/Qv2ray/releases/tag/v1.99.6 
选择下图文件

![61](.\img\项目开发书-61.jpg)

下载核心文件，下载链接：

https://github.com/v2ray/v2ray-core/releases/download/v4.22.1/v2ray-linux-64.zip

或者打开网站：https://github.com/v2ray/v2ray-core/releases/ 选择下图文件

![62](.\img\项目开发书-62.jpg)

进入v2ray下载的根目录，执行以下命令：

```shell
sudo chmod +x ./Qv2ray-refs.tags.v1.99.6-linux.AppImage
```

仍然在v2ray根目录下打开终端，输入以下命令(非root用户执行)：

```
./Qv2ray-refs.tags.v1.99.6-linux.AppImage
```

打开软件后点击Preferences

![63](.\img\项目开发书-63.jpg)

在常规设置里面按照图示操作,最后点击ok保存：

![64](.\img\项目开发书-64.jpg)

回到主界面，点击订阅：

![65](.\img\项目开发书-65.jpg)

输入订阅的名字：iKuuu

链接为iKuuu网站上得到的链接。

点击update后有了VPN。

将软件的代理模式打开，如下图所示；

![66](.\img\项目开发书-66.jpg)

**安装录屏软件**

sudo add-apt-repository ppa:maarten-baert/simplescreenrecorder
sudo apt-get update
sudo apt-get install simplescreenrecorder

#### **ubuntu16.04下面安装ROS**

```
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
```

```shell
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
```

or

```shell
curl-sSL 'http://keyserver.ubuntu.com/pks/lookup?op=get&search=0xC1CF6E31E6BADE8868B172B4F42ED6FBAB17C654' | sudo apt-key add -
```



```shell
sudo apt-get update
sudo apt-get install ros-kinetic-desktop-full
echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc
source ~/.bashrc
sudo apt install python-rosdep python-rosinstall python-rosinstall-generator python-wstool build-essential
sudo rosdep init
rosdep update
```

或者直接参考ros-wiki安装。

sudo rosdep init 出现 

```
ERROR: cannot download default sources list from:
```

第一步：

首先进入https://github.com/ros/rosdistro

去把这个包下载下来。

第二步：

修改这个包中rosdep/source.list.d/下的文件20-default.list，将这个文件中指向raw.githubusercontent.com的url地址全部修改为指向本地文件的地址，也就是该下载好的包的地址，以下是改好的样例：

```html
# os-specific listings first

yaml file:///home/xxx/rosdistro/rosdep/osx-homebrew.yaml osx

# generic

yaml file:///home/xxx/rosdistro/rosdep/base.yaml

yaml file:///home/xxx/rosdistro/rosdep/python.yaml

yaml file:///home/xxx/rosdistro/rosdep/ruby.yaml

gbpdistro file:///home/xxx/rosdistro/releases/fuerte.yaml fuerte

# newer distributions (Groovy, Hydro, ...) must not be listed anymore, they are being fetched from the rosdistro index.yaml instead
```

 

***注意：***在py语言中：url本地文件地址格式是：file://+文件地址，后面更改其他文件中地址的时候也一样。

第三步：

修改/usr/lib/python2.7/dist-packages/rosdep2该文件夹下面的sources_list.py文件，如下：

 

```
# default file to download with 'init' command in order to bootstrap

# rosdep

DEFAULT_SOURCES_LIST_URL = 'file:///home/xxx/rosdistro/rosdep/sources.list.d/20-default.list'

 

# seconds to wait before aborting download of rosdep data
```

 

第四步

修改以下两个文件里面的代码：

/usr/lib/python2.7/dist-packages/rosdep2/rep3.py

/usr/lib/python2.7/dist-packages/rosdistro/__init__.py

下面是分别修改后的样例：

/usr/lib/python2.7/dist-packages/rosdep2/rep3.py文件：

 

```
# location of targets file for processing gbpdistro files

REP3_TARGETS_URL = 'file:///home/xxx/rosdistro/releases/targets.yaml'

 

# seconds to wait before aborting download of gbpdistro data
```

 

/usr/lib/python2.7/dist-packages/rosdistro/__init__.py的文件：

 

```
# index information


DEFAULT_INDEX_URL = 'file:///home/xxx/rosdistro/index-v4.yaml'
 

def get_index_url():
```

 

然后进行sudo rosdep init :

![67](.\img\项目开发书-56.jpg)

#### ubuntu18.04下面安装ROS

安装ros：

选择清华源

```shell
sudo sh -c '. /etc/lsb-release && echo "deb http://mirrors.tuna.tsinghua.edu.cn/ros/ubuntu/ `lsb_release -cs` main" > /etc/apt/sources.list.d/ros-latest.list'
```

```shell
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
```

```shell
sudo apt update
```

```shell
sudo apt install ros-melodic-desktop-full
```

```shell
echo "source /opt/ros/melodic/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

```shell
sudo apt install python-rosdep python-rosinstall python-rosinstall-generator python-wstool build-essential
```

```shell
sudo apt install python-rosdep
```

```shell
sudo rosdep init
rosdep update
```

sudo rosdep init 出现 

```
ERROR: cannot download default sources list from:
```

参考前面kinetic的安装。

#### **安装ros开发环境**

官网下载deb

https://code.visualstudio.com/docs/?dv=linux64_deb

```
sudo dpkg -i xxxx.deb
```

安装这些插件

![项目开发书-80](.\img\项目开发书-80.jpg)

创建工作空间：

```shell
mkdir -p test_ws/src
cd test_ws/
catkin_make
code .
```

vscode 中编译 ros

快捷键 ctrl + shift + B 调用编译，选择:`catkin_make:build`

或修改.vscode/tasks.json 文件

```json
{
// 有关 tasks.json 格式的文档，请参见
    // https://go.microsoft.com/fwlink/?LinkId=733558
    "version": "2.0.0",
    "tasks": [
        {
            "label": "catkin_make:debug", //代表提示的描述性信息
            "type": "shell",  //可以选择shell或者process,如果是shell代码是在shell里面运行一个命令，如果是process代表作为一个进程来运行
            "command": "catkin_make",//这个是我们需要运行的命令
            "args": [],//如果需要在命令后面加一些后缀，可以写在这里，比如-DCATKIN_WHITELIST_PACKAGES=“pac1;pac2”
            "group": {"kind":"build","isDefault":true},
            "presentation": {
                "reveal": "always"//可选always或者silence，代表是否输出信息
            },
            "problemMatcher": "$msCompile"
        }
    ]
}
```

创建 ROS 功能包

选定 src 右击 ---> create catkin package

设置包名 添加依赖

添加cpp

```cpp
/*
    控制台输出 HelloVSCode !!!

*/
#include "ros/ros.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    //执行节点初始化
    ros::init(argc,argv,"HelloVSCode");

    //输出日志
    ROS_INFO("Hello VSCode!!!");
    return 0;
}
```

如果没有代码提示

修改 .vscode/c_cpp_properties.json

设置 "cppStandard": "c++17"



配置 CMakeLists.txt

```cmake
add_executable(节点名称
  src/C++源文件名.cpp
)
target_link_libraries(节点名称
  ${catkin_LIBRARIES}
)
```

目前这里好像会出节点tab不到的情况，执行这一句后可以了

```
rospack list
```



#### 安装QT

官网下载QT
http://download.qt.io/archive/qt/
安装后

```
sudo apt-get install qt5-default
```

#### 安装clion

官网下载软件后，解压后运行./clion.sh  
如果忘记创建快捷方式，则
sudo touch命令创建一个clion.desktop文件

用sudo vim打开这个文件

输入：（更改为自己的clion路径）

```
[Desktop Entry]
Encoding=UTF-8

Name=CLion

Comment=clion-2018.1.1

Exec=/home/sqwlly/Downloads/clion-2018.1.1/bin/clion.sh

Icon=/home/sqwlly/Downloads/clion-2018.1.1/bin/clion.svg

Categories=Application;Development;Java;IDE

Version=2018.1.1

Type=Application

#Terminal=1
```

#### 安装typora

https://typora.io/dev_release.html

这里选择beta版本（0.几的都是beta版本）

```bash
apt-cache showpkg typora # 列出所有可用版本
```

```bash
sudo apt-get install typora=0.11.18-1 # 安装指定版本的
sudo apt-mark hold typora # 保持当前版本的软件包，阻止自动升级
```

### 安装各类要开发的框架

安装雷达驱动环境：
安装pcap：

```
sudo apt-get install -y libpcap-dev
```

github下载rslidar_sdk（速腾雷达驱动）
修改CMakeLists.txt和package.xml：
（1） 将文件顶部的set(COMPILE_METHOD ORIGINAL)改为set(COMPILE_METHOD CATKIN)
（2） 将set(POINT_TYPE XYZI) 改为set(POINT_TYPE XYZIRT)
（3） 将rslidar_sdk工程目录下的package_ros1.xml文件重命名为package.xml。
（4） 可能需要更改config.yaml文件中雷达型号为RS16

建立ros工作空间，在工作空间编译
GitHub下载rs雷达话题转velodyne雷达话题
https://github.com/HViktorTsoi/rs_to_velodyne.git  并编译，可放在同个工作空间编译

livox雷达SDK下载和安装，GitHub下载最新源码。

```shell
git clone https://github.com/Livox-SDK/Livox-SDK.git
cd Livox-SDK
cd build && cmake ..
make
sudo make install
```

安装ros驱动
新建文件夹ws_livox/src

```shell
git clone https://github.com/Livox-SDK/livox_ros_driver.git ws_livox/src
cd ws_livox
catkin_make
source ./devel/setup.sh
```

下载并安装编译livox_mapping（这其实跟livox-loam是一样的，香港大学火星实验室有跟大疆合作），可放在同个工作空间编译。

```shell
cd ~/catkin_ws/src
git clone https://github.com/Livox-SDK/livox_mapping.git
cd ..
catkin_make
source ~/catkin_ws/devel/setup.bash
```

安装Ceres Solver
http://ceres-solver.org/installation.html
官网有安装的教程
如下：
https://github.com/google/glog.git   下载glog 并编译安装 
直接cmake 后 make 然后 make install 

https://github.com/gflags/gflags.git  下载gflags 并编译安装 
直接cmake 后 make 然后 make install 

https://ceres-solver.googlesource.com/ceres-solver

下载ceres-solver并编译安装 
直接cmake 后 make 然后 make install 

```shell
cd ~/catkin_ws/src
git clone https://github.com/HKUST-Aerial-Robotics/A-LOAM.git
cd ../
catkin_make
source ~/catkin_ws/devel/setup.bash
```

在CMakeLists.txt文件中if（GTSAM_USE_SYSTEM_EIGEN）判断的前边添加如下代码：
set(GTSAM_USE_SYSTEM_EIGEN ON)

安装gtsam

```shell
wget -O ~/Downloads/gtsam.zip https://github.com/borglab/gtsam/archive/4.0.0-alpha2.zip
cd ~/Downloads/ && unzip gtsam.zip -d ~/Downloads/
cd ~/Downloads/gtsam-4.0.0-alpha2/
mkdir build && cd build
cmake ..
sudo make install
```

下载编译LeGO-LOAM

```shell
cd ~/catkin_ws/src
git clone https://github.com/RobustFieldAutonomyLab/LeGO-LOAM.git
cd ..
catkin_make -j1
```

修改LeGO-LOAM的launch文件：
将这句话的true改成false (实时跑是false，跑bag是true)

```html
<param name="/use_sim_time" value="false"/>
```


SBG AHRS传感器驱动安装

```shell
cd catkin_ws/src
git clone https://github.com/SBG-Systems/sbg_ros_driver.git
cd ../
catkin_make
```

yaml文进行修改：
1)	需要找到传感器的串口号，然后更改portName: "/dev/ttyUSB0"
2)	更改波特率： baudRate: 921600
3)	更改运动模型： motionProfile: 2 （注：车载模型）
4)	更改输出：改成ros时间：time_reference: "ros"    
5)	更改输出：改成ROS的IMU消息格式 ：    ros_standard: true
6)	更改输出：更改为ENU坐标系，    use_enu: true
7)	更改输出：更改frame_id ：        frame_id: "imu_link"   
8)	更改输出：更改系统状态刷新频率：log_status: 200         
9)	更改输出：更改IMU数据刷新频率：  log_imu_data: 1   
10)	更改输出：更改UTC时间刷新频率：  log_utc_time: 200
11)	更改输出：更改磁力计数据刷新频率：    log_mag: 4

修改话题名称：
sbg_ellipseN.launch文件中增加：

```html
<remap from="/imu/data" to="imu_raw" />
```

把IMU话题改成LIO-SAM需要的话题

```shell
sudo apt-get install -y ros-kinetic-navigation
sudo apt-get install -y ros-kinetic-robot-localization
sudo apt-get install -y ros-kinetic-robot-state-publisher
```

下载编译LIO-SAM

```shell
cd ~/catkin_ws/src
git clone https://github.com/TixiaoShan/LIO-SAM.git
cd ..
catkin_make
```

在LIO-SAM中config文件夹中的配置yaml文件中修改雷达话题名称
pointCloudTopic: "points_raw"   （改成雷达的话题）

**ubuntu18.04下：**

安装lio-sam框架依赖

```shell
sudo apt-get install -y ros-melodic-navigation
sudo apt-get install -y ros-melodic-robot-localization
sudo apt-get install -y ros-melodic-robot-state-publisher
```

出现：

```shell
fatal error: cloud_msgs/cloud_info.h: No such file or directory
```

解决办法

```shell
catkin_make -j1
```

或者：

```shell
catkin_make -DCATKIN_WHITELIST_PACKAGES="cloud_msgs"
```

ubuntu18.04下安装lio-sam框架需要安装gtsam4.02版本，不然boost会报错。

```shell
git clone https://github.com/borglab/gtsam/archive/4.0.2.zip
mkdir build && cd build
cmake -DGTSAM_BUILD_WITH_MARCH_NATIVE=OFF ..
sudo make install -j4
```

### 系统备份

```shell
tar cvpzf backup.tgz --exclude=/proc --exclude=/lost+found --exclude=/backup.tgz --exclude=/mnt --exclude=/sys /
```

恢复

```shell
tar xvpfz backup.tgz -C /
```

**注：**

这个备份有问题。

## 多传感器标定

### IMU内参标定

https://github.com/gaowenliang/imu_utils
https://github.com/gaowenliang/code_utils

先github下载这两个包。

安装依赖：

```
sudo apt-get install libdw-dev
```

code_utils 依赖 ceres库， 需要先全局安装ceres库，

imu_utils 依赖 code_utils，故先catkin_make code_utils, 再将imu_utils放入编译。



gflags报错，gflags是使用源码安装,因此解决方法是:

进入gflags/build中,找到CMakeCache.txt

改为CMAKE_CXX_FLAGS:STRING=-fPIC

再执行就可以

```
export CXXFLAGS="-fPIC" && cmake .. && make VERBOSE=1
make && make install
```



编译code_utils 报错：

```
fatal error: backward.hpp: No such file or directory
```

解决办法： 在code_utils下面找到sumpixel_test.cpp，修改#include “backward.hpp” 为 #include“code_utils/backward.hpp”即可编译成功。

然后将imu_utils放入再编译一次。

 

标定方式一： 直接运行sbg惯导，只需要改话题名称即可。

```
roslaunch imu_utils sbg.launch 
```

imu标定包的launch文件如下：

```
<launch>
    <node pkg="imu_utils" type="imu_an" name="imu_an" output="screen">
        <param name="imu_topic" type="string" value= "/imu_raw"/>
        <param name="imu_name" type="string" value= "sbg"/>
        <param name="data_save_path" type="string" value= "$(find imu_utils)/data/"/>
        <param name="max_time_min" type="int" value= "80"/>
        <param name="max_cluster" type="int" value= "100"/>
    </node>
</launch>
```

标定方式二：录制bag包

```
rosbag record -o 20211117.bag out /imu_raw 
```

然后运行bag，运行标定程序进行标定。

注：标定时间根据自己设备运行时间来设定，如果设备一般是运行2个小时，那么就标定2个小时即可。

输出参数：

![68](.\img\项目开发书-67.jpg)

输出文件在data文件夹中，我们只需要加速度和角速度平均噪音和偏置，不需要每个方向的。

![69](.\img\项目开发书-68.jpg)

替换LIO-SAM中config文件夹中的配置yaml文件中这四个参数，第五个参数需要连续采样Z轴重力加速度做均值。

![70](.\img\项目开发书-69.jpg)

### IMU与雷达外参标定

即确认惯导与雷达的旋转变换矩阵

平移向量影响不是很大，直接从结构图纸中得到即可，即是惯导原点到雷达原点的向量。旋转矩阵对建图影响比较大，首先我们得确认理论的旋转矩阵，然后再进行标定。

 确认理论的旋转矩阵的方法：

将imageProjection.cpp里这个回调函数中的注释打开。

```c++
    void imuHandler(const sensor_msgs::Imu::ConstPtr& imuMsg)
    {
        sensor_msgs::Imu thisImu = imuConverter(*imuMsg);   // 对imu做一个坐标转换
        // 加一个线程锁，把imu数据保存进队列
        std::lock_guard<std::mutex> lock1(imuLock);
        imuQueue.push_back(thisImu);

        // debug IMU data
        // cout << std::setprecision(6);
        // cout << "IMU acc: " << endl;
        // cout << "x: " << thisImu.linear_acceleration.x << 
        //       ", y: " << thisImu.linear_acceleration.y << 
        //       ", z: " << thisImu.linear_acceleration.z << endl;
        // cout << "IMU gyro: " << endl;
        // cout << "x: " << thisImu.angular_velocity.x << 
        //       ", y: " << thisImu.angular_velocity.y << 
        //       ", z: " << thisImu.angular_velocity.z << endl;
        // double imuRoll, imuPitch, imuYaw;
        // tf::Quaternion orientation;
        // tf::quaternionMsgToTF(thisImu.orientation, orientation);
        // tf::Matrix3x3(orientation).getRPY(imuRoll, imuPitch, imuYaw);
        // cout << "IMU roll pitch yaw: " << endl;
        // cout << "roll: " << imuRoll << ", pitch: " << imuPitch << ", yaw: " << imuYaw << endl << endl;
    }
```

参考这个视频https://youtu.be/BOUK8LYQhHs

确认惯导的输出是否正常，如果Z轴的加速度是负数，则Z轴反过来了，查看

LIO-SAM中config文件夹中的配置yaml文件：

![71](.\img\项目开发书-70.jpg)

这两个矩阵需要做调整，根据与视频的现象进行的对比，这里发现单位阵是可以达到Z轴加速度是正数9.8的（重力加速度）。

这里就已经确定了Z轴，我们暂时无法确定X轴和Y轴的朝向，可以参考后面标定出来的旋转矩阵。

其中extrinsicRot表示imu->lidar的坐标变换, 用于旋转imu坐标系下的加速度和角速度到lidar坐标系下, extrinsicRPY则用于旋转imu坐标系下的欧拉角到lidar坐标下, 由于lio-sam作者使用的imu的欧拉角旋转方向与lidar坐标系不一致（即是按照什么旋转顺序旋转）, 因此使用了两个旋转不同, 但是大部分的设备两个旋转应该是设置为相同的，我们这里也是设置为相同即可。

下面进行标定：

第一种标定软件（不可行）：

下载标定工具

```shell
mkdir -p catkin_ws/src   
cd catkin_ws/src
git clone https://github.com/chennuo0125-HIT/lidar_imu_calib.git
cd ..
catkin_make -DCATKIN_WHITELIST_PACKAGES="ndt_omp;lidar_imu_calib"
```

第二种标定软件（目前可行）：

https://github.com/stevengj/nlopt.git

github下载nlopt 并cmake编译

https://github.com/ethz-asl/lidar_align.git

github下载源码进行ROS编译



编译时出现Could not find NLOPTConfig.cmake

解决办法：找到这个文件并将其放入到工程目录下，并在CMakeLists.txt里加上这样一句话：

```
list(APPEND CMAKE_FIND_ROOT_PATH ${PROJECT_SOURCE_DIR})
```

由于这个标定软件没有IMU的数据接口，所以改写loader.cc

改写如下：

```c++
#include <geometry_msgs/TransformStamped.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <sensor_msgs/Imu.h>
#include "lidar_align/loader.h"
#include "lidar_align/transform.h"

namespace lidar_align {

Loader::Loader(const Config& config) : config_(config) {}

Loader::Config Loader::getConfig(ros::NodeHandle* nh) {
  Loader::Config config;
  nh->param("use_n_scans", config.use_n_scans, config.use_n_scans);
  return config;
}

void Loader::parsePointcloudMsg(const sensor_msgs::PointCloud2 msg,
                                LoaderPointcloud* pointcloud) {
  bool has_timing = false;
  bool has_intensity = false;
  for (const sensor_msgs::PointField& field : msg.fields) {
    if (field.name == "time_offset_us") {
      has_timing = true;
    } else if (field.name == "intensity") {
      has_intensity = true;
    }
  }

  if (has_timing) {
    pcl::fromROSMsg(msg, *pointcloud);
    return;
  } else if (has_intensity) {
    Pointcloud raw_pointcloud;
    pcl::fromROSMsg(msg, raw_pointcloud);

    for (const Point& raw_point : raw_pointcloud) {
      PointAllFields point;
      point.x = raw_point.x;
      point.y = raw_point.y;
      point.z = raw_point.z;
      point.intensity = raw_point.intensity;

      if (!std::isfinite(point.x) || !std::isfinite(point.y) ||
          !std::isfinite(point.z) || !std::isfinite(point.intensity)) {
        continue;
      }

      pointcloud->push_back(point);
    }
    pointcloud->header = raw_pointcloud.header;
  } else {
    pcl::PointCloud<pcl::PointXYZ> raw_pointcloud;
    pcl::fromROSMsg(msg, raw_pointcloud);

    for (const pcl::PointXYZ& raw_point : raw_pointcloud) {
      PointAllFields point;
      point.x = raw_point.x;
      point.y = raw_point.y;
      point.z = raw_point.z;

      if (!std::isfinite(point.x) || !std::isfinite(point.y) ||
          !std::isfinite(point.z)) {
        continue;
      }

      pointcloud->push_back(point);
    }
    pointcloud->header = raw_pointcloud.header;
  }
}

bool Loader::loadPointcloudFromROSBag(const std::string& bag_path,
                                      const Scan::Config& scan_config,
                                      Lidar* lidar) {
  rosbag::Bag bag;
  try {
    bag.open(bag_path, rosbag::bagmode::Read);
  } catch (rosbag::BagException e) {
    ROS_ERROR_STREAM("LOADING BAG FAILED: " << e.what());
    return false;
  }

  std::vector<std::string> types;
  types.push_back(std::string("sensor_msgs/PointCloud2"));
  rosbag::View view(bag, rosbag::TypeQuery(types));

  size_t scan_num = 0;
  for (const rosbag::MessageInstance& m : view) {
    std::cout << " Loading scan: \e[1m" << scan_num++ << "\e[0m from ros bag"
              << '\r' << std::flush;

    LoaderPointcloud pointcloud;
    parsePointcloudMsg(*(m.instantiate<sensor_msgs::PointCloud2>()),
                       &pointcloud);

    lidar->addPointcloud(pointcloud, scan_config);

    if (lidar->getNumberOfScans() >= config_.use_n_scans) {
      break;
    }
  }
  if (lidar->getTotalPoints() == 0) {
    ROS_ERROR_STREAM(
        "No points were loaded, verify that the bag contains populated "
        "messages of type sensor_msgs/PointCloud2");
    return false;
  }

  return true;
}

bool Loader::loadTformFromROSBag(const std::string& bag_path, Odom* odom) {
  rosbag::Bag bag;
  try {
    bag.open(bag_path, rosbag::bagmode::Read);
  } catch (rosbag::BagException e) {
    ROS_ERROR_STREAM("LOADING BAG FAILED: " << e.what());
    return false;
  }

  std::vector<std::string> types;
  types.push_back(std::string("sensor_msgs/Imu"));
   rosbag::View view(bag, rosbag::TypeQuery(types));
    size_t imu_num = 0;
    double shiftX=0,shiftY=0,shiftZ=0,velX=0,velY=0,velZ=0;
    ros::Time time;
    double timeDiff,lastShiftX,lastShiftY,lastShiftZ;
    for (const rosbag::MessageInstance& m : view){
      std::cout <<"Loading imu: \e[1m"<< imu_num++<<"\e[0m from ros bag"<<'\r'<< std::flush;
 
     sensor_msgs::Imu imu=*(m.instantiate<sensor_msgs::Imu>());
 
     Timestamp stamp = imu.header.stamp.sec * 1000000ll +imu.header.stamp.nsec / 1000ll;
     if(imu_num==1){
         time=imu.header.stamp;
             Transform T(Transform::Translation(0,0,0),Transform::Rotation(1,0,0,0));
         odom->addTransformData(stamp, T);
     }
     else{
         timeDiff=(imu.header.stamp-time).toSec();
         time=imu.header.stamp;
         velX=velX+imu.linear_acceleration.x*timeDiff;
         velY=velX+imu.linear_acceleration.y*timeDiff;
         velZ=velZ+(imu.linear_acceleration.z-9.801)*timeDiff;
         
         lastShiftX=shiftX;
         lastShiftY=shiftY;
         lastShiftZ=shiftZ;
         shiftX=lastShiftX+velX*timeDiff+imu.linear_acceleration.x*timeDiff*timeDiff/2;
         shiftY=lastShiftY+velY*timeDiff+imu.linear_acceleration.y*timeDiff*timeDiff/2;
         shiftZ=lastShiftZ+velZ*timeDiff+(imu.linear_acceleration.z-9.801)*timeDiff*timeDiff/2;
 
         Transform T(Transform::Translation(shiftX,shiftY,shiftZ),
                Transform::Rotation(imu.orientation.w,
                         imu.orientation.x,
                         imu.orientation.y,
                         imu.orientation.z));
         odom->addTransformData(stamp, T);
     }
   }
  /*
  types.push_back(std::string("geometry_msgs/TransformStamped"));
  rosbag::View view(bag, rosbag::TypeQuery(types));

  size_t tform_num = 0;
  for (const rosbag::MessageInstance& m : view) {
    std::cout << " Loading transform: \e[1m" << tform_num++
              << "\e[0m from ros bag" << '\r' << std::flush;

    geometry_msgs::TransformStamped transform_msg =
        *(m.instantiate<geometry_msgs::TransformStamped>());

    Timestamp stamp = transform_msg.header.stamp.sec * 1000000ll +
                      transform_msg.header.stamp.nsec / 1000ll;

    Transform T(Transform::Translation(transform_msg.transform.translation.x,
                                       transform_msg.transform.translation.y,
                                       transform_msg.transform.translation.z),
                Transform::Rotation(transform_msg.transform.rotation.w,
                                    transform_msg.transform.rotation.x,
                                    transform_msg.transform.rotation.y,
                                    transform_msg.transform.rotation.z));
    odom->addTransformData(stamp, T);
  }
*/
  if (odom->empty()) {
    ROS_ERROR_STREAM("No odom messages found!");
    return false;
  }

  return true;
}

bool Loader::loadTformFromMaplabCSV(const std::string& csv_path, Odom* odom) {
  std::ifstream file(csv_path, std::ifstream::in);

  size_t tform_num = 0;
  while (file.peek() != EOF) {
    std::cout << " Loading transform: \e[1m" << tform_num++
              << "\e[0m from csv file" << '\r' << std::flush;

    Timestamp stamp;
    Transform T;

    if (getNextCSVTransform(file, &stamp, &T)) {
      odom->addTransformData(stamp, T);
    }
  }

  return true;
}

// lots of potential failure cases not checked
bool Loader::getNextCSVTransform(std::istream& str, Timestamp* stamp,
                                 Transform* T) {
  std::string line;
  std::getline(str, line);

  // ignore comment lines
  if (line[0] == '#') {
    return false;
  }

  std::stringstream line_stream(line);
  std::string cell;

  std::vector<std::string> data;
  while (std::getline(line_stream, cell, ',')) {
    data.push_back(cell);
  }

  if (data.size() < 9) {
    return false;
  }

  constexpr size_t TIME = 0;
  constexpr size_t X = 2;
  constexpr size_t Y = 3;
  constexpr size_t Z = 4;
  constexpr size_t RW = 5;
  constexpr size_t RX = 6;
  constexpr size_t RY = 7;
  constexpr size_t RZ = 8;

  *stamp = std::stoll(data[TIME]) / 1000ll;
  *T = Transform(Transform::Translation(std::stod(data[X]), std::stod(data[Y]),
                                        std::stod(data[Z])),
                 Transform::Rotation(std::stod(data[RW]), std::stod(data[RX]),
                                     std::stod(data[RY]), std::stod(data[RZ])));

  return true;
}

}  // namespace lidar_align
```

录制话题数据，旋转三个轴，XY轴不要大幅度旋转

```shell
rosbag record -o 20211117.bag  out /velodyne_points /imu_raw 
```

修改标定软件包launch文件中的数据包路径，然后运行launch文件，等待漫长迭代优化时间。

![72](.\img\项目开发书-71.jpg)

最后输出数据：

![73](.\img\项目开发书-73.jpg)

这里发现标定矩阵类似于单位阵，说明单位阵是理论外参，而标定后的矩阵是考虑了小角度误差后的外参。

把旋转矩阵复制到LIO-SAM中config文件夹中的配置yaml文件中，更改这两个矩阵。

![74](.\img\项目开发书-70.jpg)

### 相机内参标定

#### window下进行标定：

opencv官网安装3.4版本

https://opencv.org/releases/#

参考这个链接：

https://www.cnblogs.com/aiguona/p/9370433.html

在VS2015进行opencv的配置。

参考这个链接：

https://blog.csdn.net/STILLxjy/article/details/98519746?ops_request_misc=&request_id=&biz_id=102&utm_term=window%20opencv3%E6%A0%87%E5%AE%9A&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-1-98519746.first_rank_v2_pc_rank_v29

进行opencv相机内参标定工程的构建和编译。

然后录制相机图像数据，使棋盘格能够在相机图像的各个位置上，一般保证各个位置的棋盘格加起来能填满图像从中点出发的2/3即可，且在一个位置上调整不同的棋盘格角度。

在可执行文件路径下存放相机图像数据，

![项目开发书-75](.\img\项目开发书-75.jpg)

然后运行：

```
.\opencv_camera_calib_dem0.exe -w=11 -h=8 .\imagelist2.yaml 
```

这里-w=11 -h=8 分别指的是棋盘格长和宽的角点数量，imagelist2.yaml是图像名字索引。

如下：

```yaml
%YAML:1.0
---
images:
   - "image(1).bmp"
   - "image(2).bmp"
   - "image(3).bmp"
   - "image(4).bmp"
   - "image(5).bmp"
   - "image(6).bmp"
   - "image(7).bmp"
   - "image(8).bmp"
   - "image(9).bmp"
   - "image(10).bmp"
```

运行效果：

![项目开发书-76](.\img\项目开发书-76.jpg)

要保证所有图片都要能够被识别出角点。

最后输出标定数据yaml文件，能够很方便的被读取，得到标定的内参（包括内参矩阵和畸变参数）。

```yaml
%YAML:1.0
---
calibration_time: "Fri Dec 17 15:34:05 2021"
image_width: 720
image_height: 540
board_width: 11
board_height: 8
square_size: 1.
aspectRatio: 1.
flags: 2
camera_matrix: !!opencv-matrix
   rows: 3
   cols: 3
   dt: d
   data: [ 7.4411367720263286e+02, 0., 3.6793591210353367e+02, 0.,
       7.4411367720263286e+02, 2.4831743372645076e+02, 0., 0., 1. ]
distortion_coefficients: !!opencv-matrix
   rows: 5
   cols: 1
   dt: d
   data: [ -9.9071967990252374e-01, 1.2320195467160143e+01,
       -2.3597235076281194e-02, 6.1246544215312713e-04,
       -6.0686233185640781e+01 ]
avg_reprojection_error: 2.1351487963865803e+00

```

#### ubuntu下进行标定：

同样也是找到opencv自带的相机内参标定源文件，编写cmakelists文件进行项目配置和编译。

#### matlab下进行标定：

matlab有相机内参标定工具：Camera Calibrator

### 相机与雷达外参标定

外参标定分为粗标定和精标定，先进行粗标定。

#### 粗标定

录制相机和雷达的bag包（雷达和相机固定安装后，跟内参标定一样，需要在不同位置不同角度观测棋盘格，每种姿态保持几秒不动，方便在标定软件中截取雷达和相机帧），数据格式如下：

![项目开发书-77](.\img\项目开发书-77.jpg)

从github下载标定包（这是从autoware分离出来的相机雷达标定包）

https://github.com/XidianLemon/calibration_camera_lidar.git

需要安装nlopt2.5版本，在github上搜索下载。

安装依赖：

```shell
sudo apt-get install ros-kinetic-jsk-recognition-msgs 
```

需要注意的是ubuntu18.04需要对calibration_camera_lidar功能包下的CMakeLists.txt进行修改，添加ROS的melodic版本的支持，修改CMakeLists.txt中所有 (indigo|jade|kinetic) 为 (indigo|jade|kinetic|melodic)

编译问题

```shell
CMakeFiles/calibrationtoolkit.dir/CalibrationToolkit/calibrationtoolkit.cpp.o: In function nlopt::opt::get_errmsg() const': /usr/local/include/nlopt.hpp:516: undefined reference tonlopt_get_errmsg’ 
```

这个错误应该是重复安装冲突了。解决办法：

```shell
sudo apt remove libnlopt-dev
```

运行实现：先运行bag如下（需要将话题改成标定包支持的）：

```shell
rosbag play your.bag /velodyne_points:=/points_raw /hikrobot_camera/rgb:=/image_raw
```

工作空间执行：

```
rosrun calibration_camera_lidar calibration_toolkit
```

先选择/image_raw话题，再选择如下：

![项目开发书-78](.\img\项目开发书-78.jpg)

注意要改棋盘格的大小和数量（数量指的是长宽的角点数据，不是格子数量），改了之后重新启动才会生效，具体操作在下面文章：

https://blog.csdn.net/nuo112/article/details/116404760

将雷达视角调至合适，如下：

![项目开发书-79](.\img\项目开发书-79.jpg)

播放bag，找到合适的标定位置后暂停播包，点击Grab，会截取一帧雷达和相机数据，在雷达数据上鼠标点击雷达线束（选择相机标定板的线束），完成一帧数据的标定，重复以上过程大约20次左右，点击Calibrate，会生成外参矩阵。

其实这个过程就是通过人为的在上位机选取标定板线束在雷达的位置（构建约束），通过建立多个方程（超定方程），求一个最优的雷达到相机变换矩阵（求解优化问题）。

#### 精标定

为什么需要精标定？

因为这样标定出来的变换矩阵误差比较大，只是点云大致投射在了图像的标定板上，需要进一步的标定三个小角度，这会影响数据融合的效果和后续建图定位的精度。

### 相机与IMU外参标定

### 多雷达外参标定

### 多相机外参标定

## 导航规划实现

### 二维路径规划

#### 仿真实现

参考http://www.autolabor.com.cn/book/ROSTutorials/chapter1.html

1.首先实现仿真平台在rviz和gazebo的实现：

编写rviz环境下的xacro：

car.urdf.xacro

lidar.urdf.xacro

camera.urdf.xacro

all_component.urdf.xacro （将分component 集成起来）

编写gazebo环境的xacro：

仿真机器人物体：

camera_gazebo.urdf.xacro

car_gazebo.urdf.xacro

lidar_gazebo.urdf.xacro

封装机器人惯性矩阵：

head.urdf.xacro

仿真传感器：

sensor_kinect_gazebo.urdf.xacro

sensor_camera_gazebo.urdf.xacro

sensor_lidar_gazebo.urdf.xacro

仿真小车运动：

move_gazebo.urdf.xacro

集成所有xacro：

all_component_gazebo.urdf.xacro

如下图：

![项目开发书-81](.\img\项目开发书-81.png)

2.编写launch文件：

仿真功能：

car_play.launch ：只显示在rviz上的

car_play_gazebo.launch  ：同时在rviz和gazebbo中显示，并运行了仿真环境

基础功能：

amcl.launch 调用amcl包

path_planning.launch 调用move_base包

2DSLAM建图功能：

运行：car_play_gazebo.launch 和 car_mapping.launch

地图服务功能：

保存地图：

先进行建图：car_play_gazebo.launch 和 car_mapping.launch

再进行保存：map_save.launch

读取地图：map_read.launch	

amcl定位演示：car_play_gazebo.launch 和 amcl_car_play.launch

路径规划：

全局路径规划：car_play_gazebo.launch 和 global_navigation.launch

局部路径规划：car_play_gazebo.launch 和 local_navigation.launch

3.其他

nav.pgm和 nav.yaml为保存的地图文件

box_house.world 为仿真环境世界文件

这个仿真功能包的目录结构：

```
├── CMakeLists.txt
├── config
│   ├── arbotix_car_play.yaml
│   ├── base_local_planner_params.yaml
│   ├── costmap_common_params.yaml
│   ├── global_costmap_params.yaml
│   ├── local_costmap_params.yaml
│   ├── robot_mapping.rviz
│   └── robot_play.rviz
├── launch
│   ├── amcl_car_play.launch
│   ├── amcl.launch
│   ├── car_mapping.launch
│   ├── car_play_gazebo.launch
│   ├── car_play.launch
│   ├── global_navigation.launch
│   ├── local_navigation.launch
│   ├── map_read.launch
│   ├── map_save.launch
│   └── path_planning.launch
├── map
│   ├── nav.pgm 
│   └── nav.yaml
├── meshes
├── package.xml
├── urdf
│   ├── urdf
│   └── xacro
│       ├── all_component_gazebo.urdf.xacro
│       ├── all_component.urdf.xacro
│       ├── camera_gazebo.urdf.xacro
│       ├── camera.urdf.xacro
│       ├── car_gazebo.urdf.xacro
│       ├── car.urdf.xacro
│       ├── head.urdf.xacro
│       ├── lidar_gazebo.urdf.xacro
│       ├── lidar.urdf.xacro
│       ├── move_gazebo.urdf.xacro
│       ├── sensor_camera_gazebo.urdf.xacro
│       ├── sensor_kinect_gazebo.urdf.xacro
│       └── sensor_lidar_gazebo.urdf.xacro
└── worlds
    └── box_house.world
```

整个项目工程代码在云平台的项目代码的二维路径规划中。

仿真效果：

![项目开发书-82](.\img\项目开发书-82.png)

#### 实物实现

利用gmapping只使用二维点云进行建图，利用move_base进行规划。

后期需要写一个三维点云转换栅格地图包（包可以实现选择性转换输出三维栅格和2.5-D高程栅格、二维栅格地图），进行gmapping的功能替换就行，接口一样的。

**1.先实现gmapping建图：**

a.开启速腾聚创 rs_lidar_16雷达驱动（跟之前一样）

b.使用pointcloud_to_laserscan包实现三维转二维（编写pointcloudtoscan.launch）

```xml
<?xml version="1.0"?>

<launch>

    <!-- run pointcloud_to_laserscan node -->
    <include file="$(find rslidar_sdk)/launch/start.launch" />
    
    <node pkg="pointcloud_to_laserscan" type="pointcloud_to_laserscan_node" name="pointcloud_to_laserscan">

        <remap from="cloud_in" to="/rslidar_points"/>
        
        <rosparam>
            # target_frame: rslidar # Leave disabled to output scan in pointcloud frame
            transform_tolerance: 0.01
            min_height: -0.4
            max_height: 1.0

            angle_min: -3.1415926 # -M_PI
            angle_max: 3.1415926 # M_PI
            angle_increment: 0.003 # 0.17degree
            scan_time: 0.1
            range_min: 0.2
            range_max: 100
            use_inf: true
            inf_epsilon: 1.0

            # Concurrency level, affects number of pointclouds queued for processing and number of threads used
            # 0 : Detect number of cores
            # 1 : Single threaded
            # 2->inf : Parallelism level
            concurrency_level: 1
        </rosparam>

    </node>

</launch>

```

c.源码下载gmapping包，这里不要用apt去下载，因为要修改源码（gmapping源码不支持2048以上的点，需要修改下）

```
git clone https://github.com/ros-perception/openslam_gmapping.git
git clone https://github.com/ros-perception/slam_gmapping.git
```

在文件目录openslam_gmapping/include/gmapping/scanmatcher/scanmatcher.h文件中

\#define LASER_MAXBEAMS 2048  设置为20480（比较大就行）

d.调用gmapping包和laser_scan_matcher包，并用tf包的static_transform_publisher进行gmapping需要的tf信息提供，又因为gmapping必须要里程计信息，而laser_scan_matcher包实现的是二维雷达前端里程计odom输出。编写demo_gmapping.launch：

```xml
<!-- 
Example launch file: uses laser_scan_matcher together with
slam_gmapping 
-->
 
<launch>
 
  #### set up data playback from bag #############################
 
  <param name="/use_sim_time" value="true"/>
 
  #### publish an example base_link -> laser transform ###########
  <include file="$(find mapping)/launch/pointcloudtoscan.launch" />

  <node pkg="tf" type="static_transform_publisher" name="base_link_to_laser" 
    args="0.0 0.0 0.0 0.0 0.0 0.0 /base_link /rslidar 40" />
 
  #### start rviz ################################################
 
  <node pkg="rviz" type="rviz" name="rviz" 
    args="-d $(find laser_scan_matcher)/demo/demo_gmapping.rviz"/>
 
  #### start the laser scan_matcher ##############################
 
  <node pkg="laser_scan_matcher" type="laser_scan_matcher_node" 
    name="laser_scan_matcher_node" output="screen">
 
    <param name="fixed_frame" value = "odom"/>
    <param name="max_iterations" value="10"/>
 
  </node>
 
  #### start gmapping ############################################
 
  <node pkg="gmapping" type="slam_gmapping" name="slam_gmapping" output="screen">
    <param name="map_udpate_interval" value="1.0"/>
    <param name="maxUrange" value="5.0"/>
    <param name="sigma" value="0.1"/>
    <param name="kernelSize" value="1"/>
    <param name="lstep" value="0.15"/>
    <param name="astep" value="0.15"/>
    <param name="iterations" value="1"/>
    <param name="lsigma" value="0.1"/>
    <param name="ogain" value="3.0"/>
    <param name="lskip" value="1"/>
    <param name="srr" value="0.1"/>
    <param name="srt" value="0.2"/>
    <param name="str" value="0.1"/>
    <param name="stt" value="0.2"/>
    <param name="linearUpdate" value="1.0"/>
    <param name="angularUpdate" value="0.5"/>
    <param name="temporalUpdate" value="0.4"/>
    <param name="resampleThreshold" value="0.5"/>
    <param name="particles" value="10"/>
    <param name="xmin" value="-5.0"/>
    <param name="ymin" value="-5.0"/>
    <param name="xmax" value="5.0"/>
    <param name="ymax" value="5.0"/>
    <param name="delta" value="0.02"/>
    <param name="llsamplerange" value="0.01"/>
    <param name="llsamplestep" value="0.05"/>
    <param name="lasamplerange" value="0.05"/>
    <param name="lasamplestep" value="0.05"/>
  </node>
 
</launch>
```

这里要注意：

```xml
  <param name="/use_sim_time" value="true"/>
```

跑bag这个标志为true，实物跑这个标志为false

实车效果：

![项目开发书-85](.\img\项目开发书-85.png)

**2.再实现move_base路径规划**

move_base的框图如下：

![项目开发书-86](.\img\项目开发书-86.png)

拷贝gmapping建图功能下面的功能包，包括雷达驱动、gmapping功能包到src。

github下载scout2小车驱动包：https://github.com/agilexrobotics/scout_ros.git

https://github.com/agilexrobotics/ugv_sdk.git

安装小车驱动依赖：

```shell
sudo apt install -y libasio-dev
sudo apt install -y ros-$ROS_DISTRO-teleop-twist-keyboard
```

设置CAN-To-USB模块

```
sudo modprobe gs_usb
```

第一次用小车驱动包则执行：

```
rosrun scout_bringup setup_can2usb.bash
```

不是第一次则执行：

```
rosrun scout_bringup bringup_can2usb.bash
```

键盘测试下小车运动，没问题则进行下一步：

github下载move_base包（集成在navigation）：

https://github.com/ros-planning/navigation.git

将之前仿真的robot_sim包拷贝到src。

因为用laser_scan_matcher功能包生成的雷达odom没有数据（不知原因），所以用了小车自带的里程计。

更改demo_mapping.launch如下：

```xml
<!-- 
Example launch file: uses laser_scan_matcher together with
slam_gmapping 
-->
 
<launch>
 
  #### set up data playback from bag #############################
  <param name="/use_sim_time" value="flase"/>

  <include file="$(find mapping)/launch/pointcloudtoscan.launch" />

  #### publish an example base_link -> laser transform ###########
 
  <node pkg="tf" type="static_transform_publisher" name="base_link_to_laser" 
    args="0.0 0.0 0.0 -1.57 0.0 0.0 /base_link /rslidar 40" />
 
  #### start rviz ################################################
 
  <node pkg="rviz" type="rviz" name="rviz" 
    args="-d $(find mapping)/config/demo_gmapping.rviz"/>
 
  <include file="$(find scout_bringup)/launch/scout_mini_robot_base.launch" />

  #### start gmapping ############################################
 
  <node pkg="gmapping" type="slam_gmapping" name="slam_gmapping" output="screen">
    <param name="base_frame" value="/base_link"/> 
    <param name="odom_frame" value="/odom" /> 
    <param name="map_frame" value="/map" />
    <param name="map_udpate_interval" value="1.0"/>
    <param name="maxUrange" value="5.0"/>
    <param name="sigma" value="0.1"/>
    <param name="kernelSize" value="1"/>
    <param name="lstep" value="0.15"/>
    <param name="astep" value="0.15"/>
    <param name="iterations" value="1"/>
    <param name="lsigma" value="0.1"/>
    <param name="ogain" value="3.0"/>
    <param name="lskip" value="1"/>
    <param name="srr" value="0.1"/>
    <param name="srt" value="0.2"/>
    <param name="str" value="0.1"/>
    <param name="stt" value="0.2"/>
    <param name="linearUpdate" value="1.0"/>
    <param name="angularUpdate" value="0.5"/>
    <param name="temporalUpdate" value="0.4"/>
    <param name="resampleThreshold" value="0.5"/>
    <param name="particles" value="10"/>
    <param name="xmin" value="-5.0"/>
    <param name="ymin" value="-5.0"/>
    <param name="xmax" value="5.0"/>
    <param name="ymax" value="5.0"/>
    <param name="delta" value="0.02"/>
    <param name="llsamplerange" value="0.01"/>
    <param name="llsamplestep" value="0.05"/>
    <param name="lasamplerange" value="0.05"/>
    <param name="lasamplestep" value="0.05"/>
  </node>
 
</launch>
```

这里要注意：

```xml
<node pkg="tf" type="static_transform_publisher" name="base_link_to_laser" 
    args="0.0 0.0 0.0 -1.57 0.0 0.0 /base_link /rslidar 40" />
```

需要调整base_link到rslidar的旋转变换（小车坐标系和雷达坐标系），由于这边是Z轴转了90度，所有要更改参数为    args="0.0 0.0 0.0 -1.57 0.0 0.0 /base_link /rslidar 40"

调好的现象是odom的箭头跟小车运动方向一致。

先实现局部路径规划（局部路径规划调用了gmapping和move_base）：

编写path_planning.launch

```xml
<launch>

    <node pkg="move_base" type="move_base" respawn="false" name="move_base" output="screen" clear_params="true">
        <rosparam file="$(find robot_sim)/config/costmap_common_params.yaml" command="load" ns="global_costmap" />
        <rosparam file="$(find robot_sim)/config/costmap_common_params.yaml" command="load" ns="local_costmap" />
        <rosparam file="$(find robot_sim)/config/local_costmap_params.yaml" command="load" />
        <rosparam file="$(find robot_sim)/config/global_costmap_params.yaml" command="load" />
        <rosparam file="$(find robot_sim)/config/base_local_planner_params.yaml" command="load" />
    </node>

</launch>
```

更改costmap_common_params.yaml

```yaml
#机器人几何参数，如果机器人是圆形，设置 robot_radius,如果是其他形状设置 footprint

# footprint: [[-0.12, -0.12], [-0.12, 0.12], [0.12, 0.12], [0.12, -0.12]] #其他形状

obstacle_range: 3.0 # 用于障碍物探测，比如: 值为 3.0，意味着检测到距离小于 3 米的障碍物时，就会引入代价地图
raytrace_range: 3.5 # 用于清除障碍物，比如：值为 3.5，意味着清除代价地图中 3.5 米以外的障碍物

footprint: [[-0.350, -0.465], [-0.350, 0.465], [0.350, 0.465], [0.350, -0.465]]


#膨胀半径，扩展在碰撞区域以外的代价区域，使得机器人规划路径避开障碍物
inflation_radius: 0.2
#代价比例系数，越大则代价值越小
cost_scaling_factor: 3.0

map_type: costmap
observation_sources: scan
scan: {sensor_frame: rslidar, data_type: LaserScan, topic: scan, marking: true, clearing: true}
```

主要要更改无人车的尺寸、膨胀半径、话题消息名称、类型、frame等。

更改global_costmap_params.yaml：

```yaml
global_costmap:
  global_frame: map
  robot_base_frame: base_link

  update_frequency: 10.0
  publish_frequency: 10.0
  transform_tolerance: 0.5

  static_map: true
```

主要更改frame

更改local_costmap_params.yaml：

```yaml
local_costmap:
  global_frame: odom
  robot_base_frame: base_link

  update_frequency: 10.0
  publish_frequency: 10.0
  transform_tolerance: 0.5  

  static_map: false  
  rolling_window: true
  width: 3
  height: 3
  resolution: 0.05
```

主要更改frame

编写local_navigation.launch

```xml
<launch>
     <include file="$(find mapping)/launch/demo_mapping.launch" />
    <!-- 运行move_base节点 -->
    <include file="$(find robot_sim)/launch/path_planning.launch" />
</launch>
```

测试局部路径规划，在局部代价地图中，点击目标点，小车会运动过去，同时局部代价地图中进行障碍的感知，小车会绕过突然出现的障碍。

然后实现全局路径规划（需要读取录制好的地图，开启amcl、move_base）：

编写amcl.launch

```xml
<launch>
    
    <node pkg="amcl" type="amcl" name="amcl" output="screen">
      <!-- Publish scans from best pose at a max of 10 Hz -->
      <param name="odom_model_type" value="diff"/><!-- 里程计模式为差分 -->
      <param name="odom_alpha5" value="0.1"/>
      <param name="transform_tolerance" value="0.2" />
      <param name="gui_publish_rate" value="10.0"/>
      <param name="laser_max_beams" value="30"/>
      <param name="min_particles" value="500"/>
      <param name="max_particles" value="5000"/>
      <param name="kld_err" value="0.05"/>
      <param name="kld_z" value="0.99"/>
      <param name="odom_alpha1" value="0.2"/>
      <param name="odom_alpha2" value="0.2"/>
      <!-- translation std dev, m -->
      <param name="odom_alpha3" value="0.8"/>
      <param name="odom_alpha4" value="0.2"/>
      <param name="laser_z_hit" value="0.5"/>
      <param name="laser_z_short" value="0.05"/>
      <param name="laser_z_max" value="0.05"/>
      <param name="laser_z_rand" value="0.5"/>
      <param name="laser_sigma_hit" value="0.2"/>
      <param name="laser_lambda_short" value="0.1"/>
      <param name="laser_lambda_short" value="0.1"/>
      <param name="laser_model_type" value="likelihood_field"/>
      <!-- <param name="laser_model_type" value="beam"/> -->
      <param name="laser_likelihood_max_dist" value="2.0"/>
      <param name="update_min_d" value="0.2"/>
      <param name="update_min_a" value="0.5"/>
    
      <param name="odom_frame_id" value="odom"/><!-- 里程计坐标系 -->
      <param name="base_frame_id" value="base_link"/><!-- 添加机器人基坐标系 -->
      <param name="global_frame_id" value="map"/><!-- 添加地图坐标系 -->
    
      <param name="resample_interval" value="1"/>
      <param name="transform_tolerance" value="0.1"/>
      <param name="recovery_alpha_slow" value="0.0"/>
      <param name="recovery_alpha_fast" value="0.0"/>
    </node>
    </launch>
```

编写map_save.launch

```xml
<launch>
    <arg name="filename" value="$(find robot_sim)/map/nav" />
    <node name="map_save" pkg="map_server" type="map_saver" args="-f $(arg filename)" />
</launch>
```

先运行建图，建好图后运行map_server包，进行地图保存。

编写global_navigation.launch

```xml
<launch>
    <param name="/use_sim_time" value="flase"/>

    <include file="$(find mapping)/launch/pointcloudtoscan.launch" />

  #### publish an example base_link -> laser transform ###########
 
  <node pkg="tf" type="static_transform_publisher" name="base_link_to_laser" 
    args="0.0 0.0 0.0 -1.573 0.0 0.0 /base_link /rslidar 40" />
 
  #### start rviz ################################################
 
  <node pkg="rviz" type="rviz" name="rviz" 
    args="-d $(find mapping)/config/demo_gmapping.rviz"/>
  

         <!-- 设置地图的配置文件 -->
    <arg name="map" default="nav.yaml" />
    <!-- 运行地图服务器，并且加载设置的地图-->
    <node name="map_server" pkg="map_server" type="map_server" args="$(find robot_sim)/map/$(arg map)"/>
    <!-- 启动AMCL节点 -->

    <include file="$(find robot_sim)/launch/amcl.launch" />

    <!-- 运行move_base节点 -->
    <include file="$(find robot_sim)/launch/path_planning.launch" />
    <!-- <node pkg="rviz" type="rviz" name="rviz" args="-d $(find robot_sim)/config/robot_mapping.rviz" /> -->
      <include file="$(find scout_bringup)/launch/scout_mini_robot_base.launch" />

</launch>

```

运行后效果如下：

![项目开发书-87](.\img\项目开发书-87.png)

全局路径规划下，小车起始位置为红色，终点为蓝色，经过10分钟左右能到达终点。

暴露出的问题点：

1. 由于使用的是小车自带里程计，所以建出的图会漂，后续要迭代的功能是用多传感器融合SLAM下的三维点云地图转换成二维栅格地图，且多传感器融合后能给出高精度里程计，保证图不漂。
2. 全局路径规划下小车运动缓慢且前进时不断左右转，运动不够平滑，需要优化小车运动控制。
3. 目前使用的ros包比较多且杂，传感器底层驱动和小车底层驱动包无须更改，SLAM、感知和规划都是用的开源包，但后续需要将SLAM、感知和规划部分重新写框架代码并整合成不同的三个功能包，接口之间方便互相调用，能够迭代升级不同算法，不断迭代升级小车的智能化水平。

### 三维路径规划

#### octomap转三维栅格地图

1.安装octomap

若需要修改OctomapServer建图包的源码，则需要通过修改源码编译安装，过程如下：（已通过编译并正常使用）

创建工作空间

```
mkdir octomap_ws
cd octomap_ws

mkdir src
cd src
catkin_init_workspace

cd ..
catkin_make
source devel/setup.bash
```

下载编译源码

```
cd src
git clone https://github.com/OctoMap/octomap_mapping.git
```

暂时不要编译，下载配置文件octomap_ros（即使现在不下载，一会儿编译报错也要下载）

```
sudo apt-get install ros-melodic-octomap-ros
```

编译

```
cd ..
catkin_make
```

若编译过程仍有报错，一般是缺少配置文件的错误，复制错误信息浏览器搜索解决。

启动测试launch

```
roslaunch octomap_server octomap_mapping.launch
```

没有问题的话可以通过rostopic list 看到/octomap_full和/octomap_binary的话题。

2.安装octomap在rviz中的插件

```
sudo apt-get install ros-melodic-octomap-rviz-plugins
```

安装后启动 Rviz，直接添加一个三维栅格地图OccupancyGrid和二维地图OccupancyMap的类型：

![项目开发书-83](.\img\项目开发书-83.png)

3.修改Octomap_server功能包中的octomap_mapping.launch文件的内容，以适应A-loam框架，改为如下

```
<launch>
	<node pkg="octomap_server" type="octomap_server_node" name="octomap_server">
	
		<param name="resolution" value="0.1" />
		
		<!-- fixed map frame (set to 'map' if SLAM or localization running!) -->
		<param name="frame_id" type="string" value="/camera_init" />
		
		<!-- maximum range to integrate (speedup!) -->
		<param name="sensor_model/max_range" value="50.0" />
		
		<!-- data source to integrate (PointCloud2) -->
		<remap from="cloud_in" to="laser_cloud_map" />
	
	</node>
</launch>
```

4.启动octomap_server，运行SLAM框架

```
#以A-Loam为例，启动A-Loam
source devel/setup.bash
roslaunch aloam_velodyne aloam_velodyne_VLP_16.launch

#启动转雷达点云话题节点
rosrun rs_to_velodyne rs_to_velodyne XYZIRT XYZIRT

#播放数据包
rosbag play xxx.bag

#启动octomap_server节点
roslaunch octomap_server octomap_server.launch
```

启动rviz后，点击“add”，分别添加"OccupancyGrid"与"OccupancyMap"，并把其话题名依次改为"octomap_full"与"octomap_binary"就可以看到建图过程了。

![项目开发书-84](.\img\项目开发书-84.png)

#### CMU团队开源自主导航和规划算法框架

autonomous_exploration_development_environment是CMU团队开源出来的仿真环境，我们可以自己去沧海校区录制地图，然后部署在仿真环境中，跑我们自己写的规划和自动导航算法。TARE Planner和DSV Planner是CMU团队开源出来的自主探索算法框架，而FAR Planner 是CMU团队开源出来的规划算法框架。

##### 安装仿真环境

安装依赖和源码

```shell
sudo apt update
sudo apt install libusb-dev
git clone https://github.com/HongbiaoZ/autonomous_exploration_development_environment.git
cd autonomous_exploration_development_environment
git checkout distribution
catkin_make
```

安装仿真地图：

```
./src/vehicle_simulator/mesh/download_environments.sh
```

如何导入自己的地图后面再研究

##### TARE Planner安装

```
git clone https://github.com/caochao39/tare_planner.git
cd tare_planner
catkin_make
```

先运行仿真环境：

```
source devel/setup.sh
roslaunch vehicle_simulator system_garage.launch
```

运行TARE Planner

```
source devel/setup.sh
roslaunch tare_planner explore_garage.launch
```

效果：

![28](.\img/科研分享记录-28.png)

![29](.\img/科研分享记录-29.png)

运行其他仿真地图环境：

```
roslaunch vehicle_simulator system_campus.launch 
roslaunch tare_planner explore_campus.launch 
```

效果：

![31](.\img/科研分享记录-31.png)

![29](.\img/科研分享记录-32.png)

![29](.\img/科研分享记录-33.png)

##### DSV Planner安装

安装依赖

```
sudo apt install  libgoogle-glog-dev libgflags-dev
sudo apt install  ros-melodic-octomap-ros
```

安装源码

```
git clone https://github.com/HongbiaoZ/dsv_planner.git
cd dsv_planner
git checkout distribution
catkin_make
```

运行：

```
roslaunch vehicle_simulator system_garage.launch
roslaunch dsvp_launch explore_garage.launch
```

效果和TARE Planner类似

##### FAR Planner安装

```
git clone https://github.com/MichaelFYang/far_planner
cd far_planner
catkin_make
```

运行

```
roslaunch vehicle_simulator system_indoor.launch
roslaunch far_planner far_planner.launch
```

![科研分享记录-35](.\img\科研分享记录-35.png)

## 多传感器融合SLAM框架解析

### 激光SLAM介绍

激光SLAM即使用了激光雷达作为主传感器的SLAM。激光雷达的优点是包含了3d信息，探测距离远，同时不受光照条件和环境纹理等影响，缺点是几何特点会在部分非结构化场景下如长走廊，空旷的草原等失效，且价格还是比较昂贵，但目前价格处于逐渐下降的趋势，相信后面激光雷达的价格会降到一个比较低的价格。

为什么选择激光雷达？

激光雷达相比图像有着对光照、纹理不敏感的优点，激光雷达地图相比通用的视觉特征点加描述子地图有着更好的稳定性，在安全性至上的自动驾驶领域，激光雷达方案比视觉方案鲁棒性更优，几乎所有L4级别的自动驾驶解决方案都会带有激光雷达（像特斯拉这样的纯视觉方案应用并不多）；因此，从实用性和安全性上来讲，激光雷达有着视觉难以比拟的优点。

目前性能最好使用最广的激光slam方案是基于LOAM的系列方案，主要是为多线机械式激光雷达设计的。基于LOAM的方案前端通常是对激光雷达当前帧提取特征（通常是面特征和线特征），后端通常结合其他传感器信息给当前帧到地图中的匹配提供一个良好的初值，或者是纯激光雷达情况下前端激光里程计提供初值。

目前基于LOAM方案的改进方案有我们这篇文章提到的A-LOAM,还有前端增加地面点提取，后端增加回环检测和回环位姿图优化的Lego-LOAM；与IMU紧耦合作为前端里程计且加入GNSS适配作为全局图优化的LIO-SAM；增加视觉传感器的LVI-SAM;以及使用混合固态激光雷达的Livox-LOAM。以上这些都是采用激光雷达作为SLAM主传感器，选择不融合或者融合其他传感器的激光SLAM框架。

### A-LOAM框架

#### 框架介绍

A-LOAM是香港科技大学、华为天才少年秦通博士对张绩的LOAM框架进行强化的一个激光SLAM框架。作为学习激光SLAM的入门框架，A-LOAM的代码可读性和框架思路都值得激光SLAM初学者仔细研读。这个框架使用Eigen以及Ceres-Solver对原始LOAM进行重构，在保证算法原理不变的前提下，对代码框架进行优化，使得代码变得十分简洁，更加容易被读懂。

#### 运行演示

1. 首先要安装ROS

Ubuntu 64-bit 16.04 or 18.04. ROS Kinetic or Melodic

1. 安装Ceres Solver

http://ceres-solver.org/installation.html

官网有安装的教程

1. 安装PCL,使用ROS自带即可。
2. 编译A-LOAM，即下载源码建立ROS工作空间并编译。

```
cd ~/catkin_ws/src
git clone https://github.com/HKUST-Aerial-Robotics/A-LOAM.git
cd ../
catkin_make
source ~/catkin_ws/devel/setup.bash
```

1. 播放bag包运行

```
roslaunch aloam_velodyne aloam_velodyne_VLP_16.launch
rosbag play YOUR_DATASET_FOLDER/nsh_indoor_outdoor.bag
```

效果如下图：

![科研分享记录-1](.\img\科研分享记录-1.jpg)

1. 使用kitti数据，注意要将数据放置到kitti_helper.launch中说明的文件夹上，这部分下一章节介绍kitti_helper.cpp文件会介绍。

```
roslaunch aloam_velodyne aloam_velodyne_HDL_64.launch
roslaunch aloam_velodyne kitti_helper.launch
```

#### 项目工程代码介绍：

整个项目是用ROS环境下的catkin make进行编译的，初学者主要关注include、launch、rviz_cfg、src文件夹和README、CMakeLists、package文件。

##### CMakeLists文件：

```
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  nav_msgs
  sensor_msgs
  roscpp
  rospy
  rosbag
  std_msgs
  image_transport
  cv_bridge
  tf
)
```

该框架使用了ROS自带的package。

```
find_package(PCL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Ceres REQUIRED)

include_directories(
  include
  ${catkin_INCLUDE_DIRS} 
  ${PCL_INCLUDE_DIRS}
  ${CERES_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS})

add_executable(ascanRegistration src/scanRegistration.cpp)
target_link_libraries(ascanRegistration ${catkin_LIBRARIES} ${PCL_LIBRARIES})

add_executable(alaserOdometry src/laserOdometry.cpp)
target_link_libraries(alaserOdometry ${catkin_LIBRARIES} ${PCL_LIBRARIES} ${CERES_LIBRARIES})

add_executable(alaserMapping src/laserMapping.cpp)
target_link_libraries(alaserMapping ${catkin_LIBRARIES} ${PCL_LIBRARIES} ${CERES_LIBRARIES})

add_executable(kittiHelper src/kittiHelper.cpp)
target_link_libraries(kittiHelper ${catkin_LIBRARIES} ${PCL_LIBRARIES} ${OpenCV_LIBS})
```

包含这些第三方库的头文件，将第三方库文件跟源文件进行关联。

```
catkin_package(
  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp rospy std_msgs
  DEPENDS EIGEN3 PCL 
  INCLUDE_DIRS include
)
```

DEPENDS 和 CATKIN_DEPENDS 用来告诉 catkin ，我们建立的程序包有哪些依赖项。

##### README文件：

介绍了A-LOAM如何部署，还有一些数据集如何获取，作者的信息和Docker的配置。

##### Package.xml文件：

这是A-LOAM包信息，包含作者邮箱、包构建和运行的依赖项。

##### Include文件夹：

###### Common.h

该头文件定义了两个函数，一个是弧度转角度，一个是角度转弧度。

```
inline double rad2deg(double radians)
{
  return radians * 180.0 / M_PI;
}

inline double deg2rad(double degrees)
{
  return degrees * M_PI / 180.0;
}
```

###### **Tic_toc.h** 

```
class TicToc
{
  public:
    TicToc()
    {
        tic();
    }

    void tic()
    {
        start = std::chrono::system_clock::now();
    }

    double toc()
    {
        end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        return elapsed_seconds.count() * 1000;
    }

  private:
    std::chrono::time_point<std::chrono::system_clock> start, end;
};
```

该头文件定义了一个类，是作者自己写的用于计时的类。

用了C++的chrono时间库，调用system_clock类里面的now方法获取当前系统时间，在一个代码块的开头调用tic()方法（构造函数只需要实例化对象就可调用），结尾调用toc()方法，传回的参数就是代码块的执行时间，单位为ms。

##### Launch文件夹：

以aloam_velodyne_VLP_16.launch为例，launch文件中通过node符号可以开启多个节点，且rosmaster也同时开启了;同时还定义了包名，这在运行launch文件中会用到。通过param符号可作为参数服务器，以便在程序中读取。同时也可设置ROS自带显示软件RVIZ是否开启和开启的RVIZ配置路径。

```
<launch>
    
    <param name="scan_line" type="int" value="16" />

    <!-- if 1, do mapping 10 Hz, if 2, do mapping 5 Hz. Suggest to use 1, it will adjust frequence automaticlly -->
    <param name="mapping_skip_frame" type="int" value="1" />

    <!-- remove too closed points -->
    <param name="minimum_range" type="double" value="0.3"/>

    <param name="mapping_line_resolution" type="double" value="0.2"/>
    <param name="mapping_plane_resolution" type="double" value="0.4"/>

    <node pkg="aloam_velodyne" type="ascanRegistration" name="ascanRegistration" output="screen" />

    <node pkg="aloam_velodyne" type="alaserOdometry" name="alaserOdometry" output="screen" />

    <node pkg="aloam_velodyne" type="alaserMapping" name="alaserMapping" output="screen" />

    <arg name="rviz" default="true" />
    <group if="$(arg rviz)">
        <node launch-prefix="nice" pkg="rviz" type="rviz" name="rviz" args="-d $(find aloam_velodyne)/rviz_cfg/aloam_velodyne.rviz" />
    </group>

</launch>
```

##### rviz_cfg文件夹：

在这个文件夹存放rviz的配置文件，可在rviz软件中修改参数后进行保存。

##### src文件夹：

###### kittiHelper.cpp

这个源文件的作用是将kitti数据转换成ros下的话题数据，并可选择保存为bag文件。首先我们先看主函数。

```
int main(int argc, char** argv)
{
    ros::init(argc, argv, "kitti_helper");  //该节点名称，且初始化
    ros::NodeHandle n("~");
    std::string dataset_folder, sequence_number, output_bag_file;
    n.getParam("dataset_folder", dataset_folder); 
    //从参数服务器获取数据集文件夹和对应号码，可在launch文件中根据数据存放地址来修改
    n.getParam("sequence_number", sequence_number);
    std::cout << "Reading sequence " << sequence_number << " from " << dataset_folder << '\n';
    bool to_bag;
    n.getParam("to_bag", to_bag); //读取是否转换成bag标志
    if (to_bag)
        n.getParam("output_bag_file", output_bag_file); //同上，获取bag输出文件夹
    int publish_delay;
    n.getParam("publish_delay", publish_delay); //同上，获取发布延迟时间
    publish_delay = publish_delay <= 0 ? 1 : publish_delay; //三目运算符，延迟时间<=0 则设为1
    //初始化发布雷达话题数据
    ros::Publisher pub_laser_cloud = n.advertise<sensor_msgs::PointCloud2>("/velodyne_points", 2);

    //用image_transport初始化发布左右相机数据，对ImageTransport类进行实例化
    image_transport::ImageTransport it(n);
    image_transport::Publisher pub_image_left = it.advertise("/image_left", 2);
    image_transport::Publisher pub_image_right = it.advertise("/image_right", 2);

    //初始化里程计发布，ground_truth包括旋转的四元数和位置坐标向量，这里没有用上
    ros::Publisher pubOdomGT = n.advertise<nav_msgs::Odometry> ("/odometry_gt", 5);
    nav_msgs::Odometry odomGT;
    odomGT.header.frame_id = "/camera_init";  
    odomGT.child_frame_id = "/ground_truth";

    ros::Publisher pubPathGT = n.advertise<nav_msgs::Path> ("/path_gt", 5);
    nav_msgs::Path pathGT;
    pathGT.header.frame_id = "/camera_init";

    //获取时间戳地址
    std::string timestamp_path = "sequences/" + sequence_number + "/times.txt";
    std::ifstream timestamp_file(dataset_folder + timestamp_path, std::ifstream::in);

    std::string ground_truth_path = "results/" + sequence_number + ".txt";
std::ifstream ground_truth_file(dataset_folder + ground_truth_path, std::ifstream::in);
  rosbag::Bag bag_out;
    if (to_bag)
        bag_out.open(output_bag_file, rosbag::bagmode::Write); //新建并打开一个bag文件
    
    Eigen::Matrix3d R_transform;  //用在ground_truth数据中，这里没有用上
    R_transform << 0, 0, 1, -1, 0, 0, 0, -1, 0;
    Eigen::Quaterniond q_transform(R_transform);

    std::string line;
    std::size_t line_num = 0;

    ros::Rate r(10.0 / publish_delay); //ros循环频率

    // 遍历时间戳这个文本文件，得到时间戳信息
    std::cout << "timestamp_file " << std::string(dataset_folder + timestamp_path) << std::endl;
    while (std::getline(timestamp_file, line) && ros::ok())
    {
        // 把string转成浮点型float
        float timestamp = stof(line);
        std::stringstream left_image_path, right_image_path;
        // 找到对应的左右目的图片位置，并用opencv接口读取
        left_image_path << dataset_folder << "sequences/" + sequence_number + "/image_0/" << std::setfill('0') << std::setw(6) << line_num << ".png";
        cv::Mat left_image = cv::imread(left_image_path.str(), CV_LOAD_IMAGE_GRAYSCALE);
        right_image_path << dataset_folder << "sequences/" + sequence_number + "/image_1/" << std::setfill('0') << std::setw(6) << line_num << ".png";
        cv::Mat right_image = cv::imread(left_image_path.str(), CV_LOAD_IMAGE_GRAYSCALE);
        std::stringstream lidar_data_path;
        // 获取lidar数据的文件名
        lidar_data_path << dataset_folder << "velodyne/sequences/" + sequence_number + "/velodyne/" 
                        << std::setfill('0') << std::setw(6) << line_num << ".bin";
        //这里调用读雷达数据的函数
        std::vector<float> lidar_data = read_lidar_data(lidar_data_path.str());
        std::cout << "totally " << lidar_data.size() / 4.0 << " points in this lidar frame \n";

        std::vector<Eigen::Vector3d> lidar_points;
        std::vector<float> lidar_intensities;
        pcl::PointCloud<pcl::PointXYZI> laser_cloud;
        // 每个点数据占四个float数据，分别是xyz，intensity，存到laser_cloud容器中
        for (std::size_t i = 0; i < lidar_data.size(); i += 4)
        {
            lidar_points.emplace_back(lidar_data[i], lidar_data[i+1], lidar_data[i+2]);
            lidar_intensities.push_back(lidar_data[i+3]);
            // 构建pcl的点云格式
            pcl::PointXYZI point;
            point.x = lidar_data[i];
            point.y = lidar_data[i + 1];
            point.z = lidar_data[i + 2];
            point.intensity = lidar_data[i + 3];
            laser_cloud.push_back(point);
        }
        // 转成ros的消息格式
        sensor_msgs::PointCloud2 laser_cloud_msg;
        pcl::toROSMsg(laser_cloud, laser_cloud_msg);
        laser_cloud_msg.header.stamp = ros::Time().fromSec(timestamp); //设定雷达时间戳
        laser_cloud_msg.header.frame_id = "/camera_init"; //设定在相机坐标系下
        // 发布点云数据
        pub_laser_cloud.publish(laser_cloud_msg);
        // 图片也转成ros的消息发布出去
        sensor_msgs::ImagePtr image_left_msg = cv_bridge::CvImage(laser_cloud_msg.header, "mono8", left_image).toImageMsg();
        sensor_msgs::ImagePtr image_right_msg = cv_bridge::CvImage(laser_cloud_msg.header, "mono8", right_image).toImageMsg();
        pub_image_left.publish(image_left_msg);
        pub_image_right.publish(image_right_msg);
        // 也可以写到rosbag包中去
        if (to_bag)
        {
            bag_out.write("/image_left", ros::Time::now(), image_left_msg);
            bag_out.write("/image_right", ros::Time::now(), image_right_msg);
            bag_out.write("/velodyne_points", ros::Time::now(), laser_cloud_msg);
            bag_out.write("/path_gt", ros::Time::now(), pathGT);  //下面两个实际没有数据
            bag_out.write("/odometry_gt", ros::Time::now(), odomGT);  
        }
        line_num ++;
        r.sleep();
    }
    bag_out.close();
    std::cout << "Done \n";

    return 0;
}
```

我们再看一下read_lidar_data函数，功能是将雷达每一帧的数据读取到一个float类型的vector容器中。

```
std::vector<float> read_lidar_data(const std::string lidar_data_path)
{
    std::ifstream lidar_data_file(lidar_data_path, std::ifstream::in | std::ifstream::binary);
    lidar_data_file.seekg(0, std::ios::end);    // 文件指针指向文件末尾
    const size_t num_elements = lidar_data_file.tellg() / sizeof(float);    // 统计一下文件有多少float数据
    lidar_data_file.seekg(0, std::ios::beg);    // 再把指针指向文件开始

    std::vector<float> lidar_data_buffer(num_elements);
    // 读取所有的数据
    lidar_data_file.read(reinterpret_cast<char*>(&lidar_data_buffer[0]), num_elements*sizeof(float));
    return lidar_data_buffer;
}
```

调用rqt_graph，我们可以看到A-LOAM定义了三个节点，每个节点之间的消息链路如下：

![科研分享记录-2](.\img\科研分享记录-2.jpg)

###### scanRegistration.cpp

先看主函数的注释：主要是SLAM前端的参数读取，订阅话题和发布话题。

```
int main(int argc, char **argv)
{
    ros::init(argc, argv, "scanRegistration");  //节点名称
    ros::NodeHandle nh; //注册ROS句柄
    nh.param<int>("scan_line", N_SCANS, 16); //从launch文件参数服务器中获取多少线的激光雷达，如果没有则默认16线

    nh.param<double>("minimum_range", MINIMUM_RANGE, 0.1);
    // 从launch文件参数服务器中获取激光雷达的最小扫描距离MINIMUM_RANGE，小于MINIMUM_RANGE的点将被滤除，单位为M，如果没有则默认0.1。
    printf("scan line number %d \n", N_SCANS);
    // 只有线束是16、32、64的才可以继续
    if(N_SCANS != 16 && N_SCANS != 32 && N_SCANS != 64)
    {
        printf("only support velodyne with 16, 32 or 64 scan line!");
        return 0;
    }
    // 订阅初始的激光雷达数据，并注册回调函数laserCloudHandler
    ros::Subscriber subLaserCloud = nh.subscribe<sensor_msgs::PointCloud2>("/velodyne_points", 100, laserCloudHandler); //100为消息队列的长度 ，第三个参数为回调函数的入口

    // 发布话题：有序点云（删除过近点、设置索引），极大边线点集合，次极大边线点集合，极小平面点集合，次极小平面点集合,删除的点云

    pubLaserCloud = nh.advertise<sensor_msgs::PointCloud2>("/velodyne_cloud_2", 100);

    pubCornerPointsSharp = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_sharp", 100);

    pubCornerPointsLessSharp = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_less_sharp", 100);

    pubSurfPointsFlat = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_flat", 100);

    pubSurfPointsLessFlat = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_less_flat", 100);

    pubRemovePoints = nh.advertise<sensor_msgs::PointCloud2>("/laser_remove_points", 100);

    if(PUB_EACH_LINE)  //这个条件没有进去
    {
        for(int i = 0; i < N_SCANS; i++)
        {
            ros::Publisher tmp = nh.advertise<sensor_msgs::PointCloud2>("/laser_scanid_" + std::to_string(i), 100); //分线进行发送点云话题
            pubEachScan.push_back(tmp);
        }
    }
    ros::spin();// 循环执行回调函数

    return 0;
}
```

接下来看订阅雷达消息的回调函数，前端雷达处理和特征提取主要都在这一块：

```
// 订阅lidar消息
void laserCloudHandler(const sensor_msgs::PointCloud2ConstPtr &laserCloudMsg)
{
    //下面是做初始化，但是这里延时为0，相当于没有延时，只是留下了初始化的接口。
    if (!systemInited)
    { 
        systemInitCount++;
        if (systemInitCount >= systemDelay)
        {
            systemInited = true;
        }
        else
            return;
}

    //作者自己设计的计时类，以构造函数为起始时间，以toc()函数为终止时间，并返回时间间隔(ms)
    TicToc t_whole; //计算整个回调函数的时间
TicToc t_prepare; //计算雷达点云有序化的时间

    //每条雷达扫描线上的可以计算曲率的点云点的起始索引和结束索引，分别用scanStartInd数组和scanEndInd数组记录
    std::vector<int> scanStartInd(N_SCANS, 0);
    std::vector<int> scanEndInd(N_SCANS, 0);

    pcl::PointCloud<pcl::PointXYZ> laserCloudIn;
    // 把点云从ros格式转到pcl的格式
    pcl::fromROSMsg(*laserCloudMsg, laserCloudIn);
    std::vector<int> indices;
    // 去除掉点云中的nan点，即调用pcl库的函数移除无效点
    pcl::removeNaNFromPointCloud(laserCloudIn, laserCloudIn, indices);
    // 去除距离小于阈值的点，这里调用作者写的函数
    removeClosedPointCloud(laserCloudIn, laserCloudIn, MINIMUM_RANGE);
```

下面要计算点云角度范围，是为了使点云有序，需要做到两件事：为每个点找到它所对应的扫描线（SCAN）；为每条扫描线上的点分配时间戳。要计算每个点的时间戳，首先我们需要确定这个点的角度范围。可以使用<cmath>中的atan2( )函数计算点云点的水平角度。

这块在后面的激光框架基本都不需要了，因为后面雷达的驱动将每个点的线号、角度、时间戳都发出来了。

```
 // 计算起始点和结束点的角度，由于激光雷达是顺时针旋转，这里取反就相当于转成了逆时针
    int cloudSize = laserCloudIn.points.size();
float startOri = -atan2(laserCloudIn.points[0].y, laserCloudIn.points[0].x);
    /*
     * atan2()函数是atan(y， x)函数的增强版，不仅可以求取arctran(y/x)还能够确定象限
     * startOri和endOri分别为起始点和终止点的方位角
     * atan2范围是[-Pi,PI]，这里加上2PI是为了保证起始到结束相差2PI符合实际
     */
    float endOri = -atan2(laserCloudIn.points[cloudSize - 1].y,
                          laserCloudIn.points[cloudSize - 1].x) +
                   2 * M_PI;
   // 但总有一些例外，比如这里大于3PI，和小于PI，就需要调整到合理范围。
    if (endOri - startOri > 3 * M_PI)
    {
        endOri -= 2 * M_PI;
    }
    else if (endOri - startOri < M_PI)
    {
        endOri += 2 * M_PI;
    }
//为点云点找到对应的扫描线，每条扫描线都有它固定的俯仰角，我们可以根据点云点的垂直角度为其寻找对应的扫描线。
    bool halfPassed = false;
    int count = cloudSize;
    PointType point;
    std::vector<pcl::PointCloud<PointType>> laserCloudScans(N_SCANS);
   
 // 遍历每一个点
    for (int i = 0; i < cloudSize; i++)
    {
        point.x = laserCloudIn.points[i].x;
        point.y = laserCloudIn.points[i].y;
        point.z = laserCloudIn.points[i].z;

             //通过计算垂直视场角确定激光点在哪个扫描线上（N_SCANS线激光雷达）
        float angle = atan(point.z / sqrt(point.x * point.x + point.y * point.y)) * 180 / M_PI;
        int scanID = 0;
        if (N_SCANS == 16)
        {
            // 如果是16线激光雷达，结算出的angle应该在-15~15之间，+-15°的垂直视场，垂直角度分辨率2°，则-15°时的scanID = 0。
            scanID = int((angle + 15) / 2 + 0.5);
            if (scanID > (N_SCANS - 1) || scanID < 0)
            {
                count--;
                continue;
            }
        }
        else if (N_SCANS == 32)
        {
            scanID = int((angle + 92.0/3.0) * 3.0 / 4.0);
            if (scanID > (N_SCANS - 1) || scanID < 0)
            {
                count--;
                continue;
            }
        }
        else if (N_SCANS == 64)
        {   
            if (angle >= -8.83)
                scanID = int((2 - angle) * 3.0 + 0.5);
            else
                scanID = N_SCANS / 2 + int((-8.83 - angle) * 2.0 + 0.5);

            // use [0 50]  如果> 50 remove outlies 
            if (angle > 2 || angle < -24.33 || scanID > 50 || scanID < 0)
            {
                count--;
                continue;
            }
        }
        else
        {
            printf("wrong scan number\n");
            ROS_BREAK();
        }

  // 计算水平角
        float ori = -atan2(point.y, point.x);
          // 根据扫描线是否旋转过半选择与起始位置还是终止位置进行差值计算，从而进行补偿，如果此时扫描没有过半，则halfPassed为false
        if (!halfPassed)
        { 
            // 确保-PI / 2 < ori - startOri < 3 / 2 * PI， 如果ori-startOri小于-0.5pi或大于1.5pi，则调整ori的角度
            if (ori < startOri - M_PI / 2)
            {
                ori += 2 * M_PI;
            }
            else if (ori > startOri + M_PI * 3 / 2)
            {
                ori -= 2 * M_PI;
            }
                 
        //扫描点过半则设定halfPassed为true，如果超过180度，就说明过了一半了
            if (ori - startOri > M_PI)
            {
                halfPassed = true;
            }
        }
        else
        {
            // 确保-PI * 3 / 2 < ori - endOri < PI / 2
            ori += 2 * M_PI;    // 先补偿2PI
            if (ori < endOri - M_PI * 3 / 2)
            {
                ori += 2 * M_PI;
            }
            else if (ori > endOri + M_PI / 2)
            {
                ori -= 2 * M_PI;
            }
        }
       /*
         * relTime 是一个0~1之间的小数，代表占用一帧扫描时间的比例，乘以扫描时间得到真实扫描时刻，
         * scanPeriod扫描时间默认为0.1s
         * 角度的计算是为了计算相对起始时刻的时间
         */
        float relTime = (ori - startOri) / (endOri - startOri);
        // 整数部分是scan线束的索引，小数部分是相对起始时刻的时间
        point.intensity = scanID + scanPeriod * relTime;
        // 根据每条线的idx送入各自数组，表示这一条扫描线上的点
        laserCloudScans[scanID].push_back(point); 
    }
    // cloudSize是有效的点云的数目
    cloudSize = count;
    printf("points size %d \n", cloudSize);
```

前面处理了雷达点云数据，下面是前端的雷达特征提取，主要提取了线特征和面特征。LOAM提出了一种简单而高效的特征点提取方式，根据点云点的曲率来提取特征点。即把特别尖锐的边线点与特别平坦的平面点作为特征点。

曲率是求取做法是同一条扫描线上取目标点左右两侧各5个点，分别与目标点的坐标作差，得到的结果就是目标点的曲率。当目标点处在棱或角的位置时，自然与周围点的差值较大，得到的曲率较大，这时属于线特征；反之当目标点在平面上时，周围点与目标点的坐标相近，得到的曲率自然较小，这时属于面特征。

下面是曲率计算的程序：

```
  pcl::PointCloud<PointType>::Ptr laserCloud(new pcl::PointCloud<PointType>());
    // 全部集合到一个点云里面去，但是使用两个数组标记起始和结果，这里分别+5和-6是为了计算曲率方便
    for (int i = 0; i < N_SCANS; i++)
    { 
        // 前5个点和后5个点都无法计算曲率，因为他们不满足左右两侧各有5个点
        scanStartInd[i] = laserCloud->size() + 5;
        *laserCloud += laserCloudScans[i];
        scanEndInd[i] = laserCloud->size() - 6;
    }
    // 将一帧无序点云转换成有序点云消耗的时间，这里指的是前面处理雷达数据的时间
    printf("prepare time %f \n", t_prepare.toc());

    // 计算每一个点的曲率，这里的laserCloud是有序的点云，故可以直接这样计算
    // 但是在每条scan的交界处计算得到的曲率是不准确的，这可通过scanStartInd[i]、scanEndInd[i]来选取
    // 表面上除了前后五个点都应该有曲率，但是由于临近点都在scan上选取，实际上每条扫描线上的前后五个点也不太准确，应该由scanStartInd[i]、scanEndInd[i]来确定范围

    for (int i = 5; i < cloudSize - 5; i++)
    { 
        float diffX = laserCloud->points[i - 5].x + laserCloud->points[i - 4].x + laserCloud->points[i - 3].x + laserCloud->points[i - 2].x + laserCloud->points[i - 1].x - 10 * laserCloud->points[i].x + laserCloud->points[i + 1].x + laserCloud->points[i + 2].x + laserCloud->points[i + 3].x + laserCloud->points[i + 4].x + laserCloud->points[i + 5].x;
        float diffY = laserCloud->points[i - 5].y + laserCloud->points[i - 4].y + laserCloud->points[i - 3].y + laserCloud->points[i - 2].y + laserCloud->points[i - 1].y - 10 * laserCloud->points[i].y + laserCloud->points[i + 1].y + laserCloud->points[i + 2].y + laserCloud->points[i + 3].y + laserCloud->points[i + 4].y + laserCloud->points[i + 5].y;
        float diffZ = laserCloud->points[i - 5].z + laserCloud->points[i - 4].z + laserCloud->points[i - 3].z + laserCloud->points[i - 2].z + laserCloud->points[i - 1].z - 10 * laserCloud->points[i].z + laserCloud->points[i + 1].z + laserCloud->points[i + 2].z + laserCloud->points[i + 3].z + laserCloud->points[i + 4].z + laserCloud->points[i + 5].z;

        //存储每个点的曲率的索引
        /* 
         * cloudSortInd[i] = i相当于所有点的初始自然序列，每个点得到它自己的序号（索引）
         * 对于每个点，选择了它附近的特征点数量初始化为0，
         * 每个点的点类型初始设置为0（次极小平面点）
         */
        cloudCurvature[i] = diffX * diffX + diffY * diffY + diffZ * diffZ;
        cloudSortInd[i] = i;
        cloudNeighborPicked[i] = 0;
        cloudLabel[i] = 0;
    }
```

曲率计算完成后进行特征分类，提取特征点有几点原则：

1. 为了提高效率，每条扫描线分成6个扇区，在每个扇区内，寻找曲率最大的20个点，作为次极大边线点，其中最大的2个点，同时作为极大边线点；
2. 寻找曲率最小的4个点，作为极小平面点，剩下未被标记的点，全部作为次极小平面点。
3. 为了防止特征点过多聚堆，每提取一个特征点（极大/次极大边线点，极小平面点），都要将这个点和它附近的点全都标记为“已选中”，在下次提取特征点时，将会跳过这些点。对于次极小平面点，采取降采样的方法避免过多聚堆。

```
 TicToc t_pts; //计算特征提取的时间

    // 特征点集合用点云类保存
    pcl::PointCloud<PointType> cornerPointsSharp; // 极大边线点
    pcl::PointCloud<PointType> cornerPointsLessSharp; // 次极大边线点
    pcl::PointCloud<PointType> surfPointsFlat; // 极小平面点
    pcl::PointCloud<PointType> surfPointsLessFlat; // 次极小平面点
float t_q_sort = 0;   // 用来记录排序花费的总时间
    // 遍历每个scan，对每条线进行操作（曲率排序，选取对应特征点）
   for (int i = 0; i < N_SCANS; i++)
    {
        // 如果最后一个可算曲率的点与第一个的数量差小于6，说明无法分成6个扇区，跳过
        if( scanEndInd[i] - scanStartInd[i] < 6)
            continue;
        pcl::PointCloud<PointType>::Ptr surfPointsLessFlatScan(new pcl::PointCloud<PointType>);         // 用来存储次极小平面点，后面会进行降采样

        //为了使特征点均匀分布，将一个scan分成6个扇区
        for (int j = 0; j < 6; j++)
        {
            // 每个等分的起始和结束点
            int sp = scanStartInd[i] + (scanEndInd[i] - scanStartInd[i]) * j / 6; 
            int ep = scanStartInd[i] + (scanEndInd[i] - scanStartInd[i]) * (j + 1) / 6 - 1;

            TicToc t_tmp; //计算排序时间
            // 对点云按照曲率进行升序排序，小的在前，大的在后 
            std::sort (cloudSortInd + sp, cloudSortInd + ep + 1, comp);
            // t_q_sort累计每个扇区曲率排序时间总和
            t_q_sort += t_tmp.toc();
            // 选取极大边线点（2个）和次极大边线点（20个）
            int largestPickedNum = 0;
            // 挑选曲率比较大的部分，从最大曲率往最小曲率遍历，寻找边线点，并要求大于0.1
            for (int k = ep; k >= sp; k--)
            {
                // 排序后顺序就乱了，这个时候索引的作用就体现出来了
                int ind = cloudSortInd[k]; 

                // 看看这个点是否是有效点，同时曲率是否大于阈值，即没被选过 && 曲率 > 0.1
                if (cloudNeighborPicked[ind] == 0 &&
                    cloudCurvature[ind] > 0.1)
                {
                    largestPickedNum++;
                    // 每段选2个曲率大的点
                    if (largestPickedNum <= 2)
                    {                        
                        // label为2是曲率大的标记，即设置标签为极大边线点                     
                        cloudLabel[ind] = 2;
                        // cornerPointsSharp用来存放大曲率的点，既放入极大边线点，也放入次极大边线点
                        cornerPointsSharp.push_back(laserCloud->points[ind]);
                        cornerPointsLessSharp.push_back(laserCloud->points[ind]);
                    }
                    // 以及20个曲率稍微大一些的点
                    else if (largestPickedNum <= 20)
                    {                        
                        // label置1表示曲率稍微大一些，超过2个选择点以后，设置为次极大边线点，放入次极大边线点容器                       
                        cloudLabel[ind] = 1; 
                        cornerPointsLessSharp.push_back(laserCloud->points[ind]);
                    }
                    // 超过20个就跳过
                    else
                    {
                        break;
                    }
                    // 这个点被选中后pick标志位置1 ，记录这个点已经被选择了
                    cloudNeighborPicked[ind] = 1; 
   // 右侧
                    // 为了保证特征点不过度集中，将选中的点周围5个点都置1,避免后续会选到
                    for (int l = 1; l <= 5; l++)
                    {
                        // 查看相邻点距离是否差异过大，如果差异过大说明点云在此不连续，是特征边缘，就会是新的特征，因此就不置位了
                        float diffX = laserCloud->points[ind + l].x - laserCloud->points[ind + l - 1].x;
                        float diffY = laserCloud->points[ind + l].y - laserCloud->points[ind + l - 1].y;
                        float diffZ = laserCloud->points[ind + l].z - laserCloud->points[ind + l - 1].z;
                        if (diffX * diffX + diffY * diffY + diffZ * diffZ > 0.05)           // 相邻scan点距离的平方 <= 0.05的点标记为选择过，避免特征点密集分布

                        {
                            break;
                        }
                        cloudNeighborPicked[ind + l] = 1;
                    }
                    // 下面同理，左侧
                 for (int l = -1; l >= -5; l--)
                    {
                        float diffX = laserCloud->points[ind + l].x - laserCloud->points[ind + l + 1].x;
                        float diffY = laserCloud->points[ind + l].y - laserCloud->points[ind + l + 1].y;
                        float diffZ = laserCloud->points[ind + l].z - laserCloud->points[ind + l + 1].z;
                        if (diffX * diffX + diffY * diffY + diffZ * diffZ > 0.05)
                        {
                            break;
                        }
                        cloudNeighborPicked[ind + l] = 1;
                    }
                }
            }
       // 下面开始挑选面点，选取极小平面点（4个）
            int smallestPickedNum = 0;
            for (int k = sp; k <= ep; k++)
            {
                int ind = cloudSortInd[k];
                // 确保这个点没有被pick且曲率小于阈值
                if (cloudNeighborPicked[ind] == 0 &&
                    cloudCurvature[ind] < 0.1)
                {
                    // -1认为是平坦的点
                    cloudLabel[ind] = -1; 
                    surfPointsFlat.push_back(laserCloud->points[ind]);
                    smallestPickedNum++;
                    // 这里不区分极小和次极小，因为剩下的点label默认是0,就是次极小
                    if (smallestPickedNum >= 4)
                    { 
                        break;
                    }

                    // 下面同理：同样距离 < 0.05的点全设为已经选择过
                    cloudNeighborPicked[ind] = 1;
                    for (int l = 1; l <= 5; l++)
                    { 
                        float diffX = laserCloud->points[ind + l].x - laserCloud->points[ind + l - 1].x;
                        float diffY = laserCloud->points[ind + l].y - laserCloud->points[ind + l - 1].y;
                        float diffZ = laserCloud->points[ind + l].z - laserCloud->points[ind + l - 1].z;
                        if (diffX * diffX + diffY * diffY + diffZ * diffZ > 0.05)
                        {
                            break;
                        }
                        cloudNeighborPicked[ind + l] = 1;
                    }
                    for (int l = -1; l >= -5; l--)
                    {
                        float diffX = laserCloud->points[ind + l].x - laserCloud->points[ind + l + 1].x;
                        float diffY = laserCloud->points[ind + l].y - laserCloud->points[ind + l + 1].y;
                        float diffZ = laserCloud->points[ind + l].z - laserCloud->points[ind + l + 1].z;
                        if (diffX * diffX + diffY * diffY + diffZ * diffZ > 0.05)
                        {
                            break;
                        }

                        cloudNeighborPicked[ind + l] = 1;
                    }
                }
            }
            // 选取次极小平面点，除了极小平面点，剩下的都是次极小平面点
            for (int k = sp; k <= ep; k++)
            {
                if (cloudLabel[k] <= 0)
                {
                    surfPointsLessFlatScan->push_back(laserCloud->points[k]);
                }
            }
        }
      pcl::PointCloud<PointType> surfPointsLessFlatScanDS;
        pcl::VoxelGrid<PointType> downSizeFilter;
        // 一般次极小平面点比较多，所以这里做一个体素滤波来降采样
        downSizeFilter.setInputCloud(surfPointsLessFlatScan);
        downSizeFilter.setLeafSize(0.2, 0.2, 0.2);
        downSizeFilter.filter(surfPointsLessFlatScanDS);
        surfPointsLessFlat += surfPointsLessFlatScanDS;
    }
    printf("sort q time %f \n", t_q_sort); //打印排序时间
    printf("seperate points time %f \n", t_pts.toc()); //打印点云分类时间
  //发布有序点云，极大/次极大边线点，极小/次极小平面点，按需发布每条扫描线上的点云

    sensor_msgs::PointCloud2 laserCloudOutMsg;  // 创建publish msg实例
    pcl::toROSMsg(*laserCloud, laserCloudOutMsg);   // 有序点云转化为msg
    laserCloudOutMsg.header.stamp = laserCloudMsg->header.stamp; // 时间戳保持不变
    laserCloudOutMsg.header.frame_id = "/camera_init";  // frame_id名字，坐标系
    pubLaserCloud.publish(laserCloudOutMsg);  

    sensor_msgs::PointCloud2 cornerPointsSharpMsg;
    pcl::toROSMsg(cornerPointsSharp, cornerPointsSharpMsg);
    cornerPointsSharpMsg.header.stamp = laserCloudMsg->header.stamp;
    cornerPointsSharpMsg.header.frame_id = "/camera_init";
    pubCornerPointsSharp.publish(cornerPointsSharpMsg);

    sensor_msgs::PointCloud2 cornerPointsLessSharpMsg;
    pcl::toROSMsg(cornerPointsLessSharp, cornerPointsLessSharpMsg);
    cornerPointsLessSharpMsg.header.stamp = laserCloudMsg->header.stamp;
    cornerPointsLessSharpMsg.header.frame_id = "/camera_init";
    pubCornerPointsLessSharp.publish(cornerPointsLessSharpMsg);

    sensor_msgs::PointCloud2 surfPointsFlat2;
    pcl::toROSMsg(surfPointsFlat, surfPointsFlat2);
    surfPointsFlat2.header.stamp = laserCloudMsg->header.stamp;
    surfPointsFlat2.header.frame_id = "/camera_init";
    pubSurfPointsFlat.publish(surfPointsFlat2);

    sensor_msgs::PointCloud2 surfPointsLessFlat2;
    pcl::toROSMsg(surfPointsLessFlat, surfPointsLessFlat2);
    surfPointsLessFlat2.header.stamp = laserCloudMsg->header.stamp;
    surfPointsLessFlat2.header.frame_id = "/camera_init";
    pubSurfPointsLessFlat.publish(surfPointsLessFlat2);
  // pub each scam
    // 可以按照每个scan发出去，不过这里是false
    if(PUB_EACH_LINE)
    {
        for(int i = 0; i< N_SCANS; i++)
        {
            sensor_msgs::PointCloud2 scanMsg;
            pcl::toROSMsg(laserCloudScans[i], scanMsg);
            scanMsg.header.stamp = laserCloudMsg->header.stamp;
            scanMsg.header.frame_id = "/camera_init";
            pubEachScan[i].publish(scanMsg);
        }
    }

    printf("scan registration time %f ms *************\n", t_whole.toc());
    if(t_whole.toc() > 100)
        ROS_WARN("scan registration process over 100ms");
    printf("scan registration time %f ms *************\n", t_whole.toc());   //特征提取的时间，不超过100ms
    if(t_whole.toc() > 100)
        ROS_WARN("scan registration process over 100ms");
}
```

其他函数说明：

removeClosedPointCloud这个函数的作用是对距离小于阈值的点云进行滤除

```
void removeClosedPointCloud(const pcl::PointCloud<PointT> &cloud_in,
                              pcl::PointCloud<PointT> &cloud_out, float thres)
{
        // 假如输入输出点云不使用同一个变量，则需要将输出点云的时间戳和容器大小与输入点云同步
    if (&cloud_in != &cloud_out)
    {
        cloud_out.header = cloud_in.header;
        cloud_out.points.resize(cloud_in.points.size());
    }

    size_t j = 0;
    // 把点云距离小于给定阈值的去除掉
    for (size_t i = 0; i < cloud_in.points.size(); ++i)
    {
        if (cloud_in.points[i].x * cloud_in.points[i].x + cloud_in.points[i].y * cloud_in.points[i].y + cloud_in.points[i].z * cloud_in.points[i].z < thres * thres)
            continue;
        cloud_out.points[j] = cloud_in.points[i];
        j++;
    }
        // 重新调整输出容器大小
    if (j != cloud_in.points.size())
    {
        cloud_out.points.resize(j);
    }
    // 这里是对每条扫描线上的点云进行直通滤波，因此设置点云的高度为1，宽度为数量，稠密点云
    cloud_out.height = 1;
    cloud_out.width = static_cast<uint32_t>(j);
    cloud_out.is_dense = true;
}
```

###### laserOdometry.cpp

laserOdometry这个节点订阅了5个话题：有序点云、极大边线点、次极大边线点、极小平面点、次极小平面点。发布了4个话题：有序点云、上一帧的平面点、上一帧的边线点、当前帧位姿粗估计。主要功能是前端的激光里程计和位姿粗估计。

我们先看主函数：

```
int main(int argc, char **argv)
{
    ros::init(argc, argv, "laserOdometry");
    ros::NodeHandle nh;
    nh.param<int>("mapping_skip_frame", skipFrameNum, 2); //    设定里程计的帧率
//if 1, do mapping 10 Hz, if 2, do mapping 5 Hz. 
    printf("Mapping %d Hz \n", 10 / skipFrameNum);
    // 从scanRegistration节点订阅的消息话题，分别为极大边线点  次极大边线点   极小平面点  次极小平面点  全部点云点

    ros::Subscriber subCornerPointsSharp = nh.subscribe<sensor_msgs::PointCloud2>("/laser_cloud_sharp", 100, laserCloudSharpHandler);

    ros::Subscriber subCornerPointsLessSharp = nh.subscribe<sensor_msgs::PointCloud2>("/laser_cloud_less_sharp", 100, laserCloudLessSharpHandler);

    ros::Subscriber subSurfPointsFlat = nh.subscribe<sensor_msgs::PointCloud2>("/laser_cloud_flat", 100, laserCloudFlatHandler);

ros::Subscriber subSurfPointsLessFlat = nh.subscribe<sensor_msgs::PointCloud2>("/laser_cloud_less_flat", 100, laserCloudLessFlatHandler);

    ros::Subscriber subLaserCloudFullRes = nh.subscribe<sensor_msgs::PointCloud2>("/velodyne_cloud_2", 100, laserCloudFullResHandler);
    // 发布上一帧的边线点   
    ros::Publisher pubLaserCloudCornerLast = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_corner_last", 100);
    // 注册发布上一帧的边线点话题   
    ros::Publisher pubLaserCloudCornerLast = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_corner_last", 100);
    // 注册发布上一帧的平面点话题  
    ros::Publisher pubLaserCloudSurfLast = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_surf_last", 100);
    // 注册发布全部有序点云话题，就是从scanRegistration订阅来的点云，未经其他处理
    ros::Publisher pubLaserCloudFullRes = nh.advertise<sensor_msgs::PointCloud2>("/velodyne_cloud_3", 100);
    // 注册发布帧间的位姿变换话题  
    ros::Publisher pubLaserOdometry = nh.advertise<nav_msgs::Odometry>("/laser_odom_to_init", 100);
    // 注册发布帧间的平移运动话题  
    ros::Publisher pubLaserPath = nh.advertise<nav_msgs::Path>("/laser_odom_path", 100);

    int frameCount = 0;
    // 循环频率
    ros::Rate rate(100);
```

下面是主函数的主循环，主要是帧间位姿估计的过程，目标是希望找到位姿变换T，使得第k帧点云左乘T得到第k+1帧点云，或者说左乘T得到k+1帧点云的误差最小。

```
 while (ros::ok())
    {
        ros::spinOnce();    // 只触发一次回调，所以每次都要调用一次；等待回调函数执行完毕，执行一次后续代码，参考https://www.cnblogs.com/liu-fa/p/5925381.html
        // 首先确保订阅的五个消息都有，有一个队列为空都不行
        if (!cornerSharpBuf.empty() && !cornerLessSharpBuf.empty() &&
            !surfFlatBuf.empty() && !surfLessFlatBuf.empty() &&
            !fullPointsBuf.empty())
        {
            // 5个queue记录了最新的极大/次极大边线点，极小/次极小平面点，全部有序点云
            // 分别求出队列第一个时间，用来分配时间戳
            timeCornerPointsSharp = cornerSharpBuf.front()->header.stamp.toSec();
            timeCornerPointsLessSharp = cornerLessSharpBuf.front()->header.stamp.toSec();
            timeSurfPointsFlat = surfFlatBuf.front()->header.stamp.toSec();
            timeSurfPointsLessFlat = surfLessFlatBuf.front()->header.stamp.toSec();
            timeLaserCloudFullRes = fullPointsBuf.front()->header.stamp.toSec();
            // 因为同一帧的时间戳都是相同的，这里比较是否是同一帧
            if (timeCornerPointsSharp != timeLaserCloudFullRes ||
                timeCornerPointsLessSharp != timeLaserCloudFullRes ||
                timeSurfPointsFlat != timeLaserCloudFullRes ||
                timeSurfPointsLessFlat != timeLaserCloudFullRes)
            {
                printf("unsync messeage!");
                ROS_BREAK();
            }
          // 分别将五个点云消息取出来，同时转成pcl的点云格式
            mBuf.lock();  //数据多个线程使用，这里先进行加锁，避免线程冲突
            cornerPointsSharp->clear();
            pcl::fromROSMsg(*cornerSharpBuf.front(), *cornerPointsSharp);
            cornerSharpBuf.pop();

            cornerPointsLessSharp->clear();
            pcl::fromROSMsg(*cornerLessSharpBuf.front(), *cornerPointsLessSharp);
            cornerLessSharpBuf.pop();

            surfPointsFlat->clear();
            pcl::fromROSMsg(*surfFlatBuf.front(), *surfPointsFlat);
            surfFlatBuf.pop();

            surfPointsLessFlat->clear();
            pcl::fromROSMsg(*surfLessFlatBuf.front(), *surfPointsLessFlat);
            surfLessFlatBuf.pop();

            laserCloudFullRes->clear();
            pcl::fromROSMsg(*fullPointsBuf.front(), *laserCloudFullRes);
            fullPointsBuf.pop();
            mBuf.unlock(); //数据取出来后进行解锁
   TicToc t_whole; //计算整个激光雷达里程计的时间
            // initializing，一个什么也不干的初始化，没有延迟时间，主要用来跳过第一帧数据，直接作为第二帧的上一帧
            if (!systemInited)
            {
                systemInited = true;
                std::cout << "Initialization finished \n";
            }
            else //特征点匹配、位姿估计
            {
                // 取出比较突出的特征点数量，极大边线点和极小平面点
                int cornerPointsSharpNum = cornerPointsSharp->points.size();
                int surfPointsFlatNum = surfPointsFlat->points.size();

                TicToc t_opt;  //计算优化的时间
          // 点到线以及点到面的非线性优化，迭代2次（选择当前优化位姿的特征点匹配，并优化位姿（4次迭代），然后重新选择特征点匹配并优化）
                for (size_t opti_counter = 0; opti_counter < 2; ++opti_counter)
                {
                    corner_correspondence = 0;
                    plane_correspondence = 0;

          //ceres::LossFunction *loss_function = NULL;
                    // 定义一下ceres的核函数，使用Huber核函数来减少外点的影响，即去除outliers
                    ceres::LossFunction *loss_function = new ceres::HuberLoss(0.1);
                    // 由于旋转不满足一般意义的加法，因此这里使用ceres自带的local param
                    ceres::LocalParameterization *q_parameterization =
                        new ceres::EigenQuaternionParameterization();
                    ceres::Problem::Options problem_options;

                    ceres::Problem problem(problem_options);  //实例化求解最优化问题
                    // 待优化的变量是帧间位姿，包括平移和旋转，这里旋转使用四元数来表示
                    problem.AddParameterBlock(para_q, 4, q_parameterization);
                    problem.AddParameterBlock(para_t, 3);
                    pcl::PointXYZI pointSel;
                    std::vector<int> pointSearchInd;
                    std::vector<float> pointSearchSqDis;

                    TicToc t_data;  //计算寻找关联点的时间
```

基于最近邻原理建立corner特征点（边线点）之间的关联，每一个极大边线点去上一帧的次极大边线点中找匹配；采用边线点匹配方法:假如在第k+1帧中发现了边线点i，通过KD-tree查询在第k帧中的最近邻点j，查询j的附近扫描线上的最近邻点l，j与l相连形成一条直线l-j，让点i与这条直线的距离最短。

构建一个非线性优化问题：以点i与直线lj的距离为代价函数，以位姿变换T(四元数表示旋转+位移t)为优化变量。下面是优化的程序。

```
                   // find correspondence for corner features
     for (int i = 0; i < cornerPointsSharpNum; ++i) //先进行线点的匹配
                    {
                        // 运动补偿去畸变          
                        TransformToStart(&(cornerPointsSharp->points[i]), &pointSel);
                        // 在上一帧所有角点（次极大边线点）构成的kdtree中寻找距离当前帧最近的一个点
                        kdtreeCornerLast->nearestKSearch(pointSel, 1, pointSearchInd, pointSearchSqDis);

                        int closestPointInd = -1, minPointInd2 = -1;
                        //如果最近邻的corner特征点（次极大边线点）之间距离平方小于阈值，则最近邻点有效
                        if (pointSearchSqDis[0] < DISTANCE_SQ_THRESHOLD)
                        {
                            closestPointInd = pointSearchInd[0];    // 目标点对应的最近距离点的索引取出来
                            // 找到其所在线束id，线束信息是intensity的整数部分
                            int closestPointScanID = int(laserCloudCornerLast->points[closestPointInd].intensity);

                            double minPointSqDis2 = DISTANCE_SQ_THRESHOLD;
                            // search in the direction of increasing scan line
                            // 在刚刚角点（次极大边线点）id附近扫描线分别继续寻找最邻近点
  for (int j = closestPointInd + 1; j < (int)laserCloudCornerLast->points.size(); ++j)
                            {
                                // if in the same scan line, continue， 不找同一根线束的
                                if (int(laserCloudCornerLast->points[j].intensity) <= closestPointScanID)
                                    continue;

                                // if not in nearby scans, end the loop，即要求找到的线束距离当前线束不能太远
                                if (int(laserCloudCornerLast->points[j].intensity) > (closestPointScanID + NEARBY_SCAN))
                                    break;

                                // 计算和当前找到的角点（次极大边线点）之间的距离
                                double pointSqDis = (laserCloudCornerLast->points[j].x - pointSel.x) *
                                                        (laserCloudCornerLast->points[j].x - pointSel.x) +
                                                    (laserCloudCornerLast->points[j].y - pointSel.y) *
                                                        (laserCloudCornerLast->points[j].y - pointSel.y) +
                                                    (laserCloudCornerLast->points[j].z - pointSel.z) *
                                                        (laserCloudCornerLast->points[j].z - pointSel.z);
                            
                                if (pointSqDis < minPointSqDis2)
                                {
                                    // find nearer point，寻找距离最小的角点（次极大边线点）及其索引，记录其索引
                                    minPointSqDis2 = pointSqDis;
                                    minPointInd2 = j;
                                }
                            }
     // search in the direction of decreasing scan line
                            // 同样另一个方向寻找对应角点（次极大边线点）
                            for (int j = closestPointInd - 1; j >= 0; --j)
                            {
                                // if in the same scan line, continue， 不找同一根线束的
                                if (int(laserCloudCornerLast->points[j].intensity) >= closestPointScanID)
                                    continue;

                                // if not in nearby scans, end the loop，即要求找到的线束距离当前线束不能太远
                                if (int(laserCloudCornerLast->points[j].intensity) < (closestPointScanID - NEARBY_SCAN))
                                    break;

                                double pointSqDis = (laserCloudCornerLast->points[j].x - pointSel.x) *
                                                        (laserCloudCornerLast->points[j].x - pointSel.x) +
                                                    (laserCloudCornerLast->points[j].y - pointSel.y) *
                                                        (laserCloudCornerLast->points[j].y - pointSel.y) +
                                                    (laserCloudCornerLast->points[j].z - pointSel.z) *
                                                        (laserCloudCornerLast->points[j].z - pointSel.z);

                                if (pointSqDis < minPointSqDis2)
                                {
                                    // find nearer point，寻找距离最小的角点（次极大边线点）及其索引， 记录其索引
                                    minPointSqDis2 = pointSqDis;
                                    minPointInd2 = j;
                                }
                            }
                        }
          // 如果特征点i的两个最近邻点j和m都有效，构建非线性优化问题
                        if (minPointInd2 >= 0) // both closestPointInd and minPointInd2 is valid
                        {
                            // 取出当前点和上一帧的两个角点
                            Eigen::Vector3d curr_point(cornerPointsSharp->points[i].x,
                                                       cornerPointsSharp->points[i].y,
                                                       cornerPointsSharp->points[i].z);
                            Eigen::Vector3d last_point_a(laserCloudCornerLast->points[closestPointInd].x,
                                                         laserCloudCornerLast->points[closestPointInd].y,
                                                         laserCloudCornerLast->points[closestPointInd].z);
                            Eigen::Vector3d last_point_b(laserCloudCornerLast->points[minPointInd2].x,
                                                         laserCloudCornerLast->points[minPointInd2].y,
                                                         laserCloudCornerLast->points[minPointInd2].z);

                            double s;
                            if (DISTORTION) //去运动畸变，这里没有做，kitii数据已经做了
                                s = (cornerPointsSharp->points[i].intensity - int(cornerPointsSharp->points[i].intensity)) / SCAN_PERIOD;
                            else
                                s = 1.0;
                            ceres::CostFunction *cost_function = LidarEdgeFactor::Create(curr_point, last_point_a, last_point_b, s);
                            problem.AddResidualBlock(cost_function, loss_function, para_q, para_t);    //构建优化问题并求解
                            corner_correspondence++;
                        }
                    }
```

下面采用平面点匹配方法：假如在第k+1帧中发现了平面点i，通过KD-tree查询在第k帧（上一帧）中的最近邻点j，查询j的附近扫描线上的最近邻点l和同一条扫描线的最近邻点m，这三点确定一个平面，让点i与这个平面的距离最短；

构建一个非线性优化问题：以点i与平面lmj的距离为代价函数，以位姿变换T(四元数表示旋转+t)为优化变量。

```
     for (int i = 0; i < surfPointsFlatNum; ++i)
                    {
                        TransformToStart(&(surfPointsFlat->points[i]), &pointSel); //运动补偿去畸变
                        // 先寻找上一帧距离这个面点最近的面点
                        kdtreeSurfLast->nearestKSearch(pointSel, 1, pointSearchInd, pointSearchSqDis);

                        int closestPointInd = -1, minPointInd2 = -1, minPointInd3 = -1;
                        // 距离必须小于给定阈值
                        if (pointSearchSqDis[0] < DISTANCE_SQ_THRESHOLD)
                        {
                            // 取出找到的上一帧面点的索引
                            closestPointInd = pointSearchInd[0];

                            // get closest point's scan ID
                            // 取出最近的面点在上一帧的第几根scan上面
                            int closestPointScanID = int(laserCloudSurfLast->points[closestPointInd].intensity);
                            double minPointSqDis2 = DISTANCE_SQ_THRESHOLD, minPointSqDis3 = DISTANCE_SQ_THRESHOLD;

                            // search in the direction of increasing scan line
                            // 额外再寻找两个点，要求一个点和最近点同一个scan，另一个是不同scan，先升序遍历搜索点寻找这些点
                            for (int j = closestPointInd + 1; j < (int)laserCloudSurfLast->points.size(); ++j)
                            {
                                // if not in nearby scans, end the loop
                                // 不能和当前找到的上一帧面点线束距离太远
                                if (int(laserCloudSurfLast->points[j].intensity) > (closestPointScanID + NEARBY_SCAN))
                                    break;
                                // 计算和当前帧该点距离
                                double pointSqDis = (laserCloudSurfLast->points[j].x - pointSel.x) *
                                                        (laserCloudSurfLast->points[j].x - pointSel.x) +
                                                    (laserCloudSurfLast->points[j].y - pointSel.y) *
                                                        (laserCloudSurfLast->points[j].y - pointSel.y) +
                                                    (laserCloudSurfLast->points[j].z - pointSel.z) *
                                                        (laserCloudSurfLast->points[j].z - pointSel.z);

                                // if in the same or lower scan line
                                // 如果是同一根scan且距离最近
                                if (int(laserCloudSurfLast->points[j].intensity) <= closestPointScanID && pointSqDis < minPointSqDis2)
                                {
                                    minPointSqDis2 = pointSqDis;
                                    minPointInd2 = j;
                                }
                                // if in the higher scan line
                                // 如果是其他线束点
                                else if (int(laserCloudSurfLast->points[j].intensity) > closestPointScanID && pointSqDis < minPointSqDis3)
                                {
                                    minPointSqDis3 = pointSqDis;
                                    minPointInd3 = j;
                                }
                            }

                            // search in the direction of decreasing scan line
                            // 同样的方式，按照降序方向寻找这两个点
                            for (int j = closestPointInd - 1; j >= 0; --j)
                            {
                                // if not in nearby scans, end the loop
                                if (int(laserCloudSurfLast->points[j].intensity) < (closestPointScanID - NEARBY_SCAN))
                                    break;

                                double pointSqDis = (laserCloudSurfLast->points[j].x - pointSel.x) *
                                                        (laserCloudSurfLast->points[j].x - pointSel.x) +
                                                    (laserCloudSurfLast->points[j].y - pointSel.y) *
                                                        (laserCloudSurfLast->points[j].y - pointSel.y) +
                                                    (laserCloudSurfLast->points[j].z - pointSel.z) *
                                                        (laserCloudSurfLast->points[j].z - pointSel.z);

                                // if in the same or higher scan line
                                if (int(laserCloudSurfLast->points[j].intensity) >= closestPointScanID && pointSqDis < minPointSqDis2)
                                {
                                    minPointSqDis2 = pointSqDis;
                                    minPointInd2 = j;
                                }
                                else if (int(laserCloudSurfLast->points[j].intensity) < closestPointScanID && pointSqDis < minPointSqDis3)
                                {
                                    // find nearer point
                                    minPointSqDis3 = pointSqDis;
                                    minPointInd3 = j;
                                }
                            }
                            // 如果另外找到的两个点是有效点，就取出他们的3d坐标
                            if (minPointInd2 >= 0 && minPointInd3 >= 0)
                            {

                                Eigen::Vector3d curr_point(surfPointsFlat->points[i].x,
                                                            surfPointsFlat->points[i].y,
                                                            surfPointsFlat->points[i].z);
                                Eigen::Vector3d last_point_a(laserCloudSurfLast->points[closestPointInd].x,
                                                                laserCloudSurfLast->points[closestPointInd].y,
                                                                laserCloudSurfLast->points[closestPointInd].z);
                                Eigen::Vector3d last_point_b(laserCloudSurfLast->points[minPointInd2].x,
                                                                laserCloudSurfLast->points[minPointInd2].y,
                                                                laserCloudSurfLast->points[minPointInd2].z);
                                Eigen::Vector3d last_point_c(laserCloudSurfLast->points[minPointInd3].x,
                                                                laserCloudSurfLast->points[minPointInd3].y,
                                                                laserCloudSurfLast->points[minPointInd3].z);

                                double s;
                                if (DISTORTION) //去运动畸变，这里没有做，kitii数据已经做了
                                    s = (surfPointsFlat->points[i].intensity - int(surfPointsFlat->points[i].intensity)) / SCAN_PERIOD;
                                else
                                    s = 1.0;
                                // 构建点到面的约束，构建cere的非线性优化问题。
                                ceres::CostFunction *cost_function = LidarPlaneFactor::Create(curr_point, last_point_a, last_point_b, last_point_c, s);
                                problem.AddResidualBlock(cost_function, loss_function, para_q, para_t); 
                                plane_correspondence++;
                            }
                        }
                    }

                    // 输出寻找关联点消耗的时间
                    //printf("coner_correspondance %d, plane_correspondence %d \n", corner_correspondence, plane_correspondence);
                    printf("data association time %f ms \n", t_data.toc());
                    // 如果总的线约束和面约束太少，就打印一下  
                    if ((corner_correspondence + plane_correspondence) < 10)
                    {
                        printf("less correspondence! *************************************************\n");
                    }
                    // 调用ceres求解器求解 ，设定求解器类型，最大迭代次数，不输出过程信息，优化报告存入summary
                    TicToc t_solver;
                    ceres::Solver::Options options;
                    options.linear_solver_type = ceres::DENSE_QR;
                    options.max_num_iterations = 4;
                    options.minimizer_progress_to_stdout = false;
                    ceres::Solver::Summary summary;
                    ceres::Solve(options, &problem, &summary);
                    printf("solver time %f ms \n", t_solver.toc());
                }
                // 经过两次LM优化消耗的时间
                printf("optimization twice time %f \n", t_opt.toc());
                // 这里的w_curr 实际上是 w_last，即上一帧
                // 更新帧间匹配的结果，得到lidar odom位姿
                t_w_curr = t_w_curr + q_w_curr * t_last_curr;
                q_w_curr = q_w_curr * q_last_curr;
            }

            TicToc t_pub; //计算发布运行时间
            // 发布lidar里程计结果
            // publish odometry
            // 创建nav_msgs::Odometry消息类型，把信息导入，并发布
            nav_msgs::Odometry laserOdometry;
            laserOdometry.header.frame_id = "/camera_init"; //选择相机坐标系
            laserOdometry.child_frame_id = "/laser_odom"; 
            laserOdometry.header.stamp = ros::Time().fromSec(timeSurfPointsLessFlat);
            // 以四元数和平移向量发出去
            laserOdometry.pose.pose.orientation.x = q_w_curr.x();
            laserOdometry.pose.pose.orientation.y = q_w_curr.y();
            laserOdometry.pose.pose.orientation.z = q_w_curr.z();
            laserOdometry.pose.pose.orientation.w = q_w_curr.w();
            laserOdometry.pose.pose.position.x = t_w_curr.x();
            laserOdometry.pose.pose.position.y = t_w_curr.y();
            laserOdometry.pose.pose.position.z = t_w_curr.z();
            pubLaserOdometry.publish(laserOdometry);

            // geometry_msgs::PoseStamped消息是laserOdometry的部分内容
            geometry_msgs::PoseStamped laserPose;
            laserPose.header = laserOdometry.header;
            laserPose.pose = laserOdometry.pose.pose;
            laserPath.header.stamp = laserOdometry.header.stamp;
            laserPath.poses.push_back(laserPose);
            laserPath.header.frame_id = "/camera_init";
            pubLaserPath.publish(laserPath);

            // transform corner features and plane features to the scan end point
            //去畸变，没有调用
            if (0)
            {
                int cornerPointsLessSharpNum = cornerPointsLessSharp->points.size();
                for (int i = 0; i < cornerPointsLessSharpNum; i++)
                {
                    TransformToEnd(&cornerPointsLessSharp->points[i], &cornerPointsLessSharp->points[i]);
                }

                int surfPointsLessFlatNum = surfPointsLessFlat->points.size();
                for (int i = 0; i < surfPointsLessFlatNum; i++)
                {
                    TransformToEnd(&surfPointsLessFlat->points[i], &surfPointsLessFlat->points[i]);
                }

                int laserCloudFullResNum = laserCloudFullRes->points.size();
                for (int i = 0; i < laserCloudFullResNum; i++)
                {
                    TransformToEnd(&laserCloudFullRes->points[i], &laserCloudFullRes->points[i]);
                }
            }

            // 位姿估计完毕之后，当前边线点和平面点就变成了上一帧的边线点和平面点，把索引和数量都转移过去
            pcl::PointCloud<PointType>::Ptr laserCloudTemp = cornerPointsLessSharp;
            cornerPointsLessSharp = laserCloudCornerLast;
            laserCloudCornerLast = laserCloudTemp;

            laserCloudTemp = surfPointsLessFlat;
            surfPointsLessFlat = laserCloudSurfLast;
            laserCloudSurfLast = laserCloudTemp;

            laserCloudCornerLastNum = laserCloudCornerLast->points.size();
            laserCloudSurfLastNum = laserCloudSurfLast->points.size();

            // std::cout << "the size of corner last is " << laserCloudCornerLastNum << ", and the size of surf last is " << laserCloudSurfLastNum << '\n';
            // kdtree设置当前帧，用来下一帧lidar odom使用
            kdtreeCornerLast->setInputCloud(laserCloudCornerLast);
            kdtreeSurfLast->setInputCloud(laserCloudSurfLast);
            // 控制后端节点的执行频率，降频后给后端发送，只有整除时才发布结果
            if (frameCount % skipFrameNum == 0)
            {
                frameCount = 0;
                // 发布边线点
                sensor_msgs::PointCloud2 laserCloudCornerLast2;
                pcl::toROSMsg(*laserCloudCornerLast, laserCloudCornerLast2);
                laserCloudCornerLast2.header.stamp = ros::Time().fromSec(timeSurfPointsLessFlat);
                laserCloudCornerLast2.header.frame_id = "/camera";
                pubLaserCloudCornerLast.publish(laserCloudCornerLast2);
                // 发布平面点
                sensor_msgs::PointCloud2 laserCloudSurfLast2;
                pcl::toROSMsg(*laserCloudSurfLast, laserCloudSurfLast2);
                laserCloudSurfLast2.header.stamp = ros::Time().fromSec(timeSurfPointsLessFlat);
                laserCloudSurfLast2.header.frame_id = "/camera";
                pubLaserCloudSurfLast.publish(laserCloudSurfLast2);
                // 原封不动的转发当前帧点云
                sensor_msgs::PointCloud2 laserCloudFullRes3;
                pcl::toROSMsg(*laserCloudFullRes, laserCloudFullRes3);
                laserCloudFullRes3.header.stamp = ros::Time().fromSec(timeSurfPointsLessFlat);
                laserCloudFullRes3.header.frame_id = "/camera";
                pubLaserCloudFullRes.publish(laserCloudFullRes3);
            }
            printf("publication time %f ms \n", t_pub.toc());  
            printf("whole laserOdometry time %f ms \n \n", t_whole.toc());
            if(t_whole.toc() > 100)  //里程计超过100ms则有问题
                ROS_WARN("odometry process over 100ms");

            frameCount++;
        }
        rate.sleep();
    }
    return 0;
}
```

回调函数注释如下：

```
// 操作都是送去各自的队列中，加了线程锁以避免线程数据冲突
void laserCloudSharpHandler(const sensor_msgs::PointCloud2ConstPtr &cornerPointsSharp2)
{
    mBuf.lock();
    cornerSharpBuf.push(cornerPointsSharp2);
    mBuf.unlock();
}

void laserCloudLessSharpHandler(const sensor_msgs::PointCloud2ConstPtr &cornerPointsLessSharp2)
{
    mBuf.lock();
    cornerLessSharpBuf.push(cornerPointsLessSharp2);
    mBuf.unlock();
}

void laserCloudFlatHandler(const sensor_msgs::PointCloud2ConstPtr &surfPointsFlat2)
{
    mBuf.lock();
    surfFlatBuf.push(surfPointsFlat2);
    mBuf.unlock();
}

void laserCloudLessFlatHandler(const sensor_msgs::PointCloud2ConstPtr &surfPointsLessFlat2)
{
    mBuf.lock();
    surfLessFlatBuf.push(surfPointsLessFlat2);
    mBuf.unlock();
}

//receive all point cloud
void laserCloudFullResHandler(const sensor_msgs::PointCloud2ConstPtr &laserCloudFullRes2)
{
    mBuf.lock();
    fullPointsBuf.push(laserCloudFullRes2);
    mBuf.unlock();
}
```

激光雷达在运动过程中存在一定的运动畸变，即同一帧点云中，各个点在采集时，LiDAR的位姿是不同的，就如同高速移动相机时拍摄的照片一样。

那么如何进行运动补偿呢？即将所有的点云补偿到某一个时刻。

常用的做法是补偿到起始时刻，如果有IMU，我们通过IMU得到的雷达高频位姿，可以求出每个点相对起始点的位姿，就可以补偿回去。

如果没有IMU，我们可以使用匀速模型假设，使⽤上⼀个帧间⾥程记的结果作为当前两帧之间的运动，假设当前帧也是匀速运动，可以估计出每个点相对起始时刻的位姿。

最后，当前点云中的点相对第一个点去除因运动产生的畸变，效果相当于静止扫描得到的点云。

```
// undistort lidar point
void TransformToStart(PointType const *const pi, PointType *const po)
{
    //interpolation ratio
    double s;
    // 由于kitti数据集上的lidar已经做过了运动补偿，因此这里就不做具体补偿了
    if (DISTORTION)
        s = (pi->intensity - int(pi->intensity)) / SCAN_PERIOD;
    else
        s = 1.0;    // s = 1.0说明全部补偿到点云结束的时刻
    // 所有点的操作方式都是一致的，相当于从结束时刻补偿到起始时刻，相当于是一个匀速模型的假设
    Eigen::Quaterniond q_point_last = Eigen::Quaterniond::Identity().slerp(s, q_last_curr);
    Eigen::Vector3d t_point_last = s * t_last_curr;
    Eigen::Vector3d point(pi->x, pi->y, pi->z);
    Eigen::Vector3d un_point = q_point_last * point + t_point_last;

    po->x = un_point.x();
    po->y = un_point.y();
    po->z = un_point.z();
    po->intensity = pi->intensity;
}
```

下面介绍下ceres优化问题如何求解：定义一个模板类，重写优化问题，在这个类中定义残差，重载（）运算符，（）运算符函数前⼏个参数是参数块的起始指针，最后⼀个参数是残差块的指针，（）运算符负责计算残差。下面是优化边线点的模板类程序。

```
struct LidarEdgeFactor
{
    LidarEdgeFactor(Eigen::Vector3d curr_point_, Eigen::Vector3d last_point_a_,
                    Eigen::Vector3d last_point_b_, double s_)
        : curr_point(curr_point_), last_point_a(last_point_a_), last_point_b(last_point_b_), s(s_) {}

    template <typename T>
    bool operator()(const T *q, const T *t, T *residual) const
    {
        // 将double数组转成eigen的数据结构，注意这里必须都写成模板
        Eigen::Matrix<T, 3, 1> cp{T(curr_point.x()), T(curr_point.y()), T(curr_point.z())};
        Eigen::Matrix<T, 3, 1> lpa{T(last_point_a.x()), T(last_point_a.y()), T(last_point_a.z())};
        Eigen::Matrix<T, 3, 1> lpb{T(last_point_b.x()), T(last_point_b.y()), T(last_point_b.z())};

        //Eigen::Quaternion<T> q_last_curr{q[3], T(s) * q[0], T(s) * q[1], T(s) * q[2]};
        Eigen::Quaternion<T> q_last_curr{q[3], q[0], q[1], q[2]};
        Eigen::Quaternion<T> q_identity{T(1), T(0), T(0), T(0)};
        // 计算的是上一帧到当前帧的位姿变换，因此根据匀速模型，计算该点对应的位姿
        // 这里暂时不考虑畸变，因此这里不做任何变换
        q_last_curr = q_identity.slerp(T(s), q_last_curr);
        Eigen::Matrix<T, 3, 1> t_last_curr{T(s) * t[0], T(s) * t[1], T(s) * t[2]};

        Eigen::Matrix<T, 3, 1> lp;
        // 把当前点根据当前计算的帧间位姿变换到上一帧
        lp = q_last_curr * cp + t_last_curr;

        Eigen::Matrix<T, 3, 1> nu = (lp - lpa).cross(lp - lpb); // 模是三角形的面积
        Eigen::Matrix<T, 3, 1> de = lpa - lpb;
        // 残差的模是该点到底边的垂线长度
        // 这里感觉不需要定义三维
        residual[0] = nu.x() / de.norm();
        residual[1] = nu.y() / de.norm();
        residual[2] = nu.z() / de.norm();

        return true;
}    
 static ceres::CostFunction *Create(const Eigen::Vector3d curr_point_, const Eigen::Vector3d last_point_a_,
                                       const Eigen::Vector3d last_point_b_, const double s_)
    {
        return (new ceres::AutoDiffCostFunction<
                LidarEdgeFactor, 3, 4, 3>(
            new LidarEdgeFactor(curr_point_, last_point_a_, last_point_b_, s_)));
    }

    Eigen::Vector3d curr_point, last_point_a, last_point_b;
    double s;
};

下面是优化面点的模板类程序。
struct LidarPlaneFactor
{
    LidarPlaneFactor(Eigen::Vector3d curr_point_, Eigen::Vector3d last_point_j_,
                     Eigen::Vector3d last_point_l_, Eigen::Vector3d last_point_m_, double s_)
        : curr_point(curr_point_), last_point_j(last_point_j_), last_point_l(last_point_l_),
          last_point_m(last_point_m_), s(s_)
    {
        // 求出平面单位法向量
        ljm_norm = (last_point_j - last_point_l).cross(last_point_j - last_point_m);
        ljm_norm.normalize();
    }

    template <typename T>
    bool operator()(const T *q, const T *t, T *residual) const
    {

        Eigen::Matrix<T, 3, 1> cp{T(curr_point.x()), T(curr_point.y()), T(curr_point.z())};
        Eigen::Matrix<T, 3, 1> lpj{T(last_point_j.x()), T(last_point_j.y()), T(last_point_j.z())};
        //Eigen::Matrix<T, 3, 1> lpl{T(last_point_l.x()), T(last_point_l.y()), T(last_point_l.z())};
        //Eigen::Matrix<T, 3, 1> lpm{T(last_point_m.x()), T(last_point_m.y()), T(last_point_m.z())};
        Eigen::Matrix<T, 3, 1> ljm{T(ljm_norm.x()), T(ljm_norm.y()), T(ljm_norm.z())};

        //Eigen::Quaternion<T> q_last_curr{q[3], T(s) * q[0], T(s) * q[1], T(s) * q[2]};
        Eigen::Quaternion<T> q_last_curr{q[3], q[0], q[1], q[2]};
        Eigen::Quaternion<T> q_identity{T(1), T(0), T(0), T(0)};
        // 根据时间戳进行插值
        q_last_curr = q_identity.slerp(T(s), q_last_curr);
        Eigen::Matrix<T, 3, 1> t_last_curr{T(s) * t[0], T(s) * t[1], T(s) * t[2]};

        Eigen::Matrix<T, 3, 1> lp;
        lp = q_last_curr * cp + t_last_curr;
        // 点到平面的距离
        residual[0] = (lp - lpj).dot(ljm);

        return true;
    }

    static ceres::CostFunction *Create(const Eigen::Vector3d curr_point_, const Eigen::Vector3d last_point_j_,
                                       const Eigen::Vector3d last_point_l_, const Eigen::Vector3d last_point_m_,
                                       const double s_)
    {
        return (new ceres::AutoDiffCostFunction<
                LidarPlaneFactor, 1, 4, 3>(
            new LidarPlaneFactor(curr_point_, last_point_j_, last_point_l_, last_point_m_, s_)));
    }

    Eigen::Vector3d curr_point, last_point_j, last_point_l, last_point_m;
    Eigen::Vector3d ljm_norm;
    double s;
};
```

###### laserMapping.cpp

laserMapping节点订阅了来自laserOdometry的四个话题：当前帧全部点云、上一帧的边线点集合，上一帧的平面点集合，以及当前帧的位姿粗估计。发布了四个话题：附近帧组成的点云子地图（submap），所有帧组成的点云地图，当前帧位姿精估计。

我们先看主函数：

```
int main(int argc, char **argv)
{
    ros::init(argc, argv, "laserMapping");
    ros::NodeHandle nh;

    float lineRes = 0;   // 次极大边线点云体素滤波分辨率
    float planeRes = 0;  // 次极小平面点云体素滤波分辨率
    nh.param<float>("mapping_line_resolution", lineRes, 0.4);
    nh.param<float>("mapping_plane_resolution", planeRes, 0.8);
    printf("line resolution %f plane resolution %f \n", lineRes, planeRes);
    downSizeFilterCorner.setLeafSize(lineRes, lineRes,lineRes); //进行体素滤波实现降采样
    downSizeFilterSurf.setLeafSize(planeRes, planeRes, planeRes);

    // 订阅了点云以及起始位姿
    // 从laserOdometry节点接收边线点
    ros::Subscriber subLaserCloudCornerLast = nh.subscribe<sensor_msgs::PointCloud2>("/laser_cloud_corner_last", 100, laserCloudCornerLastHandler);
    // 从laserOdometry节点接收平面点
    ros::Subscriber subLaserCloudSurfLast = nh.subscribe<sensor_msgs::PointCloud2>("/laser_cloud_surf_last", 100, laserCloudSurfLastHandler);
    // 从laserOdometry节点接收到的最新帧的位姿T_cur^w
    ros::Subscriber subLaserOdometry = nh.subscribe<nav_msgs::Odometry>("/laser_odom_to_init", 100, laserOdometryHandler);
    // 从laserOdometry节点接收到的当前帧原始点云（只经过一次降采样）
    ros::Subscriber subLaserCloudFullRes = nh.subscribe<sensor_msgs::PointCloud2>("/velodyne_cloud_3", 100, laserCloudFullResHandler);

    //注册发布话题
    // submap（子地图）所在cube（栅格）中的点云，发布周围5帧的点云（降采样以后的）
    pubLaserCloudSurround = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_surround", 100);
    //map地图
    pubLaserCloudMap = nh.advertise<sensor_msgs::PointCloud2>("/laser_cloud_map", 100);

    // 当前帧原始点云
    pubLaserCloudFullRes = nh.advertise<sensor_msgs::PointCloud2>("/velodyne_cloud_registered", 100);

    //经过Map to Map精估计优化后的当前帧位姿
    pubOdomAftMapped = nh.advertise<nav_msgs::Odometry>("/aft_mapped_to_init", 100);

     // 将里程计坐标系位姿转化到世界坐标系位姿（地图坐标系），相当于位姿优化初值，即Odometry odom 到  map
    pubOdomAftMappedHighFrec = nh.advertise<nav_msgs::Odometry>("/aft_mapped_to_init_high_frec", 100);
    // 经过Map to Map精估计优化后的当前帧平移
pubLaserAfterMappedPath = nh.advertise<nav_msgs::Path>("/aft_mapped_path", 100);
    //重置这两个数组，这两数组用于存储所有边线点栅格和平面点栅格
    for (int i = 0; i < laserCloudNum; i++)
    {
        laserCloudCornerArray[i].reset(new pcl::PointCloud<PointType>());
        laserCloudSurfArray[i].reset(new pcl::PointCloud<PointType>());
    }

    std::thread mapping_process{process}; //主执行程序

    ros::spin(); //不断执行回调函数

    return 0;
}
```

程序中存在雷达坐标系，地图坐标系，里程计坐标系（laseOdometry节点粗估计得到的odom坐标系），下面是坐标系转换函数。

```
// set initial guess，里程计位姿转化为地图位姿，作为后端初始估计
void transformAssociateToMap()
{
    // T_w_curr = T_w_last * T_last_curr(from lidar odom)
    q_w_curr = q_wmap_wodom * q_wodom_curr;
    t_w_curr = q_wmap_wodom * t_wodom_curr + t_wmap_wodom;
}

// 更新odom到map之间的位姿变换
void transformUpdate()
{
    q_wmap_wodom = q_w_curr * q_wodom_curr.inverse();
    t_wmap_wodom = t_w_curr - q_wmap_wodom * t_wodom_curr;
}

//雷达坐标系点转化为地图点
void pointAssociateToMap(PointType const *const pi, PointType *const po)
{
    Eigen::Vector3d point_curr(pi->x, pi->y, pi->z);
    Eigen::Vector3d point_w = q_w_curr * point_curr + t_w_curr;
    po->x = point_w.x();
    po->y = point_w.y();
    po->z = point_w.z();
    po->intensity = pi->intensity;
    //po->intensity = 1.0;
}

//地图点转化到雷达坐标系点
void pointAssociateTobeMapped(PointType const *const pi, PointType *const po)
{
    Eigen::Vector3d point_w(pi->x, pi->y, pi->z);
    Eigen::Vector3d point_curr = q_w_curr.inverse() * (point_w - t_w_curr);
    po->x = point_curr.x();
    po->y = point_curr.y();
    po->z = point_curr.z();
    po->intensity = pi->intensity;
}
```

下面是回调函数的注释：

```
// 回调函数中将消息都是送入各自队列，进行线程加锁和解锁
void laserCloudCornerLastHandler(const sensor_msgs::PointCloud2ConstPtr &laserCloudCornerLast2)
{
    mBuf.lock();
    cornerLastBuf.push(laserCloudCornerLast2);
    mBuf.unlock();
}

void laserCloudSurfLastHandler(const sensor_msgs::PointCloud2ConstPtr &laserCloudSurfLast2)
{
    mBuf.lock();
    surfLastBuf.push(laserCloudSurfLast2);
    mBuf.unlock();
}

void laserCloudFullResHandler(const sensor_msgs::PointCloud2ConstPtr &laserCloudFullRes2)
{
    mBuf.lock();
    fullResBuf.push(laserCloudFullRes2);
    mBuf.unlock();
}

//receive odomtry
//接受前端发送过来的里程计话题，并将位姿转换到世界坐标系下后发布
void laserOdometryHandler(const nav_msgs::Odometry::ConstPtr &laserOdometry)
{
    mBuf.lock();
    odometryBuf.push(laserOdometry);
    mBuf.unlock();

    // high frequence publish
    // 获取里程计位姿
    Eigen::Quaterniond q_wodom_curr;
    Eigen::Vector3d t_wodom_curr;
    q_wodom_curr.x() = laserOdometry->pose.pose.orientation.x;
    q_wodom_curr.y() = laserOdometry->pose.pose.orientation.y;
    q_wodom_curr.z() = laserOdometry->pose.pose.orientation.z;
    q_wodom_curr.w() = laserOdometry->pose.pose.orientation.w;
    t_wodom_curr.x() = laserOdometry->pose.pose.position.x;
    t_wodom_curr.y() = laserOdometry->pose.pose.position.y;
    t_wodom_curr.z() = laserOdometry->pose.pose.position.z;
    
    // 里程计坐标系位姿转化为地图坐标系位姿
    Eigen::Quaterniond q_w_curr = q_wmap_wodom * q_wodom_curr;
    Eigen::Vector3d t_w_curr = q_wmap_wodom * t_wodom_curr + t_wmap_wodom; 
    // 发布出去
    nav_msgs::Odometry odomAftMapped;
    odomAftMapped.header.frame_id = "/camera_init";
    odomAftMapped.child_frame_id = "/aft_mapped";
    odomAftMapped.header.stamp = laserOdometry->header.stamp;
    odomAftMapped.pose.pose.orientation.x = q_w_curr.x();
    odomAftMapped.pose.pose.orientation.y = q_w_curr.y();
    odomAftMapped.pose.pose.orientation.z = q_w_curr.z();
    odomAftMapped.pose.pose.orientation.w = q_w_curr.w();
    odomAftMapped.pose.pose.position.x = t_w_curr.x();
    odomAftMapped.pose.pose.position.y = t_w_curr.y();
    odomAftMapped.pose.pose.position.z = t_w_curr.z();
    pubOdomAftMappedHighFrec.publish(odomAftMapped);
}
```

下面是Scan to Map当前帧位姿精估计，即周围10帧组成的子地图submap与其他所有帧组成的全部地图进行匹配，这种位姿估计方法联系了所有帧的信息，位姿估计更准确。

但是，如果完全使用所有区域的点云进行匹配，这样的效率会很低，而且内存空间可能会爆掉。LOAM采用的是栅格（cube）地图的方法，将整个地图分成21×21×11个珊格，每个珊格是⼀个边⻓50m的正⽅体，当地图逐渐累加时，珊格之外的部分就被舍弃，这样可以保证内存空间不会随着程序的运⾏⽽爆掉，同时保证效率。

```
// 主处理线程
void process()
{
    while(1)
    {
        // 四个队列分别存放边线点、平面点、全部点、和里程计位姿，要确保需要的buffer里都有值
        // laserOdometry模块对本节点的执行频率进行了控制，laserOdometry模块publish的位姿是10Hz，点云的publish频率没这么高，限制是2hz
        while (!cornerLastBuf.empty() && !surfLastBuf.empty() &&
            !fullResBuf.empty() && !odometryBuf.empty())
        {
            mBuf.lock();  //线程加锁，避免线程冲突
        
            // 以cornerLastBuf为基准，把时间戳小于其的全部pop出去，保证其他容器的最新消息与cornerLastBuf.front()最新消息时间戳同步
            while (!odometryBuf.empty() && odometryBuf.front()->header.stamp.toSec() < cornerLastBuf.front()->header.stamp.toSec())
                odometryBuf.pop();
            if (odometryBuf.empty())
            {
                mBuf.unlock();  //如果没有数据了，则线程解锁
                break;
            }

            while (!surfLastBuf.empty() && surfLastBuf.front()->header.stamp.toSec() < cornerLastBuf.front()->header.stamp.toSec())
                surfLastBuf.pop();
            if (surfLastBuf.empty())
            {
                mBuf.unlock();
                break;
            }

            while (!fullResBuf.empty() && fullResBuf.front()->header.stamp.toSec() < cornerLastBuf.front()->header.stamp.toSec())
                fullResBuf.pop();
            if (fullResBuf.empty())
            {
                mBuf.unlock();
                break;
            }

            timeLaserCloudCornerLast = cornerLastBuf.front()->header.stamp.toSec();
            timeLaserCloudSurfLast = surfLastBuf.front()->header.stamp.toSec();
            timeLaserCloudFullRes = fullResBuf.front()->header.stamp.toSec();
            timeLaserOdometry = odometryBuf.front()->header.stamp.toSec();
            // 原则上取出来的时间戳都是一样的，如果不一样则说明有问题
            if (timeLaserCloudCornerLast != timeLaserOdometry ||
                timeLaserCloudSurfLast != timeLaserOdometry ||
                timeLaserCloudFullRes != timeLaserOdometry)
            {
                printf("time corner %f surf %f full %f odom %f \n", timeLaserCloudCornerLast, timeLaserCloudSurfLast, timeLaserCloudFullRes, timeLaserOdometry);
                printf("unsync messeage!");
                mBuf.unlock();
                break;
            }
       // 点云全部转成pcl的数据格式
            laserCloudCornerLast->clear();
            pcl::fromROSMsg(*cornerLastBuf.front(), *laserCloudCornerLast);
            cornerLastBuf.pop();

            laserCloudSurfLast->clear();
            pcl::fromROSMsg(*surfLastBuf.front(), *laserCloudSurfLast);
            surfLastBuf.pop();

            laserCloudFullRes->clear();
            pcl::fromROSMsg(*fullResBuf.front(), *laserCloudFullRes);
            fullResBuf.pop();
            // lidar odom的结果转成eigen数据格式
            q_wodom_curr.x() = odometryBuf.front()->pose.pose.orientation.x;
            q_wodom_curr.y() = odometryBuf.front()->pose.pose.orientation.y;
            q_wodom_curr.z() = odometryBuf.front()->pose.pose.orientation.z;
            q_wodom_curr.w() = odometryBuf.front()->pose.pose.orientation.w;
            t_wodom_curr.x() = odometryBuf.front()->pose.pose.position.x;
            t_wodom_curr.y() = odometryBuf.front()->pose.pose.position.y;
            t_wodom_curr.z() = odometryBuf.front()->pose.pose.position.z;
            odometryBuf.pop();
            // 考虑到实时性，Mapping线程耗时>100ms导致的队列里缓存的其他边线点都pop出去，不然可能出现处理延时的情况
            while(!cornerLastBuf.empty())
            {
                cornerLastBuf.pop();
                printf("drop lidar frame in mapping for real time performance \n");
            }

            mBuf.unlock();

            TicToc t_whole; //计算这个线程的全部时间
            // 根据前端结果，将里程计位姿转化为地图位姿得到后端优化的一个初始估计值
            transformAssociateToMap();

            TicToc t_shift;  //计算位姿转换的时间

            // 后端地图本质上是一个以当前点为中心的一个珊格地图，根据初始估计值计算寻找当前位姿在地图中的索引，一个格子的边长是50m
            int centerCubeI = int((t_w_curr.x() + 25.0) / 50.0) + laserCloudCenWidth;
            int centerCubeJ = int((t_w_curr.y() + 25.0) / 50.0) + laserCloudCenHeight;
            int centerCubeK = int((t_w_curr.z() + 25.0) / 50.0) + laserCloudCenDepth;
       // 如果小于25就向下取整，相当于四舍五入的一个过程
            if (t_w_curr.x() + 25.0 < 0)
                centerCubeI--;
            if (t_w_curr.y() + 25.0 < 0)
                centerCubeJ--;
            if (t_w_curr.z() + 25.0 < 0)
                centerCubeK--;
            // 如果当前珊格索引小于3,就说明当前点快接近地图边界了，需要进行调整，相当于地图整体往x正方向移动
            while (centerCubeI < 3)
            {
                for (int j = 0; j < laserCloudHeight; j++)
                {
                    for (int k = 0; k < laserCloudDepth; k++)
                    { 
                        int i = laserCloudWidth - 1;
                        // 从x最大值开始
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeCornerPointer =
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k]; 
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeSurfPointer =
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        // 整体右移
                        for (; i >= 1; i--)
                        {
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudCornerArray[i - 1 + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudSurfArray[i - 1 + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        }
                        // 此时i = 0,也就是最左边的格子赋值给了之前最右边的格子
                        laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeCornerPointer;
                        laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeSurfPointer;
                        // 该点云清零，由于是指针操作，相当于最左边的格子清空了
                        laserCloudCubeCornerPointer->clear();
                        laserCloudCubeSurfPointer->clear();
                    }
                }
                // 索引右移
                centerCubeI++;
                laserCloudCenWidth++;
            }
            // 以下是y和z的操作，同理
            while (centerCubeJ < 3)
            {
                for (int i = 0; i < laserCloudWidth; i++)
                {
                    for (int k = 0; k < laserCloudDepth; k++)
                    {
                        int j = laserCloudHeight - 1;
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeCornerPointer =
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeSurfPointer =
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        for (; j >= 1; j--)
                        {
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudCornerArray[i + laserCloudWidth * (j - 1) + laserCloudWidth * laserCloudHeight * k];
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudSurfArray[i + laserCloudWidth * (j - 1) + laserCloudWidth * laserCloudHeight * k];
                        }
                        laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeCornerPointer;
                        laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeSurfPointer;
                        laserCloudCubeCornerPointer->clear();
                        laserCloudCubeSurfPointer->clear();
                    }
                }
                centerCubeJ++;
                laserCloudCenHeight++;
            }

            while (centerCubeJ >= laserCloudHeight - 3)
            {
                for (int i = 0; i < laserCloudWidth; i++)
                {
                    for (int k = 0; k < laserCloudDepth; k++)
                    {
                        int j = 0;
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeCornerPointer =
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeSurfPointer =
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        for (; j < laserCloudHeight - 1; j++)
                        {
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudCornerArray[i + laserCloudWidth * (j + 1) + laserCloudWidth * laserCloudHeight * k];
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudSurfArray[i + laserCloudWidth * (j + 1) + laserCloudWidth * laserCloudHeight * k];
                        }
                        laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeCornerPointer;
                        laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeSurfPointer;
                        laserCloudCubeCornerPointer->clear();
                        laserCloudCubeSurfPointer->clear();
                    }
                }
                centerCubeJ--;
                laserCloudCenHeight--;
            }

            while (centerCubeK < 3)
            {
                for (int i = 0; i < laserCloudWidth; i++)
                {
                    for (int j = 0; j < laserCloudHeight; j++)
                    {
                        int k = laserCloudDepth - 1;
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeCornerPointer =
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeSurfPointer =
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        for (; k >= 1; k--)
                        {
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * (k - 1)];
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * (k - 1)];
                        }
                        laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeCornerPointer;
                        laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeSurfPointer;
                        laserCloudCubeCornerPointer->clear();
                        laserCloudCubeSurfPointer->clear();
                    }
                }
                centerCubeK++;
                laserCloudCenDepth++;
            }

            while (centerCubeK >= laserCloudDepth - 3)
            {
                for (int i = 0; i < laserCloudWidth; i++)
                {
                    for (int j = 0; j < laserCloudHeight; j++)
                    {
                        int k = 0;
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeCornerPointer =
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        pcl::PointCloud<PointType>::Ptr laserCloudCubeSurfPointer =
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k];
                        for (; k < laserCloudDepth - 1; k++)
                        {
                            laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * (k + 1)];
                            laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                                laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * (k + 1)];
                        }
                        laserCloudCornerArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeCornerPointer;
                        laserCloudSurfArray[i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k] =
                            laserCloudCubeSurfPointer;
                        laserCloudCubeCornerPointer->clear();
                        laserCloudCubeSurfPointer->clear();
                    }
                }
                centerCubeK--;
                laserCloudCenDepth--;
            }
            // 以上操作相当于维护了一个局部地图，保证当前帧不在这个局部地图的边缘，这样才可以从地图中获取足够的约束
            
            int laserCloudValidNum = 0;
            int laserCloudSurroundNum = 0;
            // 从当前格子为中心，选出地图中一定范围的点云
            // 即向IJ坐标轴的正负方向各拓展2个栅格，K坐标轴的正负方向各拓展1个栅格
            // 在每一维附近5个栅格(前2个，后2个，中间1个)里进行查找（前后250米范围内，总共500米范围），三个维度总共125个栅格
            // 在这125个栅格里面进一步筛选在视域范围内的栅格
            for (int i = centerCubeI - 2; i <= centerCubeI + 2; i++)
            {
                for (int j = centerCubeJ - 2; j <= centerCubeJ + 2; j++)
                {
                    for (int k = centerCubeK - 1; k <= centerCubeK + 1; k++)
                    {
                        // 如果坐标合理
                        if (i >= 0 && i < laserCloudWidth &&
                            j >= 0 && j < laserCloudHeight &&
                            k >= 0 && k < laserCloudDepth)
                        { 
                            // 把submap子地图的有效栅格索引记录下来
                            laserCloudValidInd[laserCloudValidNum] = i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k;
                            laserCloudValidNum++;
                            laserCloudSurroundInd[laserCloudSurroundNum] = i + laserCloudWidth * j + laserCloudWidth * laserCloudHeight * k;
                            laserCloudSurroundNum++;
                        }
                    }
                }
            }
  laserCloudCornerFromMap->clear();
            laserCloudSurfFromMap->clear();

            //将有效栅格的点云叠加到一起组成submap子地图的特征点云，构建用来这一帧优化的局部地图
            for (int i = 0; i < laserCloudValidNum; i++)
            {
                *laserCloudCornerFromMap += *laserCloudCornerArray[laserCloudValidInd[i]];
                *laserCloudSurfFromMap += *laserCloudSurfArray[laserCloudValidInd[i]];
            }
            int laserCloudCornerFromMapNum = laserCloudCornerFromMap->points.size();
            int laserCloudSurfFromMapNum = laserCloudSurfFromMap->points.size();

            // 为了减少运算量，对点云进行降采样
            pcl::PointCloud<PointType>::Ptr laserCloudCornerStack(new pcl::PointCloud<PointType>());
            downSizeFilterCorner.setInputCloud(laserCloudCornerLast);
            downSizeFilterCorner.filter(*laserCloudCornerStack);
            int laserCloudCornerStackNum = laserCloudCornerStack->points.size();

            pcl::PointCloud<PointType>::Ptr laserCloudSurfStack(new pcl::PointCloud<PointType>());
            downSizeFilterSurf.setInputCloud(laserCloudSurfLast);
            downSizeFilterSurf.filter(*laserCloudSurfStack);
            int laserCloudSurfStackNum = laserCloudSurfStack->points.size();

            printf("map prepare time %f ms\n", t_shift.toc());  //打印位姿转换的时间
            printf("map corner num %d  surf num %d \n", laserCloudCornerFromMapNum, laserCloudSurfFromMapNum); //打印地图中边线点和平面点的数量
```

下面是后端Scan to Map的匹配优化。Submap子地图的网格与全部地图的网格进行匹配时，在laserOdomerty中Scan to Scan之间的匹配方法不适用了。这里的匹配方法如下：

1. 取当前帧的特征点（边线点/平面点）

2. 找到全部地图特征点中，当前特征点的5个最近邻点。

3. 如果是边线点，则以这五个点的均值点为中心，以5个点的主方向向量（类似于PCA方法）为方向，作一条直线，令该边线点与直线距离最短，构建非线性优化问题。

4. 如果是平面点，则寻找五个点的法方向（反向的PCA方法），令这个平面点在法方向上与五个近邻点的距离最小，构建非线性优化问题。

5. 优化变量是雷达位姿，求解能够让以上非线性问题代价函数最小的雷达位姿。

   ```
   // 最终的地图有效点云数目进行判断
               if (laserCloudCornerFromMapNum > 10 && laserCloudSurfFromMapNum > 50)
               {
                   TicToc t_opt; //计算优化时间
                   TicToc t_tree; //计算KD-tree搜索时间
                   // 送入kdtree便于最近邻搜索
                   kdtreeCornerFromMap->setInputCloud(laserCloudCornerFromMap);
                   kdtreeSurfFromMap->setInputCloud(laserCloudSurfFromMap);
                   printf("build tree time %f ms \n", t_tree.toc()); //打印KD-tree搜索时间
   
                   // 建立对应关系的优化迭代次数不超过2次
                   for (int iterCount = 0; iterCount < 2; iterCount++)
                   {
                       //ceres::LossFunction *loss_function = NULL;
                       // 建立ceres问题
                       ceres::LossFunction *loss_function = new ceres::HuberLoss(0.1);
                       ceres::LocalParameterization *q_parameterization =
                           new ceres::EigenQuaternionParameterization();
                       ceres::Problem::Options problem_options;
   
                       ceres::Problem problem(problem_options);
                       problem.AddParameterBlock(parameters, 4, q_parameterization);
                       problem.AddParameterBlock(parameters + 4, 3);
   
                       TicToc t_data;  //计算建图数据点关联的时间
                       int corner_num = 0;
   
                       // 构建边线点（角点）相关的约束
                       for (int i = 0; i < laserCloudCornerStackNum; i++)
                       {
                           pointOri = laserCloudCornerStack->points[i];
                           //double sqrtDis = pointOri.x * pointOri.x + pointOri.y * pointOri.y + pointOri.z * pointOri.z;
                               
                           // submap子地图中的点云都是在world坐标系下，而接收到的当前帧点云都是Lidar坐标系下，所以要把当前点根据初值投到地图坐标系下去
                           //即用预测的Mapping位姿w_curr，将Lidar坐标系下的特征点变换到world坐标系下
                           pointAssociateToMap(&pointOri, &pointSel);
   
                           // 地图中寻找和该特征点最近的5个点
                           kdtreeCornerFromMap->nearestKSearch(pointSel, 5, pointSearchInd, pointSearchSqDis); 
   
                           // 判断最远的点距离不能超过1m，否则就是无效约束
                           if (pointSearchSqDis[4] < 1.0)
                           { 
                               std::vector<Eigen::Vector3d> nearCorners;
                               Eigen::Vector3d center(0, 0, 0);
                               for (int j = 0; j < 5; j++)
                               {
                                   Eigen::Vector3d tmp(laserCloudCornerFromMap->points[pointSearchInd[j]].x,
                                                       laserCloudCornerFromMap->points[pointSearchInd[j]].y,
                                                       laserCloudCornerFromMap->points[pointSearchInd[j]].z);
                                   center = center + tmp;
                                   nearCorners.push_back(tmp);
                               }
                               // 计算这五个点的均值
                               center = center / 5.0;
   
                               Eigen::Matrix3d covMat = Eigen::Matrix3d::Zero();
                               // 构建5个最近邻点的协方差矩阵
                               for (int j = 0; j < 5; j++)
                               {
                                   Eigen::Matrix<double, 3, 1> tmpZeroMean = nearCorners[j] - center;
                                   covMat = covMat + tmpZeroMean * tmpZeroMean.transpose();
                               }
                               // 进行特征值分解
                               Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> saes(covMat);
     // if is indeed line feature
                               // note Eigen library sort eigenvalues in increasing order
   
                               // PCA的原理：计算协方差矩阵的特征值和特征向量，用于判断这5个点是不是呈线状分布
                               // 如果5个点呈线状分布，最大的特征值对应的特征向量就是该线的方向向量
                               Eigen::Vector3d unit_direction = saes.eigenvectors().col(2);
                               Eigen::Vector3d curr_point(pointOri.x, pointOri.y, pointOri.z);
                               // 最大特征值大于次大特征值的3倍认为是线特征
                               if (saes.eigenvalues()[2] > 3 * saes.eigenvalues()[1])
                               { 
                                   Eigen::Vector3d point_on_line = center;
                                   Eigen::Vector3d point_a, point_b;
                                   // 根据拟合出来的线特征方向，以平均点为中心构建两个虚拟点，代替一条直线
                                   point_a = 0.1 * unit_direction + point_on_line;
                                   point_b = -0.1 * unit_direction + point_on_line;
                                   // 构建约束，和lidar odom约束一致
                                   ceres::CostFunction *cost_function = LidarEdgeFactor::Create(curr_point, point_a, point_b, 1.0);
                                   problem.AddResidualBlock(cost_function, loss_function, parameters, parameters + 4);
                                   corner_num++;   
                               }                           
                           }
                           /*
                           else if(pointSearchSqDis[4] < 0.01 * sqrtDis)
                           {
                               Eigen::Vector3d center(0, 0, 0);
                               for (int j = 0; j < 5; j++)
                               {
                                   Eigen::Vector3d tmp(laserCloudCornerFromMap->points[pointSearchInd[j]].x,
                                                       laserCloudCornerFromMap->points[pointSearchInd[j]].y,
                                                       laserCloudCornerFromMap->points[pointSearchInd[j]].z);
                                   center = center + tmp;
                               }
                               center = center / 5.0;  
                               Eigen::Vector3d curr_point(pointOri.x, pointOri.y, pointOri.z);
                               ceres::CostFunction *cost_function = LidarDistanceFactor::Create(curr_point, center);
                               problem.AddResidualBlock(cost_function, loss_function, parameters, parameters + 4);
                           }
                           */
                       }
       int surf_num = 0;
                       // 构建面点约束
                       for (int i = 0; i < laserCloudSurfStackNum; i++)
                       {
                           pointOri = laserCloudSurfStack->points[i];
                           //double sqrtDis = pointOri.x * pointOri.x + pointOri.y * pointOri.y + pointOri.z * pointOri.z;
                           pointAssociateToMap(&pointOri, &pointSel);
                           kdtreeSurfFromMap->nearestKSearch(pointSel, 5, pointSearchInd, pointSearchSqDis);
   
                           Eigen::Matrix<double, 5, 3> matA0;
                           Eigen::Matrix<double, 5, 1> matB0 = -1 * Eigen::Matrix<double, 5, 1>::Ones();
                           // 构建平面方程Ax + By +Cz + 1 = 0
                           // 通过构建一个超定方程来求解这个平面方程
                           if (pointSearchSqDis[4] < 1.0)
                           {
                               
                               for (int j = 0; j < 5; j++)
                               {
                                   matA0(j, 0) = laserCloudSurfFromMap->points[pointSearchInd[j]].x;
                                   matA0(j, 1) = laserCloudSurfFromMap->points[pointSearchInd[j]].y;
                                   matA0(j, 2) = laserCloudSurfFromMap->points[pointSearchInd[j]].z;
                                   //printf(" pts %f %f %f ", matA0(j, 0), matA0(j, 1), matA0(j, 2));
                               }
                               // find the norm of plane
                               // 调用eigen接口求解该方程，解就是这个平面的法向量
                               Eigen::Vector3d norm = matA0.colPivHouseholderQr().solve(matB0);
                               double negative_OA_dot_norm = 1 / norm.norm();  // 法向量长度的倒数  
                               norm.normalize(); // 法向量归一化
   
                               // Here n(pa, pb, pc) is unit norm of plane
                               bool planeValid = true;
                               // 根据求出来的平面方程进行校验，看看是不是符合平面约束
                               for (int j = 0; j < 5; j++)
                               {
                                   // if OX * n > 0.2, then plane is not fit well
                                   // 这里是求解点到平面的距离
                                   // 点(x0, y0, z0)到平面Ax + By + Cz + D = 0 的距离公式 = fabs(Ax0 + By0 + Cz0 + D) / sqrt(A^2 + B^2 + C^2)
                                   if (fabs(norm(0) * laserCloudSurfFromMap->points[pointSearchInd[j]].x +
                                            norm(1) * laserCloudSurfFromMap->points[pointSearchInd[j]].y +
                                            norm(2) * laserCloudSurfFromMap->points[pointSearchInd[j]].z + negative_OA_dot_norm) > 0.2)
                                   {
                                       planeValid = false; // 点如果距离平面太远，就认为这是一个拟合的不好的平面
                                       break;
                                   }
                               }
   
                               Eigen::Vector3d curr_point(pointOri.x, pointOri.y, pointOri.z);
                               // 如果平面有效就构建平面约束
                               if (planeValid)
                               {
                                   // 利用平面方程构建约束，和前端构建形式稍有不同
                                   ceres::CostFunction *cost_function = LidarPlaneNormFactor::Create(curr_point, norm, negative_OA_dot_norm);
                                   problem.AddResidualBlock(cost_function, loss_function, parameters, parameters + 4);
                                   surf_num++;
                               }
                           }
                           /*
                           else if(pointSearchSqDis[4] < 0.01 * sqrtDis)
                           {
                               Eigen::Vector3d center(0, 0, 0);
                               for (int j = 0; j < 5; j++)
                               {
                                   Eigen::Vector3d tmp(laserCloudSurfFromMap->points[pointSearchInd[j]].x,
                                                       laserCloudSurfFromMap->points[pointSearchInd[j]].y,
                                                       laserCloudSurfFromMap->points[pointSearchInd[j]].z);
                                   center = center + tmp;
                               }
                               center = center / 5.0;  
                               Eigen::Vector3d curr_point(pointOri.x, pointOri.y, pointOri.z);
                               ceres::CostFunction *cost_function = LidarDistanceFactor::Create(curr_point, center);
                               problem.AddResidualBlock(cost_function, loss_function, parameters, parameters + 4);
                           }
                           */
                       }
   
                       //printf("corner num %d used corner num %d \n", laserCloudCornerStackNum, corner_num);
                       //printf("surf num %d used surf num %d \n", laserCloudSurfStackNum, surf_num);
   
                       printf("mapping data assosiation time %f ms \n", t_data.toc());  //打印建图数据关联时间
                       // 调用ceres求解
                       TicToc t_solver;
                       ceres::Solver::Options options;
                       options.linear_solver_type = ceres::DENSE_QR;
                       options.max_num_iterations = 4;
                       options.minimizer_progress_to_stdout = false;
                       options.check_gradients = false;
                       options.gradient_check_relative_precision = 1e-4;
                       ceres::Solver::Summary summary;
                       ceres::Solve(options, &problem, &summary);
                       printf("mapping solver time %f ms \n", t_solver.toc());
   
                       //printf("time %f \n", timeLaserOdometry);
                       //printf("corner factor num %d surf factor num %d\n", corner_num, surf_num);
                       //printf("result q %f %f %f %f result t %f %f %f\n", parameters[3], parameters[0], parameters[1], parameters[2],
                       //     parameters[4], parameters[5], parameters[6]);
                   }
                   printf("mapping optimization time %f \n", t_opt.toc());  //打印建图优化的时间
               }
               else
               {
                   ROS_WARN("time Map corner and surf num are not enough");
               }
               // 完成特征匹配、优化后，用最后匹配计算出的优化变量w_curr，更新增量wmap_wodom，让下一次的Mapping初值更准确
               transformUpdate();
   ```

   下面是一些后处理的工作，即将当前帧的特征点加入到全部地图栅格中，对全部地图栅格中的点进行降采样，刷新附近点云地图，刷新全部点云地图，发布当前帧的精确位姿和平移估计。

```
         TicToc t_add;  //计算增加特征点的时间
            // 将优化后的当前帧边线点（角点）加到对应的边线点局部地图中去
            for (int i = 0; i < laserCloudCornerStackNum; i++)
            {
                // 该点根据位姿投到地图坐标系
                pointAssociateToMap(&laserCloudCornerStack->points[i], &pointSel);
                // 算出这个点所在的格子的索引
                int cubeI = int((pointSel.x + 25.0) / 50.0) + laserCloudCenWidth;
                int cubeJ = int((pointSel.y + 25.0) / 50.0) + laserCloudCenHeight;
                int cubeK = int((pointSel.z + 25.0) / 50.0) + laserCloudCenDepth;
                // 同样四舍五入一下
                if (pointSel.x + 25.0 < 0)
                    cubeI--;
                if (pointSel.y + 25.0 < 0)
                    cubeJ--;
                if (pointSel.z + 25.0 < 0)
                    cubeK--;
                // 如果超过边界的话就算了
                if (cubeI >= 0 && cubeI < laserCloudWidth &&
                    cubeJ >= 0 && cubeJ < laserCloudHeight &&
                    cubeK >= 0 && cubeK < laserCloudDepth)
                {
                    // 根据xyz的索引计算在一位数组中的索引
                    int cubeInd = cubeI + laserCloudWidth * cubeJ + laserCloudWidth * laserCloudHeight * cubeK;
                    laserCloudCornerArray[cubeInd]->push_back(pointSel);
                }
            }
            // 面点也做同样的处理
            for (int i = 0; i < laserCloudSurfStackNum; i++)
            {
                pointAssociateToMap(&laserCloudSurfStack->points[i], &pointSel);

                int cubeI = int((pointSel.x + 25.0) / 50.0) + laserCloudCenWidth;
                int cubeJ = int((pointSel.y + 25.0) / 50.0) + laserCloudCenHeight;
                int cubeK = int((pointSel.z + 25.0) / 50.0) + laserCloudCenDepth;

                if (pointSel.x + 25.0 < 0)
                    cubeI--;
                if (pointSel.y + 25.0 < 0)
                    cubeJ--;
                if (pointSel.z + 25.0 < 0)
                    cubeK--;

                if (cubeI >= 0 && cubeI < laserCloudWidth &&
                    cubeJ >= 0 && cubeJ < laserCloudHeight &&
                    cubeK >= 0 && cubeK < laserCloudDepth)
                {
                    int cubeInd = cubeI + laserCloudWidth * cubeJ + laserCloudWidth * laserCloudHeight * cubeK;
                    laserCloudSurfArray[cubeInd]->push_back(pointSel);
                }
            }
            printf("add points time %f ms\n", t_add.toc()); //打印增加特征点的时间

            
            TicToc t_filter; //计算降采样的时间
            // 把当前帧涉及到的局部地图的珊格做一个降采样
            for (int i = 0; i < laserCloudValidNum; i++)
            {
                int ind = laserCloudValidInd[i];

                pcl::PointCloud<PointType>::Ptr tmpCorner(new pcl::PointCloud<PointType>());
                downSizeFilterCorner.setInputCloud(laserCloudCornerArray[ind]);
                downSizeFilterCorner.filter(*tmpCorner);
                laserCloudCornerArray[ind] = tmpCorner;

                pcl::PointCloud<PointType>::Ptr tmpSurf(new pcl::PointCloud<PointType>());
                downSizeFilterSurf.setInputCloud(laserCloudSurfArray[ind]);
                downSizeFilterSurf.filter(*tmpSurf);
                laserCloudSurfArray[ind] = tmpSurf;
            }
            printf("filter time %f ms \n", t_filter.toc());  //打印降采样的时间
            
            TicToc t_pub; //计算发布地图话题数据的时间
            //publish surround map for every 5 frame
            // 每隔5帧对外发布一下
            if (frameCount % 5 == 0)
            {
                laserCloudSurround->clear();
                // 把该当前帧相关的局部地图发布出去
                for (int i = 0; i < laserCloudSurroundNum; i++)
                {
                    int ind = laserCloudSurroundInd[i];
                    *laserCloudSurround += *laserCloudCornerArray[ind];
                    *laserCloudSurround += *laserCloudSurfArray[ind];
                }

                sensor_msgs::PointCloud2 laserCloudSurround3;
                pcl::toROSMsg(*laserCloudSurround, laserCloudSurround3);
                laserCloudSurround3.header.stamp = ros::Time().fromSec(timeLaserOdometry);
                laserCloudSurround3.header.frame_id = "/camera_init";
                pubLaserCloudSurround.publish(laserCloudSurround3);
            }
            // 每隔20帧发布全量的局部地图
            if (frameCount % 20 == 0)
            {
                pcl::PointCloud<PointType> laserCloudMap;
                // 21 × 21 × 11 = 4851
                for (int i = 0; i < 4851; i++)
                {
                    laserCloudMap += *laserCloudCornerArray[i];
                    laserCloudMap += *laserCloudSurfArray[i];
                }
                sensor_msgs::PointCloud2 laserCloudMsg;
                pcl::toROSMsg(laserCloudMap, laserCloudMsg);
                laserCloudMsg.header.stamp = ros::Time().fromSec(timeLaserOdometry);
                laserCloudMsg.header.frame_id = "/camera_init";
                pubLaserCloudMap.publish(laserCloudMsg);
            }

            int laserCloudFullResNum = laserCloudFullRes->points.size();
            // 把当前帧发布出去
            for (int i = 0; i < laserCloudFullResNum; i++)
            {
                pointAssociateToMap(&laserCloudFullRes->points[i], &laserCloudFullRes->points[i]);
            }

            sensor_msgs::PointCloud2 laserCloudFullRes3;
            pcl::toROSMsg(*laserCloudFullRes, laserCloudFullRes3);
            laserCloudFullRes3.header.stamp = ros::Time().fromSec(timeLaserOdometry);
            laserCloudFullRes3.header.frame_id = "/camera_init";
            pubLaserCloudFullRes.publish(laserCloudFullRes3);

            printf("mapping pub time %f ms \n", t_pub.toc());  //打印发布地图话题数据的时间

            printf("whole mapping time %f ms +++++\n", t_whole.toc());
            // 发布当前位姿
            nav_msgs::Odometry odomAftMapped;
            odomAftMapped.header.frame_id = "/camera_init";
            odomAftMapped.child_frame_id = "/aft_mapped";
            odomAftMapped.header.stamp = ros::Time().fromSec(timeLaserOdometry);
            odomAftMapped.pose.pose.orientation.x = q_w_curr.x();
            odomAftMapped.pose.pose.orientation.y = q_w_curr.y();
            odomAftMapped.pose.pose.orientation.z = q_w_curr.z();
            odomAftMapped.pose.pose.orientation.w = q_w_curr.w();
            odomAftMapped.pose.pose.position.x = t_w_curr.x();
            odomAftMapped.pose.pose.position.y = t_w_curr.y();
            odomAftMapped.pose.pose.position.z = t_w_curr.z();
            pubOdomAftMapped.publish(odomAftMapped);
            // 发布当前轨迹
            geometry_msgs::PoseStamped laserAfterMappedPose;
            laserAfterMappedPose.header = odomAftMapped.header;
            laserAfterMappedPose.pose = odomAftMapped.pose.pose;
            laserAfterMappedPath.header.stamp = odomAftMapped.header.stamp;
            laserAfterMappedPath.header.frame_id = "/camera_init";
            laserAfterMappedPath.poses.push_back(laserAfterMappedPose);
            pubLaserAfterMappedPath.publish(laserAfterMappedPath);
            // 发布tf
            static tf::TransformBroadcaster br;
            tf::Transform transform;
            tf::Quaternion q;
            transform.setOrigin(tf::Vector3(t_w_curr(0),
                                            t_w_curr(1),
                                            t_w_curr(2)));
            q.setW(q_w_curr.w());
            q.setX(q_w_curr.x());
            q.setY(q_w_curr.y());
            q.setZ(q_w_curr.z());
            transform.setRotation(q);
            br.sendTransform(tf::StampedTransform(transform, odomAftMapped.header.stamp, "/camera_init", "/aft_mapped"));

            frameCount++;
        }
        std::chrono::milliseconds dura(2); //延时2ms
        std::this_thread::sleep_for(dura);
    }
}
```

#### 原理分析和第三方库函数分析

##### 结合论文总结A-LOAM框架原理并具体解释下一些程序语句

##### 激光雷达特性和原理介绍

##### PCL库函数

##### ROS库函数

##### EIGEN库函数

##### Ceres库函数

#### 速腾Robosense-16线雷达室内建图

1. 首先安装好雷达在平台上，供电准备好，雷达网口接终端，雷达本机ip是192.168.1.200，终端ip要配置成192.168.1.102，子网掩码255.255.255.0即可。终端ping 192.168.1.200可以ping通则说明雷达通讯没问题。
2. 雷达驱动下载，GitHub下载最新源码， https://github.com/RoboSense-LiDAR/rslidar_sdk.git 
3. 雷达驱动环境安装，

安装pcap：

```
sudo apt-get install -y libpcap-dev
```

安装Yaml：

```
sudo apt-get install -y libyaml-cpp-dev （若已安装ROS desktop-full, 可跳过）
```

1. 修改CMakeLists.txt。

（1）将文件顶部的set(COMPILE_METHOD ORIGINAL)改为set(COMPILE_METHOD CATKIN)

（2）将set(POINT_TYPE XYZI) 改为set(POINT_TYPE XYZIRT)

（3）可能需要更改config.yaml文件中雷达型号为RS16

1. 将rslidar_sdk工程目录下的package_ros1.xml文件重命名为package.xml。
2. 建立ros工作空间，在工作空间编译
3. GitHub下载rs雷达话题转velodyne雷达话题

https://github.com/HViktorTsoi/rs_to_velodyne.git 并编译，可放在同个工作空间编译

1. 编译A-LOAM，前面有讲，可放在同个工作空间编译
2. 运行所有节点和launch文件，先运行雷达：

```
source devel/setup.bash
roslaunch rslidar_sdk start.launch
```

1. 运行rs_to_velodyne节点，

```
rosrun rs_to_velodyne rs_to_velodyne XYZIRT XYZIRT
```

1. 运行a-loam

```
roslaunch aloam_velodyne aloam_velodyne_VLP_16.launch
```

1. 效果如下：

   ![科研分享记录-3](.\img\科研分享记录-3.jpg)

2. 后面把程序封装下，只开启一个终端就可以。并保存点云分析建图数据。

### Lego-LOAM框架

#### 框架介绍

LeGO-LOAM是Tixiao Shan提出的⼀种基于LOAM的改进激光SLAM框架，主要是为了实现小车在多变地形下的定位和建图，其针对前端和后端都做了一系列的改进。

前端的改进有：

1. 对地面点进行分类和提取，避免一些异常边缘点的提取
2. 应用点云聚类算法，剔除了一些可能产生的outlier
3. 两次迭代求解前端帧间里程记，在不影响精度的情况下减轻计算负载，保障了嵌入式平台的实时性。

后端的改进有：

1. 使用slam关键帧的概念对后端部分进行了重构
2. 引入回环检测和位姿图优化概念，使得地图的全局一致性更好

![科研分享记录-4](.\img\科研分享记录-4.jpg)

#### 运行演示

1. 首先要安装 ROS

Ubuntu 64-bit 16.04 or 18.04. ROS Kinetic or Melodic

1. 安装gtsam

```
wget -O ~/Downloads/gtsam.zip https://github.com/borglab/gtsam/archive/4.0.0-alpha2.zip
cd ~/Downloads/ && unzip gtsam.zip -d ~/Downloads/
cd ~/Downloads/gtsam-4.0.0-alpha2/
mkdir build && cd build
cmake ..
sudo make install
```

1. 下载编译LeGO-LOAM

```
cd ~/catkin_ws/src
git clone https://github.com/RobustFieldAutonomyLab/LeGO-LOAM.git
cd ..
catkin_make -j1
```

1. 播放 bag 包运行

```
roslaunch lego_loam run.launch
rosbag play *.bag --clock --topic /velodyne_points /imu/data
```

效果如下：

![科研分享记录-5](.\img\科研分享记录-5.jpg)

​	5.报错解决

```
error: static assertion failed: Error: GTSAM was built against a different version of Eigen static_assert
```

GTSAM默认编译并没有使用系统eigen

在CMakeLists.txt文件中if（GTSAM_USE_SYSTEM_EIGEN）判断的前边添加如下代码：

```
set(GTSAM_USE_SYSTEM_EIGEN ON)
```

#### 速腾Robosense-16线雷达建图

1. 雷达测试：

首先安装好雷达在平台上，供电准备好，雷达网口接终端，雷达本机ip是192.168.1.200，终端ip要配置成192.168.1.102，子网掩码255.255.255.0即可。

终端ping 192.168.1.200可以ping通则说明雷达通讯没问题。

1. 雷达驱动下载：

GitHub下载最新源码

https://github.com/RoboSense-LiDAR/rslidar_sdk

1. 雷达驱动环境安装：

安装pcap：

```
sudo apt-get install -y libpcap-dev
```

安装Yaml：

```
sudo apt-get install -y libyaml-cpp-dev （若已安装ROS desktop-full, 可跳过）
```

1. 修改CMakeLists.txt和package.xml：

（1） 将文件顶部的set(COMPILE_METHOD ORIGINAL)改为set(COMPILE_METHOD CATKIN)

（2） 将set(POINT_TYPE XYZI) 改为set(POINT_TYPE XYZIRT)

（3） 将rslidar_sdk工程目录下的package_ros1.xml文件重命名为package.xml。

（4） 可能需要更改config.yaml文件中雷达型号为RS16

1. 编译：

建立ros工作空间，在工作空间编译

1. GitHub下载rs雷达话题转velodyne雷达话题源码

https://github.com/HViktorTsoi/rs_to_velodyne.git 并编译，可放在同个工作空间编译

1. 编译LeGO-LOAM

前面有讲，可放在同个工作空间编译

但是这里要修改LeGO-LOAM的launch文件：

将这句话的true改成false(实时跑是false，跑bag是true)

```
<param name="/use_sim_time" value="false"/>
```

1. 运行：

所有节点和launch文件，

先运行雷达：

```
source devel/setup.bash
roslaunch rslidar_sdk start.launch
```

再运行rs_to_velodyne节点

```
rosrun rs_to_velodyne rs_tovelodyne XYZIRT XYZIR //（这里要注意LeGO-LOAM需要的雷达点云是XYZIR格式的，话题还是velodyne_points）
```

最后运行lego_loam

```
roslaunch lego_loam run.launch
```

1. 效果如下：

![科研分享记录-6](.\img\科研分享记录-6.jpg)

1. 修改话题的其他办法

LeGO-LOAM的launch文件增加：

```
<remap from="/rslidar_points" to="/velodyne_points"/>
```

但是要修改雷达ROS驱动的CMakeLists.txt

将set(POINT_TYPE XYZI) 改为set(POINT_TYPE XYZIR)，并重新编译。

1. 保存数据

录制地图包：

```
rosbag record -o mybag.bag out /laser_cloud_surround 
```

查看：

```
rosrun pcl_ros bag_to_pcd mybag.bag /laser_cloud_surround mypcd
pcl_viewer xxx.pcd  //（这里是查看一帧的数据）
```

录制点云包：

```
rosbag record -o mybag.bag out /velodyne_points
```

#### LeGO-LOAM的改进

##### 地面分离方法：

LeGO-LOAM中前端改进中很重要的一点就是充分利用了地面点，提供了对地面点的提取

![科研分享记录-7](.\img\科研分享记录-7.jpg)

如上图，相邻的两个扫描线束的同一列打在地面上如AB点所示（指的是同个水平角度下），他们的垂直高度差：

![科研分享记录-8](.\img\科研分享记录-8.jpg)

水平距离差：

![科研分享记录-9](.\img\科研分享记录-9.jpg)

计算垂直高度差和水平高度差的角度：

![科研分享记录-10](.\img\科研分享记录-10.jpg)

理想情况下应该接近0,但激光雷达安装无法做到绝对水平且地面也不是绝对水平，因此这个角度会略微大于0,考虑到作者在草坪之类的场景下运动，因此这个值被设置成10度。这个地面分离算法比较简单，我们也可以结合激光雷达安装高度等其他先验信息进行优化。

下面是代码讲解：

```
void groundRemoval(){
        size_t lowerInd, upperInd;
        float diffX, diffY, diffZ, angle;

        for (size_t j = 0; j < Horizon_SCAN; ++j){ //水平角
            // groundScanInd 是在 utility.h 文件中声明的线数，groundScanInd=7，提取地面点时朝下的线束才有效。
            for (size_t i = 0; i < groundScanInd; ++i){ //垂直角

                lowerInd = j + ( i )*Horizon_SCAN; //取出同一个水平角（scan）下相邻垂直角（线）的两个点的索引
                upperInd = j + (i+1)*Horizon_SCAN;

                // 如果nanPoint.intensity = -1 则是空点nanPoint或无效点
                if (fullCloud->points[lowerInd].intensity == -1 ||
                    fullCloud->points[upperInd].intensity == -1){
                    groundMat.at<int8_t>(i,j) = -1;
                    continue;
                }

                // 由上下两线之间点的XYZ位置得到两线之间的俯仰角(夹角)
                // 如果俯仰角在10度以内，则判定(i,j)为地面点,groundMat[i][j]=1
                // 否则，则不是地面点，进行后续操作
                diffX = fullCloud->points[upperInd].x - fullCloud->points[lowerInd].x;
                diffY = fullCloud->points[upperInd].y - fullCloud->points[lowerInd].y;
                diffZ = fullCloud->points[upperInd].z - fullCloud->points[lowerInd].z;

                angle = atan2(diffZ, sqrt(diffX*diffX + diffY*diffY) ) * 180 / M_PI;  

                if (abs(angle - sensorMountAngle) <= 10){
                    groundMat.at<int8_t>(i,j) = 1;
                    groundMat.at<int8_t>(i+1,j) = 1;
                }
            }
        }

        // 找到所有点中的地面点或者距离为FLT_MAX(rangeMat的初始值)的点，并将他们标记为-1
        // rangeMat[i][j]==FLT_MAX代表的含义是无效点，比如打到天空上的点
        for (size_t i = 0; i < N_SCAN; ++i){
            for (size_t j = 0; j < Horizon_SCAN; ++j){
                if (groundMat.at<int8_t>(i,j) == 1 || rangeMat.at<float>(i,j) == FLT_MAX){
                    labelMat.at<int>(i,j) = -1;
                }
            }
        }

        // 把点放到groundCloud队列中去，把地面点发布出来
        if (pubGroundCloud.getNumSubscribers() != 0){
            for (size_t i = 0; i <= groundScanInd; ++i){
                for (size_t j = 0; j < Horizon_SCAN; ++j){
                    if (groundMat.at<int8_t>(i,j) == 1)
                        groundCloud->push_back(fullCloud->points[j + i*Horizon_SCAN]);
                }
            }
        }
    }
```

##### 广度优先遍历算法BFS和基于此的点云聚类的外点剔除

广度优先遍历（BFS）和深度优先遍历（DFS）一样，是经典的图遍历算法。

LeGO-LOAM框架用的是广度优先遍历算法做点云聚类，首先从某个节点出发，一层一层地遍历，下⼀层必须等到上⼀层节点全部遍历完成之后才会开始遍历。

![科研分享记录-11](.\img\科研分享记录-11.jpg)

如上面这个无向图中：如果我们从A节点开始遍历，那么首先访问和A节点相邻的节点，就是S、B、D，然后再访问和S、B、D相邻的其他节点，就是C。因此，遍历的顺序是A->S->B->D->C；如果我们从S开始遍历，顺序就是S->A->B->C->D；可以看到，不同的起始点对应的遍历顺序是不同的。

通常我们使用BFS遍历图结构的时候，会借助一个队列std::queue来帮助我们实现，其基本步骤如下：

1. 将起始顶点A加入队列。
2. 访问A，同时把A标记为已访问，A出列，将节点(相邻点)入队列。
3. 循环从队列中访问节点(相邻点)，标记为已访问，并出列，将子节点(相邻点)入队列。

其中： 如果添加的子节点中有已访问的，则不加入队列。如果队列为空，则访问结束。

BFS算法适用于图数据结构，为了将BFS算法运用到单帧lidar点云，首先需要将其建成一个图模型，一个简单有效的办法就是将其投影到⼀个平面图上，以速腾rs-16为例，我们将其投影到一个16×1800大小的图上（这里16是垂直的16根线，1800是因为水平分辨率是0.2度，一个扫描周期有1800个点）。如图：

![科研分享记录-12](.\img\科研分享记录-12.jpg)

对于任何⼀个栅格点，其上下左右四个相邻点视为图结构中的邻接节点，要注意的是，左右边界的点和边界另⼀侧也构成邻接，因为水平方向是同⼀个扫描周期，具有物理意义上的连续性，但垂直方面没有。我们可以从任意一个点开始执行BFS搜索，直到遍历完所有近邻点，聚类点数过少的就认为是outlier，可以被剔除。

具体实现：

1. 遍历每个点，如果该点已经被处理过了就不处理。
2. 如果没有被处理就说明这是⼀个新的聚类，然后执行BFS的步骤。
3. 将队列里的首元素弹出，然后将该元素近邻塞入队列末尾（这里没有使用std::queue，使用的是普通数组，用了双指针，如下图）

![科研分享记录-13](.\img\科研分享记录-13.jpg)

1. 分别判断近邻点和自身距离是否足够近，这里没有用两点之间的距离来算。 

![科研分享记录-14](.\img\科研分享记录-14.jpg)

而是用两点夹角的补角来衡量，angle越大则认为两点越可能是同一个聚类物体上的点，则打上同样的label。

1. 循环以上操作

代码如下：

```
 void cloudSegmentation(){
        for (size_t i = 0; i < N_SCAN; ++i)
            for (size_t j = 0; j < Horizon_SCAN; ++j)
                // 如果labelMat[i][j]=0,表示没有对该点进行过分类，则需要对该点进行聚类
                if (labelMat.at<int>(i,j) == 0)
                    labelComponents(i, j);  //BFS算法聚类

        int sizeOfSegCloud = 0;
        for (size_t i = 0; i < N_SCAN; ++i) {
            
            // segMsg.startRingIndex[i]、segMsg.endRingIndex[i] 表示第i线的点云起始序列和终止序列
            // 以开始线后的第6线为开始，以结束线前的第6线为结束
            segMsg.startRingIndex[i] = sizeOfSegCloud-1 + 5;

            for (size_t j = 0; j < Horizon_SCAN; ++j) {
                // 找到可用的特征点或者地面点(不选择labelMat[i][j]=0的点)
                if (labelMat.at<int>(i,j) > 0 || groundMat.at<int8_t>(i,j) == 1){
                    // labelMat数值为999999表示这个点是因为聚类数量不够30而被舍弃的点，需要舍弃的点直接continue跳过本次循环，
                    // 当列数为5的倍数，并且行数较大（大于7），将它保存进outlierCloud（外点）中
                    if (labelMat.at<int>(i,j) == 999999){
                        if (i > groundScanInd && j % 5 == 0){
                            outlierCloud->push_back(fullCloud->points[j + i*Horizon_SCAN]);
                            continue;
                        }else{
                            continue;
                        }
                    }
                    
                    // 如果是地面点,对于列数不为5的倍数的，直接跳过不处理
                    if (groundMat.at<int8_t>(i,j) == 1){
                        if (j%5!=0 && j>5 && j<Horizon_SCAN-5)
                            continue;
                    }
                    // 上面多个if语句已经去掉了不符合条件的点，这部分直接进行信息的拷贝和保存操作
                    // 保存完毕后sizeOfSegCloud递增
                    segMsg.segmentedCloudGroundFlag[sizeOfSegCloud] = (groundMat.at<int8_t>(i,j) == 1);
                    segMsg.segmentedCloudColInd[sizeOfSegCloud] = j;
                    segMsg.segmentedCloudRange[sizeOfSegCloud]  = rangeMat.at<float>(i,j);
                    segmentedCloud->push_back(fullCloud->points[j + i*Horizon_SCAN]);
                    ++sizeOfSegCloud;
                }
            }

            // 以结束线前的第5线为结束
            segMsg.endRingIndex[i] = sizeOfSegCloud-1 - 5;
        }

        // 如果有节点订阅SegmentedCloudPure,就把点云数据保存到segmentedCloudPure中去
        if (pubSegmentedCloudPure.getNumSubscribers() != 0){
            for (size_t i = 0; i < N_SCAN; ++i){
                for (size_t j = 0; j < Horizon_SCAN; ++j){
                    // 需要选择不是地面点(labelMat[i][j]!=-1)和没被舍弃的点，就是聚类后的点
                    if (labelMat.at<int>(i,j) > 0 && labelMat.at<int>(i,j) != 999999){
                        segmentedCloudPure->push_back(fullCloud->points[j + i*Horizon_SCAN]);
                        segmentedCloudPure->points.back().intensity = labelMat.at<int>(i,j);
                    }
                }
            }
        }
    }
```

下面是基于BFS的点云聚类函数：

```
 void labelComponents(int row, int col){
        float d1, d2, alpha, angle;
        int fromIndX, fromIndY, thisIndX, thisIndY; 
        bool lineCountFlag[N_SCAN] = {false};

        queueIndX[0] = row;
        queueIndY[0] = col;
        int queueSize = 1;
        int queueStartInd = 0;
        int queueEndInd = 1;

        allPushedIndX[0] = row;
        allPushedIndY[0] = col;
        int allPushedIndSize = 1;
        
        // 标准的BFS，作用是以(row，col)为中心向外面扩散，判断(row,col)是否是这个平面中一点
        while(queueSize > 0){
            fromIndX = queueIndX[queueStartInd];
            fromIndY = queueIndY[queueStartInd];
            --queueSize;
            ++queueStartInd;
            // labelCount的初始值为1，后面会递增
            labelMat.at<int>(fromIndX, fromIndY) = labelCount;

            // neighbor=[[-1,0];[0,1];[0,-1];[1,0]]
            // 遍历点[fromIndX,fromIndY]边上的四个邻点
            for (auto iter = neighborIterator.begin(); iter != neighborIterator.end(); ++iter){

                thisIndX = fromIndX + (*iter).first;
                thisIndY = fromIndY + (*iter).second;

                if (thisIndX < 0 || thisIndX >= N_SCAN)
                    continue;

                // 由于雷达水平角度360度相连的特性是个环状的图片，左右连通
                if (thisIndY < 0)
                    thisIndY = Horizon_SCAN - 1;
                if (thisIndY >= Horizon_SCAN)
                    thisIndY = 0;

                // 如果点[thisIndX,thisIndY]已经标记过
                // labelMat中，-1代表无效点，0代表未进行标记过，其余为其他的标记
                // 如果当前的邻点已经标记过，则跳过该点。
                // 如果labelMat已经标记为正整数，则已经聚类完成，不需要再次对该点聚类
                if (labelMat.at<int>(thisIndX, thisIndY) != 0)
                    continue;

                d1 = std::max(rangeMat.at<float>(fromIndX, fromIndY), 
                              rangeMat.at<float>(thisIndX, thisIndY));
                d2 = std::min(rangeMat.at<float>(fromIndX, fromIndY), 
                              rangeMat.at<float>(thisIndX, thisIndY));

                // alpha代表角度分辨率，X方向上角度分辨率是segmentAlphaX(rad)，Y方向上角度分辨率是segmentAlphaY(rad)
                if ((*iter).first == 0)
                    alpha = segmentAlphaX;
                else
                    alpha = segmentAlphaY;

                // 通过下面的公式计算这两点之间是否有平面特征，是否是同一类
                // atan2(y,x)的值越大，d1，d2之间的差距越小,越接近，越平坦
                angle = atan2(d2*sin(alpha), (d1 -d2*cos(alpha)));

                if (angle > segmentTheta){
                    // segmentTheta=1.0472<==>60度
                    // 如果算出角度大于60度，则假设这是个平面
                    queueIndX[queueEndInd] = thisIndX;
                    queueIndY[queueEndInd] = thisIndY;
                    ++queueSize;
                    ++queueEndInd;

                    labelMat.at<int>(thisIndX, thisIndY) = labelCount;
                    lineCountFlag[thisIndX] = true;

                    allPushedIndX[allPushedIndSize] = thisIndX;
                    allPushedIndY[allPushedIndSize] = thisIndY;
                    ++allPushedIndSize;
                }
            }
        }

        bool feasibleSegment = false;

        // 如果聚类超过30个点，直接标记为一个可用聚类，labelCount需要递增
        if (allPushedIndSize >= 30)
            feasibleSegment = true;
        else if (allPushedIndSize >= segmentValidPointNum){
            // 如果聚类点数小于30大于等于5，统计竖直方向上的聚类点数，这种情况典型例子是电线杆
            int lineCount = 0;
            for (size_t i = 0; i < N_SCAN; ++i)
                if (lineCountFlag[i] == true)
                    ++lineCount;

            // 竖直方向上超过3个也将它标记为有效聚类
            if (lineCount >= segmentValidLineNum)
                feasibleSegment = true;            
        }

        if (feasibleSegment == true){
            ++labelCount;
        }else{
            for (size_t i = 0; i < allPushedIndSize; ++i){
                // 标记为999999的是需要舍弃的聚类的点，因为他们的数量小于30个，且竖直方向少于3
                labelMat.at<int>(allPushedIndX[i], allPushedIndY[i]) = 999999;
            }
        }
    }
```

##### 两步优化的帧间⾥程计

和A-LOAM一样，通过前后两帧点云来估计两帧之间的运动，累加得到前端里程记的输出，和A-LOAM构建线面约束同时优化六自由度帧间位姿不同，LeGO-LOAM的前端分成两个步骤，每个步骤估计三自由度的变量:

**第一步** **利用地面点优化**

地面点更符合面特征的性质，因此地面点的优化问题就使用点到面的约束来构建，但地面点之间的约束对x，y和yaw这三个自由度是不能观的，就是当这三个自由度的值发生变化时，点到面的残差不会发生显著变化，所以，地面点之间的优化只会对pitch，roll以及z进行约束和优化。（很显然，pitch，roll变化时两次地面叠加会不重叠，同样Z方向也是。）

**第二步** **利用角点优化**

第一步优化完pitch、roll以及z之后，我们仍需对另外三个自由度的变量进行估计，此时我们用提取的角点进行优化，由于多线激光雷达提取的角点通常是垂直的边缘特征，比如电线杆。这些特征对x、y以及yaw有着比较好的能观性，通过对角点的优化结合地面点优化的结果可以得到六自由度的帧间优化结果。

代码如下，分成两步优化更新前端优化后的六自由度的位姿：

```
 void updateTransformation(){

        if (laserCloudCornerLastNum < 10 || laserCloudSurfLastNum < 100)  //角点要大于10，面点要大于100
            return;

        for (int iterCount1 = 0; iterCount1 < 25; iterCount1++) {
            laserCloudOri->clear();
            coeffSel->clear();

            // 这里是作者手写的非线性优化，利用点面约束优化row、pitch和Z。找到对应的特征平面,然后计算协方差矩阵，保存在coeffSel队列中
            // laserCloudOri中保存的是对应于coeffSel的未转换到开始时刻的原始点云数据
            findCorrespondingSurfFeatures(iterCount1);

            if (laserCloudOri->points.size() < 10)
                continue;
            // 通过面特征的匹配，计算变换矩阵
            if (calculateTransformationSurf(iterCount1) == false)
                break;
        }

        for (int iterCount2 = 0; iterCount2 < 25; iterCount2++) {

            laserCloudOri->clear();
            coeffSel->clear();

            // 这里是作者手写的非线性优化，利用点和角点之间约束优化yaw、x和y。找到对应的特征边/角点
            // 寻找边特征的方法和寻找平面特征的很类似
            findCorrespondingCornerFeatures(iterCount2);

            if (laserCloudOri->points.size() < 10)
                continue;
            // 通过角/边特征的匹配，计算变换矩阵
            if (calculateTransformationCorner(iterCount2) == false)
                break;
        }
    }
```

总结：LeGO-LOAM这个框架主要还是针对纯激光雷达SLAM部署在轻量化嵌入式平台，由于主要用了激光雷达这个传感器，其里程计精度不会很高。虽然也用了IMU的数据，但只是松耦合的方式。后续作者开源的激光雷达和IMU紧耦合的LIO-SAM框架的里程计精度提升更大。而对于后端，LeGO-LOAM比A-LOAM还是有很多提升，增加了回环检测和图优化，使得地图全局一致性更好，这在后面LIO-SAM框架中再具体分析下。

#### 项目工程代码介绍：

由于LeGO-LOAM框架的很多代码是直接从LOAM框架中直接移植过来的，移植过来的代码可读性不强。比如mapping模块中有大量欧拉角的计算，同时手写基于高斯牛顿法的非线性优化代码，如果用第三方库调用模板类，代码可读性会更强（但是可以学习其算法实现思路）。所以，就代码工程而言LeGO-LOAM框架的参考价值没有A-LOAM框架的大。我们主要关注LeGO-LOAM框架对LOAM的改进，对其工程就不做太多介绍，简单介绍下代码框架就可以。而且框架作者后面开源的LIO-SAM和LVI-SAM框架的可读性强了很多，这些框架的代码工程参考价值更大。

​     项目工程中包括很多图片，主要是展示效果和实物，readme里面包含了作者信息和项目信息。

​     我们主要看src文件夹，cloud _msgs定义了一些自定义消息类型。Readme文件是这个工程如何部署的一些信息。LeGO-LOAM文件夹中主要是src源文件和include头文件还有launch文件。Include文件夹里面的utility.h包含很多参数文件，其实这部分应该写到yaml文件或者launch文件中，每次运行程序初始化时读取参数配置，这样方便使用和调试。

我们看src文件夹源码。featureAssociation.cpp和 imageProjection.cpp 两个源文件主要是前端的处理。因为这块代码实现比较乱，比如雷达和惯导数据读取在不同的节点中，再加上后面LIO-SAM的代码结构好了很多，所以这里就不深入分析，只是简单介绍下代码。这里包括：

1. 数据的读取（两个传感器话题数据的回调函数）
2. 雷达和IMU空间、时间的对齐，即坐标变换和时间同步，然后进行雷达运动畸变的去除（adjustDistortion函数）
3. 计算曲率，这里和A-LOAM框架一样（calculateSmoothness函数）
4. 标记近点（markOccludedPoints函数）
5. 点云特征提取和分类，和A-LOAM框架一样（extractFeatures函数）
6. IMU提供初始位姿（updateInitialGuess函数）
7. 分两步优化帧间里程计，更新位姿（updateTransformation函数）
8. 更新IMU位姿（integrateTransformation函数）

imageProjection节点主要是完成地面分离和点云聚类。这一块前面已经讲了，后面再结合LIO-SAM深入分析。

transformFusion主要是手写了里程计位姿的变换，太多公式，可读性比较差。

mapOptmization是后端优化和回环检测，在LIO-SAM再详细解读。

主要代码如下：

```
void runFeatureAssociation()
    {
        // 如果有新数据进来则执行，否则不执行任何操作
        if (newSegmentedCloud && newSegmentedCloudInfo && newOutlierCloud &&
            std::abs(timeNewSegmentedCloudInfo - timeNewSegmentedCloud) < 0.05 &&
            std::abs(timeNewOutlierCloud - timeNewSegmentedCloud) < 0.05){

            newSegmentedCloud = false;
            newSegmentedCloudInfo = false;
            newOutlierCloud = false;
        }else{
            return;
        }

        // 主要进行的处理是将点云数据进行坐标变换，时间同步等工作
        adjustDistortion();

        // 不完全按照公式进行曲率计算
        calculateSmoothness();

        // 标记过近的点，指在点云中可能出现的互相遮挡的情况
        markOccludedPoints();

        // 特征提取，分别保存到cornerPointsSharp等队列中去，保存到不同的队列是不同类型的点云，进行了分类的工作，这一步中减少了点云数量，使计算量减少
        extractFeatures();

        // 发布cornerPointsSharp等4种类型的点云数据
        publishCloud();

        if (!systemInitedLM) {
            checkSystemInitialization(); 
            return;
        }

        // IMU提供初始位姿
        updateInitialGuess();

        // 更新变换
        updateTransformation();

        // IMU位姿变换
        integrateTransformation();

        publishOdometry();

        publishCloudsLast();   
    }
```

#### 参考的博客和Github链接：

##### CSDN博客的内容链接：   

1.[地图优化代码理解](https://blog.csdn.net/wykxwyc/article/details/98316127)  

2.[图像重投影代码理解](https://blog.csdn.net/wykxwyc/article/details/98317100)  

3.[特征关联代码理解](https://blog.csdn.net/wykxwyc/article/details/98317544)  

4.[LeGO-LOAM代码中数学公式的解析]

(https://blog.csdn.net/wykxwyc/article/details/98318294)   

5.[LeGO-LOAM论文翻译(内容精简)](https://blog.csdn.net/wykxwyc/article/details/89605721)   

6.[关于位姿变换的一道题](https://blog.csdn.net/wykxwyc/article/details/101712524)   

##### Github链接:  

1.[地图优化代码理解](https://wykxwyc.github.io/2019/01/21/LeGO-LOAM-code-review-mapOptmization/)  

2.[图像重投影代码理解](https://wykxwyc.github.io/2019/01/23/LeGO-LOAM-code-review-imageProjection/)  

3.[特征关联代码理解](https://wykxwyc.github.io/2019/01/24/LeGO-LOAM-code-review-featureAssociation/)  

4.[LeGO-LOAM代码中数学公式的解析](https://wykxwyc.github.io/2019/08/01/The-Math-Formula-in-LeGO-LOAM/)    

5.[LeGO-LOAM论文翻译(内容精简)](https://wykxwyc.github.io/2019/04/26/LeGO-LOAM-Paper-Traslation-and-Summary/)  

#### **总结：**

后面在读LIO-SAM框架的时候再进行详细分析，并且结合A-LOAM框架中调用第三方库，分析算法代码如何实现，如何手写和调用库，之间的区别是？

### LIO-SAM框架

#### 框架介绍

LIO-SAM是Tixiao Shan在2020的IROS会议提出的一种激光惯导紧耦合的SLAM框架，可在室内和室外实现效果不错的建图。

这是作者附在Github上面的框架图：

![科研分享记录-15](.\img\科研分享记录-15.jpg)

特点：

1. 一共使用了三个传感器，GPS可选；
2. Odometry 以IMU的frequency（要收到雷达里程计信息后才发出），这样前端频率更高；
3. IMU odometry提供初始估计值并做了预积分处理，同时IMU原始数据进行雷达进行运动补偿；
4. 后端因子图优化包括四个因子，IMU预积分结合雷达里程计的帧间约束因子在预积分节点维护，其他三个在后端节点维护，分别是GPS因子、雷达里程计因子和回环检测因子。
5. 紧耦合：IMU的零偏可以被估计，利用雷达里程计的帧间约束进行反馈，使IMU解算更加准确，提供更好的初值。

#### 运行演示

1. 首先要安装 ROS

Ubuntu 64-bit 16.04 or 18.04. ROS Kinetic or Melodic

1. 安装ROS依赖

```
sudo apt-get install -y ros-kinetic-navigation
sudo apt-get install -y ros-kinetic-robot-localization
sudo apt-get install -y ros-kinetic-robot-state-publisher
```

1. 安装gtsam

```
wget -O ~/Downloads/gtsam.zip https://github.com/borglab/gtsam/archive/4.0.0-alpha2.zip
cd ~/Downloads/ && unzip gtsam.zip -d ~/Downloads/
cd ~/Downloads/gtsam-4.0.0-alpha2/
mkdir build && cd build
cmake ..
sudo make install
```

1. 下载编译LIO-SAM

```
cd ~/catkin_ws/src
git clone https://github.com/TixiaoShan/LIO-SAM.git
cd ..
catkin_make
```

1. 播放 bag 包运行(github上有数据集下载)

```
roslaunch lio_sam run.launch
rosbag play your-bag.bag 
```

这里有些数据集需要更改yaml文件参数，具体请看github。 效果如下：

![科研分享记录-16](.\img\科研分享记录-16.jpg)

1. 报错解决

```
error: static assertion failed: Error: GTSAM was built against a different version of Eigen static_assert
```

GTSAM默认编译并没有使用系统eigen

在CMakeLists.txt文件中if（GTSAM_USE_SYSTEM_EIGEN）判断的前边添加如下代码：

```
set(GTSAM_USE_SYSTEM_EIGEN ON)
```

运行时报错 [lio_sam_mapOptmization-5] process has died [pid 260348, exit code -11

解决: gtsam编译时带上这个参数

```
cmake -DGTSAM_BUILD_WITH_MARCH_NATIVE=OFF ..
```

#### 速腾Robosense-16线雷达和SBG-Ellipse-N AHRS传感器室外建图

1. 雷达测试： 首先安装好雷达在平台上，供电准备好，雷达网口接终端，雷达本机ip是192.168.1.200，终端ip要配置成192.168.1.102，子网掩码255.255.255.0即可。 终端ping 192.168.1.200可以ping通则说明雷达通讯没问题。

2. 雷达驱动下载： GitHub下载最新源码 https://github.com/RoboSense-LiDAR/rslidar_sdk.git

3. 雷达驱动环境安装： 安装pcap：

   ```
   sudo apt-get install -y libpcap-dev
   ```

   安装Yaml：

   ```
   sudo apt-get install -y libyaml-cpp-dev （若已安装ROS desktop-full, 可跳过）
   ```

4. 修改CMakeLists.txt。 （1）将文件顶部的set(COMPILE_METHOD ORIGINAL)改为set(COMPILE_METHOD CATKIN) （2）将set(POINT_TYPE XYZI) 改为set(POINT_TYPE XYZIRT)

5. 将rslidar_sdk工程目录下的package_ros1.xml文件重命名为package.xml。   

6. 建立ros工作空间，在工作空间编译。

7. GitHub下载rs雷达话题转velodyne雷达话题 https://github.com/HViktorTsoi/rs_to_velodyne.git  并编译，可放在同个工作空间编译。

8. 编译LIO-SAM，前面有讲，可放在同个工作空间编译

9. SBG AHRS传感器驱动安装： SBG传感器具体如何通过上位机或者yaml文件更改配置，这个部分之前有讲过。 这里主要就是SBG的ROS驱动如何安装，IMU话题如何给到LIO-SAM中。 下载并编译最新的源码：

   ```
   cd catkin_ws/src
   ```

   ```
   git clone https://github.com/SBG-Systems/sbg_ros_driver.git
   ```

   ```
   cd ../
   ```

   ```
   catkin_make
   ```

惯导ROS驱动包yaml文件可以修改传感器参数，但是不建议。建议的做法是在上位机进行修改，修改后yaml文件也同步进行修改。比如： 1)	需要找到传感器的串口号，然后更改portName: "/dev/ttyUSB0" 2)	更改波特率： baudRate: 921600 3)	更改运动模型： motionProfile: 2 （注：车载模型） 4)	更改输出：改成ros时间：time_reference: "ros"     5)	更改输出：改成ROS的IMU消息格式 ：    ros_standard: true 6)	更改输出：更改为ENU坐标系，    use_enu: true 7)	更改输出：更改frame_id ：        frame_id: "imu_link"    8)	更改输出：更改系统状态刷新频率：log_status: 200          9)	更改输出：更改IMU数据刷新频率：  log_imu_data: 1    10)	更改输出：更改UTC时间刷新频率：  log_utc_time: 200 11)	更改输出：更改磁力计数据刷新频率：    log_mag: 4

1. 修改话题名称： sbg_ellipseN.launch文件中增加：	

   ```
   <remap from="/imu/data" to="imu_raw" />
   ```

   把IMU话题改成LIO-SAM需要的话题 在LIO-SAM中config文件夹中的配置yaml文件中修改雷达话题名称

   ```
   pointCloudTopic: "points_raw"   
   ```

   （改成雷达的话题velodyne _points）

   1. 标定内参：

   前面已经讲解如何标定IMU内参，这里不做讲解，就是替换前面四个参数。第五个参数需要连续采样Z轴重力加速度做均值。

   ![科研分享记录-17](.\img\科研分享记录-17.jpg)

   1. 标定外参

   即确认惯导与雷达的旋转变换矩阵

   查看LIO-SAM中config文件夹中的配置yaml文件：

   ![科研分享记录-18](.\img\科研分享记录-18.jpg)

   其中extrinsicRot表示imu->lidar的坐标变换, 用于旋转imu坐标系下的加速度和角速度到lidar坐标系下, extrinsicRPY则用于旋转imu坐标系下的欧拉角到lidar坐标下, 由于lio-sam作者使用的imu的欧拉角旋转方向与lidar坐标系不一致（即是按照什么旋转顺序旋转）, 因此使用了两个旋转不同, 但是大部分的设备两个旋转应该是设置为相同的，我们这里也是设置为相同即可。

   前面标定有做讲解，这里就不说如何标定了，标定好了替换这两个矩阵。

   平移向量影响不是很大，直接从结构图纸中得到即可，即是惯导原点到雷达原点的向量。

2. 运行 在运行之前要给惯导串口给权限：

```
sudo chmod 777 dev/ttyUSB0
```

永久权限：后面再加 然后运行：

```
source devel/setup.bash
roslaunch sbg_driver sbg_ellipseN.launch
source devel/setup.bash
roslaunch rslidar_sdk start.launch
source devel/setup.bash
rosrun rs_to_velodyne rs_to_velodyne XYZIRT XYZIRT
source devel/setup.bash
roslaunch lio_sam run.launch
```

1. 测试效果

   ![科研分享记录-19](.\img\科研分享记录-19.jpg)

2. 数据保存

   录制地图包：

   ```
   rosbag record -o mybag.bag out /laser_cloud_surround 
   ```

   查看：

   ```
   rosrun pcl_ros bag_to_pcd mybag.bag /laser_cloud_surround mypcd
   ```

   ```
   pcl_viewer xxx.pcd  （这里是查看一帧的数据）
   ```

   录制点云包：

   ```
   rosbag record -o mybag.bag out /velodyne_points /imu_raw  
   ```

   如果想要保存地图，需要对config/params.yaml文件进行如下修改

   ```
   # 保存地图设置为true
   ```

   ```
   savePCD: true                            
   ```

   ```
   # 设置地图保存路径
   ```

   ```
   savePCDDirectory: "自己设置的路径"     
   ```

   注意：这里的路径是home folder, starts and ends with "/"，即在home folder下建立一个文件夹即可。

   更改了配置文件后，还需更改一下*TIMEOUT_SIGINT参数，否则可能造成地图保存失败(这是由于ros会在*TIMEOUT_SIGINT秒后关闭ros节点，但是地图过大时，保存地图会花费一些时间，如果*TIMEOUT_SIGINT太小，很可能造成地图还未保存，节点就已经关闭了，所以需要适当调高*TIMEOUT_SIGINT值)，具体方法如下：

   sudo gedit /opt/ros/kinetic/lib/python2.7/dist-packages/roslaunch/nodeprocess.py

   找到_TIMEOUT_SIGINT并调整数值(默认15s，参考值60s)

   还有一个方法是直接终端敲：

   ```
   rosservice call /lio_sam/save_map 0.2 "/data/"
   ```

   保存下来的pcd文件：

   ![科研分享记录-20](.\img\科研分享记录-20.jpg)

   CC打开：

   ![科研分享记录-21](.\img\科研分享记录-21.jpg)

3. 总结

IMU内参和IMU与雷达外参在低速运行的情况下建图效果差距不大，也就是我们可以不用标定，直接用理论值就行。只要保证IMU使用正常即可，即是保证IMU运动方向与车前进方向相同，不然点云会错乱。后面再对比几个基于雷达的SLAM框架数据。

#### IMU预积分

##### 介绍及选型

IMU的全称是惯性测量单元，包括⼀个三轴的加速度计以及一个三轴的陀螺仪，分别测量出物体的加速度和角速度信息，不受周围环境结构，光照等外界因素影响;IMU可以获得当前时刻的角速度和加速度值，通过该值可以对系统状态（位置，速度，姿态）进行推算。

通常IMU的输出频率在100-1000hz之间，远高于相机或者激光雷达的输出频率，一方面可以提高整体系统的输出频率，另一方面，可以在视觉或者激光短期失效的时候提供一段时间的位姿推算。 在大多数的LIO（雷达惯性里程计）或者VIO（视觉惯性里程计）中，关于IMU输出的建模方式为

![科研分享记录-22](.\img\科研分享记录-22.jpg)

输出的加速度计和陀螺仪的数据受零偏（ba和bw）以及高斯白噪声（na和nw）的影响，紧耦合的LIO或者VIO都会实时估计IMU的零偏，以实现IMU数据的最大利用率，达到融合效果最大化。

这里的加速度要减去重力加速度或者重力加速度的分量才是实际的加速度，而且传感器测得的是世界坐标系下的加速度，然后左乘一个将世界坐标系转换到机体坐标系的旋转矩阵，得到的才是物体的加速度。

然而，实际的IMU数据并没有这么理想，除了零偏和高斯白噪声，还有可能受到刻度误差，尺度因子，轴向偏差等影响，如果把这些因素都通过建模的方式考虑进来，就显得过于复杂；因此，通常的做法是在IMU选型的时候就最大化避免上述误差的影响。也就是说，在选择IMU型号时除了关注价格（价格很多时候是非常重要甚至是决定性的因素），还需要关心其出厂标定情况，是否做过温度补偿等。

##### 为什么需要预积分

当k时刻的状态发生变化时，通过imu积分得到的k+1时刻的状态也会发生相应的变化，而在基于滑窗的后端优化或者因子图的优化中，对一些状态量进行调整是必然发生的。如果每次状态发生调整时imu的积分过程都会重新执行一遍，则实时性无法得到保证。因此，预积分理论就是为解决这个问题而提出的，其核心思想就是IMU积分的结果和上⼀时刻系统的状态无关。这样，当系统状态在优化过程中发生调整的时候，就不需要对下⼀时刻的系统状态重新积分。

##### 关于零偏的建模

通常来说，IMU的零偏会随着时间的变化而偏移，因此为了系统的准确性，零偏也是系统的优化变量之一。预积分量虽然和两帧的具体位姿和速度等状态量无关，但是和零偏相关，因此，当零偏作为优化变量被优化后，预积分量也会发生相应的变化，那么此时我们是否需要重新积分呢？答案是不用的。为了避免零偏的变化导致预积分量重新积分，通常认为零偏的变化在短时间内（100ms）非常小，因此，我们可以使用一阶泰勒展开来进行近似。

预积分量对于零偏的雅克比矩阵会在预积分计算的时候一并用于计算，因此，当零偏被优化调整之后， 会计算预积分量对于零偏的雅克比矩阵，然后进行预积分时更新过去即可（即更换预积分量对于零偏的雅克比矩阵）。

##### 离散时间的预积分更新

实际系统是离散的IMU数据，我们的目的是得到两个关键帧（视觉 or lidar）之间的预积分结果，而IMU的频率是远大于雷达和相机的。常见的做法就是每收到⼀帧新的IMU数据就更新⼀次预积分量，这是⼀个求和而非连续积分的过程，而到了相机或者雷达的数据帧后，就用当前更新的预积分量去融合。

##### 预积分在优化问题中的建模

预积分量约束相邻两帧的状态量（位置、速度、姿态），同时考虑到IMU零偏的性质（即短时间内变换速率比较缓慢，可以认为两帧之间的零偏不变），也就是还可以约束两帧的零偏变化。 在slam问题中，IMU预积分约束通常会和其他传感器的约束一起构建一个优化问题。

###### 视觉VIO中预积分和视觉的优化建模

这里黄色代表IMU预积分，只对相邻帧发生约束，五角星代表视觉地图点，同一个地图点可以通过视觉重投影误差对多个关键帧的位姿形成约束。

![科研分享记录-23](.\img\科研分享记录-23.jpg)

###### LIO-SAM中预积分和lidar位姿的优化建模

![科研分享记录-24](.\img\科研分享记录-24.jpg)

如图，蓝色圆圈代表关键帧位姿，蓝色矩形代表关键帧速度和零偏，橙色矩形代表IMU预积分约束，可 以看到，它可以约束相邻帧的位姿、速度和零偏，绿色矩形代表lidar里程记的帧间约束，其约束相邻两 帧的位置和姿态。这里涉及到lidar里程计因子和预积分因子，是预积分节点因子图的优化模型。

#### IMU标定方法简介

无论是VIO还是LIO，IMU和其他传感器的标定结果往往对最终的里程记和建图性能有着显著的影响。同样，在多传感器融合算法中，传感器之间的标定结果的精度对多传感器融合的效果也有着非常大的影响。在lidar-imu融合的算法中，lidar和IMU之间的外参标定非常重要，在一些数据集上往往有着经过良好标定的结果，然而，绝大多数情况下，传感器之间的外参需要我们自己去标定。 

LIO-SAM中，作者推荐了一个Lidar-IMU标定的功能包来实现lidar和IMU之间的外参标定

https://github.com/chennuo0125-HIT/lidar_imu_calib

通常来讲，lidar和IMU之间的旋转外参相比于平移外参对里程记的性能有着更显著的影响。因此，条件有限的情况下，可以只标定lidar和IMU之间的旋转外参，平移外参则可以通过手工测量等方式获取。

该旋转标定算法本质上是⼀个手眼标定算法，即先计算出两帧lidar之间的旋转（通过ICP、NDT等点云配准算法），然后计算出两帧IMU之间的旋转（IMU陀螺仪积分），然后通过手眼标定的方式求解出外参，和VINS-Mono中相机IMU旋转外参初始化的过程⼀致，具体方式为：

1. 使用四元数来表示旋转，四元数的乘法可以通过一个公式转成矩阵的乘法。
2. 收集若干组IMU和lidar的相对旋转和平移，联立求解，相当于已知一个4n×4大小的矩阵，求出⼀个4×1向量的最优解，通常n>4；这是一个基本的超定方程求解问题，通常使用SVD方法求解，即将A矩阵进行SVD分解。求解出来的x转换成四元数即为所求的旋转。

前面讲过IMU内参的标定和外参的标定，外参用的是瑞士苏黎世理工大学-自动驾驶实验室开源的lidar_align工程进行标定。

#### 项目工程代码介绍

**代码框架**

```
lio-sam:.
│  CMakeLists.txt   #项目工程配置文件，可以知道作者用了哪些第三方库
│  LICENSE          #软件版权
│  package.xml      #ROS包配置文件
│  README.md        #项目工程说明文件：文件构成、依赖、运行等
│  
├─config
│  │  params.yaml  #参数文件
│  │  
│  └─doc   #存储效果图、流程图、论文等
│      │  
│      └─kitti2bag  #将kitti数据集转换成bag格式
│              kitti2bag.py
│              README.md
│              
├─include
│      utility.h  #参数服务器类，初始化参数；各类公用函数
│      
├─launch
│  │  run.launch #总运行launch文件
│  │  
│  └─include  #分模块运行文件
│      │  module_loam.launch
│      │  module_navsat.launch
│      │  module_robot_state_publisher.launch
│      │  module_rviz.launch
│      │  
│      ├─config #存储rviz参数文件和机器人坐标系参数
│      │      rviz.rviz
│      │      robot.urdf.xacro
│              
├─msg
│      cloud_info.msg #自定义ROS数据格式
│      
├─src #源文件
│      featureExtraction.cpp #提取雷达线面特征，发布雷达点云
│      imageProjection.cpp   #订阅提取的雷达点云、IMU数据和IMU里程计数据，对雷达做畸变矫正，进行雷达前端里程计位姿粗估计的发布（以IMU频率）
│      imuPreintegration.cpp #IMU预积分，订阅雷达里程计和IMU数据，估计IMU偏置，进行雷达里程计、IMU预积分因子的图优化，输出IMU里程计。
│      mapOptmization.cpp    #订阅雷达前端信息、GPS信息，进行点云配准，进行雷达里程计、全局GPS、回环检测因子的图优化。
│      
└─srv
        save_map.srv
```

下面是代码框架示意图：

![科研分享记录-25](.\img\科研分享记录-25.png)

这里主要解读src文件夹中的四个源码文件，还有utility.h和params.yaml文件。

##### utility.h

主要包括ParamServer类和其他函数。

ParamServer类中定义了很多公共的成员变量，用于存储参数服务器读取的参数，主要包括Topics、Frames、GPS Settings、Save pcd、Lidar Sensor Configuration、IMU、LOAM、voxel filter paprams、CPU Params、Surrounding map、Loop closure、global map visualization radius

其构造函数ParamServer()读取参数服务器（params.yaml）中的参数，并存到公共成员变量中。

其他源文件中的类继承ParamServer类，实例化对象时就调用了父类的构造函数ParamServer，读取了参数。

这样做的好处是，将需要经常改动的参数写在yaml文件中，程序留个接口读取参数，方便调试。

其他函数：

```
sensor_msgs::Imu imuConverter(const sensor_msgs::Imu& imu_in)
```

把imu的数据旋转到前左上坐标系下，并判断是不是用了九轴IMU，因为使用了磁力计的姿态信息。

```
sensor_msgs::PointCloud2 publishCloud(ros::Publisher *thisPub, pcl::PointCloud<PointType>::Ptr thisCloud, ros::Time thisStamp, std::string thisFrame)
```

这是发布点云函数。

```
double ROS_TIME(T msg)
```

这是打印ROS时间的函数。

```
void imuAngular2rosAngular(sensor_msgs::Imu *thisImuMsg, T *angular_x, T *angular_y, T *angular_z)
```

进行加速度数据读取。

```
void imuRPY2rosRPY(sensor_msgs::Imu *thisImuMsg, T *rosRoll, T *rosPitch, T *rosYaw)
```

将IMU的RPY角转到ROS定义的RPY角。

```
float pointDistance(PointType p)
```

计算点的距离。

```
float pointDistance(PointType p1, PointType p2)
```

计算两点的距离。

##### params.yaml

参数文件参数如下：

1. Topics类（各个传感器的话题，根据使用的传感器话题更改）

```
pointCloudTopic: "points_raw"               # Point cloud data 
imuTopic: "imu_correct"                     # IMU data
odomTopic: "odometry/imu"        # IMU pre-preintegration odometry, same frequency as IMU
gpsTopic: "odometry/gpsz" # GPS odometry topic from navsat, see module_navsat.launch file
```

1. Frames类（不同坐标系）

```
  lidarFrame: "base_link"   #基座标，机体坐标
  baselinkFrame: "base_link"  #基座标，机体坐标
  odometryFrame: "odom"   #里程计坐标系
  mapFrame: "map"   #世界坐标系
```

1. GPS类（如果使用了GPS，则useImuHeadingInitialization和useGpsElevation为true）

```
  # GPS Settings
  useImuHeadingInitialization: true           # if using GPS data, set to "true"
  useGpsElevation: false                      # if GPS elevation is bad, set to "false"
  gpsCovThreshold: 2.0                        # m^2, threshold for using GPS data
  poseCovThreshold: 25.0                      # m^2, threshold for using GPS data
```

1. 保存数据类

```
# Export settings
  savePCD: false                              # 保存数据则置为true
  savePCDDirectory: "/Downloads/LOAM/"        # 保存路径，开始和结尾都要有/,且为根目录下。例：如果在根目录下创建了data文件，要保存在data文件夹，则参数为/data/
```

1. 雷达参数类（根据使用雷达的型号更改）

```
# Sensor Settings
 sensor: velodyne                    # lidar sensor type, either 'velodyne' or  'ouster'
  N_SCAN: 16                          # number of lidar channel (i.e., 16, 32, 64, 128)
  Horizon_SCAN: 1800      
  # lidar horizontal resolution (Velodyne:1800, Ouster:512,1024,2048)
  downsampleRate: 1                           
  # default: 1. Downsample your data if too many points. i.e., 16 = 64 / 4, 16 = 16 / 1
  lidarMinRange: 1.0                       # default: 1.0, minimum lidar range to be used
  lidarMaxRange: 1000.0                 # default: 1000.0, maximum lidar range to be used
```

1. IMU Settings类（更改IMU的噪音和偏置，还有重力加速度值）

```
  imuAccNoise: 3.9939570888238808e-03
  imuGyrNoise: 1.5636343949698187e-03
  imuAccBiasN: 6.4356659353532566e-05
  imuGyrBiasN: 3.5640318696367613e-05
  imuGravity: 9.80511  
  imuRPYWeight: 0.01  //磁力计所占权重
```

1. 外参矩阵（雷达到惯导，这里前面文章有作解释）



```
# Extrinsics (lidar -> IMU)
  extrinsicTrans: [0.0, 0.0, 0.0]
  extrinsicRot: [1, 0, 0,
                  0, 1, 0,
                  0, 0, 1]
  extrinsicRPY: [1, 0, 0,
                  0, 1, 0,
                  0, 0, 1]
```

1. loam参数设置（作者使用loam前端雷达里程计的参数，主要是角点和边缘点的数量和阈值）

```
# LOAM feature threshold
  edgeThreshold: 1.0  
  surfThreshold: 0.1
  edgeFeatureMinValidNum: 10
  surfFeatureMinValidNum: 100
```

1. 体素滤波器参数（立方体格子大小）

```
  # voxel filter paprams
  odometrySurfLeafSize: 0.4                     # default: 0.4 - outdoor, 0.2 - indoor
  mappingCornerLeafSize: 0.2                    # default: 0.2 - outdoor, 0.1 - indoor
  mappingSurfLeafSize: 0.4                      # default: 0.4 - outdoor, 0.2 - indoor
```

1. 机器人限制参数（限制Z轴和旋转）

```
  # robot motion constraint (in case you are using a 2D robot)
  z_tollerance: 1000                            # meters
  rotation_tollerance: 1000                     # radians
```

1. CPU Params

```
 numberOfCores: 4                # number of cores for mapping optimization CPU核数
 mappingProcessInterval: 0.15          # seconds, regulate mapping frequency 建图频率
```

1. 地图参数（关键帧阈值和点云密度、关键帧滑动窗口距离）

```
 # Surrounding map
  surroundingkeyframeAddingDistThreshold: 1.0  
  # meters, regulate keyframe adding threshold
  surroundingkeyframeAddingAngleThreshold: 0.2  
  # radians, regulate keyframe adding threshold
  surroundingKeyframeDensity: 2.0         # meters, downsample surrounding keyframe poses 
  surroundingKeyframeSearchRadius: 50.0         
  # meters, within n meters scan-to-map optimization (when loop closure disabled)
```

1. 回环参数（是否开启回环，回环频率，搜索范围、关键帧大小、数量等）

```
  # Loop closure
loopClosureEnableFlag: true
loopClosureFrequency: 1.0            # Hz, regulate loop closure constraint add frequency
surroundingKeyframeSize: 50                   # submap size (when loop closure enabled)
historyKeyframeSearchRadius: 15.0             # meters, key frame that is within n meters from current pose will be considerd for loop closure
  historyKeyframeSearchTimeDiff: 30.0           # seconds, key frame that is n seconds older will be considered for loop closure
  historyKeyframeSearchNum: 25                  # number of hostory key frames will be fused into a submap for loop closure
  historyKeyframeFitnessScore: 0.3     # icp threshold, the smaller the better alignment
```

1. 可视化参数（全局地图密度、范围）

```
  # Visualization
globalMapVisualizationSearchRadius: 1000.0    # meters, global map visualization radius
globalMapVisualizationPoseDensity: 10.0    
# meters, global map visualization keyframe density
globalMapVisualizationLeafSize: 1.0     # meters, global map visualization cloud density
```

1. GPS参数（详细参数，这里还不太懂）

主要有navsat和ekf_gps系列参数

##### 基础知识：

###### 因子图优化及GTSAM库介绍

**因子图**

在slam的后端优化问题中，通常会通过⼀些传感器的观测，比如视觉特征点，IMU预积分量，Lidar面点和边缘点（角点）的约束去构建一个优化问题，求解状态量（如位姿、速度等）。这个时候存在一个问题，当给这个系统新增⼀个约束时，就会重新建立所有的约束对状态量的优化问题进行求解；当优化模型增大时，显然进行一次优化的时间也会增加很多；一方面实时性遭遇了挑战，另一方面，很久之前的状态也没有继续更新的必要。为了解决这个问题，⼀种方式是使用滑动窗口来控制优化问题的规模，而滑动窗口需要处理好边缘化的问题；另一种方式，可以使用因子图的模型来解决这个问题。 Kaess等科研人员提出iSAM，即增量平滑和建图，使其可以自动增量处理大规模优化问题；具体来说，其内部使用一种基于概率的贝叶斯树，使得每次给因子图增加一个约束时，会根据贝叶斯树的连接关系，调整和当前结点“关系比较密切”的结点（概率比较大）；这样既保障了优化问题的求解精度，也使得耗时不会随着优化问题的增大而增大。关于因子图优化理论可以参考iSAM，iSAM2相关论文。

**因子图中一些概念**

变量结点：类似g2O中的顶点或者ceres中的参数块，代表需要被优化的变量。 因子结点：类似g2O中的边或者ceres中的cost function（代价函数），代表约束，如预积分约束、位姿先验约束、帧间位姿约束等。

**GTSAM库的介绍** GTSAM全称是 Georgia Tech Smoothing and Mapping library，是佐治亚理工学院的科研人员基于因子图和贝叶斯网络推出的⼀个C++库文件，如果想在工程中使用因子图优化的相关算法，最常用的方式就是借助GTSAM这个库来实现，因为其内部已经封装了关于因子图优化以及iSAM相关的算法实现，只需要像调用其他第三方库的方式（如openCV，PCL等）调用GTSAM库即可。 关于GTSAM库的详细介绍可以参考其官方文档

https://gtsam.org/

**GTSAM关于IMU预积分相关接口介绍**

对两个雷达关键帧之间的若干帧IMU进行预积分，以形成预积分约束，对两帧之间的位置、速度、姿态以及零偏进行约束。GTSAM从4.0版本开始就在内部增加了IMU预积分相关的接口。

预积分相关参数：

```
gtsam::PreintegrationParams
```

对IMU数据进行预积分之前需要事先知道IMU的噪声，重力方向等参数。

跟预积分相关的计算在这个类中实现：

```
gtsam::PreintegratedImuMeasurements
```

这个类有⼀些重要的接口。

预积分量复位：

```
resetIntegrationAndSetBias
```

也就是清空之前的预积分量，重新开始一个新的预积分量。预积分的计算依赖一个初始的IMU零偏，因此在复位之后需要输入零偏值，这里的复位和重设零偏在一个接口里。

输入IMU的测量值，内部会自动实现预积分量的更新以及协方差矩阵的更新：

```
integrateMeasurement
```

预积分量的时间长度：

```
deltaTij
```

预积分量可以计算出两帧之间的相对位置、速度、姿态的变化量，根据预积分结果，结合上一帧的状态量就可以计算出下一关键帧的预测：

```
predict
```

**预积分前端代码**

**变量结点**

也是预积分模型中涉及到的三种状态变量

```
gtsam::Pose3 // 表⽰六⾃由度位姿
gtsam::Vector3 // 表⽰三⾃由度速度
gtsam::imuBias::ConstantBias // 表⽰IMU零偏
```

**因子结点**

先验因子：

```
gtsam::PriorFactor<T>
```

表示对某个状态量T的一个先验估计，约束某个状态变量的值不会离该先验值过远。

imu因子：

```
gtsam::ImuFactor
```

通过IMU预积分量构造出的IMU因子，即IMU约束

状态量间的约束：

```
gtsam::BetweenFactor
```

约束相邻两状态量之间的差值不会距离该约束的值过远。

##### 源码分析：

**前端源码分析（featureExtraction.cpp、imageProjection.cpp、imuPreintegration.cpp）**

**imageProjection**

主要功能是：

1. 利用当前激光帧起止时刻间的imu数据计算旋转增量，IMU里程计数据（来自ImuPreintegration）计算平移增量，进而对该帧激光每一时刻的激光点进行运动畸变校正（利用相对于激光帧起始时刻的位姿增量，变换当前激光点到起始时刻激光点的坐标系下，实现校正）；
2. 同时用IMU数据的姿态角（RPY，roll、pitch、yaw）、IMU里程计数据的的位姿，对当前帧激光位姿进行粗略初始化。

**featureExtraction**

这块跟张绩博士的LOAM代码是一致的，对经过运动畸变校正之后的当前帧激光点云，计算每个点的曲率，进而提取角点、平面点（用曲率的大小进行判定）。

imuPreintegration

 

#### 多传感器融合工程实践和展望

##### A-LOAM、LeGO-LOAM、LIO-SAM 不同算法的对比 

A-LOAM(LOAM)根据多线激光雷达的性质，提取激光雷达的线特征和面特征，并以此为基础实现了一个高频率低精度的前端以及低频率高                                                         精度的后端。后面很多的slam框架都是类似，将tracking和mapping分为两个线程，借此完成了一个高精度的里程记和建图功能。 

LeGO-LOAM在LOAM的基础上进行了一些改进，如 1、利用车载激光雷达大多水平安装的特征，提取出地面点。 

2、使用聚类算法，使得前端特征更为干净

3、前端使用两步优化方法，减少运算负载，使其在嵌入式平台上也能运行

4、后端引入关键帧概念，同时加入了回环检测 

LIO-SAM在上述基础上的改进：

1、由于其支持手持设备，因此没有对地面点进行特殊处理 

2、紧耦合的lidar+IMU融合模块，使得其充分利用IMU的数据，对快速旋转等场景有着更好的鲁棒性 

3、融合GPS，使得全局地图可以在没有回环的情况下有着更好的全局一致性 

4、易于扩展的框架，方便将其他传感器融合进来

总体来说，这三种框架随着时间顺序都是在前有基础进行的改造。

算法中工程化技巧总结  

1、LeGO-LOAM前端，对地面点的提取，利用地面点的一些性质对roll，pitch以及z进行一些约束和优化 

2、通过BFS算法对前端特征进行过滤，使得更干净的特征留存了下来 

3、后端滑动窗口的引入，使得构建局部地图更加方便，也更加方便实现纯里程记功能

4、对GPS的融合的谨慎使用方式，使得既引入了全局观测，又不会对当前里程计的平滑性产生负面影响 

展望 

多传感器融合的目的是取长补短，比如在以激光雷达为主的融合方案中，需要明确激光雷达有什么缺陷，以此确定使用什么传感器进行融合 。

1、激光雷达需要运动补偿，需要短期内可靠的运动观测源，IMU以及轮速计就可以被充分利用 

2、激光雷达匹配本质上是一个优化问题，需要提供一个很好的初始值，也是需要可靠的短期运动观测源，紧耦合的IMU融合或者轮速计也是非常好的处理方式 

3、激光雷达频率不高，为了提高频率，需要高频的其他传感器以获得高频输出，此时IMU和轮速计又可以成为备选 

4、里程记会有累计漂移的问题，全局观测是解决里程记该问题的非常好的方式，GPS作为常见的全局定位传感器，提供了修正累计漂移的功能 

多传感器融合未来发展趋势 

对传感器的正确使用会使得整个系统的鲁棒性越强，因此，在未来一段时间内，会尝试将更多新的传感器应用到SLAM中来，比如固态雷达-LIVOX激光雷达等，和现有传感器⼀起取长补短，来提升整体性能的鲁棒性。 同时基于已有的传感器组合，也会探索更多紧耦合的融合方式，使得传感器之间的融合更有效率， 做出精度更高的SLAM系统。 

企业对多传感器融合或者slam要求

1、对常见传感器特点，优缺点要足够了解，掌握在不同需求不同场景下的多传感器融合方案 

2、扎实的slam基本功，对常用的视觉/激光slam理论及常见的开源框架熟练掌握 

3、扎实的编程能力，对常见的数据结构和算法有足够的了解 

4、相关度较高的项目/实习/工作经历 

实践：使用LIO-SAM建好的地图和原数据包实现⼀个基于已知地图的定位

思路 （1）可以参考LOAM建图过程，保存边缘特征和面特征地图，然后使用类似的配准方式实现定位 

（2）保存完成的点云地图，使用ICP或NDT实现当前帧到地图的配准 

（3）初始化是⼀个棘手的问题，为简化问题可以直接初始化到地图的原点



### VINS-Mono框架

### ORB-SLAM框架

### LVI-SAM框架

### R3LIVE框架

香港大学火星实验室的r3live框架，是一个实时鲁棒的带有RGB颜色信息的激光雷达-惯性-视觉紧耦合系统。

github如下：

https://github.com/hku-mars/r3live.git

安装依赖：

```
sudo apt-get install ros-XXX-cv-bridge ros-XXX-tf ros-XXX-message-filters ros-XXX-image-transport ros-XXX-image-transport*
```

安装livox雷达：https://github.com/Livox-SDK/livox_ros_driver.git

```
sudo apt-get install libcgal-dev pcl-tools
```

查看opencv版本：pkg-config --modversion opencv

安装**3.3.1**, **3.4.16**, **4.2.1** and **4.5.3**. 都可

编译：

```
cd ~/catkin_ws/src
git clone https://github.com/hku-mars/r3live.git
cd ../
catkin_make
source ~/catkin_ws/devel/setup.bash
```

报错：

```
/usr/include/flann/ext/lz4.h:196:57: error: conflicting declaration ‘typedef struct LZ4_stream_t LZ4_stream_t’
 typedef struct { long long table[LZ4_STREAMSIZE_U64]; } LZ4_stream_t;
```

解决办法：

```
sudo mv /usr/include/flann/ext/lz4.h /usr/include/flann/ext/lz4.h.bak
sudo mv /usr/include/flann/ext/lz4hc.h /usr/include/flann/ext/lz4.h.bak
sudo ln -s /usr/include/lz4.h /usr/include/flann/ext/lz4.h
sudo ln -s /usr/include/lz4hc.h /usr/include/flann/ext/lz4hc.h
```

运行：

```
roslaunch r3live r3live_bag.launch
rosbag play YOUR_DOWNLOADED.bag
```

效果如下：

![科研分享记录-39](.\img\科研分享记录-39.png)

保存pcd如下：

![科研分享记录-40](.\img\科研分享记录-40.png)

重建为ply格式

```
roslaunch r3live r3live_reconstruct_mesh.launch
```

安装meshlab

```
sudo apt-get install pcl-tools meshlab
cd ${HOME}/r3live_output
meshlab textured_mesh.ply
```

显示如下：

![科研分享记录-41](.\img\科研分享记录-41.png)

### Livox-LOAM框架

#### 框架介绍

#### 运行演示

1. 首先要安装 ROS

Ubuntu 64-bit 16.04 or 18.04. ROS Kinetic or Melodic

 

1. 安装Ceres Solver

http://ceres-solver.org/installation.html

官网有安装的教程

1. 安装PCL,使用ROS自带即可。

 

1. 编译Livox-loam，即下载源码建立ROS工作空间并编译。

Github下载源码： https://github.com/hku-mars/loam_livox.git

```
cd ~/catkin_ws/src
git clone https://github.com/hku-mars/loam_livox.git
cd ../
catkin_make
source ~/catkin_ws/devel/setup.bash
```

5.播放bag包运行（github里面有提供bag数据集）

```
roslaunch loam_livox rosbag.launch
rosbag play YOUR_DOWNLOADED.bag
```

效果如下图：

![科研分享记录-26](.\img\科研分享记录-26.jpg)

或者从livox 的github下载livox_mapping，用这个建图也可以

https://github.com/Livox-SDK/livox_mapping.git

#### Livox-loam跑livox-Mid70雷达

1. 首先安装好雷达在平台上，供电准备好，雷达网口接终端，雷达本机ip没说明，终端ip要配置成192.168.1.50，子网掩码255.255.255.0即可。
2. 雷达SDK下载和安装，GitHub下载最新源码。（注意：必须先编译雷达驱动后才能解压建图包再次进行编译）

```
git clone https://github.com/Livox-SDK/Livox-SDK.git
cd Livox-SDK
cd build && cmake ..
make
sudo make install
```

1. 安装ros驱动

新建文件夹ws_livox/src

```
git clone https://github.com/Livox-SDK/livox_ros_driver.git ws_livox/src
cd ws_livox
catkin_make
source ./devel/setup.sh
```

1. 下载并安装编译livox_mapping（这其实跟livox-loam是一样的，香港大学火星实验室有跟大疆合作），可放在同个工作空间编译。

环境安装跟前面livox-loam一致，

```
cd ~/catkin_ws/src
git clone https://github.com/Livox-SDK/livox_mapping.git
cd ..
catkin_make
source ~/catkin_ws/devel/setup.bash
```

1. 运行launch文件

```
roslaunch livox_mapping mapping_mid.launch
roslaunch livox_ros_driver livox_lidar.launch
```

6.效果如下：

![科研分享记录-36](.\img\科研分享记录-36.png)

![科研分享记录-37](.\img\科研分享记录-37.png)

#### 工程代码解析



### FAST-LIO2框架

#### 跑官方数据集

FAST-LIO2是香港大学火星实验室推出的高效固态LiDAR 惯性里程计框架，使用紧耦合的迭代扩展卡尔曼滤波器将固态LiDAR 特征与 IMU 数据融合，以允许在发生退化的快速运动或杂乱环境中进行鲁棒导航。相较第一代版本FAST-LIO，第二代使用ikd-Tree进行点云增量索引进行特征匹配，提高了计算速度，实现100Hz的速率，并且支持更多的LiDAR。

论文地址：https://arxiv.org/abs/2010.08196

在ubuntu18.04和ros-melodic环境下测试成功：

1. 下载livox_ros_driver包：

```
git clone https://github.com/Livox-SDK/livox_ros_driver.git
```

并需要先安装Livox-SDK，然后放在一个工作空间跟FAST-LIO包一起编译。

1. 下载FAST-LIO包并编译，这里要进行版本更新的原因是要下载ikd-Tree源文件，如果没下载也可以自己下载。

```
cd ~/$A_ROS_DIR$/src
git clone https://github.com/hku-mars/FAST_LIO.git
cd FAST_LIO
git submodule update --init
cd ../..
catkin_make
source devel/setup.bash
```

1. 运行，跑官方数据集

```
roslaunch fast_lio mapping_avia.launch
rosbag play YOUR_DOWNLOADED.bag
```

1. 效果如下：

![科研分享记录-42](.\img\科研分享记录-42.png)

![科研分享记录-43](.\img\科研分享记录-43.png)

1. 保存pcd

avia.yaml中pcd_save_en参数默认是true，运行后pcd会保存在pcd文件中。

```
common:
    lid_topic:  "/livox/lidar"
    imu_topic:  "/livox/imu"
    time_sync_en: false         # ONLY turn on when external time synchronization is really not possible

preprocess:
    lidar_type: 1                # 1 for Livox serials LiDAR, 2 for Velodyne LiDAR, 3 for ouster LiDAR, 
    scan_line: 6
    blind: 4

mapping:
    acc_cov: 0.1
    gyr_cov: 0.1
    b_acc_cov: 0.0001
    b_gyr_cov: 0.0001
    fov_degree:    90
    det_range:     450.0
    extrinsic_est_en:  false      # true: enable the online estimation of IMU-LiDAR extrinsic
    extrinsic_T: [ 0.04165, 0.02326, -0.0284 ]
    extrinsic_R: [ 1, 0, 0,
                   0, 1, 0,
                   0, 0, 1]

publish:
    path_en:  false
    scan_publish_en:  true       # false: close all the point cloud output
    dense_publish_en: true       # false: low down the points number in a global-frame point clouds scan.
    scan_bodyframe_pub_en: true  # true: output the point cloud scans in IMU-body-frame

pcd_save:
    pcd_save_en: true
    interval: -1                 # how many LiDAR frames saved in each pcd file; 
                                 # -1 : all frames will be saved in ONE pcd file, may lead to memory crash when having too much frames.
```

1. CC显示如下：

![科研分享记录-44](.\img\科研分享记录-44.jpg)

#### MID-70和SBG-Ellipse-N AHRS传感器室外建图（返校做）

1. 雷达驱动包Livox ROS Driver需要运行livox_lidar_msg.launch，发出livox自定义雷达数据格式。
2. 旋转外参已确认是单位阵，即IMU和固态雷达刚好坐标重合。
3. set parameter `time_sync_en` to ture 
4. 考虑需不需要时间同步和外参标定，因为Avia和Horizon有内置IMU，已经做了内部时间同步和外参标定。
5. 重定位包的测试。



### COVINS框架

#### 运行演示

1. 首先要安装 ROS

Ubuntu 64-bit 18.04 or 20.04. ROS Melodic or Noetic

1. 安装依赖

```
sudo apt-get install doxygen libsuitesparse-dev libyaml-cpp-dev libvtk6-dev python3-wstool libomp-dev libglew-dev
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu `lsb_release -sc` main" > /etc/apt/sources.list.d/ros-latest.list'
wget http://packages.ros.org/ros.key -O - | sudo apt-key add -
sudo apt-get update
sudo apt-get install python3-catkin-tools
```

安装不上用pip3

```
pip3 install catkin-tools catkin-tools-python
```

在home下执行：

```
mkdir -p ws/covins_ws/src
cd ~/ws/covins_ws
catkin init
```

ROS Melodic情况下：

```
catkin config --extend /opt/ros/melodic/
catkin config --merge-devel
catkin config --cmake-args -DCMAKE_BUILD_TYPE=RelWithDebInfo
```

下载源码编译：

```
cd ~/ws/covins_ws/src
git clone https://github.com/VIS4ROB-lab/covins.git
cd ~/ws/covins_ws
sudo chmod +x src/covins/install_file.sh
sudo ./src/covins/install_file.sh 8
```

出现这些错误：

```
Traceback (most recent call last):
  File "/usr/bin/wstool", line 62, in <module>
    sys.exit(wstool.wstool_cli.wstool_main(sys.argv))
```

则su进入root后执行

```
/home/qjs/ws/covins_ws/src/covins/install_file.sh 4
```



 这里好像会和evo的安装冲突



最好进行科学上网，因为软件依赖包都是在github下载的，如果网速不行，则根据报错情况自行下载无法下载的包。

源码包1个，依赖包一共需要17个，总共如下18个，同时也要改名字如下：

![科研分享记录-38](.\img\科研分享记录-38.png)

然后skip跳过这些依赖包下载即可。

类似如下报错：

```
CMake Error at eigen_src-stamp/download-eigen_src.cmake:157 (message):
  Each download failed!
  error: downloading 'https://gitlab.com/libeigen/eigen/-/archive/3.3.4/eigen-3.3.4.tar.bz2' failed
```

则自行去github下载（下载了有时名字会不对，还需要根据工程的github下载链接上的文件改名字，要改成一模一样，不然会识别不了）

放在（都是放在某个依赖包的src-prefix文件夹的src文件夹中，如eigen的是eigen_src-prefix）

```
/home/qjs/ws/covins_ws/build/eigen_catkin/eigen_src-prefix/src
```

如果还是搜索不了，如yaml，则在yaml_cpp_catkin包的cmakelists注释以下语句，自己创建yaml_cpp_src文件夹，并下载源码（其实全部都可以这样做，根据catkin包里面的cmakelists代码指示的源码文件名，自己下载自己建立）

```
ExternalProject_Add(yaml_cpp_src
#  GIT_REPOSITORY  https://github.com/jbeder/yaml-cpp
#  GIT_TAG ${YAML_CPP_TAG}
#  UPDATE_COMMAND ""
#  PATCH_COMMAND patch -p0 < ${CMAKE_SOURCE_DIR}/extra_version.patch
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CATKIN_DEVEL_PREFIX} -DBUILD_SHARED_LIBS=ON
)
```



出现报错:

```
opencv_contrib/modules/xfeatures2d/src/boostdesc.cpp:653:20: fatal error: boostdesc_bgm.i: 没有那个文件或目录
```

链接: https://pan.baidu.com/s/1PZguH-f9lNo6EerJMXpqEQ 密码: cnk0

下载文件，将文件夹中的所有文件放在

```
/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_contrib_src/modules/xfeatures2d/src
```



出现报错：

```
/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_src/modules/stitching/include/opencv2/stitching/detail/matchers.hpp:52:12: fatal error: opencv2/xfeatures2d/cuda.hpp: No such file or directory
 #  include "opencv2/xfeatures2d/cuda.hpp"
```

把对应文件#include调用的这句改成绝对路径，例如 #include "opencv2/xfeatures2d/cuda.hpp"改成

```
#include"/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_contrib_src/modules/xfeatures2d/include/opencv2/xfeatures2d/cuda.hpp"
```

要改的文件有：

```
/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_src/modules/stitching/src/precomp.hpp
/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_src/modules/stitching/include/opencv2/stitching/detail/matchers.hpp
```



出现报错

```
/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_src/modules/stitching/src/matchers.cpp:52:10: fatal error: opencv2/xfeatures2d.hpp: No such file or directory
 #include "opencv2/xfeatures2d.hpp"
```

\#include "opencv2/xfeatures2d.hpp"改成

```
#include"/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_contrib_src/modules/xfeatures2d/include/opencv2/xfeatures2d.hpp"
```

要改的文件有：

```
/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_src/modules/stitching/src/matchers.cpp
```



后面出现找不到文件，都是类似操作：

还有一个要改的文件：

```
/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_contrib_src/modules/xfeatures2d/include/opencv2/xfeatures2d.hpp
```

\#include"opencv2/xfeatures2d/nonfree.hpp"

改成：

```
/home/qjs/ws/covins_ws/build/opencv3_catkin/opencv3_contrib_src/modules/xfeatures2d/include/opencv2/xfeatures2d.hpp
```

出现错误：

```
CMake Error at cmake_install.cmake:156 (file):
  file INSTALL cannot copy file
  "/home/qjs/ws/covins_ws/build/gflags_catkin/gflags_src-prefix/src/gflags_src/gflags-registry-entry"
  to "/home/qjs/.cmake/packages/gflags/ee3444847972f99a487c8134b15c8285".
```

可能需要权限，则用su进入root执行

```
./src/covins/install_file.sh 8
```

 后面发现以上方法太麻烦，容易出差。改为修改科学上网终端代理。

https://github.com/Dreamacro/clash/releases

下载clash-linux-amd64-v1.8.0.gz

新建文件夹为clash，clash-linux-amd64-v1.8.0.gz解压到该文件夹并改名为clash

在终端 `cd` 到 Clash 二进制文件所在的目录，执行 `wget -O config.yaml "https://api.ikuuu.science/link/dPG6X5qdeyHx6IYZ?clash=3"` 下载 Clash 配置文件

执行 `./clash -d .` 即可启动 Clash，同时启动 HTTP 代理和 Socks5 代理。

如提示权限不足，请执行 `chmod +x clash`

打开系统设置，选择网络，点击网络代理右边的 ⚙ 按钮，选择手动，填写 HTTP 和 HTTPS 代理为 `127.0.0.1:7890`，填写 Socks 主机为 `127.0.0.1:7891`，即可启用系统代理。

## 其他

### 生成目录结构树：

windows程序

```shell
tree /f > list.txt
```

ubuntu：

安装：

```
sudo apt-get -y install tree
```

当前文件下输入tree

### EVO精度评估工具

查询ubuntu软件版本

```
python -V //查看python2版本
python3 -V //查看python3版本
nvcc -V //查看显卡版本
pkg-config --modversion opencv  //查看opencv版本
```

python2切换成python3：设置优先级，数字越大越优先，将python3设置成150，优先级就提高了

```
sudo update-alternatives --install /usr/bin/python python /usr/bin/python2 100
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 150
```

查看当前版本：

```
python --version
```

安装pip3

```
sudo apt install python3-pip
```

```
pip3 -V  //如果报错则安装下pip3
```

安装evo

```
sudo pip3 install evo --upgrade --no-binary evo
```

自带test测试

```
git clone https://github.com/MichaelGrupp/evo.git
cd evo/test/data 
evo_traj kitti KITTI_00_ORB.txt KITTI_00_SPTAM.txt --ref=KITTI_00_gt.txt -p --plot_mode=xyz
```

### GTSAM Matlab toolbox

先校内网用学生邮箱注册matlab，并安装matlab。

修改gtsam cmakelists文件：

```
option(GTSAM_INSTALL_MATLAB_TOOLBOX      "Enable/Disable installation of matlab toolbox"  OFF)
```

改成

```
option(GTSAM_INSTALL_MATLAB_TOOLBOX      "Enable/Disable installation of matlab toolbox"  ON)
```

然后进行编译

打开matlab测试一下GTSAM toolbox 的测试demo（test_gtsam），一般安装在/usr/local





