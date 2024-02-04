#include "CViewer3D_Actor.h"
#include<vtkOBJImporter.h>
#include<vtkAssembly.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include<iostream>
CViewer3D_Actor::CViewer3D_Actor()
{
    m_actor = nullptr;
    m_actorName = "";
    m_id = -1;
}

bool CViewer3D_Actor::setActorFromFile(QString objFilePath)
{
    vtkNew<vtkOBJImporter> car;
    objFilePath.replace("/","\\");
    car->SetFileName(objFilePath.toStdString().data());
    car->SetFileNameMTL((objFilePath.left(objFilePath.length()-4)+".mtl").toStdString().data());
    car->SetTexturePath(objFilePath.left(objFilePath.lastIndexOf('\\')).toStdString().data());
    vtkNew<vtkRenderer>carRenderTemp;
    vtkNew<vtkRenderWindow> carRenWinTemp;
    carRenWinTemp->AddRenderer(carRenderTemp);
    car->SetRenderWindow(carRenWinTemp);
    car->Update();
    auto actors = carRenderTemp->GetActors();
    vtkNew<vtkAssembly>carActor;
    actors->InitTraversal();
    std::cout << "There are " << actors->GetNumberOfItems() << " actors" << std::endl;
    for (vtkIdType a = 0; a < actors->GetNumberOfItems(); ++a)
    {
        carActor->AddPart(actors->GetNextActor());
    }
    m_actor = carActor;
    m_actorName= objFilePath.mid(objFilePath.lastIndexOf('/')+1,objFilePath.lastIndexOf('.')-(objFilePath.lastIndexOf('/')+1));
    return true;
}

bool CViewer3D_Actor::setActor(vtkProp3D *actor)
{
    if(actor==nullptr)
        return false;
    this->m_actor = actor;
    return true;
}

void CViewer3D_Actor::addRoutePoint(Point3f point)
{
    m_route.append(point);
}

QVector<Point3f> CViewer3D_Actor::getRoute()
{
    return m_route;
}

void CViewer3D_Actor::setRoute(QVector<Point3f> route)
{
    m_route=route;
}

vtkProp3D *CViewer3D_Actor::getActor()
{
    return m_actor;
}

QString CViewer3D_Actor::actorName() const
{
    return m_actorName;
}

void CViewer3D_Actor::setActorName(const QString &newActorName)
{
    m_actorName = newActorName;
}

int CViewer3D_Actor::id() const
{
    return m_id;
}

void CViewer3D_Actor::setId(int newId)
{
    m_id = newId;
}
