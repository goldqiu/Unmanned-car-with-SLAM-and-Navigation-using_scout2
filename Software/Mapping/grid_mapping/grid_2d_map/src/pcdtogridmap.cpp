#include <ros/ros.h>
#include <ros/console.h>

#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/GetMap.h>
#include <sensor_msgs/PointCloud2.h>

#include <pcl/io/pcd_io.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>  //直通滤波器头文件
#include <pcl/filters/voxel_grid.h>  //体素滤波器头文件
#include <pcl/filters/statistical_outlier_removal.h>   //统计滤波器头文件
#include <pcl/filters/conditional_removal.h>    //条件滤波器头文件
#include <pcl/filters/radius_outlier_removal.h>   //半径滤波器头文件
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

#include <iostream>
#include <Eigen/Eigen>
#include <mutex>

using namespace std;
using namespace Eigen;
std::string file_directory;
std::string file_name;
std::string pcd_file;

std::string map_topic_name;
const std::string pcd_format = ".pcd";
nav_msgs::OccupancyGrid map_topic_msg;

double thre_z_min = 0.3;
double thre_z_max = 2.0;
int flag_pass_through = 0;

double grid_x = 0.1;
double grid_y = 0.1;
double grid_z = 0.1;

double map_resolution = 0.05;
double thre_radius = 0.1;
double z_adjust;

//3d
double  _resolution =  0.15 ;
double  _cloud_margin = 0.3;
double _inv_resolution;
double _x_size = 120.0; //要生成的地图大小 20*20*3 单位为m
double _y_size = 120.0;
double _z_size = 7.0 ;

double gl_xl, gl_yl, gl_zl;
double gl_xu, gl_yu, gl_zu;
int GLX_SIZE;
int GLY_SIZE;
int GLZ_SIZE;
int GLYZ_SIZE;
int GLXYZ_SIZE;
int tmp_id_x, tmp_id_y, tmp_id_z;
uint8_t * data;
Eigen::Vector3d _map_lower,_map_upper;
struct GridNode
{     
   int id;        // 1--> open set, -1 --> closed set
   Eigen::Vector3d coord;
   Eigen::Vector3i index;
   
   uint8_t * occupancy; 

   GridNode(Eigen::Vector3i _index)
   {  
      id = 0;
      index = _index;
   }

   GridNode(Eigen::Vector3i _index, Eigen::Vector3d _coord)
   {  
      id = 0;
      index = _index;
      coord = _coord;
   }

   GridNode(){};
   
   ~GridNode(){};
};

typedef GridNode* GridNodePtr;
GridNodePtr *** GridNodeMap;

std::mutex mBuf;
std::queue<sensor_msgs::PointCloud2ConstPtr> laserCloudSurfBuf;
pcl::PointCloud<pcl::PointXYZ>::Ptr laserCloudSharp(new pcl::PointCloud<pcl::PointXYZ>());
pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_after_PassThrough(new pcl::PointCloud<pcl::PointXYZ>());

void PassThroughFilter(pcl::PointCloud<pcl::PointXYZ>::Ptr& pcd_cloud,pcl::PointCloud<pcl::PointXYZ>& cloud_after_PassThrough,double &thre_low, const double &thre_high, const bool &flag_in);
//void RadiusOutlierFilter(const pcl::PointCloud<pcl::PointXYZ>::Ptr& pcd_cloud, const double &radius, const int &thre_count);
void SetMapTopicMsg(const pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, nav_msgs::OccupancyGrid& msg);
void laserCloudsurroundHandler(const sensor_msgs::PointCloud2ConstPtr &laserCloudSurfLast2);
Eigen::Vector3i coord2gridIndex(const Eigen::Vector3d pt); 
bool setObs(const double coord_x, const double coord_y, const double coord_z);
Eigen::Vector3d gridIndex2coord(const Eigen::Vector3i index);
void initGridMap(double _resolution, Vector3d global_xyz_l, Vector3d global_xyz_u);

