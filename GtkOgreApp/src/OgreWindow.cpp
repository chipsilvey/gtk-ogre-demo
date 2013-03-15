/*
 * OgreWindow.cpp
 *
 *  Created on: Mar 9, 2013
 *      Author: chip
 */

#include "OgreWindow.h"

#include <iostream>

OgreWindow::OgreWindow( OgreWidget* ogreWidget ) :
	mExited( false )
{
	set_border_width( 10 );

	Glib::RefPtr<Gtk::Builder> gtkBuilder = Gtk::Builder::create_from_file( "buttons.glade" );

	gtkBuilder->get_widget( "playButton", mPlayButton );
	gtkBuilder->get_widget( "pauseButton", mPauseButton );

	//disables the pause button initially.
	mPauseButton->set_sensitive( false );

	//this is needed to make images show up on buttons for some reason.
	GtkSettings *default_settings = gtk_settings_get_default();
	g_object_set( default_settings, "gtk-button-images", TRUE, NULL );

	mVertBox = new Gtk::VBox( false, 10 );
	mHorzBox = new Gtk::HBox( false, 10 );

	double hScalemin = 0;
	double hScalemax = 101;
	double hScalestep = 1;
	mFrameSlider = new Gtk::HScale( hScalemin, hScalemax, hScalestep );

	mHorzBox->pack_start( *mPauseButton, true, true, 10 );
	mHorzBox->pack_start( *mPlayButton, true, true, 10 );

	mVertBox->pack_start( *ogreWidget, true, true, 10 );
	mVertBox->pack_start( *mHorzBox, true, true, 10 );
	mVertBox->pack_start( *mFrameSlider, true, true, 10 );

	add(*mVertBox);
}

OgreWindow::~OgreWindow()
{
	delete mPlayButton;
	delete mPauseButton;
	delete mVertBox;
	delete mHorzBox;
	delete mFrameSlider;
}

void OgreWindow::setPlayButtonEnabled( bool value )
{
	mPlayButton->set_sensitive( !value );
}

void OgreWindow::setPauseButtonEnabled( bool value )
{
	mPauseButton->set_sensitive( !value );
}

void OgreWindow::setFrameSliderPosition( double value )
{
	mFrameSlider->get_adjustment()->set_value( value );
}

bool OgreWindow::on_delete_event( GdkEventAny* event )
{
	mExited = true;
	return false;
}
