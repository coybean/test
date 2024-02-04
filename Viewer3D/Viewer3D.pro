QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CCamera.cpp \
    CCameraCapThread.cpp \
    CChooseActor.cpp \
    CCommonFun.cpp \
    CDCameraReconImpl.cpp \
    CDoubleCameraDlg.cpp \
    CMyInteractorStyle.cpp \
    CViewer3D_Actor.cpp \
    CViewerImpl.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    AnimateActors.h \
    Animation/CAnimation.h \
    CCamera.h \
    CCameraCapThread.h \
    CChooseActor.h \
    CCommonFun.h \
    CDCameraReconImpl.h \
    CDoubleCameraDlg.h \
    CMyInteractorStyle.h \
    CViewer3D_Actor.h \
    CViewerImpl.h \
    Viewer3D_Common.h \
    mainwindow.h

FORMS += \
    CCamera.ui \
    CChooseActor.ui \
    CDoubleCameraDlg.ui \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release):\
LIBS += \
    $$PWD/../3rdParty/PCL_1.13.1/lib/*.lib\
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/Boost/lib/*.lib \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/FLANN/lib/*.lib \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/OpenNI2/Lib/*.lib \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/Qhull/lib/*.lib \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/VTK/lib/*.lib \
    $$PWD/../3rdParty/opencv460/x64/vc17/lib/*460.lib \
    $$PWD/../3rdParty/AI/ONNX/cpu/onnxruntime-win-x64-1.16.3/onnxruntime-win-x64-1.16.3/lib/*.lib\

else:win32:CONFIG(debug, debug|release):\
LIBS += \
    $$PWD/../3rdParty/PCL_1.13.1/lib/*d.lib\
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/Boost/lib/*d.lib \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/FLANN/lib/*d.lib \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/OpenNI2/Lib/*d.lib \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/Qhull/lib/*d.lib \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/VTK/lib/*d.lib \
    $$PWD/../3rdParty/opencv460/x64/vc17/lib/*460d.lib \
    $$PWD/../3rdParty/AI/ONNX/cpu/onnxruntime-win-x64-1.16.3/onnxruntime-win-x64-1.16.3/lib/*.lib\

INCLUDEPATH += \
    $$PWD/../3rdParty/PCL_1.13.1/include/pcl-1.13 \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/Eigen/eigen3 \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/VTK/include/vtk-9.2\
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/Boost/include/boost-1_82 \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/OpenNI2/Include \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/Qhull/include \
    $$PWD/../3rdParty/PCL_1.13.1/3rdParty/FLANN/include \
    $$PWD/../3rdParty/DCamera/eSPDI_DM/include \
    $$PWD/../3rdParty/DCamera/utility \
    $$PWD/../3rdParty/opencv460/include \
    $$PWD/../3rdParty/opencv460/include/opencv2 \
    $$PWD/../3rdParty/opencv460/include/opencv2 \
    $$PWD/../3rdParty/AI/ONNX/cpu/onnxruntime-win-x64-1.16.3/onnxruntime-win-x64-1.16.3/include\
