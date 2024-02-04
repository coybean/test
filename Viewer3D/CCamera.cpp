#include "CCamera.h"
#include "ui_CCamera.h"

CCamera::CCamera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CCamera)
{
    ui->setupUi(this);
    m_pCamera = nullptr;
}

CCamera::~CCamera()
{
    delete ui;
}

vtkSmartPointer<vtkCamera> CCamera::getCamera() const
{
    return m_pCamera;
}

void CCamera::setCamera(vtkSmartPointer<vtkCamera> newPCamera)
{
    m_pCamera = newPCamera;
}

void CCamera::setCameraProperty(double posX, double posY, double posZ,
                                double ViewUpX, double ViewUpY, double ViewUpZ, double FocalX, double FocalY, double FocalZ, double ViewAngle)
{
    ui->lineEdit_CameraPosX->setText(QString::number(posX));
    ui->lineEdit_CameraPosY->setText(QString::number(posY));
    ui->lineEdit_CameraPosZ->setText(QString::number(posZ));

    ui->lineEdit_CameraViewUpX->setText(QString::number(ViewUpX));
    ui->lineEdit_CameraViewUpY->setText(QString::number(ViewUpY));
    ui->lineEdit_CameraViewUpZ->setText(QString::number(ViewUpZ));

    ui->lineEdit_CameraFocusX->setText(QString::number(FocalX));
    ui->lineEdit_CameraFocusY->setText(QString::number(FocalY));
    ui->lineEdit_CameraFocusZ->setText(QString::number(FocalZ));
}

void CCamera::setCameraProperty(double* pos, double* ViewUp, double* Focal, double ViewAngle)
{
    ui->lineEdit_CameraPosX->setText(QString::number(pos[0]));
    ui->lineEdit_CameraPosY->setText(QString::number(pos[1]));
    ui->lineEdit_CameraPosZ->setText(QString::number(pos[2]));

    ui->lineEdit_CameraViewUpX->setText(QString::number(ViewUp[0]));
    ui->lineEdit_CameraViewUpY->setText(QString::number(ViewUp[1]));
    ui->lineEdit_CameraViewUpZ->setText(QString::number(ViewUp[2]));

    ui->lineEdit_CameraFocusX->setText(QString::number(Focal[0]));
    ui->lineEdit_CameraFocusY->setText(QString::number(Focal[1]));
    ui->lineEdit_CameraFocusZ->setText(QString::number(Focal[2]));
}

void CCamera::setCameraPosition(float x, float y, float z)
{
    m_pCamera->SetPosition(x,y,z);
}

void CCamera::setCameraViewUp(float x, float y, float z)
{
    m_pCamera->SetViewUp(x,y,z);
}

void CCamera::setCameraFocalPoint(float x, float y, float z)
{
    m_pCamera->SetFocalPoint(x,y,z);
}

void CCamera::setCameraViewAngle(float radian)
{
    m_pCamera->SetViewAngle(radian);
}

void CCamera::on_pushButton_Ok_clicked()
{
    m_pCamera->SetPosition(ui->lineEdit_CameraPosX->text().toDouble(),ui->lineEdit_CameraPosY->text().toDouble(),ui->lineEdit_CameraPosZ->text().toDouble());
    m_pCamera->SetViewUp(ui->lineEdit_CameraViewUpX->text().toDouble(),ui->lineEdit_CameraViewUpY->text().toDouble(),ui->lineEdit_CameraViewUpZ->text().toDouble());
    m_pCamera->SetFocalPoint(ui->lineEdit_CameraFocusX->text().toDouble(),ui->lineEdit_CameraFocusY->text().toDouble(),ui->lineEdit_CameraFocusZ->text().toDouble());
    emit(updateRenderWindow());
    this->close();
}


void CCamera::on_pushButton_Cancel_clicked()
{
    setCameraProperty(m_pCamera->GetPosition(), m_pCamera->GetViewUp(), m_pCamera->GetFocalPoint(), m_pCamera->GetViewAngle());
}


void CCamera::on_pushButton_Apply_clicked()
{
    m_pCamera->SetPosition(ui->lineEdit_CameraPosX->text().toDouble(),ui->lineEdit_CameraPosY->text().toDouble(),ui->lineEdit_CameraPosZ->text().toDouble());
    m_pCamera->SetViewUp(ui->lineEdit_CameraViewUpX->text().toDouble(),ui->lineEdit_CameraViewUpY->text().toDouble(),ui->lineEdit_CameraViewUpZ->text().toDouble());
    m_pCamera->SetFocalPoint(ui->lineEdit_CameraFocusX->text().toDouble(),ui->lineEdit_CameraFocusY->text().toDouble(),ui->lineEdit_CameraFocusZ->text().toDouble());
    emit(updateRenderWindow());
}

