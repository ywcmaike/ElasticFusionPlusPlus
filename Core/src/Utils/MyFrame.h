//
// Created by yeweicai on 18-3-30.
//

#ifndef ELASTICFUSION_MYFRAME_H
#define ELASTICFUSION_MYFRAME_H

#include <cassert>


#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <opencv2/core/eigen.hpp>


class MyFrame
{
public:
    int height, width;
    cv::Mat rgb, depth;
    cv::Mat descriptor;
    std::vector<cv::KeyPoint> keyPoint;
//
//    MyFrame(int width, int height, cv::Mat rgb, cv::Mat depth, cv::Mat descriptor, std::vector<cv::KeyPoint> keyPoint)
//    : width(width), height(height), rgb(rgb), depth(depth), descriptor(descriptor), keyPoint(keyPoint) {
//        assert(width > 0 && height > 0 && "You haven't initialised the Resolution class!");
//    }
//public:
//    void setRGB(cv::Mat _rgb) {
//        rgb = _rgb;
//    }
//    cv::Mat getRGB() {
//        return rgb;
//    }
//    void setDepth(cv::Mat _depth) {
//        depth = _depth;
//    }
//    cv::Mat getDepth() {
//        return depth;
//    }
//    void setDescriptor(cv::Mat _descriptor) {
//        descriptor = _descriptor;
//    }
//    cv::Mat getDescriptor() {
//        return descriptor;
//    }
//    void setKeyPoint(std::vector<cv::KeyPoint> _keyPoint) {
//        keyPoint = _keyPoint;
//    }
//    std::vector<cv::KeyPoint> getKeyPoint() {
//        return keyPoint;
//    }
//    void setHeight(int _height) {
//        height = _height;
//    }
//    int getHeight() {
//        return height;
//    }
//    void setWidth(int _width) {
//        width = _width;
//    }
//    int getWidth() {
//        return width;
//    }

};





#endif //ELASTICFUSION_MYFRAME_H
