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
    publisher_1 = n.advertise<sensor_msgs::LaserScan>("/new_LaserScan_2d", 2000);
    subscriber_1 = n.subscribe("/velodyne_packets",2000,&VelodyneScan_Convert_LaserScan::chatterCallback, this);
    
   
}

void chatterCallback(const velodyne_msgs::VelodyneScan::ConstPtr& msg)
{
   
  //ROS_INFO("%d %d %s", msg->header.seq, msg->header.stamp.nsec, msg->header.frame_id.c_str());
    sensor_msgs::LaserScan cartographer_1;
  cartographer_1.header.stamp.sec=msg->header.stamp.sec;
  cartographer_1.header.stamp.nsec=msg->header.stamp.nsec;
  cartographer_1.header.seq = msg->header.seq+1;
  cartographer_1.header.frame_id = "velodyne_packets";
  cartographer_1.angle_min=-3.14159274101;
  cartographer_1.angle_max=3.14159274101;
  cartographer_1.angle_increment=0.003490659;
  cartographer_1.time_increment=5.5296e-06;
  cartographer_1.scan_time=9.9525e-02;
  cartographer_1.range_min=0;
  cartographer_1.range_max=200;
   //frame_id is not defined! 
    
  //int* range_begin=&(cartographer_1->ranges[0]);
  // maybe:      &((cartographer_1->ranges)[0])

  //I guess ROS is 32 bit.
//  int* intensity_begin=&(cartographer_1->intensities[0]);
    for(int i=0;i<75;i++)
    {
        for(int j=0;j<12;j++)
        {
        int flag=0;
//        int* p=&(msg->packets[i].data[0]);
            while(flag<2)
            {
                if(flag==0)
                {
//                    cartographer_1->ranges[i*24+j]=float(msg->packets[i].data[100*j+6]);
                        cartographer_1.intensities.push_back(float(msg->packets[i].data[6+100*j]));
 //                   cartographer_1.intensities[i*24+j*2+flag]=float(msg->packets[i].data[6+100*j]);
 //                   cartographer_1.ranges[i*24+j*2+flag]=(uint32_t(msg->packets[i].data[6+100*j+2]))<<8;
 //                   cartographer_1.ranges[i*24+j*2+flag]+=uint32_t(int(msg->packets[i].data[6+100*j+1]));
  //                  cartographer_1.ranges[i*24+j*2+flag]=TwoUint8_to_Float32(&(msg->packets[i].data[6+100*j+1]));
                      int addr =int(msg->packets[i].data[6+100*j+1]) & 0xFF;  
                    addr |= int(msg->packets[i].data[6+100*j+2])<<8 & 0xFF00;  
                    float range_dis=float(addr)*2/1000;
                    if(range_dis>200)
                    {
                    cartographer_1.ranges.push_back(cartographer_1.range_max)
                    }
           //          cartographer_1.ranges[i*24+j*2+flag]=float(addr)*2/1000;
                    else{
                     cartographer_1.ranges.push_back(range_dis);
                    }
  //                   cartographer_1.ranges[i*24+j*2+flag]=addr; 
                     //float strong convertion
                    flag++;
                }
                else if(flag==1)
                {
 //                  cartographer_1.intensities[i*24+j*2+flag]=float(msg->packets[i].data[54+100*j]);
                    cartographer_1.intensities.push_back(float(msg->packets[i].data[54+100*j]));
 //                   cartographer_1.ranges[i*24*j*2+flag]=(uint32_t(msg->packets[i].data[54+100*j+2]))<<8;
 //                   cartographer_1.ranges[i*24+j*2+flag]+=uint32_t(msg->packets[i].data[54+100*j+1]);
  //            cartographer_1.ranges[i*24+j*2+flag]=TwoUint8_to_Float32(&(msg->packets[i].data[54+100*j+1]));
                    int addr =int(msg->packets[i].data[54+100*j+1]) & 0xFF;  
                     addr |= int(msg->packets[i].data[54+100*j+2])<<8 & 0xFF00;  
                     float range_dis=float(addr)*2/1000;

                    if(range_dis>200)
                    {
                    cartographer_1.ranges.push_back(cartographer_1.range_max);
                    }
    //                cartographer_1.ranges[i*24+j*2+flag]=range_dis;
                    else
                    {
                    cartographer_1.ranges.push_back(range_dis);
                    }
 //                    cartographer_1.ranges[i*24+j*2+flag]=addr; 
                     // float strong convertion
                    flag++;
                }
            }
        }
    }
//  for(i=0;msg->packets[i].stamp.nsec<msg->header.stamp.nsec;i++);
// ROS_INFO("%d %d %d", i,msg->packets[0].stamp.nsec,msg->packets[i].stamp.nsec);
     publisher_1.publish(cartographer_1);
}
/*
uint32_t TwoUint8_to_Uint32(const unsigned char* uint8s)
{
    int addr =uint8s[0] & 0xFF;  
    addr |= (uint8s[1]<<8 & 0xFF00);  
    return uint32_t(addr); 
}
*/

/*
float TwoUint8_to_Float32(const unsigned char* uint8s)
{
    int addr =uint8s[0] & 0xFF;  
    addr |= (uint8s[1]<<8 & 0xFF00);  
    return float(addr); 
}
*/

private:
    ros::NodeHandle n;
    ros::Publisher publisher_1;
    ros::Subscriber subscriber_1;
  
};

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"trans_1");
    VelodyneScan_Convert_LaserScan fire;
    ros::spin(); 
    return 0;
}
