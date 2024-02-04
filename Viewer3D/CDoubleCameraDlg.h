#ifndef CDOUBLECAMERADLG_H
#define CDOUBLECAMERADLG_H

#include <QDialog>
#include<opencv2/opencv.hpp>
#include <LenaDDIUtility.h>
#include"CDCameraReconImpl.h"
namespace Ui {
class CDoubleCameraDlg;
}

class CDoubleCameraDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CDoubleCameraDlg(QWidget *parent = nullptr);
    ~CDoubleCameraDlg();
public slots:
    void updateShowImage(QImage leftImg, QImage rightImg);
signals:
    void stopCapImage();
private:
    Ui::CDoubleCameraDlg *ui;
private:
    cv::Mat m_leftMat;
    cv::Mat m_rightMat;
    QImage m_leftImg;
    QImage m_rightImg;
    CDCameraReconImpl m_DCameraRecon;
    int imageIndex=0;
    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // CDOUBLECAMERADLG_H
