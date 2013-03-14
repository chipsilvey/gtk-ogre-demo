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

	//TODO: don't these need to be deleted in deconstructor? or does gtk take care of that?

	Gtk::VBox *vertBox = new Gtk::VBox( false, 10 );
	Gtk::HBox *horzBox = new Gtk::HBox( false, 10 );

	mBtnPlayRotateCube = new Gtk::Button( "Play Cube Rotation" );
	mBtnPauseRotateCube = new Gtk::Button( "Pause Cube Rotation" );

	double hScalemin = 0;
	double hScalemax = 101;
	double hScalestep = 1;
	mFrameSlider = new Gtk::HScale( hScalemin, hScalemax, hScalestep ); //value_changed or changed function/signal

	horzBox->pack_start( *mBtnPlayRotateCube, true, true, 10 );
	horzBox->pack_start( *mBtnPauseRotateCube, true, true, 10 );

	vertBox->pack_start( *ogreWidget, true, true, 10 );
	vertBox->pack_start( *horzBox, true, true, 10 );
	vertBox->pack_start( *mFrameSlider, true, true, 10 );

	add(*vertBox);
}

OgreWindow::~OgreWindow()
{
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

bool OgreWindow::on_key_press_event( GdkEventKey *event )
{
	std::cout << "keydown\n";
	return true;
}

bool OgreWindow::on_key_release_event( GdkEventKey *event )
{
	std::cout << "keyup\n";
	return true;
}
