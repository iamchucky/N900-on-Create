#ifndef ARTAGLOCALIZER_H
#define ARTAGLOCALIZER_H

#include "ARToolKitPlus/ARToolKitPlus.h"
#include "ARtag.h"

class ARtagLocalizer
{
public:
	ARtagLocalizer();
	~ARtagLocalizer();
	int initARtagPose(int width, int height, float markerWidth, float x_offset, float y_offset, float yaw_offset, float ffactor = 0.97);
	bool getARtagPose(IplImage * src, IplImage * dst, int camID);
	ARtag * getARtag(int index);
	int getARtagSize();
	void setARtagOffset(float x_offset, float y_offset, float yaw_offset);
	int cleanupARtagPose(void);

	static ARtag * tags[50];

	static bool allStop;
	
private:
	int imgwidth;
	int imgheight;
	bool useBCH;
	bool init;
	float xoffset;
	float yoffset;
	float yawoffset;
	float fudge;

	std::vector<ARtag> mytag;
	float patternWidth_;
	float patternCenter_[2];
	ARToolKitPlus::TrackerSingleMarker *tracker;
};
#endif
