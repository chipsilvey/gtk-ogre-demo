/*
 * OgreWindow.h
 *
 *  Created on: Mar 9, 2013
 *      Author: chip
 */

#ifndef OGREWINDOW_H
#define OGREWINDOW_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/builder.h>

#include "OgreWidget.h"

// Controller
class OgreWindow : public Gtk::Window
{

public:
	OgreWindow( OgreWidget* ogreWidget );
	virtual ~OgreWindow();

	bool hasExited() { return mExited; }

	virtual void setPlayButtonEnabled( bool value );
	virtual void setPauseButtonEnabled( bool value );
	virtual void setFrameSliderPosition( double value );

	virtual bool on_delete_event( GdkEventAny* event );

	Gtk::Button* mPauseButton;
	Gtk::Button* mPlayButton;
	Gtk::HScale* mFrameSlider;

	Gtk::HBox* mHorzBox;
	Gtk::VBox* mVertBox;

protected:

	bool mExited;
};

#endif
