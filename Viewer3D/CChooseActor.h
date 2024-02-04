#ifndef CCHOOSEACTOR_H
#define CCHOOSEACTOR_H

#include <QDialog>

namespace Ui {
class CChooseActor;
}

class CChooseActor : public QDialog
{
    Q_OBJECT

public:
    explicit CChooseActor(QWidget *parent = nullptr);
    ~CChooseActor();
    void setComboboxActorsContext(QStringList Names);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

signals:
    void changeActor(int index);
private:
    Ui::CChooseActor *ui;
};

#endif // CCHOOSEACTOR_H
