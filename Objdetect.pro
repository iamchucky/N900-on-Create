#####################################################################
# Automatically generated by qmake (2.01a) Tue Sep 21 01:40:19 2010
######################################################################

TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += /home/cs4670/FCam/include ARToolKitPlus/include .
CONFIG += debug warn_on qdbus
LIBS += -L/usr/local/lib -lopencv_features2d -lopencv_imgproc -lopencv_core -lopencv_highgui -lopencv_calib3d -lopencv_video -lopencv_ml -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann -L/usr/local/share/opencv/3rdparty/lib/ -llibjasper -lopencv_lapack -lzlib -ltiff -ljpeg -lpng -lpthread -lrt -L.
QT += network

# Input
HEADERS += CameraThread.h \
           N900Helpers.h \
           MaemoUI.h \
           Viewfinder.h \
           artag/ARtagLocalizer.h \
           artag/ARtag.h \
           ObjDetectThread.h \
           ObjDetect.h \
    Sender.h


SOURCES += MaemoUI.cpp \
           main.cpp \
           N900Helpers.cpp \
           CameraThread.cpp \
           Viewfinder.cpp \
           artag/ARtagLocalizer.cpp \
           artag/ARtag.cpp \
           ObjDetect.cpp \
           ObjDetectThread.cpp \
    Sender.cpp



LIBS += -L/home/cs4670/FCam -lFCam -LARToolKitPlus/lib -lARToolKitPlus
