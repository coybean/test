#ifndef CCOMMONFUN_H
#define CCOMMONFUN_H

#include<opencv2/opencv.hpp>
#include<QImage>
class CCommonFun
{
private:
    CCommonFun();
public:
    static CCommonFun* getInstance();
    void Destory();
public:
    cv::Mat QImage2Mat(QImage image);
    QImage Mat2QImage(cv::Mat mat);
private:
    static CCommonFun* m_pSelf;
};

#endif // CCOMMONFUN_H
