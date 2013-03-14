/*
 * OgreWidget.h
 *
 *  Created on: Mar 9, 2013
 *      Author: chip
 */

#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <gtkmm.h>

#include <glibmm/timer.h>
#include <glibmm/objectbase.h>

#include <Ogre.h>

// View
class OgreWidget : public Gtk::Widget
{
public:
  OgreWidget();
  virtual ~OgreWidget();

  virtual void renderFrame(void);
  virtual void setBackgroundColor( int r, int g, int b );
  virtual void rotateCube( double rotation );

protected:

  //Gtk::Widget
  virtual void on_size_request( Gtk::Requisition* requisition );
  virtual void on_size_allocate( Gtk::Allocation& allocation );

  virtual void on_map();
  virtual void on_unmap();

  virtual void on_realize();
  virtual void on_unrealize();

  //Ogre scene initialization
  virtual void createScene();
  virtual void setupResources(void);
  virtual void loadResources(void);
  virtual void initCamera(void);
  virtual void initViewport(void);

  Glib::RefPtr<Gdk::Window> mRefGdkWindow;

  Ogre::RenderWindow* mRenderWindow;
  Ogre::SceneManager* mSceneMgr;
  Ogre::Viewport* mViewport;
  Ogre::Camera* mCamera;

  //Ogre config file
  Ogre::String mResourcesCfg;
  bool mUseResourcesCfg;

  bool mVerticalSync;
};
#endif
