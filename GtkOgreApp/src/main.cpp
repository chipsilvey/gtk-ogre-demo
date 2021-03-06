/*
 * main.cpp
 *
 *  Created on: Mar 9, 2013
 *      Author: chip
 */

#include <gtkmm/main.h>
#include <gtkmm.h>

#include "OgreWidget.h"
#include "OgreWindow.h"
#include "DemoModel.h"

#include <time.h>

void connectControllerToModel( OgreWindow* controller, DemoModel* model )
{
	controller->mPlayButton->signal_clicked().connect( sigc::mem_fun( *model, &DemoModel::playRotation ) );
	controller->mPauseButton->signal_clicked().connect( sigc::mem_fun( *model, &DemoModel::pauseRotation ) );
	controller->mFrameSlider->signal_change_value().connect( sigc::mem_fun( *model, &DemoModel::FrameSliderChanged ) );

	model->mSig_SetPlayButtonEnabled.connect( sigc::mem_fun( *controller, &OgreWindow::setPlayButtonEnabled ) );
	model->mSig_SetPauseButtonEnabled.connect( sigc::mem_fun( *controller, &OgreWindow::setPauseButtonEnabled ) );
	model->mSig_SetFrameSlider.connect( sigc::mem_fun( *controller, &OgreWindow::setFrameSliderPosition ) );
}

void connectModelToView( DemoModel* model, OgreWidget* view )
{
	model->mSig_RotateCube.connect( sigc::mem_fun( *view, &OgreWidget::rotateCube ) );
	model->mSig_RenderFrame.connect( sigc::mem_fun( *view, &OgreWidget::renderFrame ) );
}

int main ( int argc, char *argv[] )
{
	Ogre::Root* ogreRoot = new Ogre::Root();

	if ( !ogreRoot->showConfigDialog() )
	{
		return -1;
	}

	ogreRoot->initialise( false );

	Gtk::Main gtkKit( argc, argv );

	// instances
	OgreWidget ogreWidget; // view
	OgreWindow ogreWindow( &ogreWidget ); // controller ( also view role )
	DemoModel  demoModel;  // model

	// subscriptions
	connectControllerToModel( &ogreWindow, &demoModel );
	connectModelToView( &demoModel, &ogreWidget );

	ogreWindow.show_all();

	Ogre::Timer* pTime = new Ogre::Timer();
	pTime->reset();

	double startTime = 0;
	double deltaTime = 0;

	while ( !ogreWindow.hasExited() )
	{
		startTime = pTime->getMillisecondsCPU();

		demoModel.Update( deltaTime );

		// blocks only when events are pending
		if ( gtkKit.events_pending() )
		{
			gtkKit.iteration();
		}
		else
		{
			gtkKit.iteration( false );
		}

		deltaTime = pTime->getMillisecondsCPU() - startTime;
	}

	std::cout << "ogreWindow has Exited\n";

	delete pTime;
	delete ogreRoot;
	
	return 0;
}

