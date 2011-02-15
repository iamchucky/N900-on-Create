#include "ObjDetect.h"
#include "N900Helpers.h"
#include <QBoxLayout>
#include "ObjDetectThread.h"
#include "MaemoUI.h"
#include <QPainter>
#include <QPicture>
#include <opencv/highgui.h>

ObjDetect::ObjDetect ( MaemoUI * parent )
    : QDialog ( parent ), img1 ( 0 ), img2 ( 0 ), ui ( parent ), isready ( false ),
    completed ( false )
{
    //  progressBar = new QProgressBar ( this );
    //  progressBar->setMaximum ( 100 );
    //  QHBoxLayout * layout = new QHBoxLayout ( this );
    //  layout->addWidget ( progressBar );
    //  setLayout ( layout );
    printf("[ObjDetect]: Constructor\n");
    arloc.initARtagPose(480,360,160.f,0.f,0.f,0.f);
    printf("[ObjDetect]: Constructor done\n");
}

ObjDetect::~ObjDetect()
{
    if ( img1 )
    {
        cvReleaseImage ( &img1 );
    }
    if ( img2 )
    {
        cvReleaseImage ( &img2 );
    }
}

void ObjDetect::emit_alert ( const QString & msg )
{
    emit alert ( msg );
}

bool ObjDetect::ready ()
{
    return isready;
}

void ObjDetect::mousePressEvent ( QMouseEvent * event )
{
    if ( completed )
    {
        ui->activateViewfinder ();
    }
    QDialog::mousePressEvent ( event );
}

void ObjDetect::imageCaptured ( const FCam::Frame & frame )
{
    IplImage * img = capture_rgb24 ( frame );
    printf("[imageCaptured]: I got here! yeye!\n");
    IplImage * gray = cvCreateImage(cvSize(img->width,img->height),IPL_DEPTH_8U,1);
    cvCvtColor(img,gray,CV_RGB2GRAY);

//    if ( !img1 )
//    {
        img1 = img;
//    }
//    else
//    {
//        img2 = img;
        isready = true;
        arloc.getARtagPose(gray,img1,0);
//        thread = new ObjDetectThread ( this, img1, img2,
//                                       ui->getFeatureType (), ui->getMatchType () );
//        //    QObject::connect ( thread, SIGNAL ( progressUpdate ( int ) ),
//        //                       progressBar, SLOT ( setValue ( int ) ) );
//        QObject::connect ( thread, SIGNAL ( finished () ),
//                           this, SLOT ( computed () ) );
//        thread->start ();
//    }

     cvReleaseImage(&gray);
     isready = true;
     completed = true;
     image = QImage ( ( const uchar * )img1->imageData,
                      img1->width,
                      img1->height,
                      img1->widthStep,
                      QImage::Format_RGB888 );
     canvas = new QLabel ( this );
     //  progressBar->hide ();
//     layout ()->addWidget ( canvas );
     QHBoxLayout * layout = new QHBoxLayout ( this );
     layout->addWidget ( canvas );
     setLayout ( layout );
     QPicture picture;
     QPainter painter;
     painter.begin ( &picture );
     painter.drawImage ( QRect ( QPoint ( 0, 0 ), QPoint ( 800, 600 ) ), image );
     painter.end ();
     canvas->setPicture ( picture );
     canvas->show ();
}

void ObjDetect::computed ()
{
    completed = true;
//    IplImage * output = thread->getComposite ();
//    IplImage * temp = cvCreateImage ( cvGetSize ( output ), IPL_DEPTH_8U, 3 );
//    cvConvertScale ( output, temp, 255, 0 );
//    cvReleaseImage ( &output );
//    output = temp;
    image = QImage ( ( const uchar * )img1->imageData,
                     img1->width,
                     img1->height,
                     img1->widthStep,
                     QImage::Format_RGB888 );
    canvas = new QLabel ( this );
    //  progressBar->hide ();
    layout ()->addWidget ( canvas );
    QPicture picture;
    QPainter painter;
    painter.begin ( &picture );
    painter.drawImage ( QRect ( QPoint ( 0, 0 ), QPoint ( 800, 600 ) ), image );
    painter.end ();
    canvas->setPicture ( picture );
    canvas->show ();
//    cvCvtColor ( output, output, CV_RGB2BGR );
//    std::stringstream ss;
//    ss << "/home/user/MyDocs/ " << rand () << ".png";
//    cvSaveImage ( ss.str ().c_str (), output );
}
