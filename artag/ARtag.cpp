#include "ARtag.h"


ARtag::ARtag()  : id_(-1), pose_age_(0)
{
	pose_ = cvCreateMat( 4, 4, CV_32F);
}

ARtag::~ARtag()
{
}

void ARtag::setId(int id)
{
	id_ = id;
}

unsigned int ARtag::getId() const
{
	return id_;
}

void ARtag::setPose(CvMat * pose)
{
	for (int i = 0; i < pose->rows; ++i)
		for (int j = 0; j < pose->cols; ++j)
			CV_MAT_ELEM(*pose_ , float, i, j) = CV_MAT_ELEM(*pose , float, i, j);
}

CvMat * ARtag::getPose() const
{
	return pose_;
}

void ARtag::setPoseAge(unsigned int age)
{
	pose_age_ = age;
}

unsigned int ARtag::getPoseAge() const
{
	return pose_age_;
}

void ARtag::setCamId(int id)
{
	cam_id_ = id;
}

unsigned int ARtag::getCamId() const
{
	return cam_id_;
}