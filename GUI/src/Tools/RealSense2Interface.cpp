#include "RealSense2Interface.h"
#include <functional>
#include <iostream>
#include <cstdio>
#include <iomanip>
#include <atomic>
#include <string.h>
#include <algorithm>
#include <map>
#include <opencv2/opencv.hpp>
#include <future>
//#ifdef WITH_REALSENSE2

static void print_device_information(const rs2::device& dev) {

    std::cout << "Device information: " << std::endl;
    for (int i = 0; i < static_cast<int>(RS2_CAMERA_INFO_COUNT); i++) {
        rs2_camera_info info_type = static_cast<rs2_camera_info>(i);
        std::cout << " " << std::left << std::setw(20) << info_type << " : ";
        if (dev.supports(info_type)) {
            std::cout << dev.get_info(info_type) << std::endl;
        } else {
            std::cout << "N/A" << std::endl;
        }

    }
}


RealSense2Interface::RealSense2Interface(int inWidth,int inHeight,int inFps)
  : width(inWidth),
  height(inHeight),
  fps(inFps),
//  dev(nullptr),
  initSuccessful(true)
{
    rs2::device_list availableDevices = ctx.query_devices();
    if (availableDevices.size() == 0)

  {
    errorText = "No device connected.";
    initSuccessful = false;
    return;
  }

    dev = rs2::device(availableDevices.front());
    print_device_information(dev);
    std::cout << "run here: config...." << std::endl;
    config.enable_stream(RS2_STREAM_DEPTH, width, height, RS2_FORMAT_Z16, fps);
    config.enable_stream(RS2_STREAM_COLOR, width, height, RS2_FORMAT_RGB8, fps);
    std::cout << "run here initial latestIndex...." << std::endl;


  latestDepthIndex.assign(-1);
  latestRgbIndex.assign(-1);

  for(int i = 0; i < numBuffers; i++)
  {
    uint8_t * newImage = (uint8_t *)calloc(width * height * 3,sizeof(uint8_t));
    rgbBuffers[i] = std::pair<uint8_t *,int64_t>(newImage,0);
  }

  for(int i = 0; i < numBuffers; i++)
  {
    uint8_t * newDepth = (uint8_t *)calloc(width * height * 2,sizeof(uint8_t));
    uint8_t * newImage = (uint8_t *)calloc(width * height * 3,sizeof(uint8_t));
    frameBuffers[i] = std::pair<std::pair<uint8_t *,uint8_t *>,int64_t>(std::pair<uint8_t *,uint8_t *>(newDepth,newImage),0);
  }

//  setAutoExposure(true);
//  setAutoWhiteBalance(true);

    std::cout << "run here: pipeline ..." << std::endl;
    rs2::pipeline_profile pipeline_profile = pipe.start(config);
    rs2::video_stream_profile depth_stream_profile = pipeline_profile.get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>();
    rs2::video_stream_profile color_stream_profile = pipeline_profile.get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>();

//    std::atomic_bool stopped(false);
//    std::thread processing_thread([&]() {
//        while (!stopped) {
//            if (pipe.poll_for_frames(&frameset)) {
//                depth_frame = frameset.get_depth_frame();
//                if (depth_frame) {
//                    executeDetpth(depth_frame, lastDepthTime,
//                                  latestDepthIndex,
//                                  latestRgbIndex,
//                                  rgbBuffers,
//                                  frameBuffers);
//                }
//                color_frame = frameset.get_color_frame();
//                if (color_frame) {
//                    executeRGB(color_frame, lastRgbTime,
//                               latestRgbIndex,
//                               rgbBuffers);
//                }
//            }
//        }
//    });
//    stopped = true;
//    processing_thread.join();


////    std::thread processing_thread([&]() {
//        rs2::frame_queue depth_queue, color_queue;
//        while (true)
////    for (int i = 0; i < 1000; i++)
//        {
//            frameset = pipe.wait_for_frames();
//            depth_queue.enqueue(frameset.get_depth_frame());
//            color_queue.enqueue(frameset.get_color_frame());
////            rs2::depth_frame frame;
//            rs2::frame depth_frame, color_frame;
//            if (depth_queue.poll_for_frame(&depth_frame)) {
////                frame.get_data();
//                // Do processing on the frame
//                std::cout << "run here : get the depth frame...." << std::endl;
//
//                lastDepthTime = std::chrono::duration_cast<std::chrono::milliseconds>(
//                        std::chrono::system_clock::now().time_since_epoch()).count();
//                int bufferIndex = (latestDepthIndex.getValue() + 1) % numBuffers;
//                memcpy(frameBuffers[bufferIndex].first.first, depth_frame.get_data(),
//                       depth_frame.as<rs2::video_frame>().get_width() *
//                       depth_frame.as<rs2::video_frame>().get_height() * 2);
//
//                frameBuffers[bufferIndex].second = lastDepthTime;
//                int lastImageVal = latestRgbIndex.getValue();
//                if (lastImageVal == -1) {
//                    return;
//                }
//                lastImageVal %= numBuffers;
//                memcpy(frameBuffers[bufferIndex].first.second, rgbBuffers[lastImageVal].first,
//                       color_frame.as<rs2::video_frame>().get_width() *
//                       color_frame.as<rs2::video_frame>().get_height() * 3);
//                latestDepthIndex++;
//            }
//
//
//            if (color_queue.poll_for_frame(&color_frame)) {
////                frame.get_data();
//                // Do processing on the frame
//                std::cout << "run here : get the color frame...." << std::endl;
//                lastRgbTime = std::chrono::duration_cast<std::chrono::milliseconds>(
//                        std::chrono::system_clock::now().time_since_epoch()).count();
//                int bufferIndex = (latestRgbIndex.getValue() + 1) % numBuffers;
//                memcpy(rgbBuffers[bufferIndex].first, color_frame.get_data(),
//                       color_frame.as<rs2::video_frame>().get_width() *
//                       color_frame.as<rs2::video_frame>().get_height() * 3);
//
//                rgbBuffers[bufferIndex].second = lastRgbTime;
//                latestRgbIndex++;
//            }
//        }
////    });
////    processing_thread.detach();
/*
        std::future<void> fuResult = std::async([]() {
    while (true)

    {
        int64_t lastDepthTime;
        ThreadMutexObject<int> latestDepthIndex;
        ThreadMutexObject<int> latestRgbIndex;

        std::pair<uint8_t *,int64_t> * rgbBuffers;
        std::pair<std::pair<uint8_t *,uint8_t *>,int64_t> * frameBuffers;


        int64_t lastRgbTime;

        static rs2::frameset frameset;
        rs2::frame depth_frame;
        rs2::frame color_frame;

//        rs2::frame_queue depth_queue, color_queue;
        frameset = pipe.wait_for_frames();
//        depth_queue.enqueue(frameset.get_depth_frame());
//        color_queue.enqueue(frameset.get_color_frame());
        depth_frame = frameset.get_depth_frame();
        if (depth_frame) {

//        if (depth_queue.poll_for_frame(&depth_frame)) {

            std::cout << "run here : get the depth frame...." << std::endl;

            lastDepthTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
            int bufferIndex = (latestDepthIndex.getValue() + 1) % numBuffers;
            memcpy(frameBuffers[bufferIndex].first.first, depth_frame.get_data(),
                   depth_frame.as<rs2::video_frame>().get_width() *
                   depth_frame.as<rs2::video_frame>().get_height() * 2);

            frameBuffers[bufferIndex].second = lastDepthTime;
            int lastImageVal = latestRgbIndex.getValue();
            if (lastImageVal == -1) {
                return;
            }
            lastImageVal %= numBuffers;
            memcpy(frameBuffers[bufferIndex].first.second, rgbBuffers[lastImageVal].first,
                   color_frame.as<rs2::video_frame>().get_width() *
                   color_frame.as<rs2::video_frame>().get_height() * 3);
            latestDepthIndex++;
        }

        color_frame = frameset.get_color_frame();
        if (color_frame) {
//        if (color_queue.poll_for_frame(&color_frame)) {
//                frame.get_data();
            // Do processing on the frame
            std::cout << "run here : get the color frame...." << std::endl;
            lastRgbTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
            int bufferIndex = (latestRgbIndex.getValue() + 1) % numBuffers;
            memcpy(rgbBuffers[bufferIndex].first, color_frame.get_data(),
                   color_frame.as<rs2::video_frame>().get_width() *
                   color_frame.as<rs2::video_frame>().get_height() * 3);

            rgbBuffers[bufferIndex].second = lastRgbTime;
            latestRgbIndex++;
        }
    }
    });
//    processing_thread.detach();

 */
}


