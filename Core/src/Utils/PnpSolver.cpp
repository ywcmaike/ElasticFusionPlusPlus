//
// Created by yeweicai on 18-3-30.
//

//#include "PnpSolver.h"
//
//cv::Point3f RESULT_OF_PNP::point2dTo3d(cv::Point3f& point, Intrinsics & intrinsics)
//{
//    cv::Point3f p;
//    double scale = 100.0;
//    p.z = double(point.z) / scale;
//    p.x = (point.x - intrinsics.cx()) * p.z / intrinsics.fx();
//    p.y = (point.y - intrinsics.cy()) * p.z / intrinsics.fy();
//    return p;
//}
//
//
//
//void RESULT_OF_PNP::estimateMotion(MyFrame& frame1, MyFrame& frame2, Intrinsics & intrinsics, RESULT_OF_PNP & result)
//{
//
//    std::vector<cv::DMatch> matches;
//    cv::BFMatcher matcher;
//    matcher.match(frame1.getDescriptor(), frame2.getDescriptor(), matches);
//
//    std::vector<cv::DMatch> goodMatches;
//    double minDis = 9999;
//    double good_match_threshold = 10;
//    for (size_t i = 0; i < matches.size(); i++)
//    {
//        if (matches[i].distance < minDis) {
//            minDis = matches[i].distance;
//        }
//    }
//
//    if (minDis < 10)
//    {
//        minDis = 10;
//    }
//    for (size_t i = 0; i < matches.size(); i++)
//    {
//        if (matches[i].distance < good_match_threshold*minDis)
//        {
//            goodMatches.push_back(matches[i]);
//        }
//    }
//
//    if (goodMatches.size() <= 5) {
//        result.inliers = -1;
//        return;
//    }
//    //第一个帧的三维点
//    std::vector<cv::Point3f> pts_obj;
//    //第二个帧的图像点
//    std::vector<cv::Point2f> pts_img;
//
//    //相机内参
//    for (size_t i = 0; i < goodMatches.size(); i++)
//    {
//        //query 是第一个，train是第二个
//        cv::Point2f p = frame1.getKeyPoint()[goodMatches[i].queryIdx].pt;
//        //获取d要小心，x是向右的，y是向下的，所以y才是行，x是列
//        ushort d = frame1.getDepth().ptr<ushort>(int(p.y))[int(p.x)];
//        if (d == 0)
//        {
//            continue;
//        }
//        pts_img.push_back(cv::Point2f(frame2.getKeyPoint()[goodMatches[i].trainIdx].pt));
//
//        //将(u,v,d)转成(x, y,z);
//        cv::Point3f pt(p.x, p.y, d);
//        cv::Point3f pd = point2dTo3d(pt, intrinsics);
//
//        pts_obj.push_back(pd);
//    }
//
//    if (pts_obj.size() == 0 || pts_img.size() == 0)
//    {
//        result.inliers = -1;
//        return;
//    }
//    double camera_matrix_data[3][3] = {
//            {intrinsics.fx(), 0, intrinsics.cx()},
//            {0, intrinsics.fy(), intrinsics.cy()},
//            {0, 0, 1}
//    };
//
//    cv::Mat cameraMatrix(3, 3, CV_64F, camera_matrix_data);
//    cv::Mat rvec, tvec, inliers;
//    cv::solvePnPRansac(pts_obj, pts_img, cameraMatrix, cv::Mat(), rvec, tvec, false, 100, 1.0, 100, inliers);
//
//    result.rvec = rvec;
//    result.tvec = tvec;
//    result.inliers = inliers.rows;
//
//    return;
//
//}
//
//double normTransform(cv::Mat rvec, cv::Mat tvec)
//{
//    return fabs(std::min(cv::norm(rvec), 2 * M_PI - cv::norm(rvec))) + fabs(cv::norm(tvec));
//}