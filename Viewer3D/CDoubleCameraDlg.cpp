#include "CDoubleCameraDlg.h"
#include "ui_CDoubleCameraDlg.h"
#include "CCameraCapThread.h"
#include "CCommonFun.h"
#include<QDebug>
CDoubleCameraDlg::CDoubleCameraDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDoubleCameraDlg)
{
    ui->setupUi(this);
    ui->label_LeftImage->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);//解决图像显示后，窗口大小没法变化
    ui->label_RightImage->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    CCameraCapThread* capThread=new CCameraCapThread(nullptr);
    connect(capThread,SIGNAL(updateCapImage(QImage ,QImage )),this,SLOT(updateShowImage(QImage,QImage)));
    connect(this,SIGNAL(stopCapImage()),capThread,SLOT(setStopCapImageState()));
    capThread->start();
}

CDoubleCameraDlg::~CDoubleCameraDlg()
{

    delete ui;
}


void CDoubleCameraDlg::updateShowImage(QImage leftImg, QImage rightImg)
{
    imageIndex++;

    if(ui->checkBox_capImg->checkState()==Qt::CheckState::Checked)//拍摄原图选中
    {
        ui->label_LeftImage->setPixmap(QPixmap::fromImage(leftImg).scaled(ui->label_LeftImage->size(),Qt::KeepAspectRatio));
        ui->label_RightImage->setPixmap(QPixmap::fromImage(rightImg).scaled(ui->label_RightImage->size(),Qt::KeepAspectRatio));
        if(imageIndex%5!=0)
            return;
        cv::Mat left = CCommonFun::getInstance()->QImage2Mat(leftImg);
        cv::Mat right = CCommonFun::getInstance()->QImage2Mat(rightImg);
        cv::Mat correctLeft,correctRight;
        if(ui->checkBox_CorrectImg->checkState()==Qt::CheckState::Checked)
        {
            m_DCameraRecon.correctImage(left,correctLeft);
            m_DCameraRecon.correctImage(right,correctRight);

            ui->label_LeftCorrectImage->setPixmap(QPixmap::fromImage(CCommonFun::getInstance()->Mat2QImage(correctLeft)).scaled(ui->label_LeftImage->size(),Qt::KeepAspectRatio));
            ui->label_RightCorrectImage->setPixmap(QPixmap::fromImage(CCommonFun::getInstance()->Mat2QImage(correctRight)).scaled(ui->label_RightImage->size(),Qt::KeepAspectRatio));
        }

        if(imageIndex%30==0 && ui->checkBox_GenDepthImg->checkState()==Qt::CheckState::Checked)
        {
            cv::Mat depthImg;
            m_DCameraRecon.genDepthImage(correctLeft,correctRight,depthImg);
           // cv::imshow("depth",depthImg);
           // cv::waitKey(1000);
            ui->label_DepthImage->setPixmap(QPixmap::fromImage(CCommonFun::getInstance()->Mat2QImage(depthImg)).scaled(ui->label_RightImage->size(),Qt::KeepAspectRatio));
            qDebug()<<"ddd";
        }
        if(ui->checkBox_GenPlyImg->checkState()==Qt::CheckState::Checked)
        {

        }
    }

}

void CDoubleCameraDlg::closeEvent(QCloseEvent *event)
{
    emit(stopCapImage());
}

