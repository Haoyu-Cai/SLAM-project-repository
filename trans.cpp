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
    subscriber_1 = n.subscribe("/velodyne_packets",1000,&VelodyneScan_Convert_LaserScan::chatterCallback, this);
    publisher_1 = n.advertise<sensor_msgs::LaserScan>("/new_LaserScan_2d", 1000);
    publisher_1.publish(cartographer);
}




void chatterCallback(const velodyne_msgs::VelodyneScan::ConstPtr& msg)
{
   
  //ROS_INFO("%d %d %s", msg->header.seq, msg->header.stamp.nsec, msg->header.frame_id.c_str());
  
  cartographer.header.stamp.sec=msg->header.stamp.sec;
  cartographer.header.stamp.nsec=msg->header.stamp.nsec;
  cartographer.header.seq = msg->header.seq+1;
  cartographer.header.frame_id = "velodyne_packets";
  cartographer.angle_min=-3.14159274101;
  cartographer.angle_max=3.14159274101;
  cartographer.angle_increment=0.003490659;
  cartographer.time_increment=5.5296e-06;
  cartographer.scan_time=9.9525e-02;
  cartographer.range_min=0;
  cartographer.range_max=200;
   //frame_id is not defined! 
    
  //int* range_begin=&(cartographer->ranges[0]);
  // maybe:      &((cartographer->ranges)[0])

  //I guess ROS is 32 bit.
//  int* intensity_begin=&(cartographer->intensities[0]);
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
//                    cartographer->ranges[i*24+j]=float(msg->packets[i].data[100*j+6]);
 //                   cartographer.intensities[i*24+j*2+flag]=float(msg->packets[i].data[6+100*j]);
 //                   cartographer.ranges[i*24+j*2+flag]=(uint32_t(msg->packets[i].data[6+100*j+2]))<<8;
 //                   cartographer.ranges[i*24+j*2+flag]+=uint32_t(int(msg->packets[i].data[6+100*j+1]));
                    cartographer.ranges[i*24+j*2+flag]=TwoBytes_to_float(&(msg->packets[i].data[6+100*j+1]))
                    flag++;
                }
                else if(flag==1)
                {
 //                   cartographer.intensities[i*24+j*2+flag]=float(msg->packets[i].data[54+100*j]);
 //                   cartographer.ranges[i*24*j*2+flag]=(uint32_t(msg->packets[i].data[54+100*j+2]))<<8;
 //                   cartographer.ranges[i*24+j*2+flag]+=uint32_t(msg->packets[i].data[54+100*j+1]);
                    cartographer.ranges[i*24+j*2+flag]=TwoBytes_to_float(&(msg->packets[i].data[54+100*j+1]));
                    flag++;
                }
            }
        }
    }
//  for(i=0;msg->packets[i].stamp.nsec<msg->header.stamp.nsec;i++);
// ROS_INFO("%d %d %d", i,msg->packets[0].stamp.nsec,msg->packets[i].stamp.nsec);

}

uint32_t TwoBytes_to_float(byte* bytes)
{
    int addr = bytes[0] & 0xFF;  
    addr |= (bytes[1]<<8 & 0xFF00);  
    return uint32_t(addr); 
}

private:
    ros::NodeHandle n;
    ros::Publisher publisher_1;
    ros::Subscriber subscriber_1;
    sensor_msgs::LaserScan cartographer;
};

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"trans");
    VelodyneScan_Convert_LaserScan fire;
    ros::spin(); 
    return 0;
}
