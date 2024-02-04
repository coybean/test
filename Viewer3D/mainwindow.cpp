#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QFileDialog>
#include<QDebug>
#include <QResizeEvent>
#include <QThread>
#include"CChooseActor.h"
#include<QProcess>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pViewer = new CViewerImpl;
    connect(&m_pViewer->style->m_Commu,SIGNAL(updateCameraProperty()),this,SLOT(updateCameraProperty()));
    m_pViewer->renWin->SetParentId((HWND)ui->widget->winId());
    m_pViewer->startRender();
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionObjFile_obj_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose Obj"), "", tr("Obj Files (*.obj)"));
    m_pViewer->addActor(fileName);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QSize windowSize = event->size();
    m_pViewer->renWin->SetSize(windowSize.width(),windowSize.height());
    m_pViewer->renWin->Render();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
        std::cout<<"dfd"<<std::endl;
        break;
    case Qt::Key_Down:
        break;
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    }
}

void MainWindow::on_actioncameraProperty_triggered()
{
    m_pViewer->m_pCamera->setCameraProperty(m_pViewer->m_pCamera->getCamera()->GetPosition(),m_pViewer->m_pCamera->getCamera()->GetViewUp(),
                                            m_pViewer->m_pCamera->getCamera()->GetFocalPoint(),m_pViewer->m_pCamera->getCamera()->GetViewAngle());
    m_pViewer->m_pCamera->show();
}

void MainWindow::on_actionsetupMoveTrace_2_triggered()
{
    CChooseActor* chooseActorWin = new CChooseActor(this);
    QStringList actorNames;
    for(auto i:m_pViewer->m_actors)
    {
        actorNames.append(i.actorName());
    }
    chooseActorWin->setComboboxActorsContext(actorNames);
    connect(chooseActorWin,SIGNAL(changeActor(int)),m_pViewer,SLOT(setCurOperateActorIndex(int)));
    chooseActorWin->show();
    ((CMyInteractorStyle*)m_pViewer->iren->GetInteractorStyle())->setTraceState(1);
}

void MainWindow::on_action_FinishSetTrace_triggered()
{
    m_pViewer->m_actors[m_pViewer->m_curOperateActorIndex].setRoute(((CMyInteractorStyle*)m_pViewer->iren->GetInteractorStyle())->m_routePoints);
    for(auto element: m_pViewer->m_actors[m_pViewer->m_curOperateActorIndex].getRoute() )
    {
        std::cout<<element.x<<", "<<element.y<<", "<<element.z<<std::endl;
    }
    ((CMyInteractorStyle*)m_pViewer->iren->GetInteractorStyle())->setTraceState(1);
}

#include<thread>


void MainWindow::on_actionstartMove_triggered()
{
//    std::cout<<"你好"<<std::endl;
//    std::thread t(&CViewerImpl::startMove,m_pViewer);
//    t.detach();

//    workThread->start();
    m_pViewer->startMove();
}


void MainWindow::on_actionpauseMove_triggered()
{

}


void MainWindow::on_actionstopMove_triggered()
{
    m_pViewer->stopMove();
}

void MainWindow::updateCameraProperty()
{
    m_pViewer->m_pCamera->setCameraProperty(m_pViewer->m_pCamera->getCamera()->GetPosition(),m_pViewer->m_pCamera->getCamera()->GetViewUp(),
                                            m_pViewer->m_pCamera->getCamera()->GetFocalPoint(),m_pViewer->m_pCamera->getCamera()->GetViewAngle());
}




void MainWindow::on_actiondoubleCamera_2_triggered()
{
    DCameraDlg=new CDoubleCameraDlg();
    DCameraDlg->exec();
}


void MainWindow::on_actionAMCap_triggered()
{
    QProcess::execute("D:/AMCap.exe");


}

