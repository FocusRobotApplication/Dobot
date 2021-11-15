#include "ros/ros.h"
#include "ros/console.h"
#include "std_msgs/String.h"
#include "dobot/SetCmdTimeout.h"
#include "dobot/SetQueuedCmdClear.h"
#include "dobot/SetQueuedCmdStartExec.h"
#include "dobot/SetQueuedCmdForceStopExec.h"
#include "dobot/GetDeviceVersion.h"
#include <unistd.h>
 
#include "dobot/SetEndEffectorParams.h"
#include "dobot/SetPTPJointParams.h"
#include "dobot/SetPTPCoordinateParams.h"
#include "dobot/SetPTPJumpParams.h"
#include "dobot/SetPTPCommonParams.h"
#include "dobot/SetPTPCmd.h"
#include "dobot/SetEndEffectorSuctionCup.h"
#include "dobot/SetHOMEParams.h"
#include "dobot/SetHOMECmd.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "DobotClient");
    ros::NodeHandle n;
 
    ros::ServiceClient client;

    client = n.serviceClient<dobot::SetEndEffectorSuctionCup>("/DobotServer/SetEndEffectorSuctionCup");
        dobot::SetEndEffectorSuctionCup sck2,sck3;
 
    // SetCmdTimeout
    client = n.serviceClient<dobot::SetCmdTimeout>("/DobotServer/SetCmdTimeout");
    dobot::SetCmdTimeout srv1;
    srv1.request.timeout = 3000;
    if (client.call(srv1) == false) {
        ROS_ERROR("Failed to call SetCmdTimeout. Maybe DobotServer isn't started yet!");
        return -1;
    }
 
    // Clear the command queue
    client = n.serviceClient<dobot::SetQueuedCmdClear>("/DobotServer/SetQueuedCmdClear");
    dobot::SetQueuedCmdClear srv2;
    client.call(srv2);
 
    // Start running the command queue
    client = n.serviceClient<dobot::SetQueuedCmdStartExec>("/DobotServer/SetQueuedCmdStartExec");
    dobot::SetQueuedCmdStartExec srv3;
    client.call(srv3);
 
    // Get device version information
    client = n.serviceClient<dobot::GetDeviceVersion>("/DobotServer/GetDeviceVersion");
    dobot::GetDeviceVersion srv4;
    client.call(srv4);
    if (srv4.response.result == 0) {
        ROS_INFO("Device version:%d.%d.%d", srv4.response.majorVersion, srv4.response.minorVersion, srv4.response.revision);
    } else {
        ROS_ERROR("Failed to get device version information!");
    }
 
    // Set PTP coordinate parameters
    do {
        client = n.serviceClient<dobot::SetPTPCoordinateParams>("/DobotServer/SetPTPCoordinateParams");
        dobot::SetPTPCoordinateParams srv;
 
        srv.request.xyzVelocity = 100;
        srv.request.xyzAcceleration = 100;
        srv.request.rVelocity = 100;
        srv.request.rAcceleration = 100;
        client.call(srv);
    } while (0);
 
    // Set PTP common parameters
    do {
        client = n.serviceClient<dobot::SetPTPCommonParams>("/DobotServer/SetPTPCommonParams");
        dobot::SetPTPCommonParams srv;
 
        srv.request.velocityRatio = 50;
        srv.request.accelerationRatio = 50;
        client.call(srv);
    } while (0);
 
    do{
        client = n.serviceClient<dobot::SetHOMEParams>("/DobotServer/SetHOMEParams");
        dobot::SetHOMEParams home;
        home.request.x = 200;
        home.request.y = 0;
        home.request.z = 0;
        home.request.r = 0;
        home.request.isQueued = 1;
        client.call(home); 
 
    } while(0);
 
    do{
        client = n.serviceClient<dobot::SetHOMECmd>("/DobotServer/SetHOMECmd");
        dobot::SetHOMECmd home1;
        client.call(home1);
    } while(0);
 
    ros::spinOnce();
    while (ros::ok()){
        client = n.serviceClient<dobot::SetEndEffectorSuctionCup>("/DobotServer/SetEndEffectorSuctionCup");
        dobot::SetEndEffectorSuctionCup sck1;   
        do{
            sck1.request.enableCtrl = 1; // When enableCtr == 1 the motor will operate. 
            sck1.request.suck = 1; // When suck == 1 the suction cup will suck.
            sck1.request.isQueued = true; // This command puts the request in the queue.
            ros::spinOnce(); 
        } while(0);
 
        client = n.serviceClient<dobot::SetPTPCmd>("/DobotServer/SetPTPCmd");
        dobot::SetPTPCmd srv;
        // The first point
        do{
            srv.request.ptpMode = 1;
            srv.request.x = 200;
            srv.request.y = 0;
            srv.request.z = 0;
            srv.request.r = 0;
            client.call(srv);
 
             if (srv.response.result == 0) {
                break; 
            }     
            ros::spinOnce(); //spinOnce() will execute any callbacks.
            if (ros::ok() == false) {
                break;
            }
 
        }while(0);
 
        do{
        
            srv.request.ptpMode = 1;
            srv.request.x = 250;
            srv.request.y = 0;
            srv.request.z = 0;
            srv.request.r = 0;
            client.call(srv);
 
            if (srv.response.result == 0) {
                break; 
            }     
            ros::spinOnce(); //spinOnce() will execute any commands in the queue.
            if (ros::ok() == false) {
                break;
            }
        } while(0);
 
        
        // pick up 
        do{
            sck2.request.enableCtrl = 1;
            sck2.request.suck = 1; // Enable suction 
            sck2.request.isQueued = true;
            client.call(sck2);
 
            ros::spinOnce(); //spinOnce() will execute any commands in the queue.
            if (ros::ok() == false) {
                sck2.request.suck = 0;
		break;
            }
        } while(0);
        client = n.serviceClient<dobot::SetPTPCmd>("/DobotServer/SetPTPCmd");
        dobot::SetPTPCmd srv6;
        do{
            srv6.request.ptpMode = 1;
            srv6.request.x = 250;
            srv6.request.y = 0;
            srv6.request.z = -45;
            srv6.request.r = 0;
            client.call(srv6);
 
             if (srv6.response.result == 0) {
                break; 
            }     
            ros::spinOnce(); //spinOnce() will execute any commands in the queue.
            if (ros::ok() == false) {
                break;
            }
 
        }while(0);
 
        do{
 
            srv6.request.ptpMode = 1;
            srv6.request.x = 250;
            srv6.request.y = 50;
            srv6.request.z = 0;
            srv6.request.r = 0;
            client.call(srv6);
 
             if (srv6.response.result == 0) {
                break; 
            }     
            ros::spinOnce(); //spinOnce() will execute any commands in the queue.
            if (ros::ok() == false) {
                break;
            }
        }while(0);
 
        do{
 
            srv6.request.ptpMode = 1;
            srv6.request.x = 200;
            srv6.request.y = 50;
            srv6.request.z = -45;
            srv6.request.r = 0;
            client.call(srv6);
 
            if (srv6.response.result == 0) {
                break; 
            }     
            ros::spinOnce(); //spinOnce() will execute any commands in the queue.
            if (ros::ok() == false) {
                break;
            }
 
        } while(0);
 
        /* client = n.serviceClient<dobot::SetEndEffectorSuctionCup>("/DobotServer/SetEndEffectorSuctionCup");
        dobot::SetEndEffectorSuctionCup sck3; */
        //drop off 
        do{    
            sck3.request.enableCtrl = 1;
	    sck3.request.suck = 0;
            sck3.request.isQueued = true;
            client.call(sck3);
 
            ros::spinOnce(); //spinOnce() will execute any commands in the queue.
            if (ros::ok() == false) {
                break;
            }
        }while (0);
     
       ros::spinOnce();
    }
  

    sck2.request.suck = 0;
    sck3.request.suck = 0;
    return 0;
}
