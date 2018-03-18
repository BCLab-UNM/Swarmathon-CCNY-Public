#ifndef CONTROLLER_H
#define CONTROLLER_H


#include "Result.h"
#include "ccny_srvs/GetPickup.h"
#include "ccny_srvs/SetPickup.h"
#include "ccny_srvs/GetStatus.h"
#include "ccny_srvs/SetStatus.h"
#include <ros/ros.h>

typedef ccny_srvs::GetStatus GStatus;
typedef ccny_srvs::SetStatus SStatus;
typedef ccny_srvs::SetPickup SPickup;
typedef ccny_srvs::GetPickup GPickup;
/*
 * This class is meant to serve as a template for all Controllers,
 * including new Controllers defined by each team.
 *
 * It also exists so that Controllers have a generic interface to
 * use for processing interrupts in LogicController.
 */

class Controller {



public:
  Controller() {
  }
  ~Controller() {}

  //Resets internal state to defaults
  virtual void Reset() = 0;

  //Determines what action should be taken based on current
  //internal state and data
  virtual Result DoWork() = 0;

  //Returns whether or not an interrupt must be thrown
  virtual bool ShouldInterrupt() = 0;

  //Returns whether or not a controller should be polled for a Result
  virtual bool HasWork() = 0;
  //to do check this enum type
  virtual string toString()=0;

protected:
  //Looks at external data and determines if an interrupt must be thrown
  //or if the controller should be polled
  virtual void ProcessData() = 0;


  enum change{
         OBSTACLE=1,
         ROVER=1<<1,
         ROUTE=1<<2,
         SAMPLE=1<<3,
         ANY=1|1<<1|1<<2
     };

    // Service List
    // All Services that are to be used by Controllers must go here.
    ros::ServiceClient set_sample;
    ros::ServiceClient set_robot;
    ros::ServiceClient set_object;
    ros::ServiceClient set_route;
    ros::ServiceClient is_robot;
    ros::ServiceClient is_obstacle;
    ros::ServiceClient is_any;
    ros::ServiceClient is_route;
    ros::ServiceClient has_sample;
    ros::ServiceClient has_searched;
    ros::ServiceClient get_pickup;
    ros::ServiceClient set_pickup;
    ros::ServiceClient get_pickuplist_size;

    SStatus statusSet;
    GStatus statusGet;
    SPickup pickupSet;
    GPickup pickupGet;

  //Defines what each bit in a Grid cell is used for.
    void initialize_services(){
        ros::NodeHandle nm;
        set_sample = nm.serviceClient <SStatus>("mark_samples");
        set_robot= nm.serviceClient<SStatus>("set_occupied_rover");
        set_object=nm.serviceClient<SStatus>("set_occupied_object");
        set_route=nm.serviceClient<SStatus>("set_occupied_planned_route");
        is_robot=nm.serviceClient<GStatus>("is_occupied_rover");
        is_obstacle=nm.serviceClient<GStatus>("is_occupied_obstacle");
        is_any=nm.serviceClient<GStatus>("is_occupied_any");
        is_route=nm.serviceClient<GStatus>("is_occupied_planned_route");
        has_sample=nm.serviceClient<GStatus>("has_samples");
        has_searched=nm.serviceClient<GStatus>("has_been_searched");
        get_pickup=nm.serviceClient<GPickup>("pickup_getter");
        set_pickup=nm.serviceClient<SPickup>("pickup_setter");
        get_pickuplist_size = nm.serviceClient<GPickup>("get_size");
    }
    void reset_msgs(){
    statusSet.request.x=0;
    statusSet.request.y=0;
    statusSet.request.data=change::ROVER;
    statusGet.request.x=0;
    statusGet.request.y=0;
    pickupGet.request.pickup = false;


    }

};

#endif // CONTROLLER_H
