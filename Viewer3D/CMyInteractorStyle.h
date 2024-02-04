#ifndef CMYINTERACTORSTYLE_H
#define CMYINTERACTORSTYLE_H

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkActor.h>
#include <vtkOutlineFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include<vtkProperty.h>
#include<vtkRenderer.h>
#include<vtkRendererCollection.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkAbstractPicker.h>
#include<QObject>
#include"Viewer3D_Common.h"
#include<QVector.h>
class Commu:public QObject
{
    Q_OBJECT
public:
    Commu(QObject *parent) : QObject(parent){}
    Commu(){}
signals:
    void updateCameraProperty();
};

class CMyInteractorStyle : public vtkInteractorStyleTrackballCamera
{

public:
    static CMyInteractorStyle* New();
    vtkTypeMacro(CMyInteractorStyle, vtkInteractorStyleTrackballCamera);
protected:
    CMyInteractorStyle(){}
    ~CMyInteractorStyle() override{}
    // vtkInteractorStyle interface
public:
    void OnLeftButtonDown();
    void OnMouseMove();
    void OnLeftButtonUp();
    void OnMiddleButtonDown();
    void OnMiddleButtonUp();
    void OnRightButtonDown();
    void OnRightButtonUp();
    void OnMouseWheelForward();
    void OnMouseWheelBackward();
    Commu m_Commu;

public:
    void setTraceState(int);
     QVector<Point3f> m_routePoints;
private:
    enum TraceState{OFF,ON};
    TraceState m_traceState;

    double m_tempPoint[3];//临时点击的点，需要右键按下进行确认是否加入m_routePoints中
    // vtkInteractorStyle interface
public:
};


#endif
