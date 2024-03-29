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
  m_fname = _fname;
}

NGLScene::~NGLScene()
{
  std::cout << "Shutting down NGL, removing VAO's and Shaders\n";
}

void NGLScene::resizeGL(int _w, int _h)
{
  m_win.width = static_cast<int>(_w * devicePixelRatio());
  m_win.height = static_cast<int>(_h * devicePixelRatio());
  m_cam.project = ngl::perspective(45.0f, static_cast<float>(m_win.width) / m_win.height, 0.5f, 50.0f);
  m_actorCam.project = ngl::perspective(45.0f, static_cast<float>(m_win.width) / m_win.height, 0.5f, 50.0f);
}

void NGLScene::initializeGL()
{
  // we need to initialise the NGL lib which will load all of the OpenGL functions, this must
  // be done once we have a valid GL context but before we call any GL commands. If we dont do
  // this everything will crash
  ngl::NGLInit::initialize();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  m_cam.view = ngl::lookAt(ngl::Vec3(0, 30, 0), ngl::Vec3::zero(), ngl::Vec3::in());
  m_actorCam.view = ngl::lookAt(ngl::Vec3(2, 0, 2), ngl::Vec3(2, 0, 3), ngl::Vec3::up());

  m_activeCam = &m_cam;
  m_map.reset(new Map(m_fname, &m_activeCam->view, &m_activeCam->project));

  ngl::VAOPrimitives::createTrianglePlane("ground", 40, 40, 10, 10, ngl::Vec3::up());
  m_actor.setPos(2, 2);
  m_actor.setMap(m_map);
  m_actor.setParent(this);
}

void NGLScene::paintGL()
{
  // clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_win.width, m_win.height);
  // Rotation based on the mouse position for our global transform
  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);

  auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);
  // multiply the rotations
  m_mouseGlobalTX = rotY * rotX;
  // add the translations
  m_mouseGlobalTX.m_m[3][0] = m_modelPos.m_x;
  m_mouseGlobalTX.m_m[3][1] = m_modelPos.m_y;
  m_mouseGlobalTX.m_m[3][2] = m_modelPos.m_z;
  if (m_wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  m_map->draw(m_mouseGlobalTX);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  m_actor.draw();
  ngl::ShaderLib::setUniform("Colour", 0.3f, 0.3f, 0.3f, 1.0f);

  auto pos = ngl::Mat4::translate(0, -0.55f, 0);
  ngl::ShaderLib::setUniform("MVP", m_activeCam->project * m_activeCam->view * pos * m_mouseGlobalTX);
  ngl::VAOPrimitives::draw("ground");
}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape:
    QGuiApplication::exit(EXIT_SUCCESS);
    break;
  case Qt::Key_Space:
    m_win.spinXFace = 0;
    m_win.spinYFace = 0;
    m_modelPos.set(ngl::Vec3::zero());

    break;
  case Qt::Key_W:
    m_wireframe ^= true;
    break;
  case Qt::Key_Up:
    m_actor.move(Actor::DIRECTION::NORTH);
    break;
  case Qt::Key_Down:
    m_actor.move(Actor::DIRECTION::SOUTH);
    break;
  case Qt::Key_Left:
    m_actor.move(Actor::DIRECTION::WEST);
    break;
  case Qt::Key_Right:
    m_actor.move(Actor::DIRECTION::EAST);
    break;
  case Qt::Key_1:
    m_activeCam = &m_cam;
    break;
  case Qt::Key_2:
    m_activeCam = &m_actorCam;
    break;
  default:
    break;
  }
  // finally update the GLWindow and re-draw
  m_actorCam.view = ngl::lookAt(ngl::Vec3(m_actor.getPos().first, 0, m_actor.getPos().second),
                                ngl::Vec3(m_actor.getPos().first, 0, m_actor.getPos().second + 1),
                                ngl::Vec3::up());
  update();
}
