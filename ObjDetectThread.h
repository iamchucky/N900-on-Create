#ifndef OBJDETECT_THREAD_H
#define OBJDETECT_THREAD_H

#include <QThread>
#include <QWidget>
#include "MaemoUI.h"
#include <opencv/cv.h>
//#include "Progressable.h"

class ObjDetect;

class ObjDetectThread : public QThread//, public Progressable
{
Q_OBJECT

ObjDetect * objdetect;
IplImage * img1;
IplImage * img2;
IplImage * output;
FeatureType featureType;
MatchType matchType;

protected:
void run ();

public:
ObjDetectThread ( ObjDetect *, IplImage *, IplImage *, FeatureType, MatchType );
IplImage * getComposite ();

signals:
void progressUpdate ( int );
};

#endif
