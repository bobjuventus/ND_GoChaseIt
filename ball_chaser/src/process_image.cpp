#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include "string"
#include "sstream"

// Define a global client that can request services
ros::ServiceClient client;
bool ball_presence = false;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ROS_INFO_STREAM("Requesting rover to move");

    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // Call the safe_move service and pass the requested joint angles
    if (!client.call(srv))
        ROS_ERROR("Failed to call service safe_move");
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    int left_count = 0;
    int mid_count = 0;
    int right_count = 0;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_robot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
    int rows = sizeof img.data / sizeof img.data[0];
    int cols = sizeof img.data[0] / sizeof(uint8_t);

    std::stringstream ss;
    ss << "My rols:" << rows << ", My cols: " << cols << ", official rols: " << img.height << ", official cols: " << img.width;
    std::string s = ss.str();
    ROS_INFO_STREAM(s);

    // for i in row:
    //     for j in col[0,1/3]:
    //         if img[i, j] == white_pixel:
    //             left_count++;

    // for i in row:
    //     for j in col[1/3, 2/3]:
    //         if img[i, j] == white_pixel:
    //             mid_count++;

    // for i in row:
    //     for j in col[2/3, 1]:
    //         if img[i, j] == white_pixel:
    //             right_count++;

    // // Did not find the ball
    // if left_count == 0 && mid_count == 0 && right_count == 0:
    //     ball_presence = false;
    //     drive_robot(0.0, 0.0);
    //     return;
    // // Find the ball
    // if left_count >= max(mid_count, right_count):
    //     ball_presence = true;
    //     drive_robot(0.1, 0.1);
    //     return;
    // if mid_count >= max(left_count, right_count):
    //     ball_presence = true;
    //     drive_robot(0.1, 0.0);
    //     return;
    // if right_count >= max(left_count, mid_count):
    //     drive_robot(0.1, -0.1);
    //     return;

    // TODO: add in the lidar data so the robot does not run into the ball


}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}