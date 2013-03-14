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

	this->setPlaying( true );
}

void DemoModel::pauseRotation()
{
	this->setPlaying( false );
}

bool DemoModel::FrameSliderChanged( Gtk::ScrollType scrollType, double value )
{
	if ( value > mFrameSliderMax )
	{
		value = mFrameSliderMax;
	}

	if ( mPlaying )
	{
		setPlaying( false );
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

			setFrameSlider();

			rotateCube();

			mDirty = true;
		}
		else
		{
			setPlaying( false );
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

void DemoModel::setPlaying( bool value )
{
	setPlayButtonEnabled( value );
	setPauseButtonEnabled( !value );

	this->mPlaying = value;
}

void DemoModel::rotateCube()
{
	this->mSig_RotateCube( mCurrentFrameNumber * ( mMaxCubeRotation / mEndFrameNumber ) );
}

void DemoModel::setPlayButtonEnabled( bool value )
{
	this->mSig_SetPlayButtonEnabled( value );
}

void DemoModel::setPauseButtonEnabled( bool value )
{
	this->mSig_SetPauseButtonEnabled( value );
}

void DemoModel::setFrameSlider()
{
	this->mSig_SetFrameSlider( mCurrentFrameNumber * ( 100 / mEndFrameNumber ) );
}