int main(int argc, char** argv)
{
   double max_z=-10000;
   double min_z=100000;
   pcl::PointXYZ pt, pt_inf;
   pcl::PointCloud<pcl::PointXYZ> cloud_inf;

   ros::init(argc, argv, "pcl_filters");
   ros::NodeHandle nh;
   ros::NodeHandle private_nh("~");
   ros::Rate loop_rate(100.0);
   //ros::Subscriber subLaserCloudSurfLast = nh.subscribe<sensor_msgs::PointCloud2>("/laser_cloud_map", 100, laserCloudsurroundHandler);

   ros::Subscriber subLaserCloudSurfLast = nh.subscribe<sensor_msgs::PointCloud2>("/velodyne_cloud_registered", 100, laserCloudsurroundHandler);
   
   ros::Publisher map_topic_pub = nh.advertise<nav_msgs::OccupancyGrid>("/grid_map_realtime", 1);
   ros::Publisher pcl_pub = nh.advertise<sensor_msgs::PointCloud2> ("pcl_output", 1);
   ros::Publisher _vis_inf_map_pub  = nh.advertise<sensor_msgs::PointCloud2>("inflation_map", 10);

   sensor_msgs::PointCloud2 output;
   sensor_msgs::PointCloud2 map_inflation;

   private_nh.param("file_directory", file_directory, std::string("/home/qjs/"));
   ROS_INFO("*** file_directory = %s ***\n", file_directory.c_str());


   private_nh.param("file_name", file_name, std::string("GlobalMap"));
   ROS_INFO("*** file_name = %s ***\n", file_name.c_str());

   pcd_file = file_directory + file_name + pcd_format;
   ROS_INFO("*** pcd_file = %s ***\n", pcd_file.c_str());

   private_nh.param("z_adjust", z_adjust);

   private_nh.param("thre_z_min", thre_z_min, 0.8);
   private_nh.param("thre_z_max", thre_z_max, 5.02967);
   private_nh.param("flag_pass_through", flag_pass_through, 0);
   private_nh.param("grid_x", grid_x, 0.1);
   private_nh.param("grid_y", grid_y, 0.1);
   private_nh.param("grid_z", grid_z, 0.1);
   private_nh.param("thre_radius", thre_radius, 0.5);
   private_nh.param("map_resolution", map_resolution, 0.05);
   //private_nh.param("map_topic_name", map_topic_name, std::string("grid_map_realtime"));
   //3d grid
	 _map_lower << -_y_size/2.0, -_y_size/2.0, 0.0;
	 _map_upper << +_x_size/2.0, +_y_size/2.0, _z_size;
	
	 _inv_resolution = 1.0 / _resolution;
	 int _max_x_id = (int)(_x_size * _inv_resolution);
	 int _max_y_id = (int)(_y_size * _inv_resolution);
	 int _max_z_id = (int)(_z_size * _inv_resolution);
							   
   GLX_SIZE = _max_x_id;
   GLY_SIZE = _max_y_id;
   GLZ_SIZE = _max_z_id;
   GLYZ_SIZE  = GLY_SIZE * GLZ_SIZE;
   GLXYZ_SIZE = GLX_SIZE * GLYZ_SIZE;
   initGridMap(_resolution, _map_lower, _map_upper);//要初始化地图 

   while(ros::ok())
   {
     ros::spinOnce();
     if (!laserCloudSurfBuf.empty())
     {
        mBuf.lock();
        laserCloudSharp->clear();
        pcl::fromROSMsg(*laserCloudSurfBuf.front(), *laserCloudSharp);
        laserCloudSurfBuf.pop();
        mBuf.unlock();
        std::cout << "初始点云数据点数：" << laserCloudSharp->points.size() << std::endl;
        double max_z=-10000;
        double min_z=100000;
          for(int i = 0; i < laserCloudSharp->points.size() - 1; i++){
              laserCloudSharp->points[i].z = laserCloudSharp->points[i].z +0.7;
        }
        for(int i = 0; i < laserCloudSharp->points.size() - 1; i++){
          if(laserCloudSharp->points[i].z>max_z){
            max_z=laserCloudSharp->points[i].z;
          }
          if(laserCloudSharp->points[i].z<min_z){
            min_z=laserCloudSharp->points[i].z;
          }
        }
        std::cout<<"orig max_z="<<max_z<<",min_z="<<min_z<<std::endl;
        //方法一：直通滤波器对点云进行处理
        pcl::PassThrough<pcl::PointXYZ> passthrough;
        passthrough.setInputCloud(laserCloudSharp);//输入点云
        passthrough.setFilterFieldName("z");//对z轴进行操作
        passthrough.setFilterLimits(min_z+0.27, max_z);//设置直通滤波器操作范围
        passthrough.setFilterLimitsNegative(false);//true表示保留范围外，false表示保留范围内
        passthrough.filter(*cloud_after_PassThrough);//执行滤波，过滤结果保存在 cloud_after_PassThrough
        std::cout << "直通滤波后点云数据点数：" << cloud_after_PassThrough->points.size() << std::endl;

        for(int i = 0; i < cloud_after_PassThrough->points.size() - 1; i++){
          if(cloud_after_PassThrough->points[i].z>max_z){
            max_z=cloud_after_PassThrough->points[i].z;
          }
          if(cloud_after_PassThrough->points[i].z<min_z){
            min_z=cloud_after_PassThrough->points[i].z;
          }
        }
    
       int inf_step   = round(_cloud_margin * _inv_resolution); 
       //这里涉及膨胀地图的大小，resolution 为一个栅格大小，
			 //单位为m,设置为[0.15 0.30]之间，越大地图越稀疏，_inv_resolution,为resolution的倒数                                                 
       int inf_step_z = max(1, inf_step / 2);
       for (int idx = 0; idx < (int)cloud_after_PassThrough->points.size(); idx++)
       {    
          pt = cloud_after_PassThrough->points[idx];        
          for(int x = -inf_step ; x <= inf_step; x ++ )
        {
                  for(int y = -inf_step ; y <= inf_step; y ++ )
                  {
                      for(int z = -inf_step_z; z <= inf_step_z; z ++ )
                      {
                          double inf_x = pt.x + x * _resolution;
                          double inf_y = pt.y + y * _resolution;
                          double inf_z = pt.z + z * _resolution;

                          if(isnan(inf_x) || isnan(inf_y) || isnan(inf_z)) continue;

                          Vector3d vec_inf(inf_x, inf_y, inf_z);
                          Vector3i idx_inf = coord2gridIndex(vec_inf);     

                          // set in obstacle points
                          bool flag_newly_occupied = setObs(inf_x, inf_y, inf_z); 
                          
                          // rounding for visualizing the grid map
                          if(flag_newly_occupied) 
                          {
                              Vector3d coor_round = gridIndex2coord(idx_inf); 
                              pt_inf.x = coor_round(0);
                              pt_inf.y = coor_round(1);
                              pt_inf.z = coor_round(2);
                              cloud_inf.points.push_back(pt_inf);
                          }
                       }
                   }
        }
      }

        cloud_inf.width    = cloud_inf.points.size();
        cloud_inf.height   = 1;
        cloud_inf.is_dense = true;

        pcl::toROSMsg(cloud_inf, map_inflation);
        map_inflation.header.frame_id = "/camera_init";
        _vis_inf_map_pub.publish(map_inflation);
        std::cout<<"aft pass through filter: max_z="<<max_z<<",min_z="<<min_z<<std::endl;
        // PassThroughFilter(*laserCloudSharp,*cloud_after_PassThrough,thre_z_min, thre_z_max, false);
        pcl::toROSMsg(*cloud_after_PassThrough, output);
        output.header.frame_id = "camera_init";
        pcl_pub.publish(output);
        SetMapTopicMsg(cloud_after_PassThrough, map_topic_msg);
        map_topic_msg.header.frame_id = "camera_init";
        map_topic_pub.publish(map_topic_msg);

     }
     loop_rate.sleep();
   }
   return 0;
}

