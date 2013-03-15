/*
 * OgreWidget.cpp
 *
 *  Created on: Mar 9, 2013
 *      Author: chip
 */

#include "OgreWidget.h"

#include <gdkmm/drawable.h>
#include <gdkmm/general.h>

#include <gdk/gdkx.h>

#include <glibmm/objectbase.h>

#include <iostream>
#include <cstring>

#include <OgreSphere.h>
#include <OgreVector3.h>

#ifdef WIN32
   #include <gdk/gdkwin32.h>
#endif

//TODO why can eclipse not resolve Glib::ObjectBase("ogrewidget"), ObjectBase necessary for gobj();
OgreWidget::OgreWidget() :
	Glib::ObjectBase( "ogrewidget" ), Gtk::Widget(),
	mRenderWindow( 0 ), mSceneMgr( 0 ), mViewport( 0 ), mCamera( 0 ),
	mResourcesCfg( "resources.cfg" ), mUseResourcesCfg( false ),
	mVerticalSync( true )
{
  set_flags( Gtk::NO_WINDOW ); //depreciated

  std::cout << "GType name: " << G_OBJECT_TYPE_NAME( gobj() ) << std::endl;
}

OgreWidget::~OgreWidget()
{
}

//----Public----

void OgreWidget::renderFrame(void)
{
	if ( mVerticalSync )
	{
		mRenderWindow->update( false );
		mRenderWindow->swapBuffers( true );
	}
	else
	{
		mRenderWindow->update();
	}
}

void OgreWidget::setBackgroundColor( int r, int g, int b )
{
	this->mViewport->setBackgroundColour( Ogre::ColourValue( r, g, b ) );
}

void OgreWidget::rotateCube( double rotation )
{
	Ogre::SceneNode* cubeNode = mSceneMgr->getSceneNode( "cubeNode" );

	cubeNode->setOrientation( cubeNode->getInitialOrientation() );

	Ogre::Quaternion cubeQuat = Ogre::Quaternion( Ogre::Degree( rotation ), Ogre::Vector3::UNIT_Y );
	cubeNode->setOrientation( cubeQuat );
}

//----Ogre setup----

void OgreWidget::createScene()
{
	//adds resources from resources.cfg to ResourceGroupManager
	if ( mUseResourcesCfg )
	{
		setupResources();
	}

	// Set default mipmap level & texture filtering
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps( 5 );
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering( Ogre::TFO_TRILINEAR );

	// Create scene manager
	mSceneMgr = Ogre::Root::getSingletonPtr()->createSceneManager( Ogre::ST_GENERIC, "SceneManager" );

	initCamera();
	initViewport();

	// Set the scene's ambient light
	mSceneMgr->setAmbientLight( Ogre::ColourValue( 0.5f, 0.5f, 0.5f ) );

	//TODO: trying to set materials up.  not sure how to apply materials to an entity
	Ogre::MaterialPtr cube_material_ptr =
			Ogre::MaterialManager::getSingleton().create(
					"checker.png",
					Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );

	//loads all resources in ResourceGroupManager
	loadResources();

	Ogre::Pass *pass = cube_material_ptr->getTechnique(0)->getPass(0);
	pass->setDiffuse( Ogre::ColourValue( 1.0, 0.2, 0.2 ) );

	Ogre::Entity* cube = mSceneMgr->createEntity( "cube", Ogre::SceneManager::PT_CUBE );
	Ogre::SceneNode* cubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "cubeNode" );
	cubeNode->setScale( 0.3, 0.3, 0.3 );
	cube->setMaterialName( "checker.png" ); //not working
	cubeNode->attachObject( cube );

	// Create a Light and set its position
	Ogre::Light* light = mSceneMgr->createLight( "MainLight" );
	light->setPosition(20.0f, 80.0f, 50.0f);
}

void OgreWidget::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load( mResourcesCfg );

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while ( seci.hasMoreElements() )
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for ( i = settings->begin(); i != settings->end(); ++i )
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName );
        }
    }
}

