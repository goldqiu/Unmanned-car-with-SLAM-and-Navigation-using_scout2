#  grid_2d_map(暂定)

## A grid mapping package

## News

**[Sep 30, 2022]**  **Release of the Vesion 1.0**

## 1. Introduction

![Map_Conversion-1](.\pic\Map_Conversion-1.jpg)

优化方向：
Map_Conversion承担的是Mapping、Localization到planning的中间件功能
完善各类型地图，包括2.5D高程地图、costmap。
全局定位信息的接口、输出数据的接口（为基于学习的方法提供数据）
代码的规范化：封装接口（数据输入输出）、地图参数外置（yaml）、地图生成算法的多态实现、Google log等。
大场景、长时间的测试
多人员开发：代码管理、使用文档等

![Map_Conversion-2](.\pic\Map_Conversion-2.jpg)

![Map_Conversion-3](.\pic\Map_Conversion-3.gif)

![Map_Conversion-2](.\pic\Map_Conversion-2.gif)

![Map_Conversion-3](.\pic\Map_Conversion-3.gif)

## 2. What can grid_2d_map(暂定) do?

目前实现功能：
三维点云到二维栅格地图的实时输出
三维点云到三维栅格地图的实时输出
局部规划所需二维栅格地图、里程计的更新（基础）



## 3. Prerequisites



## 4. Build grid_2d_map(暂定) on ROS:



## 5. Run our examples



## 6. Sample and run your own data



## 7. Report our problems and bugs



## 8. Acknowledgments



## License



PS：

软件readme文档暂定这样。

V1.0：gridmap实时输出二维、三维栅格功能完成，后续需要增加2.5D，且将代码规范化，方便更改参数，同时更换包名。

