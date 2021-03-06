/*
 * Copyright (C) 2016 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <functional>
#include <mutex>

#include <gtest/gtest.h>

#include "test/ServerFixture.hh"

using namespace gazebo;
class DepthCameraSensor_TEST : public ServerFixture
{
};

std::mutex g_depthMutex;
unsigned int g_depthCounter = 0;
float *g_depthBuffer = NULL;

/////////////////////////////////////////////////
void OnNewDepthFrame(const float * _image,
    unsigned int _width, unsigned int _height,
    unsigned int /*_depth*/, const std::string &/*_format*/)
{
  if (!_image)
    return;
  std::lock_guard<std::mutex> lock(g_depthMutex);
  if (!g_depthBuffer)
    g_depthBuffer = new float[_width * _height];
  memcpy(g_depthBuffer,  _image, _width * _height * sizeof(_image[0]));
  g_depthCounter++;
}

/////////////////////////////////////////////////
/// \brief Test Creation of a Depth Camera sensor
TEST_F(DepthCameraSensor_TEST, CreateDepthCamera)
{
  Load("worlds/depth_camera.world");
  sensors::SensorManager *mgr = sensors::SensorManager::Instance();

  // Create the camera sensor
  std::string sensorName = "default::camera_model::my_link::camera";

  // Get a pointer to the depth camera sensor
  sensors::DepthCameraSensorPtr sensor =
     boost::dynamic_pointer_cast<sensors::DepthCameraSensor>
     (mgr->GetSensor(sensorName));

  // Make sure the above dynamic cast worked.
  EXPECT_TRUE(sensor != NULL);

  unsigned int imageWidth = sensor->GetDepthCamera()->GetImageWidth();
  unsigned int imageHeight = sensor->GetDepthCamera()->GetImageHeight();
  EXPECT_EQ(imageWidth, 640u);
  EXPECT_EQ(imageHeight, 480u);
  EXPECT_TRUE(sensor->IsActive());

  rendering::DepthCameraPtr depthCamera = sensor->GetDepthCamera();
  EXPECT_TRUE(depthCamera != NULL);

  event::ConnectionPtr c = depthCamera->ConnectNewDepthFrame(
      std::bind(&::OnNewDepthFrame, std::placeholders::_1,
      std::placeholders::_2, std::placeholders::_3, std::placeholders::_4,
      std::placeholders::_5));

  // wait for a few depth camera frames
  int i = 0;
  while (g_depthCounter < 10 && i < 300)
  {
    common::Time::MSleep(10);
    i++;
  }
  EXPECT_LT(i, 300);

  unsigned int imageSize = imageWidth * imageHeight;

  std::lock_guard<std::mutex> lock(g_depthMutex);
  // Check that the depth values are valid
  for (unsigned int i = 0; i < imageSize; ++i)
  {
    // in gazebo versions < 8.0.0,
    // depth camera returns 0.0 if nothing is detected within range
    if (!gazebo::math::equal(g_depthBuffer[i], 0.0f))
    {
      EXPECT_TRUE(g_depthBuffer[i] <= depthCamera->GetFarClip());
      EXPECT_TRUE(g_depthBuffer[i] >= depthCamera->GetNearClip());
    }
  }

  // sphere with radius 1m is at 2m in front of depth camera
  // so verify depth readings are between 1-2m in the mid row
  unsigned int halfHeight = static_cast<unsigned int>(imageHeight*0.5)-1;
  for (unsigned int i = imageWidth*halfHeight;
      i < imageWidth*(halfHeight+1); ++i)
  {
    EXPECT_TRUE(g_depthBuffer[i] < 2.0f);
    EXPECT_TRUE(g_depthBuffer[i] >= 1.0f);
  }

  if (g_depthBuffer)
    delete [] g_depthBuffer;
}

/////////////////////////////////////////////////
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
