#include <cassert>
#include <fstream>
//#include "Features.h"
//#ifdef Q_WS_MAEMO_5
#include "MaemoUI.h"
//#endif
#include <opencv/highgui.h>
#include <opencv/cv.h>
#include <exception>
#include <QApplication>

void convertToFloatImage ( IplImage * byteImage, IplImage * floatImage )
{
    cvConvertScale ( byteImage, floatImage, 1 / 255., 0 );
}

class ObjdetectError : public std::exception
{
    std::string message;
public:
    ObjdetectError ( const std::string & msg, const std::string & file )
    {
        std::stringstream ss;

        ss << msg << " " << file;
        message = msg.c_str ();
    }
    ~ObjdetectError () throw ( )
    {
    }
};

IplImage * loadByteImage ( const char * name )
{
    IplImage * image = cvLoadImage ( name );

    if ( !image )
    {
        return 0;
    }
    cvCvtColor ( image, image, CV_BGR2RGB );
    return image;
}

IplImage * loadFloatImage ( const char * name )
{
    IplImage * image = cvLoadImage ( name );

    if ( !image )
    {
        return 0;
    }
    cvCvtColor ( image, image, CV_BGR2RGB );
    IplImage * floatingImage = cvCreateImage ( cvGetSize ( image ),
                                               IPL_DEPTH_32F, 3 );
    cvConvertScale ( image, floatingImage, 1 / 255., 0 );
    cvReleaseImage ( &image );
    return floatingImage;
}


int main ( int argc, char * * argv )
{
    QApplication app ( argc, argv );
    // Use the GUI.
    QWidget * ui = new MaemoUI;

    app.setActiveWindow ( ui );
    ui->show ();
    return app.exec ();
}
