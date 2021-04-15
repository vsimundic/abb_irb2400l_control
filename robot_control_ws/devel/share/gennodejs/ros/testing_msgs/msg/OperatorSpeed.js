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

class OperatorSpeed {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.humanSpeed = null;
      this.robotVelocity = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('humanSpeed')) {
        this.humanSpeed = initObj.humanSpeed
      }
      else {
        this.humanSpeed = 0.0;
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
    // Serializes a message object of type OperatorSpeed
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [humanSpeed]
    bufferOffset = _serializer.float64(obj.humanSpeed, buffer, bufferOffset);
    // Serialize message field [robotVelocity]
    bufferOffset = _serializer.float64(obj.robotVelocity, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type OperatorSpeed
    let len;
    let data = new OperatorSpeed(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [humanSpeed]
    data.humanSpeed = _deserializer.float64(buffer, bufferOffset);
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
    return 'testing_msgs/OperatorSpeed';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e5cb69c02c631c93ca058b4ec5827b97';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    Header header
    float64 humanSpeed
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
    const resolved = new OperatorSpeed(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.humanSpeed !== undefined) {
      resolved.humanSpeed = msg.humanSpeed;
    }
    else {
      resolved.humanSpeed = 0.0
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

module.exports = OperatorSpeed;
