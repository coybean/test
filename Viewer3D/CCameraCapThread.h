#ifndef CCAMERACAPTHREAD_H
#define CCAMERACAPTHREAD_H

#include <QThread>
#include<QImage>
class CCameraCapThread : public QThread
{
    Q_OBJECT
public:
    explicit CCameraCapThread(QThread *parent = nullptr);
private:
    bool isStopCap;
signals:
    void updateCapImage(QImage,QImage);
public slots:
    void setStopCapImageState();
    // QThread interface
protected:
    void run();
};

#endif // CCAMERACAPTHREAD_H
