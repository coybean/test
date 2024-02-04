#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<vtkSmartPointer.h>
#include"CViewerImpl.h"
#include"CDoubleCameraDlg.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     CViewerImpl* m_pViewer;
    CDoubleCameraDlg* DCameraDlg;
private slots:
    void on_actionObjFile_obj_triggered();

    void on_actioncameraProperty_triggered();

    void on_actionsetupMoveTrace_2_triggered();

    void on_actionstartMove_triggered();

    void on_actionpauseMove_triggered();

    void on_actionstopMove_triggered();

    void updateCameraProperty();

    void on_action_FinishSetTrace_triggered();

    void on_actiondoubleCamera_2_triggered();

    void on_actionAMCap_triggered();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event);

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