RealSense2Interface::~RealSense2Interface()
{
  if(initSuccessful)
  {
//      dev.stop();
//    dev->stop();
//    if(pipe) {
        pipe.stop();
//    }
    for(int i = 0; i < numBuffers; i++)
    {
      free(rgbBuffers[i].first);
    }

    for(int i = 0; i < numBuffers; i++)
    {
      free(frameBuffers[i].first.first);
      free(frameBuffers[i].first.second);
    }

//    delete rgbCallback;
//    delete depthCallback;

  }
}

void RealSense2Interface::setAutoExposure(bool value)
{
//  dev->set_option(rs2::option::color_enable_auto_exposure,value);

}

void RealSense2Interface::setAutoWhiteBalance(bool value)
{
//  dev->set_option(rs2::option::color_enable_auto_white_balance,value);

}

bool RealSense2Interface::getAutoExposure()
{
//  return dev->get_option(rs2::option::color_enable_auto_exposure);
    return false;

}

bool RealSense2Interface::getAutoWhiteBalance()
{
//  return dev->get_option(rs2::option::color_enable_auto_white_balance);
    return false;
}
//#else
//
//RealSense2Interface::RealSense2Interface(int inWidth,int inHeight,int inFps)
//  : width(inWidth),
//  height(inHeight),
//  fps(inFps),
//  initSuccessful(false)
//{
//  errorText = "Compiled without Intel RealSense2 library";
//}
//
//RealSense2Interface::~RealSense2Interface()
//{
//}
//
//void RealSense2Interface::setAutoExposure(bool value)
//{
//}
//
//void RealSense2Interface::setAutoWhiteBalance(bool value)
//{
//}
//
//bool RealSense2Interface::getAutoExposure()
//{
//  return false;
//}
//
//bool RealSense2Interface::getAutoWhiteBalance()
//{
//  return false;
//}
//#endif