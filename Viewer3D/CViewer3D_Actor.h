#ifndef CVIEWER3D_ACTOR_H
#define CVIEWER3D_ACTOR_H

#include<QVector>
#include<vtkSmartPointer.h>
#include<vtkProp3D.h>
#include"Viewer3D_Common.h"

class CViewer3D_Actor
{
public:
    explicit CViewer3D_Actor();
    bool setActorFromFile(QString filePath);
    bool setActor(vtkProp3D* actor);
    void addRoutePoint(Point3f point);
    QVector<Point3f> getRoute();
    void setRoute(QVector<Point3f>);
    vtkProp3D* getActor();

    QString actorName() const;
    void setActorName(const QString &newActorName);

    int id() const;
    void setId(int newId);

private:
    vtkSmartPointer<vtkProp3D> m_actor;
    QVector<Point3f> m_route;//actor运动路线
    QString m_actorName;
    int m_id;


};

#endif // CVIEWER3D_ACTOR_H
