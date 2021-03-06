#ifndef MAEMOUI_H
#define MAEMOUI_H

#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QString>
#include <map>
#include <opencv/cv.h>
#include <FCam/N900.h>
#include <QStackedWidget>
#include <QPainter>
#include "ObjDetect.h"
#include "Sender.h"

class Viewfinder;
class Sender;
//class FeaturesWizard;

enum MatchType { SSD = 1, Ratio };
enum FeatureType { Dummy = 1, MOPS, SURF };

// The MaemoUI class controls the Maemo.  Feel free to play around with
// the Maemo if you'd like.
class MaemoUI : public QMainWindow {
    Q_OBJECT

    std::map<QString, MatchType> matchTypes;
    std::map<QString, FeatureType> featureTypes;
    Viewfinder * viewFinder;
    QStackedWidget * stack;
    FeatureType featureType;
    MatchType matchType;
    ObjDetect * objdetect_widget;
    QPainter * painter;
    bool panorama;

public:
    // Create the Maemo.
    MaemoUI ( QWidget * = 0 );
    void activateViewfinder ();
    void deactivateViewfinder ();
    Viewfinder * getViewfinder();
    FeatureType getFeatureType ();
    MatchType getMatchType ();

    Sender * udpSender;

signals:
    void alert ( const QString & );

    // Here are the callback functions.
public slots:
    void updateFeatureType ( const QString & );
    void updateMatchType ( const QString & );
    void updateMode ( const QString & );
    void setFeatureTypeAction ();
    void setMatchTypeAction ();
    void setModeAction ();
    void pictureTaken ( const FCam::Frame & );
};

#endif
