
#include <face_detector.h>
#include <pantilt_tracker.h>
#include <base_tracker.h>
#include <ros/ros.h>

int main(int argc, char** argv)
{
    ros::init(argc, argv, "face_detector_node");
    ros::NodeHandle nh;

    FaceDetector face_detector(nh);
    BaseTracker base_tracker(nh);
    PanTiltTracker pantilt_tracker(nh);

    //TODO: GET OP MODE AS PARAM FROM LAUNCH FILE
    OpMode operation_mode = OpMode::PAN;

    pantilt_tracker.move(0, 0);

    while(ros::ok())
    {
        CvPoint face_xy;
        cv::Rect frame;
        bool detected = face_detector.detectAndDisplay(face_xy, frame);

        //ROS_INFO("x -> %i,\ty -> %i,\tw -> %i,\th -> %i", face_xy.x, face_xy.y, frame.width, frame.height);

        if (detected)
        {
            pantilt_tracker.trackFace(face_xy, frame);
            base_tracker.track(operation_mode, face_xy, frame);
        }
        else
        {
            base_tracker.stop();
        }

        int c = cv::waitKey(10);
        if( (char)c == 'c' )
            break;

        ros::spinOnce();
    }

    return 0;
}