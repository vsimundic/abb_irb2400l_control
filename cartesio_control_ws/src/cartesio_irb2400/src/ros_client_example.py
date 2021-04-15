#!/usr/bin/env python

from cartesian_interface.pyci_all import *
import numpy as np
import math
import time


cli = pyci.CartesianInterfaceRos()

# print cli

# print cli.getTaskList()

task_name = 'tool0'
tool0_task = cli.getTask(task_name)

print 'Task name is', tool0_task.getName()
print 'Task type is', tool0_task.getType()
print 'Task activation state is', tool0_task.getActivationState()
print 'Task size is', tool0_task.getSize()
print 'Task lambda is', tool0_task.getLambda()
print 'Task active indices are', tool0_task.getIndices()
print 'Task weight is: \n', tool0_task.getWeight()

print type(tool0_task)
print '\n'

print 'Task distal link is', tool0_task.getDistalLink()
print 'Task base link is', tool0_task.getBaseLink()
print 'Task control mode is', tool0_task.getControlMode()
print 'Task state is', tool0_task.getTaskState()
print 'Task current reference is \n', tool0_task.getPoseReference()[0] # [0] for pose, [1] for velocity

cli.update()
Tref, _, _ = tool0_task.getPoseReference() # just return the pose ref, skip vel & acc
print Tref

# Tref.translation_ref()[0] -= 0.3
# time = 6.0
# tool0_task.setPoseTarget(Tref, time)
# tool0_task.waitReachCompleted(10.0) # blocks till action is completed (or timeout has passed)

# cli.update()
# Tref, _, _ = tool0_task.getPoseReference()
# print Tref

# print tool0_task.getVelocityLimits()
# print tool0_task.getAccelerationLimits()

# tool0_task.setVelocityLimits(100, 100)
# tool0_task.setAccelerationLimits(10000, 10000)

# print tool0_task.getLambda()
# tool0_task.setLambda(1.0)




cli.update()
Tinit = Affine3()
Tinit.translation = [1.035, 0, 1.62]
Tinit.quaternion = [0, 0, 0, 1]

t = 0.0
dt = 0.01
period = 3.0
radius = 0.12

while t < 3*period:
    
    delta_x = radius * math.cos(t*2.0*math.pi/period) - radius # + math.pi/2.0)
    delta_y = radius * math.sin(t*2.0*math.pi/period)
    Tref = Tinit.copy()
    # Tref.translation_ref()[0] += delta_x
    Tref.translation_ref()[1] += delta_y
    
    tool0_task.setPoseReference(Tref) # this publishes the reference
    
    t += dt
    time.sleep(dt)
