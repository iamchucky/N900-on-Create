#ifndef OBJDETECT_H
#define OBJDETECT_H

#include <QLabel>
#include <QDialog>
#include <QWidget>
#include <opencv/cv.h>
#include <FCam/N900.h>
#include <QProgressBar>
#include <QImage>
#include <QMouseEvent>
#include "artag/ARtagLocalizer.h"

class ObjDetectThread;
class MaemoUI;
class ARtag;
class ARtagLocalizer;

class ObjDetect : public QDialog
{
    Q_OBJECT

    IplImage * img1;
    IplImage * img2;
    MaemoUI * ui;
    QProgressBar * progressBar;
    ObjDetectThread * thread;
    bool isready;
    bool completed;
    QImage image;
    QLabel * canvas;

    ARtagLocalizer arloc;

    void mousePressEvent ( QMouseEvent * );

public:
    ObjDetect ( MaemoUI * );
    ~ObjDetect();
    bool ready ();
    void emit_alert ( const QString & );

signals:
    void alert ( const QString & );
    void progressUpdate ( int );

public slots:
    void imageCaptured ( const FCam::Frame & );
    void computed ();
};

#endif
