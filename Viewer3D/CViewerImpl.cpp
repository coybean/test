#include "CViewerImpl.h"

CViewerImpl::CViewerImpl()
{
    renderer= vtkSmartPointer<vtkRenderer> ::New();
    renWin=vtkSmartPointer<vtkRenderWindow> ::New();
    iren =  vtkSmartPointer<vtkRenderWindowInteractor>::New();
    style =vtkSmartPointer<CMyInteractorStyle>::New();
    m_pCamera = new CCamera(nullptr);
    m_pCamera->setCamera(renderer->GetActiveCamera());
    connect(m_pCamera,SIGNAL(updateRenderWindow()),this,SLOT(updateWindow()));


    Aniscene = vtkSmartPointer<vtkAnimationScene>::New();
    //scene->SetModeToRealTime();
    Aniscene->SetModeToSequence();
    Aniscene->SetLoop(0);

    Aniscene->SetFrameRate(30);
    Aniscene->SetStartTime(0);
    Aniscene->SetEndTime(20);
    Aniscene->SetAnimationTime(20);
    Aniscene->AddObserver(vtkCommand::AnimationCueTickEvent, renWin.GetPointer(), &vtkWindow::Render);
    Aniscene->AddObserver(vtkCommand::LeftButtonPressEvent, renWin.GetPointer(), &vtkWindow::Render);
    // Create an Animation Cue for each actor
    cue = vtkSmartPointer<vtkAnimationCue>::New();
    cue->SetStartTime(0);
    cue->SetEndTime(1);
    cue->SetTimeModeToRelative();
    cue->GetAnimationTime();
    Aniscene->AddCue(cue);
    animateSphere=new ActorAnimator;
    animateSphere->AddObserversToCue(cue);
}

int CViewerImpl::startRender()
{
    iren->SetRenderWindow(renWin);

    iren->SetInteractorStyle(style);
    renderer->UseHiddenLineRemovalOn();
    renWin->AddRenderer(renderer);
    renWin->SetSize(800, 600);
    renderer->SetBackground(0.5,0.5,0.5);
    renWin->Render();
    return 1;
}

int CViewerImpl::addActor(QString objFilePath)
{
    CViewer3D_Actor actor;
    actor.setActorFromFile(objFilePath);
    double* actorPos = actor.getActor()->GetPosition();
    double* actorcenterPos = actor.getActor()->GetCenter();
    vtkCamera* camera = m_pCamera->getCamera();
    camera->SetPosition(actorcenterPos[0]+5, actorcenterPos[1], actorcenterPos[2]+3);
    camera->SetViewUp(-1, 0, 0);
    camera->SetFocalPoint(actorcenterPos[0] - 50, actorcenterPos[1], actorcenterPos[2]);

    renderer->AddActor(actor.getActor());
    renWin->Render();
    actor.setId(m_actors.size());
    m_actors.push_back(actor);
    return 1;
}

int CViewerImpl::curOperateActorIndex() const
{
    return m_curOperateActorIndex;
}

void CViewerImpl::setCurOperateActorIndex(int newCurOperateActorIndex)
{
    m_curOperateActorIndex = newCurOperateActorIndex;
}

void CViewerImpl::updateWindow()
{
    renWin->Render();
}

void CViewerImpl::startMove()
{
//    animateSphere->SetActors(m_actors);
//    Aniscene->Play();

//    Aniscene->Stop();
    int count = 1;
    while(count<1000)
    {
        for(int i = 0;i<this->m_actors.size();i++)
        {
            if(this->m_actors[i].getRoute().size()==0)
                continue;
            vtkVector3d position;
            position.SetX((this->m_actors[i].getRoute()[0].x-this->m_actors[i].getActor()->GetCenter()[0]) * 0.001);
            position.SetY((this->m_actors[i].getRoute()[0].y-this->m_actors[i].getActor()->GetCenter()[1]) * 0.001);
            position.SetZ((this->m_actors[i].getRoute()[0].z-this->m_actors[i].getActor()->GetCenter()[2]) * 0.001);
            this->m_actors[i].getActor()->AddPosition(position.GetData());
        }
        this->renWin->Render();
        Sleep(20);
        count++;
    }
}

void CViewerImpl::pauseMove()
{

}
#include<thread>
void CViewerImpl::stopMove()
{
    std::thread t(&CViewerImpl::startMove,this);
    t.detach();
}