void laserCloudsurroundHandler(const sensor_msgs::PointCloud2ConstPtr &laserCloudSurfLast2)
{
    mBuf.lock();
    laserCloudSurfBuf.push(laserCloudSurfLast2);
    mBuf.unlock();
  //  std::cout<<"TEE"<<std::endl;
}

void PassThroughFilter(pcl::PointCloud<pcl::PointXYZ>::Ptr& pcd_cloud,pcl::PointCloud<pcl::PointXYZ>& cloud_after_PassThrough,double &thre_low, const double &thre_high, const bool &flag_in)
{
    /*方法一：直通滤波器对点云进行处理。*/
    pcl::PassThrough<pcl::PointXYZ> passthrough;
    passthrough.setInputCloud(pcd_cloud);//输入点云
    passthrough.setFilterFieldName("z");//对z轴进行操作
    passthrough.setFilterLimits(thre_low, thre_high);//设置直通滤波器操作范围
    passthrough.setFilterLimitsNegative(flag_in);//true表示保留范围外，false表示保留范围内
    passthrough.filter(cloud_after_PassThrough);//执行滤波，过滤结果保存在 cloud_after_PassThrough
    std::cout << "直通滤波后点云数据点数：" << cloud_after_PassThrough.points.size() << std::endl;
    double max_z=-10000;
    double min_z=100000;
    for(int i = 0; i < cloud_after_PassThrough.points.size() - 1; i++){
     if(cloud_after_PassThrough.points[i].z>max_z){
       max_z=cloud_after_PassThrough.points[i].z;
     }
     if(cloud_after_PassThrough.points[i].z<min_z){
       min_z=cloud_after_PassThrough.points[i].z;
     }
  }
  std::cout<<"aft pass through filter: max_z="<<max_z<<",min_z="<<min_z<<std::endl;
}

