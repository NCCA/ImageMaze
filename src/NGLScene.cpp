#include <QMouseEvent>
#include <QGuiApplication>

#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <iostream>

NGLScene::NGLScene(const std::string &_fname)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
  setTitle("Blank NGL");
  m_map.reset(new Map(_fname,&m_cam));
}


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}



void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );
  m_cam.setShape(45.0f,static_cast<float>(m_win.width)/m_win.height,0.5f,50.0f);
}


void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::instance();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  m_cam.set(ngl::Vec3(2,2,-10),ngl::Vec3::zero(),ngl::Vec3::up());
  ngl::VAOPrimitives::instance()->createTrianglePlane("ground",40,40,10,10,ngl::Vec3::up());
}



void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
// Rotation based on the mouse position for our global transform
  ngl::Mat4 rotX;
  ngl::Mat4 rotY;
  // create the rotation matrices
  rotX.rotateX( m_win.spinXFace );
  rotY.rotateY( m_win.spinYFace );
  // multiply the rotations
  m_mouseGlobalTX = rotY * rotX;
  // add the translations
  m_mouseGlobalTX.m_m[ 3 ][ 0 ] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[ 3 ][ 1 ] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[ 3 ][ 2 ] = m_modelPos.m_z;
  if(m_wireframe)
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  m_map->draw(m_mouseGlobalTX);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  shader->setUniform("Colour",0.3f,0.3f,0.3f,1.0f);
  ngl::Mat4 pos;
  pos.translate(0,-0.55f,0);
  shader->setUniform("MVP", m_mouseGlobalTX * pos* m_cam.getVPMatrix() );
  ngl::VAOPrimitives::instance()->draw("ground");
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

  break;
  case Qt::Key_W : m_wireframe^=true; break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}
