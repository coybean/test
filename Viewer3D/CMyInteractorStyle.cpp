#include "CMyInteractorStyle.h"

#include <vtkCamera.h>

vtkStandardNewMacro(CMyInteractorStyle);

void CMyInteractorStyle::OnLeftButtonDown()
{
    //打印鼠标左键像素位置
//    std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0]
//              << " " << this->Interactor->GetEventPosition()[1] << std::endl;
    //注册拾取点函数
    this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],this->Interactor->GetEventPosition()[1],
                                        0, this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()
                                        );
    //打印拾取点空间位置
    double picked[3];
    this->Interactor->GetPicker()->GetPickPosition(picked);
    std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
    //对拾取点进行标记
    vtkSmartPointer<vtkSphereSource> sphereSource =
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();
    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);
    actor->SetPosition(picked);
    actor->SetScale(0.3);
    actor->GetProperty()->SetColor(1.0, 0.0, 0.0);
    this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(actor);
    emit(m_Commu.updateCameraProperty());
    if(m_traceState==TraceState::ON)
    {
        m_tempPoint[0] =picked[0];
        m_tempPoint[1] =picked[1];
        m_tempPoint[2] =picked[2];
    }
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void CMyInteractorStyle::OnMouseMove()
{
    this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],this->Interactor->GetEventPosition()[1],
                                        0, this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()
                                        );
    //打印拾取点空间位置
    double picked[3];
    this->Interactor->GetPicker()->GetPickPosition(picked);
   // std::cout << "Picked value: " << this->Interactor->GetEventPosition()[0]<<" "<<this->Interactor->GetEventPosition()[1]<<"   "<<picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
    emit(m_Commu.updateCameraProperty());
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void CMyInteractorStyle::OnLeftButtonUp()
{
    emit(m_Commu.updateCameraProperty());
    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
}

void CMyInteractorStyle::OnMiddleButtonDown()
{
    emit(m_Commu.updateCameraProperty());
    vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}

void CMyInteractorStyle::OnMiddleButtonUp()
{
    emit(m_Commu.updateCameraProperty());
    vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
}

void CMyInteractorStyle::OnRightButtonDown()
{
    if(m_traceState==TraceState::ON)
    {
        m_routePoints.push_back(Point3f(m_tempPoint[0],m_tempPoint[1],m_tempPoint[2]));
    }
    emit(m_Commu.updateCameraProperty());
    vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}

void CMyInteractorStyle::OnRightButtonUp()
{
    emit(m_Commu.updateCameraProperty());
    vtkInteractorStyleTrackballCamera::OnRightButtonUp();
}

void CMyInteractorStyle::OnMouseWheelForward()
{

    emit(m_Commu.updateCameraProperty());
    vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
}

void CMyInteractorStyle::OnMouseWheelBackward()
{
    emit(m_Commu.updateCameraProperty());
    vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
}

void CMyInteractorStyle::setTraceState(int state)
{
    if(state = 1)
        m_traceState = TraceState::ON;
    else
        m_traceState = TraceState::OFF;

}

