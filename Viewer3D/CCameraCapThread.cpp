#include "CCameraCapThread.h"
#include<QDebug>
#include"CCommonFun.h"

CCameraCapThread::CCameraCapThread(QThread *parent)
    : QThread{parent}
{
    isStopCap=false;
}

void CCameraCapThread::setStopCapImageState()
{
    isStopCap = true;
}

void CCameraCapThread::run()
{
    cv::VideoCapture cap;
    int Image_Width = 1280;
    int Image_height = 720;
    cap.open(0);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, Image_Width * 2); //设置捕获图像的宽度，为双目图像的宽度
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, Image_height);  //设置捕获图像的高度
    if(!cap.isOpened())
        return;
    while(!isStopCap)
    {
        cv::Mat frame;
        cap>>frame;
        if(frame.empty())
            return;
        cv::Mat leftImg = frame.colRange(0,1280);
        cv::Mat rightImg = frame.colRange(1280,2560);
       // resize(leftImg,leftImg,cv::Size(640,360));
        //resize(rightImg,rightImg,cv::Size(640,360));
        emit(updateCapImage(CCommonFun::getInstance()->Mat2QImage(leftImg),
                            CCommonFun::getInstance()->Mat2QImage(rightImg)));
        cv::waitKey(30);
    }
    cap.release();
}
