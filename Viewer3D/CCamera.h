#ifndef CCAMERA_H
#define CCAMERA_H

#include <QDialog>
#include<vtkCamera.h>
#include<vtkSmartPointer.h>
namespace Ui {
class CCamera;
}

class CCamera : public QDialog
{
    Q_OBJECT

public:
    explicit CCamera(QWidget *parent = nullptr);
    ~CCamera();
    vtkSmartPointer<vtkCamera> getCamera() const;
    void setCamera(vtkSmartPointer<vtkCamera> newPCamera);
    void setCameraProperty(double posX,double posY,double posZ,double ViewUpX,
                           double ViewUpY,double ViewUpZ,double FocalX,double FocalY,double FocalZ,double ViewAngle);
//    void setCameraProperty(double *pos, double *ViewUp, double *Focal, double ViewAngle);
private:
    vtkSmartPointer<vtkCamera>m_pCamera;
public slots:
    void setCameraPosition(float x,float y,float z);
    void setCameraViewUp(float x,float y,float z);
    void setCameraFocalPoint(float x,float y,float z);
    void setCameraViewAngle(float radian);
    void setCameraProperty(double *pos, double *ViewUp, double *Focal, double ViewAngle);
private slots:
    void on_pushButton_Ok_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_Apply_clicked();
signals:
    void updateRenderWindow();
private:
    Ui::CCamera *ui;
};

#endif // CCAMERA_H
