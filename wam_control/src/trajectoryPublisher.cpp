#include "ros/ros.h"
#include "trajectory_msgs/JointTrajectory.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"
#include "control_msgs/FollowJointTrajectoryGoal.h"
#include "control_msgs/FollowJointTrajectoryActionGoal.h"
#include "actionlib_msgs/GoalID.h"

#define GESTURE_START_DELAY 3

int main(int argc, char **argv)
{
	// Setup ROS
	ros::init(argc, argv, "trajectoryPublisher");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<control_msgs::FollowJointTrajectoryActionGoal>("wam/arm_trajectory_controller/follow_joint_trajectory/goal", 1000);

	while (pub.getNumSubscribers() <= 0) {
		usleep(1000);
	}

	ROS_INFO("Performing gesture in %d seconds.", GESTURE_START_DELAY);

	sleep(GESTURE_START_DELAY);

	// Create Joint Trajectory
	trajectory_msgs::JointTrajectory jt;

	jt.header.stamp = ros::Time::now();

	jt.joint_names.resize(7);

	jt.joint_names[0] = "wam/base_yaw_joint";
	jt.joint_names[1] = "wam/shoulder_pitch_joint";
	jt.joint_names[2] = "wam/shoulder_yaw_joint";
	jt.joint_names[3] = "wam/elbow_pitch_joint";
	jt.joint_names[4] = "wam/wrist_yaw_joint";
	jt.joint_names[5] = "wam/wrist_pitch_joint";
	jt.joint_names[6] = "wam/palm_yaw_joint";

	int INTERPCOUNT = 2;

	jt.points.resize(INTERPCOUNT);

	//Interpolation goes here
	for (int i = 0; i < INTERPCOUNT; i++) {

		trajectory_msgs::JointTrajectoryPoint p;

		p.positions.resize(7);

		p.positions[0] = i + 1;
		p.positions[1] = i + 1;
		p.positions[2] = i + 1;
		p.positions[3] = i + 1;
		p.positions[4] = i + 1;
		p.positions[5] = i + 1;
		p.positions[6] = i + 1;

		p.velocities.resize(7);

		for (int j = 0; j < 7; j++) {
			p.velocities[j] = 0.0;
		}

		p.time_from_start = ros::Duration(1 + i);

		jt.points[i]= (p);

	}

	// Create Joint Trajectory Goal
	control_msgs::FollowJointTrajectoryGoal goal;

	goal.trajectory = jt;

	// Create Joint Trajectory Action Goal
	control_msgs::FollowJointTrajectoryActionGoal actionGoal;

	actionGoal.header.stamp = ros::Time::now();
	actionGoal.goal_id.stamp = ros::Time::now();
	actionGoal.goal_id.id = "12345";
	actionGoal.goal = goal;

	// Publish message
	pub.publish(actionGoal);

	ROS_INFO("Published");

	ros::spin();


	return 0;
}
