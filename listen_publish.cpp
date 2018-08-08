#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/MultiEchoLaserScan.h"

class VelodyneScan_Convert_LaserScan
{
public:

VelodyneScan_Convert_LaserScan()
{   
    ros::sensor_msgs::LaserScan catographer_1 = new ros::sensor_msgs::LaserScan();
    ros::Subscriber subscriber_1 = n.subscribe("/scan",1000,chatterCallback);
    ros::Publisher pub_1 = n.advertise<sensor_msgs::LaserScan>("/horizontal_laser_2d", 1000);
    pub_1.publish();
}

void chatterCallback(const velodyne_msgs::VelodyneScan::ConstPtr& msg)
{
   
  //ROS_INFO("%d %d %s", msg->header.seq, msg->header.stamp.nsec, msg->header.frame_id.c_str());
  
  catographer_1->header.stamp.sec=msg->header.stamp.sec;
  catographer_1->header.stamp.nsec=msg->header.stamp.nsec;
  catographer_1->header.seq = 0;
  catographer_1->header.frame_id = Null;
  //int* range_begin=&(catographer_1->ranges[0]);
  // maybe:      &((catographer_1->ranges)[0])

  //I guess ROS is 32 bit.
//  int* intensity_begin=&(catographer_1->intensities[0]);
    for(int i=0;i<75;i++)
    {
        for(int j=0;j<11;j++)
        {
        int flag=0;
//        int* p=&(msg->packets[i].data[0]);
            while(flag<2)
            {
                if(flag==0)
                {
//                    catographer_1->ranges[i*24+j]=float(msg->packets[i].data[100*j+6]);
                    catographer_1->intensities[i*24+j*2+flag]=float(msg->packets[i].data[6+100*j]);
                    catographer_1->ranges[i*24+j*2+flag]=(float(msg->packets[i].data[6+100*j+2]))<<24;
                    catographer_1->ranges[i*24+j*2+flag]+=float(msg->packets[i].data[6+100*j+1])<<16;
                }
                else if(flag==1)
                {
                    catographer_1->intensities[i*24+j*2+flag]=float(msg->packets[i].data[54+100*j]);
                    catographer_1->ranges[i*24*j*2+flag]=(float(msg->packets[i].data[54+100*j+2]))<<24;
                    catographer_1->ranges[i*24+j*2+flag]+=float(msg->packets[i].data[54+100*j+1])<<16;
                }
            }
        }
    }
//  for(i=0;msg->packets[i].stamp.nsec<msg->header.stamp.nsec;i++);
// ROS_INFO("%d %d %d", i,msg->packets[0].stamp.nsec,msg->packets[i].stamp.nsec);

}
private:
    ros::NodeHandle n;
    ros::Publisher pub_1;
    ros::Subscriber subscriber_1;
    ros::sensor_msgs::LaserScan catographer_1;
}

int main(int argc, char const *argv[])
{
    ros::init(argc,argv,"listener");
    VelodyneScan_Convert_LaserScan fire;
    ros::spin(); 
    return 0;
}


/*
class convert_LaserScan_to_MultiEchoLaserScan
 {
 public:
 convert_LaserScan_to_MultiEchoLaserScan()
  {
 //Topic you want to publish
  pub_ = n_.advertise<sensor_msgs::LaserScan>("/horizontal_laser_2d", 1000);

//Topic you want to subscribe
  sub_ = n_.subscribe("/scan", 1000, &convert_LaserScan_to_MultiEchoLaserScan::callback, this);
 }


 void callback(const sensor_msgs::LaserScan::ConstPtr& input)
{
sensor_msgs::LaserScan output;

sensor_msgs::MultiEchoLaserScan muls;
for (int i = 0; i < input->ranges.size(); ++i) {
    const float &range = input->ranges[i];
    ROS_INFO("ranges: [%f]",range);
    sensor_msgs::LaserEcho echo;
    echo.echoes.push_back(range);
    muls.ranges.push_back(echo);

}

pub_.publish(output);
 }

  private:
  ros::NodeHandle n_;
  ros::Publisher pub_;
  ros::Subscriber sub_;

  };//End of class convert_LaserScan_to_MultiEchoLaserScan


  int main(int argc, char **argv)
 {  
  //Initiate ROS
  ros::init(argc, argv, "convert_LaserScan_to_MultiEchoLaserScan");

  //Create an object of class SubscribeAndPublish that will take care of everything
  convert_LaserScan_to_MultiEchoLaserScan SAPObject;

  ros::spin();



  return 0;
}

*/