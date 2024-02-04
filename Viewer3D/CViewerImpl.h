#ifndef CVIEWERIMPL_H
#define CVIEWERIMPL_H

#include <QWidget>
#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
#include <vtkOBJReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkPLYReader.h>
#include <string>
#include<vtkVertexGlyphFilter.h>
#include<vtkSTLReader.h>
#include <vtkImageReader2Factory.h>
#include<vtkImageReader2.h>
#include<vtkTexture.h>
#include<vtkJPEGReader.h>
#include <vtkTextureMapToCylinder.h>
#include <vtkTransformTextureCoords.h>
#include <vtkTextureMapToPlane.h>
#include <vtkNamedColors.h>
#include <vtkOBJImporter.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkAssembly.h>
#include <vtkAnimationScene.h>
#include<vtkAnimationCue.h>
#include "AnimateActors.h"
#include<vtkCamera.h>
#include<thread>
#include<Windows.h>
#include<vtkPointPicker.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include<vtkSphereSource.h>
#include<vtkPolyDataMapper.h>
#include<vtkRendererCollection.h>
#include<vtkProperty.h>
#include<vtkProp3D.h>
#include<QVector>
#include"CViewer3D_Actor.h"
#include"CCamera.h"
#include "CMyInteractorStyle.h"
using namespace std;

class CViewerImpl :public QWidget
{
    Q_OBJECT
public:
    CViewerImpl();
public:
    vtkSmartPointer<CMyInteractorStyle> style;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renWin;
    vtkSmartPointer<vtkRenderWindowInteractor>iren;

    vtkSmartPointer<vtkAnimationScene>Aniscene;
    vtkSmartPointer<vtkAnimationCue>cue;
    ActorAnimator* animateSphere;

    CCamera* m_pCamera;
public:
    int startRender();
    int addActor(QString objFilePath);

public:
    std::vector<CViewer3D_Actor> m_actors;
    int m_curOperateActorIndex;
    int curOperateActorIndex() const;
    void startMove();
    void pauseMove();
    void stopMove();
public slots:
    void setCurOperateActorIndex(int newCurOperateActorIndex);
    void updateWindow();
};

#endif // CVIEWERIMPL_H
