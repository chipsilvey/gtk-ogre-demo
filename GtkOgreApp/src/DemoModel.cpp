/*
 * DemoModel.cpp
 *
 *  Created on: Mar 11, 2013
 *      Author: chip
 */

#include "DemoModel.h"

#include <iostream>

DemoModel::DemoModel() :
mFrameSliderMax( 100 ),
mCurrentFrameNumber( 0 ), mNewFrameNumber( 0 ), mEndFrameNumber( 100 ), mMaxCubeRotation( 360 ),
mTimeSinceLastFrame( 0 ), mTimeBetweenFrames( 0 ), mFramesPerSecond( 30 ),
mFirstRun( true ), mPlaying( false ), mDirty( true )
{
	//assumes that deltaTime is passed to Update in milliseconds.
	mTimeBetweenFrames = 1000 / mFramesPerSecond;
}

DemoModel::~DemoModel()
{

}

//----public----

void DemoModel::playRotation()
{
	//replays from beginning if at end.
	if ( mCurrentFrameNumber >= mEndFrameNumber )
	{
		mCurrentFrameNumber = 0;
	}

	mPlaying = true;
}

void DemoModel::pauseRotation()
{
	mPlaying = false;
}

bool DemoModel::FrameSliderChanged( Gtk::ScrollType scrollType, double value )
{
	if ( value > mFrameSliderMax )
	{
		value = mFrameSliderMax;
	}

	if ( mPlaying )
	{
		mPlaying = false;
	}

	mNewFrameNumber = value * ( mEndFrameNumber / mFrameSliderMax );

	return true;
}

void DemoModel::Update( double deltaTime )
{
	mTimeSinceLastFrame = mTimeSinceLastFrame + deltaTime;

	if ( mTimeSinceLastFrame > mTimeBetweenFrames || mFirstRun )
	{
		if ( mFirstRun )
		{
			mFirstRun = false;
		}

		//TODO: would this be better or worse than setting to 0?
		//mTimeSinceLastFrame = mTimeSinceLastFrame - mTimeBetweenFrames;
		mTimeSinceLastFrame = 0;

		updateView();

		if ( mDirty )
		{
			this->mSig_RenderFrame();
			mDirty = false;
		}
	}
}

//----protected----

void DemoModel::updateView()
{
	if ( mPlaying )
	{
		if ( mCurrentFrameNumber < mEndFrameNumber )
		{
			mCurrentFrameNumber++;
			mNewFrameNumber = mCurrentFrameNumber;

			updateFrameSlider();

			rotateCube();

			mDirty = true;
		}
		else
		{
			mPlaying = false;
		}
	}
	else
	{
		if ( mCurrentFrameNumber != mNewFrameNumber )
		{
			mCurrentFrameNumber = mNewFrameNumber;

			rotateCube();

			mDirty = true;
		}
	}
}

void DemoModel::rotateCube()
{
	this->mSig_RotateCube( mCurrentFrameNumber * ( mMaxCubeRotation / mEndFrameNumber ) );
}

void DemoModel::updateFrameSlider()
{
	this->mSig_SetFrameSlider( mCurrentFrameNumber * ( 100 / mEndFrameNumber ) );
}
