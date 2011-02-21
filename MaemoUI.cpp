#include "MaemoUI.h"
//#include "FeaturesWizard.h"
#include "Viewfinder.h"
#include <QFileDialog>
#include <QWidget>
#include <QMessageBox>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <sstream>
#include <iostream>
#include "N900Helpers.h"
#include "ObjDetect.h"
#include "mce/mode-names.h"
#include "mce/dbus-names.h"
#include <QtDBus>


// Create and initialize the Maemo.
MaemoUI::MaemoUI ( QWidget * parent ) : QMainWindow ( parent ),
featureType ( SURF ), matchType ( Ratio ), panorama ( false )
{
    QStatusBar * statusBar = new QStatusBar ( this );

    QObject::connect ( this, SIGNAL ( alert ( const QString & ) ),
                       statusBar, SLOT ( showMessage ( const QString & ) ) );
    setStatusBar ( statusBar );

    // Create the main window.
    setWindowTitle ( tr ( "ObjDetect" ) );

    stack = new QStackedWidget ( this );
    setCentralWidget ( stack );

    udpSender = new Sender;

//    // Create the menu bar.
//    QMenuBar * menuBar = new QMenuBar ( this );

//    QAction * setFeatureType = new QAction ( tr ( "Set Feature Type" ), menuBar );
//    QObject::connect ( setFeatureType, SIGNAL ( triggered () ),
//                       this, SLOT ( setFeatureTypeAction () ) );
//    menuBar->addAction ( setFeatureType );

//    QAction * setMatchType = new QAction ( tr ( "Set Match Type" ), menuBar );
//    QObject::connect ( setMatchType, SIGNAL ( triggered () ),
//                       this, SLOT ( setMatchTypeAction () ) );
//    menuBar->addAction ( setMatchType );

//    QAction * setMode = new QAction ( tr ( "Set Mode" ), menuBar );
//    QObject::connect ( setMode, SIGNAL ( triggered () ),
//                       this, SLOT ( setModeAction () ) );
//    menuBar->addAction ( setMode );

//    setMenuBar ( menuBar );

    viewFinder = new Viewfinder ( this );
    stack->addWidget ( viewFinder );

    featureTypes ["Dummy"] = Dummy;
    featureTypes ["MOPS"] = MOPS;
    featureTypes ["SURF"] = SURF;

    matchTypes ["SSD"] = SSD;
    matchTypes ["Ratio"] = Ratio;

    QObject::connect ( viewFinder, SIGNAL ( alert ( const QString & ) ),
                       statusBar, SLOT ( showMessage ( const QString & ) ) );
    activateViewfinder ();
}

void MaemoUI::activateViewfinder ()
{
    stack->setCurrentIndex ( 0 );
    qRegisterMetaType<FCam::Frame>( "FCam::Frame" );
    QObject::connect ( &CameraThread::getInstance (),
                       SIGNAL ( imageCaptured ( const FCam::Frame & ) ), this,
                       SLOT ( pictureTaken ( const FCam::Frame & ) ) );

    viewFinder->show ();
}

void MaemoUI::deactivateViewfinder ()
{
    QObject::disconnect ( &CameraThread::getInstance (),
                          SIGNAL ( imageCaptured ( const FCam::Frame & ) ), this,
                          SLOT ( pictureTaken ( const FCam::Frame & ) ) );
    stack->setCurrentIndex ( 1 );
}

Viewfinder * MaemoUI::getViewfinder()
{
//    printf("[MaemoUI] getViewfinder\n");
    return viewFinder;
}

FeatureType MaemoUI::getFeatureType ()
{
    return featureType;
}

MatchType MaemoUI::getMatchType ()
{
    return matchType;
}

void MaemoUI::setFeatureTypeAction ()
{
    QDialog * dialog = new QDialog ( this );

    dialog->setModal ( true );
    dialog->setWindowTitle ( tr ( "Feature Type" ) );
    QVBoxLayout * layout = new QVBoxLayout ( dialog );
    QPushButton * okay = new QPushButton ( tr ( "Okay" ), dialog );

    QComboBox * combobox = new QComboBox ( dialog );
    for ( std::map<QString, FeatureType>::iterator i = featureTypes.begin ();
    i != featureTypes.end (); ++i )
    {
        combobox->addItem ( i->first );
        if ( featureType == i->second )
        {
            combobox->setCurrentIndex ( combobox->count () - 1 );
        }
    }
    QObject::connect ( combobox, SIGNAL ( currentIndexChanged ( const QString & ) ),
                       this, SLOT ( updateFeatureType ( const QString & ) ) );
    layout->addWidget ( combobox );
    layout->addWidget ( okay );
    QObject::connect ( okay, SIGNAL ( clicked () ),
                       dialog, SLOT ( close () ) );
    dialog->setLayout ( layout );
    dialog->show ();
}

