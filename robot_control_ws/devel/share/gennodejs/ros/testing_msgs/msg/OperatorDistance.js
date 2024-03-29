// Auto-generated. Do not edit!

// (in-package testing_msgs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class OperatorDistance {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.distanceFromEnvelope = null;
      this.robotVelocity = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('distanceFromEnvelope')) {
        this.distanceFromEnvelope = initObj.distanceFromEnvelope
      }
      else {
        this.distanceFromEnvelope = 0.0;
      }
      if (initObj.hasOwnProperty('robotVelocity')) {
        this.robotVelocity = initObj.robotVelocity
      }
      else {
        this.robotVelocity = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type OperatorDistance
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [distanceFromEnvelope]
    bufferOffset = _serializer.float64(obj.distanceFromEnvelope, buffer, bufferOffset);
    // Serialize message field [robotVelocity]
    bufferOffset = _serializer.float64(obj.robotVelocity, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type OperatorDistance
    let len;
    let data = new OperatorDistance(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [distanceFromEnvelope]
    data.distanceFromEnvelope = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [robotVelocity]
    data.robotVelocity = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 16;
  }

  static datatype() {
    // Returns string type for a message object
    return 'testing_msgs/OperatorDistance';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '7a42ca980d49efae772557b6a6049115';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
    float64 distanceFromEnvelope
    float64 robotVelocity
    ================================================================================
    MSG: std_msgs/Header
    # Standard metadata for higher-level stamped data types.
    # This is generally used to communicate timestamped data 
    # in a particular coordinate frame.
    # 
    # sequence ID: consecutively increasing ID 
    uint32 seq
    #Two-integer timestamp that is expressed as:
    # * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
    # * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
    # time-handling sugar is provided by the client library
    time stamp
    #Frame this data is associated with
    string frame_id
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new OperatorDistance(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.distanceFromEnvelope !== undefined) {
      resolved.distanceFromEnvelope = msg.distanceFromEnvelope;
    }
    else {
      resolved.distanceFromEnvelope = 0.0
    }

    if (msg.robotVelocity !== undefined) {
      resolved.robotVelocity = msg.robotVelocity;
    }
    else {
      resolved.robotVelocity = 0.0
    }

    return resolved;
    }
};

module.exports = OperatorDistance;
