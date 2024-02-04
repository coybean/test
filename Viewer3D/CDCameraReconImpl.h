#ifndef CDCAMERARECONIMPL_H
#define CDCAMERARECONIMPL_H
#include<Windows.h>
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<vector>

class ONNXParserner;
class CDCameraReconImpl
{
public:
    CDCameraReconImpl();
    ~CDCameraReconImpl();
    void correctImage(cv::Mat srcImage,cv::Mat& dstImage);
    void genDepthImage(cv::Mat leftCorrectImg,cv::Mat rightCorrectImg,cv::Mat& depthImg);
    void genPlyImg(cv::Mat leftCorrectImg,cv::Mat depthImgFloat);
private:

    cv::Mat Klc; //左相机内参

    cv::Mat dist_Lcamera;//左相机畸变系数

    cv::Mat Krc;//右相机内参

    cv::Mat dist_Rcamera; //右相机畸变系数

    cv::Mat Rotate_L2R; //两相机旋转矩阵

    cv::Mat Trans_L2R;//两相机平移矩阵
    cv::Size ImgSize; //图像大小

     cv::Mat undistmap1l, undistmap2l, undistmap1r, undistmap2r;//左相机输出1/2映射，右相机输出1/2映射

    cv::Mat RL, RR, PL, PR, Q;//左相机校正旋转矩阵，右相机校正旋转矩阵，左相机校正平移矩阵，右相机校正平移矩阵，Q深度映射矩阵
    ONNXParserner* m_pOnnxparser;
};

#endif // CDCAMERARECONIMPL_H
