#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/LaserScan.h"
#include "velodyne_msgs/VelodyneScan.h"
#include "velodyne_msgs/VelodynePacket.h"
//#include "sensor_msgs/MultiEchoLaserScan.h"

class VelodyneScan_Convert_LaserScan
{
public:

VelodyneScan_Convert_LaserScan()
{   
    ros::Subscriber subscriber_1 = n.subscribe("/scan",1000,chatterCallback);
    ros::Publisher publisher_1 = n.advertise<sensor_msgs::LaserScan>("/new_LaserScan_2d", 1000);
    publisher_1.publish(catographer_1);
}

void chatterCallback(const velodyne_msgs::VelodyneScan::ConstPtr& msg)
{
   
  //ROS_INFO("%d %d %s", msg->header.seq, msg->header.stamp.nsec, msg->header.frame_id.c_str());
  
  catographer_1->header.stamp.sec=msg->header.stamp.sec;
  catographer_1->header.stamp.nsec=msg->header.stamp.nsec;
  catographer_1->header.seq = 0;
  catographer_1->header.frame_id = Null;
   //frame_id is not defined! 
    
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
    ros::Publisher publisher_1;
    ros::Subscriber subscriber_1;
    ros::sensor_msgs::LaserScan catographer_1;
}

int main(int argc, char const *argv[])
{
    ros::init(argc,argv,"listener_publisher");
    VelodyneScan_Convert_LaserScan fire;
    ros::spin(); 
    return 0;
}


