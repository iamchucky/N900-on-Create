#ifndef ARTAG_H
#define ARTAG_H 
#include "opencv/cv.h"
#include "opencv/highgui.h"

class ARtag
{
	public:
		ARtag();
		virtual ~ARtag();

		void setId(int id);
		unsigned int getId() const;

		void setPose(CvMat * pose);
		CvMat * getPose() const;

		void setPoseAge(unsigned int age);
		unsigned int getPoseAge() const;

		void setCamId(int id);
		unsigned int getCamId() const;

                void setLocation(int u, int v);
                int getU() const;
                int getV() const;

	protected:
		unsigned int id_;
		CvMat * pose_;
		unsigned int pose_age_;
		unsigned int cam_id_;
                int u_;
                int v_;
};

#endif
