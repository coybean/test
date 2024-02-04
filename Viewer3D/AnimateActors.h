#pragma once
#include <vtkActor.h>
#include <vtkAnimationCue.h>
#include <vtkCommand.h>
#include <vtkSmartPointer.h>
#include <vtkVector.h>
#include <vtkVectorOperators.h>
#include<vtkActorCollection.h>
#include<vtkAssembly.h>
#include<vtkCamera.h>
#include<vector>
#include"CViewer3D_Actor.h"
// int View;//0:  һ ˳  1       ˳
class ActorAnimator
{
public:
    ActorAnimator()
        : camera(nullptr)
        , StartPosition(0, 0, 0)
        , EndPosition(0.5, 0.5, 0.5)
    {
    }

    ~ActorAnimator() = default;

    void SetActors(std::vector<CViewer3D_Actor> actor) { this->Actors = actor; }
    
//    void SetStartPosition(const vtkVector3d& position)
//    {
//        this->StartPosition = position;
//    }
//    void SetEndPosition(const vtkVector3d& position)
//    {
//        this->EndPosition = position;
//    }

    void AddObserversToCue(vtkAnimationCue* cue)
    {
        cue->AddObserver(vtkCommand::StartAnimationCueEvent, this, &ActorAnimator::Start);
        cue->AddObserver(vtkCommand::EndAnimationCueEvent, this, &ActorAnimator::End);
        cue->AddObserver(vtkCommand::AnimationCueTickEvent, this, &ActorAnimator::Tick);
    }

private:
    //@{
    /**
     * These are callbacks that called when corresponding events are fired by the
     * cue (see AddObserversToCue)
     */
    void Start()
    {
        for(int i = 0;i<this->Actors.size();i++)
        {
            if(this->Actors[i].getRoute().size()==0)
                continue;
            double* position=this->Actors[i].getActor()->GetCenter();
            std::cout<<"actor pos: "<<position[0]<<", "<<position[1]<<", "<<position[2]<<std::endl;
            Point3f pos = this->Actors[i].getRoute()[0];
            std::cout<<"start pos: "<<pos.x<<", "<<pos.y<<", "<<pos.z<<std::endl;
        }

       setCamera(View);
    }

    void Tick(vtkObject* vtkNotUsed(caller), unsigned long vtkNotUsed(event), void* calldata)
    {
        vtkAnimationCue::AnimationCueInfo* info =
            reinterpret_cast<vtkAnimationCue::AnimationCueInfo*>(calldata);
        const double t = (info->AnimationTime - info->StartTime) /
            (info->EndTime - info->StartTime);
        for(int i = 0;i<this->Actors.size();i++)
        {
            if(this->Actors[i].getRoute().size()==0)
                continue;
            vtkVector3d position;
            position.SetX((this->Actors[i].getRoute()[0].x-this->Actors[i].getActor()->GetCenter()[0]) * t);
            position.SetY((this->Actors[i].getRoute()[0].y-this->Actors[i].getActor()->GetCenter()[1]) * t);
            position.SetZ((this->Actors[i].getRoute()[0].z-this->Actors[i].getActor()->GetCenter()[2]) * t);
            this->Actors[i].getActor()->AddPosition(position.GetData());
        }

       setCamera(View);

    }

    void End()
    {

        setCamera(View); 
    }

    void setCamera(int view)
    {
        if (cameraDirection == 0)
        {
            switch (view)
            {
            case 0:
            {
//                double* actorcenterPos = this->Actor->GetCenter();
//                camera->SetPosition(actorcenterPos[0] + 5, actorcenterPos[1], actorcenterPos[2] + 3);
//                camera->SetViewUp(-1, 0, 0);
//                camera->SetFocalPoint(actorcenterPos[0] - 50, actorcenterPos[1], actorcenterPos[2]);
                break;
            }
            case 1:
            {
//                double* actorPos = this->Actor->GetCenter();
//                camera->SetPosition(actorPos[0] - 10, actorPos[1], actorPos[2] + 200);
//                camera->SetViewUp(-1, 0, 0);
//                camera->SetFocalPoint(actorPos[0], actorPos[1], actorPos[2]);
                break;
            }
            case 2:
                break;
            default:
                break;
            }
        }
        if (cameraDirection == 1)
        {
            switch (view)
            {
            case 0:
            {
//                double* actorcenterPos = this->Actor->GetCenter();
//                camera->SetPosition(actorcenterPos[0] , actorcenterPos[1]+ 5, actorcenterPos[2] + 3);
//                camera->SetViewUp(0,-1,  0);
//                camera->SetFocalPoint(actorcenterPos[0] , actorcenterPos[1]- 50, actorcenterPos[2]);
                break;
            }
            case 1:
            {
//                double* actorPos = this->Actor->GetCenter();
//                camera->SetPosition(actorPos[0] , actorPos[1]- 10, actorPos[2] + 200);
//                camera->SetViewUp( 0,-1, 0);
//                camera->SetFocalPoint(actorPos[0], actorPos[1], actorPos[2]);
                break;
            }
            case 2:
                break;
            default:
                break;
            }
        }
        if (cameraDirection == 2)
        {
            switch (view)
            {
            case 0:
            {
//                double* actorcenterPos = this->Actor->GetCenter();
              
//                camera->SetPosition(actorcenterPos[0] - 5, actorcenterPos[1], actorcenterPos[2] + 3);
//                camera->SetViewUp(1, 0, 0);
//                camera->SetFocalPoint(actorcenterPos[0] + 50, actorcenterPos[1], actorcenterPos[2]);
                break;
            }
            case 1:
            {
//                double* actorPos = this->Actor->GetCenter();
//                camera->SetPosition(actorPos[0] + 10, actorPos[1], actorPos[2] + 200);
//                camera->SetViewUp(1, 0, 0);
//                camera->SetFocalPoint(actorPos[0], actorPos[1], actorPos[2]);
                break;
            }
            case 2:
                break;
            default:
                break;
            }
        }
        
    }
    //@}

    std::vector<CViewer3D_Actor> Actors;
    vtkVector3d StartPosition;
    vtkVector3d EndPosition;
public:
    vtkSmartPointer<vtkCamera>camera;
    int cameraDirection;
    int View;
};
