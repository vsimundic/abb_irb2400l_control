// Generated by gencpp from file testing_msgs/AngleRotation.msg
// DO NOT EDIT!


#ifndef TESTING_MSGS_MESSAGE_ANGLEROTATION_H
#define TESTING_MSGS_MESSAGE_ANGLEROTATION_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>

namespace testing_msgs
{
template <class ContainerAllocator>
struct AngleRotation_
{
  typedef AngleRotation_<ContainerAllocator> Type;

  AngleRotation_()
    : header()
    , radialVelocity(0.0)
    , robotVelocity(0.0)  {
    }
  AngleRotation_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , radialVelocity(0.0)
    , robotVelocity(0.0)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef double _radialVelocity_type;
  _radialVelocity_type radialVelocity;

   typedef double _robotVelocity_type;
  _robotVelocity_type robotVelocity;





  typedef boost::shared_ptr< ::testing_msgs::AngleRotation_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::testing_msgs::AngleRotation_<ContainerAllocator> const> ConstPtr;

}; // struct AngleRotation_

typedef ::testing_msgs::AngleRotation_<std::allocator<void> > AngleRotation;

typedef boost::shared_ptr< ::testing_msgs::AngleRotation > AngleRotationPtr;
typedef boost::shared_ptr< ::testing_msgs::AngleRotation const> AngleRotationConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::testing_msgs::AngleRotation_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::testing_msgs::AngleRotation_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::testing_msgs::AngleRotation_<ContainerAllocator1> & lhs, const ::testing_msgs::AngleRotation_<ContainerAllocator2> & rhs)
{
  return lhs.header == rhs.header &&
    lhs.radialVelocity == rhs.radialVelocity &&
    lhs.robotVelocity == rhs.robotVelocity;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::testing_msgs::AngleRotation_<ContainerAllocator1> & lhs, const ::testing_msgs::AngleRotation_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace testing_msgs

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::testing_msgs::AngleRotation_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::testing_msgs::AngleRotation_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct IsMessage< ::testing_msgs::AngleRotation_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::testing_msgs::AngleRotation_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::testing_msgs::AngleRotation_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::testing_msgs::AngleRotation_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::testing_msgs::AngleRotation_<ContainerAllocator> >
{
  static const char* value()
  {
    return "de165192dcde44004c7a45b00482db8e";
  }

  static const char* value(const ::testing_msgs::AngleRotation_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xde165192dcde4400ULL;
  static const uint64_t static_value2 = 0x4c7a45b00482db8eULL;
};

template<class ContainerAllocator>
struct DataType< ::testing_msgs::AngleRotation_<ContainerAllocator> >
{
  static const char* value()
  {
    return "testing_msgs/AngleRotation";
  }

  static const char* value(const ::testing_msgs::AngleRotation_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::testing_msgs::AngleRotation_<ContainerAllocator> >
{
  static const char* value()
  {
    return "Header header\n"
"float64 radialVelocity\n"
"float64 robotVelocity\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
;
  }

  static const char* value(const ::testing_msgs::AngleRotation_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::testing_msgs::AngleRotation_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.radialVelocity);
      stream.next(m.robotVelocity);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct AngleRotation_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::testing_msgs::AngleRotation_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::testing_msgs::AngleRotation_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "radialVelocity: ";
    Printer<double>::stream(s, indent + "  ", v.radialVelocity);
    s << indent << "robotVelocity: ";
    Printer<double>::stream(s, indent + "  ", v.robotVelocity);
  }
};

} // namespace message_operations
} // namespace ros

#endif // TESTING_MSGS_MESSAGE_ANGLEROTATION_H