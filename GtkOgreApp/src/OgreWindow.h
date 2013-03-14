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

#include "OgreWidget.h"

// Controller
class OgreWindow : public Gtk::Window
{

public:
	OgreWindow( OgreWidget* ogreWidget );
	virtual ~OgreWindow();

	bool hasExited() { return mExited; }

	virtual void setFrameSliderPosition( double value );

	virtual bool on_delete_event( GdkEventAny* event );
	virtual bool on_key_press_event( GdkEventKey *event );
	virtual bool on_key_release_event( GdkEventKey *event );

	Gtk::Button* mBtnPauseRotateCube;
	Gtk::Button* mBtnPlayRotateCube;
	Gtk::HScale* mFrameSlider;

protected:

	bool mExited;
};

#endif