// void RadiusOutlierFilter(const pcl::PointCloud<pcl::PointXYZ>::Ptr& pcd_cloud0, const double &radius, const int &thre_count)
// {
//     pcl::RadiusOutlierRemoval<pcl::PointXYZ> radiusoutlier;  //创建滤波器

//     radiusoutlier.setInputCloud(pcd_cloud0);    //设置输入点云
//     radiusoutlier.setRadiusSearch(radius);     //设置半径为100的范围内找临近点
//     radiusoutlier.setMinNeighborsInRadius(thre_count); //设置查询点的邻域点集数小于2的删除

//     radiusoutlier.filter(*cloud_after_Radius);
//     std::cout << "半径滤波后点云数据点数：" << cloud_after_Radius->points.size() << std::endl;
// }

void SetMapTopicMsg(const pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, nav_msgs::OccupancyGrid& msg)
{
  msg.header.seq = 0;
  msg.header.stamp = ros::Time::now();
  msg.header.frame_id = "map";

  msg.info.map_load_time = ros::Time::now();
  msg.info.resolution = map_resolution;

  double x_min, x_max, y_min, y_max;
  double z_max_grey_rate = 0.05;
  double z_min_grey_rate = 0.95;
  double k_line = (z_max_grey_rate - z_min_grey_rate) / (thre_z_max - thre_z_min);
  double b_line = (thre_z_max * z_min_grey_rate - thre_z_min * z_max_grey_rate) / (thre_z_max - thre_z_min);

  if(cloud->points.empty())
  {
    ROS_WARN("pcd is empty!\n");

    return;
  }

  for(int i = 0; i < cloud->points.size() - 1; i++)
  {
    if(i == 0)
    {
      x_min = x_max = cloud->points[i].x;
      y_min = y_max = cloud->points[i].y;
    }

    double x = cloud->points[i].x;
    double y = cloud->points[i].y;

    if(x < x_min) x_min = x;
    if(x > x_max) x_max = x;

    if(y < y_min) y_min = y;
    if(y > y_max) y_max = y;
  }

  msg.info.origin.position.x = x_min;
  msg.info.origin.position.y = y_min;
  msg.info.origin.position.z = 0.0;
  msg.info.origin.orientation.x = 0.0;
  msg.info.origin.orientation.y = 0.0;
  msg.info.origin.orientation.z = 0.0;
  msg.info.origin.orientation.w = 1.0;

  msg.info.width = int((x_max - x_min) / map_resolution);
  msg.info.height = int((y_max - y_min) / map_resolution);

  msg.data.resize(msg.info.width * msg.info.height);
  msg.data.assign(msg.info.width * msg.info.height, 0);

  ROS_INFO("data size = %d\n", msg.data.size());

  for(int iter = 0; iter < cloud->points.size(); iter++)
  {
    int i = int((cloud->points[iter].x - x_min) / map_resolution);
    if(i < 0 || i >= msg.info.width) continue;

    int j = int((cloud->points[iter].y - y_min) / map_resolution);
    if(j < 0 || j >= msg.info.height - 1) continue;

    msg.data[i + j * msg.info.width] = 100;
//    msg.data[i + j * msg.info.width] = int(255 * (cloud->points[iter].z * k_line + b_line)) % 255;
  }
}

