#include "ARtagLocalizer.h"

#define FUDGE_FACTOR 0.97

using namespace std;

const float THIN_PATTERN_BORDER = 0.125;
const float THICK_PATTERN_BORDER = 0.25;

ARtag * ARtagLocalizer::tags[50];
bool ARtagLocalizer::allStop = false;

ARtagLocalizer::ARtagLocalizer()
{
    imgwidth = 640;
    imgheight = 480;
    init = false;
    useBCH = true;
    patternCenter_[0] = patternCenter_[1] = 0.0;
    patternWidth_ = 80.0;
    xoffset = 0;
    yoffset = 0;
    fudge = 0.97;
    for (int n = 0; n < 50; ++n)
    {
        tags[n] = new ARtag();
    }
    //	InitializeCriticalSection(&tags_mutex);
}

ARtagLocalizer::~ARtagLocalizer()
{
    for (int n = 0; n < 50; ++n)
    {
        delete tags[n];
    }
    //	DeleteCriticalSection (&tags_mutex);
}

int ARtagLocalizer::initARtagPose(int width, int height, float markerWidth, float x_offset, float y_offset, float yaw_offset, float ffactor)
{
    // create a tracker that does:
    //  - 6x6 sized marker images (required for binary markers)
    //  - samples at a maximum of 6x6
    //  - works with luminance (gray) images
    //  - can load a maximum of 0 non-binary pattern
    //  - can detect a maximum of 8 patterns in one imagege
    tracker = new ARToolKitPlus::TrackerSingleMarker(width, height, 8, 6, 6, 6, 0);

    imgwidth = width;
    imgheight = height;
    patternCenter_[0] = patternCenter_[1] = 0.0;
    xoffset = x_offset;
    yoffset = y_offset;
    yawoffset = yaw_offset;
    fudge = ffactor;

    tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);
    // load a camera file.
    //if(!tracker->init("..\\..\\ARToolKitPlus\\data\\Unibrain_640x480.cal", 1.0f, 1000.0f))
//    if(!tracker->init("..\\..\\ARToolKitPlus\\data\\Unibrain_640x4801.cal", 1.0f, 1000.0f))
    if(!tracker->init("no_distortion.cal", 1.0f, 1000.0f))
    {
        printf("ERROR: init() failed\n");
        delete tracker;
        return -1;
    }

    patternWidth_ = markerWidth;
    // define size of the marker
    tracker->setPatternWidth(patternWidth_);

    // the marker in the BCH test image has a thin border...
    tracker->setBorderWidth(THIN_PATTERN_BORDER);

    // set a threshold. alternatively we could also activate automatic thresholding
    tracker->setThreshold(150);

    // let's use lookup-table undistortion for high-speed
    // note: LUT only works with images up to 1024x1024
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

    // RPP is more robust than ARToolKit's standard pose estimator but uses more CPU resource
    // so using standard pose estimator instead
    tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_ORIGINAL);
    //tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);

    // switch to simple ID based markers
    // use the tool in tools/IdPatGen to generate markers
    tracker->setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);

    init = true;
    return 0;
}

bool ARtagLocalizer::getARtagPose(IplImage* src, IplImage* dst, int camID)
{
    if (!init)
    {
        printf("Did not initalize the ARtagLocalizer!!\n");
        return NULL;
    }
    if (src->width != imgwidth || src->height != imgheight)
    {
        printf("src->width: %d src->height %d\n", src->width, src->height);
        printf("imgwidth: %d imgheight %d\n", imgwidth, imgheight);
        printf("Image passed in does not match initialized image size!!\n");
        return NULL;
    }
    if (src->nChannels != 1)
    {
        printf("Please pass in grayscale image into ARtagLocalizer! \n");
        return NULL;
    }

    int numMarkers = 0;
    ARToolKitPlus::ARMarkerInfo* markers = NULL;
    if (tracker->arDetectMarker(const_cast<unsigned char*>((unsigned char*)src->imageData), 150, &markers, &numMarkers) < 0)
    {
        return false;
    }

    mytag.clear();

    float modelViewMatrix_[16];
    for(int m = 0; m < numMarkers; ++m) {
        if(markers[m].id != -1 && markers[m].cf >= 0.5) {
            tracker->calcOpenGLMatrixFromMarker(&markers[m], patternCenter_, patternWidth_, modelViewMatrix_);

            float x = modelViewMatrix_[12] / 1000.0;
            float y = modelViewMatrix_[13] / 1000.0;
            float z = modelViewMatrix_[14] / 1000.0;
            float yaw = -atan2(modelViewMatrix_[1], modelViewMatrix_[0]);
            if (yaw < 0)
            {
                yaw += 6.28;
            }

            if ((x == 0.0 && y == 0.0 && yaw == 0.0) || (x > 10000.0 && y > 10000.0) || (x < -10000.0 && y < -10000.0) || (z <= 0.001))
            {
                // ARTKPlus bug that occurs sometimes
                continue;
            }

//            printf("Id: %d\t Conf: %.2f\n", markers[m].id, markers[m].cf);
//			printf("x: %.2f \t y: %.2f \t z: %.2f \t yaw: %.2f\n", x,y,z,yaw);
//                        printf("\n");

//            char str[30];
//            sprintf(str,"%d",markers[m].id);
//            cvPutText (dst,str,cvPoint( markers[m].pos[0]+25,markers[m].pos[1]+10),&cvFont(3,3),cvScalar(255,0,0));
//            sprintf(str,"(%.2f,%.2f,%.2f)", x*fudge + xoffset, -(y*fudge + yoffset), yaw + yawoffset);
//            cvPutText (dst,str,cvPoint( markers[m].pos[0]+25,markers[m].pos[1]+25),&cvFont(1,1),cvScalar(255,0,0));

            cv::Mat PoseM(4, 4, CV_32F, modelViewMatrix_);
            cv::transpose(PoseM,PoseM);
            CvMat pose = PoseM;

            // save artag struct for access later
            if (markers[m].id >= 0 && markers[m].id < 50 && !allStop)
            {
                //				EnterCriticalSection(&tags_mutex);
//                ARtag * ar = tags[markers[m].id];
//                ar->setId(markers[m].id);
//                ar->setPose(&pose);
//                ar->setPoseAge(0);
//                ar->setCamId(camID);
//                ar->setLocation(markers[m].pos[0], markers[m].pos[1]);

                ARtag mt;
                mt.setId(markers[m].id);
                mt.setPose(&pose);
                mt.setPoseAge(0);
                mt.setCamId(camID);
                mt.setLocation(markers[m].pos[0], markers[m].pos[1]);
                //				LeaveCriticalSection(&tags_mutex);
                mytag.push_back(mt);
            }
        }
    }

    return true;
}

ARtag * ARtagLocalizer::getARtag(int index)
{
    return &mytag[index];
}

int ARtagLocalizer::getARtagSize()
{
    return mytag.size();
}

void ARtagLocalizer::setARtagOffset(float x_offset, float y_offset, float yaw_offset)
{
    xoffset = x_offset;
    yoffset = y_offset;
    yawoffset = yaw_offset;
}

int ARtagLocalizer::cleanupARtagPose(void)
{
    delete tracker;
    return 0;
}