void OgreWidget::loadResources(void)
{
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void OgreWidget::initCamera(void)
{
	// Create the camera
	mCamera = mSceneMgr->createCamera( "Camera" );

	mCamera->setPosition( Ogre::Vector3( 0, 30, 120 ) );

	// Look back along -Z and slightly down (-Y)
	mCamera->lookAt( Ogre::Vector3( 0, -60, -300 ) );
	mCamera->setNearClipDistance( 5 );
}

void OgreWidget::initViewport(void)
{
	// Create one viewport, entire window
	mViewport = mRenderWindow->addViewport( mCamera );

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio( Ogre::Real( mViewport->getActualWidth() ) / Ogre::Real( mViewport->getActualHeight() ) );

	mViewport->setAutoUpdated( true );
}

//----Gtk::Widget----

void OgreWidget::on_size_request( Gtk::Requisition* requisition )
{
	*requisition = Gtk::Requisition();

	requisition->width = 800;
	requisition->height = 600;
}

void OgreWidget::on_size_allocate( Gtk::Allocation& allocation )
{
	set_allocation( allocation );

	if( mRefGdkWindow )
	{
		mRefGdkWindow->move_resize(
				allocation.get_x(), allocation.get_y(),
				allocation.get_width(), allocation.get_height()
		);
	}

	//this is only hit on resize events
	if ( mRenderWindow )
	{
		mRenderWindow->windowMovedOrResized();

		mCamera->setAspectRatio( Ogre::Real( allocation.get_width() ) / Ogre::Real( allocation.get_height() ) );

		renderFrame();
	}
}

void OgreWidget::on_map()
{
	//call to base class
	Gtk::Widget::on_map();
}

void OgreWidget::on_unmap()
{
	//call to base class
	Gtk::Widget::on_unmap();
}

void OgreWidget::on_realize()
{
	//call to base class.
	Gtk::Widget::on_realize();

	Gtk::Allocation allocation = get_allocation();

	if( !mRefGdkWindow )
	{
		//Create the GdkWindow:
		GdkWindowAttr attributes;
		memset( &attributes, 0, sizeof( attributes ) );

		//Set initial position and size of the Gdk::Window
		attributes.x = allocation.get_x();
		attributes.y = allocation.get_y();
		attributes.width = allocation.get_width();
		attributes.height = allocation.get_height();

		attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK | Gdk::ALL_EVENTS_MASK ;
		attributes.window_type = GDK_WINDOW_CHILD;
		attributes.wclass = GDK_INPUT_OUTPUT;

		mRefGdkWindow = Gdk::Window::create( get_window(), &attributes, GDK_WA_X | GDK_WA_Y );
	}

	//Initialize Ogre::RenderWindow
	if ( !mRenderWindow )
	{
		Ogre::NameValuePairList params;

		GdkWindow* parent = mRefGdkWindow->gobj();
		GdkDisplay* display = gdk_drawable_get_display( GDK_DRAWABLE( parent ) );
		GdkScreen* screen = gdk_drawable_get_screen( GDK_DRAWABLE( parent ) );

		Display* xdisplay = GDK_DISPLAY_XDISPLAY( display );

		Screen* xscreen = GDK_SCREEN_XSCREEN( screen );

		int screen_number = XScreenNumberOfScreen( xscreen );
	   	XID xid_parent = GDK_WINDOW_XWINDOW( parent );

	   	params["externalWindowHandle"] =
			   Ogre::StringConverter::toString( reinterpret_cast<unsigned long>( xdisplay ) ) + ":" +
			   Ogre::StringConverter::toString( static_cast<unsigned int>( screen_number ) ) + ":" +
			   Ogre::StringConverter::toString( static_cast<unsigned long>( xid_parent ) );

	   	mRenderWindow = Ogre::Root::getSingleton().createRenderWindow(
			   "Gtk+Ogre Widget",
			   allocation.get_width(),
			   allocation.get_height(),
			   false,
			   &params);

	   	mRenderWindow->setAutoUpdated( false );

	   	unset_flags( Gtk::NO_WINDOW );

	   	set_window( mRefGdkWindow );

	   	set_double_buffered( true );

	   	createScene();
   }
}

void OgreWidget::on_unrealize()
{
	mRefGdkWindow.clear();

	//call to base class
	Gtk::Widget::on_unrealize();
}
