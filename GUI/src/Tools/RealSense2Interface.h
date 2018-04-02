#pragma once

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <memory>
#include <string.h>

//#ifdef WITH_REALSENSE2
#include "librealsense2/rs.hpp"
//#endif

#include "ThreadMutexObject.h"
#include "CameraInterface.h"
namespace rs2 {
    class pipeline;
    class context;
    class device;
}

class RealSense2Interface : public CameraInterface
{
public:
    RealSense2Interface(int width = 640,int height = 480,int fps = 30);
    virtual ~RealSense2Interface();

    const int width,height,fps;


//    void executeDetpth(rs2::frame &depth_frame, int64_t & lastDepthTime,
//                              ThreadMutexObject<int> & latestDepthIndex,
//                              ThreadMutexObject<int> & latestRgbIndex,
//                              std::pair<uint8_t *,int64_t> * rgbBuffers,
//                              std::pair<std::pair<uint8_t *,uint8_t *>,int64_t> * frameBuffers) {
////    if (depth_frame) {
//        std::cout << "run here : get the depth frame...." << std::endl;
//
//        lastDepthTime = std::chrono::duration_cast<std::chrono::milliseconds>(
//                std::chrono::system_clock::now().time_since_epoch()).count();
//        int bufferIndex = (latestDepthIndex.getValue() + 1) % numBuffers;
//        memcpy(frameBuffers[bufferIndex].first.first, depth_frame.get_data(),
//               depth_frame.as<rs2::video_frame>().get_width() * depth_frame.as<rs2::video_frame>().get_height() * 2);
//        frameBuffers[bufferIndex].second = lastDepthTime;
//        int lastImageVal = latestRgbIndex.getValue();
//        if(lastImageVal == -1)
//        {
//            return;
//        }
//        lastImageVal %= numBuffers;
//        memcpy(frameBuffers[bufferIndex].first.second,rgbBuffers[lastImageVal].first,
//               color_frame.as<rs2::video_frame>().get_width() * color_frame.as<rs2::video_frame>().get_height() * 3);
//        latestDepthIndex++;
////    }
//    }
//
//    void executeRGB(rs2::frame &color_frame, int64_t & lastRgbTime,
//                           ThreadMutexObject<int> & latestRgbIndex,
//                           std::pair<uint8_t *,int64_t> * rgbBuffers) {
////    if (color_frame) {
//        std::cout << "run here : get the color frame...." << std::endl;
//        lastRgbTime = std::chrono::duration_cast<std::chrono::milliseconds>(
//                std::chrono::system_clock::now().time_since_epoch()).count();
//        int bufferIndex = (latestRgbIndex.getValue() + 1) % numBuffers;
//        memcpy(rgbBuffers[bufferIndex].first,color_frame.get_data(),
//               color_frame.as<rs2::video_frame>().get_width() * color_frame.as<rs2::video_frame>().get_height() * 3);
//        rgbBuffers[bufferIndex].second = lastRgbTime;
//        latestRgbIndex++;
////    }
//
//    }

    bool getAutoExposure();
    bool getAutoWhiteBalance();
    virtual void setAutoExposure(bool value);
    virtual void setAutoWhiteBalance(bool value);

    virtual bool ok()
    {
        return initSuccessful;
    }

    virtual std::string error()
    {
        return errorText;
    }


private:
//#ifdef WITH_REALSENSE2
    rs2::pipeline pipe;
    rs2::device dev;
    rs2::context ctx;

    rs2::config config;


//#endif

  bool initSuccessful;
  std::string errorText;
  
  ThreadMutexObject<int> latestRgbIndex;
  std::pair<uint8_t *,int64_t> rgbBuffers[numBuffers];

  int64_t lastRgbTime;
  int64_t lastDepthTime;

    ThreadMutexObject<int> latestDepthIndex;
    std::pair<std::pair<uint8_t *,uint8_t *>,int64_t> * frameBuffers;
};
