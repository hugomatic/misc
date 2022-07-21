import pybullet as p
import time
import pybullet_data
import math
import cv2
import numpy as np

phyisicsClient = p.connect(p.DIRECT)# (p.GUI)#or p.DIRECT for non-graphical version
p.setAdditionalSearchPath(pybullet_data.getDataPath()) #optionally
p.setGravity(0,0,-10)
planeId = p.loadURDF("plane.urdf")
startPos = [0,0,1]
startOrientation = p.getQuaternionFromEuler([0,0,0])
boxId = p.loadURDF("r2d2.urdf",startPos, startOrientation)
#set the center of mass frame (loadURDF sets base link frame) startPos/Ornp.resetBasePositionAndOrientation(boxId, startPos, startOrientation)

for i in range (100):
    p.stepSimulation()
    time.sleep(1./240.)
    angle = i /100
    viewMatrix = p.computeViewMatrix(
      cameraEyePosition=[2*math.cos(angle), 2*math.sin(angle), 3],
      cameraTargetPosition=[0, 0, 0],
      cameraUpVector=[0, 0, 1])
    projectionMatrix = p.computeProjectionMatrixFOV(
      fov=45.0,
      aspect=1.0,
      nearVal=0.1,
      farVal=100)
    w = 800
    h = 600
    img = p.getCameraImage(800, 600,
      viewMatrix=viewMatrix,
      projectionMatrix=projectionMatrix,
      renderer=p.ER_BULLET_HARDWARE_OPENGL)
    rgbBuffer = img[2]
    depthBuffer = img[3] # .astype(np.uint8) ?
    im_rgb = cv2.cvtColor(rgbBuffer, cv2.COLOR_BGR2RGB)
    path = f'test_img/rgbtest{i:03d}.png'
    print(path)
    cv2.imwrite(path, im_rgb)

cubePos, cubeOrn = p.getBasePositionAndOrientation(boxId)
print(cubePos,cubeOrn)
p.disconnect()