void MaemoUI::setModeAction ()
{
    QDialog * dialog = new QDialog ( this );

    dialog->setModal ( true );
    dialog->setWindowTitle ( tr ( "Mode" ) );
    QVBoxLayout * layout = new QVBoxLayout ( dialog );
    QPushButton * okay = new QPushButton ( tr ( "Okay" ), dialog );

    QComboBox * combobox = new QComboBox ( dialog );
    combobox->addItem ( "Object Detection" );
    combobox->addItem ( "Panorama" );
    combobox->setEditable ( false );
    combobox->setCurrentIndex ( panorama ? 1 : 0 );
    QObject::connect ( combobox, SIGNAL ( currentIndexChanged ( const QString & ) ),
                       this, SLOT ( updateMode ( const QString & ) ) );
    layout->addWidget ( combobox );
    layout->addWidget ( okay );
    QObject::connect ( okay, SIGNAL ( clicked () ),
                       dialog, SLOT ( close () ) );
    dialog->setLayout ( layout );
    dialog->show ();
}


void MaemoUI::updateFeatureType ( const QString & name )
{
    featureType = featureTypes [name];
    std::stringstream ss;
    ss << "Feature type set to " << name.toStdString ();
    emit alert ( tr ( ss.str ().c_str () ) );
}

void MaemoUI::updateMode ( const QString & name )
{
    panorama = ( name == "Panorama" );
    std::stringstream ss;
    ss << "Mode set to " << name.toStdString ();
    emit alert ( tr ( ss.str ().c_str () ) );
}

void MaemoUI::setMatchTypeAction ()
{
    QDialog * dialog = new QDialog ( this );

    dialog->setModal ( true );
    dialog->setWindowTitle ( tr ( "Match Type" ) );
    QVBoxLayout * layout = new QVBoxLayout ( dialog );
    QPushButton * okay = new QPushButton ( tr ( "Okay" ), dialog );

    QComboBox * combobox = new QComboBox ( dialog );
    for ( std::map<QString, MatchType>::iterator i = matchTypes.begin ();
    i != matchTypes.end (); ++i )
    {
        combobox->addItem ( i->first );
        if ( matchType == i->second )
        {
            combobox->setCurrentIndex ( combobox->count () - 1 );
        }
    }
    combobox->setEditable ( false );
    QObject::connect ( combobox, SIGNAL ( currentIndexChanged ( const QString & ) ),
                       this, SLOT ( updateMatchType ( const QString & ) ) );
    layout->addWidget ( combobox );
    layout->addWidget ( okay );
    QObject::connect ( okay, SIGNAL ( clicked () ),
                       dialog, SLOT ( close () ) );
    dialog->setLayout ( layout );
    dialog->show ();
}

void MaemoUI::updateMatchType ( const QString & name )
{
    matchType = matchTypes [name];
    std::stringstream ss;
    ss << "Match type set to " << name.toStdString ();
    emit alert ( tr ( ss.str ().c_str () ) );
}

void MaemoUI::pictureTaken ( const FCam::Frame & frame )
{
//    printf("[pictureTaken]: you got here!\n");
    QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(MCE_SERVICE,
    MCE_REQUEST_PATH,MCE_REQUEST_IF, MCE_PREVENT_BLANK_REQ));
    if ( stack->count () == 2 )
    {
        QWidget * widget = stack->widget ( 1 );
        stack->removeWidget ( widget );
    }
    //  if ( !panorama )
    //  {
    ////    QWidget * wizard = new FeaturesWizard ( this, frame );
    ////    stack->addWidget ( wizard );
    ////    QObject::connect ( wizard, SIGNAL ( alert ( const QString & ) ),
    ////                       statusBar (), SLOT ( showMessage ( const QString & ) ) );
    ////    wizard->show ();
    //    deactivateViewfinder ();
    //  }
    //  else
    //  {
//    if ( !objdetect_widget || objdetect_widget->ready () )
    if ( !objdetect_widget )
    {
//        printf("[pictureTaken]: create objdetect_widget\n");
        objdetect_widget = new ObjDetect ( this );
    }
//    printf("[pictureTaken]: going imageCaptured\n");
    objdetect_widget->imageCaptured ( frame );
//    printf("[pictureTaken]: done imageCaptured\n");
    if ( objdetect_widget->ready () )
    {
//        stack->addWidget ( objdetect_widget );
        QObject::connect ( objdetect_widget, SIGNAL ( alert ( const QString & ) ),
                           statusBar (), SLOT ( showMessage ( const QString & ) ) );
//        objdetect_widget->show ();
//        deactivateViewfinder ();
//        activateViewfinder ();
        this->viewFinder->repaint();
        objdetect_widget->unready();
        CameraThread::getInstance().okayToCapture();
    }
    //  }
}
