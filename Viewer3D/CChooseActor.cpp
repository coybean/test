#include "CChooseActor.h"
#include "ui_CChooseActor.h"
#include<iostream>
CChooseActor::CChooseActor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CChooseActor)
{
    ui->setupUi(this);
}

CChooseActor::~CChooseActor()
{
    delete ui;
}

void CChooseActor::setComboboxActorsContext(QStringList Names)
{

    ui->comboBox_Actors->addItems(Names);

}


void CChooseActor::on_buttonBox_accepted()
{
    int index = ui->comboBox_Actors->currentIndex();
    emit(changeActor(index));
}


void CChooseActor::on_buttonBox_rejected()
{
    int index = -1;
    emit(changeActor(index));
}