Eigen::Vector3i coord2gridIndex(const Eigen::Vector3d pt) 
{
    Eigen::Vector3i idx;
    idx <<  std::min( std::max( int( (pt(0) - gl_xl) * _inv_resolution), 0), GLX_SIZE - 1),
            std::min( std::max( int( (pt(1) - gl_yl) * _inv_resolution), 0), GLY_SIZE - 1),
            std::min( std::max( int( (pt(2) - gl_zl) * _inv_resolution), 0), GLZ_SIZE - 1);      		    
  
    return idx;
};

bool setObs(const double coord_x, const double coord_y, const double coord_z)
{   
    if( coord_x < gl_xl  || coord_y < gl_yl  || coord_z <  gl_zl || 
        coord_x >= gl_xu || coord_y >= gl_yu || coord_z >= gl_zu )
        return false;

    tmp_id_x = static_cast<int>( (coord_x - gl_xl) * _inv_resolution);
    tmp_id_y = static_cast<int>( (coord_y - gl_yl) * _inv_resolution);
    tmp_id_z = static_cast<int>( (coord_z - gl_zl) * _inv_resolution);      

    if(data[tmp_id_x * GLYZ_SIZE + tmp_id_y * GLZ_SIZE + tmp_id_z] == 0){
        data[tmp_id_x * GLYZ_SIZE + tmp_id_y * GLZ_SIZE + tmp_id_z] = 1;
        return true;
    }
    else{
        return false;
    }
}

Eigen::Vector3d gridIndex2coord(const Eigen::Vector3i index) 
{
    Eigen::Vector3d pt;

    pt(0) = ((double)index(0) + 0.5) * _resolution + gl_xl;
    pt(1) = ((double)index(1) + 0.5) * _resolution + gl_yl;
    pt(2) = ((double)index(2) + 0.5) * _resolution + gl_zl;

    return pt;
};

void initGridMap(double _resolution, Vector3d global_xyz_l, Vector3d global_xyz_u)
{   
    gl_xl = global_xyz_l(0);
    gl_yl = global_xyz_l(1);
    gl_zl = global_xyz_l(2);

    gl_xu = global_xyz_u(0);
    gl_yu = global_xyz_u(1);
    gl_zu = global_xyz_u(2);

    double resolution = _resolution;
    double inv_resolution = 1.0 / _resolution;    

    data    = new uint8_t[GLXYZ_SIZE];

    memset(data, 0, GLXYZ_SIZE * sizeof(uint8_t));
    
    GridNodeMap = new GridNodePtr ** [GLX_SIZE];
    for(int i = 0; i < GLX_SIZE; i++)
    {
        GridNodeMap[i] = new GridNodePtr * [GLY_SIZE];
        for(int j = 0; j < GLY_SIZE; j++)
        {
            GridNodeMap[i][j] = new GridNodePtr [GLZ_SIZE];
            for( int k = 0; k < GLZ_SIZE;k++)
            {
                Vector3i tmpIdx(i,j,k);
                Vector3d pos = gridIndex2coord(tmpIdx);
                GridNodeMap[i][j][k] = new GridNode(tmpIdx, pos);
                GridNodeMap[i][j][k]->occupancy = & data[i * GLYZ_SIZE + j * GLZ_SIZE + k];
            }
        }
    }
}
