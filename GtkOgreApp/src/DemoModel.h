/*
 * DemoModel.h
 *
 *  Created on: Mar 11, 2013
 *      Author: chip
 */

#ifndef DEMOMODEL_H_
#define DEMOMODEL_H_

#include <gtkmm.h>

class DemoModel
{
public:
	DemoModel();
	virtual ~DemoModel();

	void Update( double deltaTime );

	virtual bool FrameSliderChanged( Gtk::ScrollType scrollType, double value );
	virtual void playRotation(void);
	virtual void pauseRotation(void);

	sigc::signal< void, int > mSig_RotateCube;
	Glib::Dispatcher mSig_RenderFrame;

	sigc::signal< void, bool > mSig_SetPlayButtonEnabled;
	sigc::signal< void, bool > mSig_SetPauseButtonEnabled;
	sigc::signal< void, double > mSig_SetFrameSlider;

protected:

	virtual void updateView(void);

	virtual void setPlaying( bool value );

	virtual void rotateCube(void);
	virtual void setPlayButtonEnabled( bool value );
	virtual void setPauseButtonEnabled( bool value );
	virtual void setFrameSlider(void);

	// mFrameSliderMax is set on initialization.
	//	it would be better to pass or retrieve the value.
	double mFrameSliderMax;

	double mCurrentFrameNumber;
	double mNewFrameNumber;
	double mEndFrameNumber;

	double mMaxCubeRotation;

	double mTimeSinceLastFrame;
	double mTimeBetweenFrames;
	double mFramesPerSecond;

	bool mFirstRun;
	bool mPlaying;
	bool mDirty;
};

#endif /* DEMOMODEL_H_ */
