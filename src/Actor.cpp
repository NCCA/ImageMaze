#include "Actor.h"
#include "NGLScene.h"
#include <ngl/ShaderLib.h>
#include <ngl/Transformation.h>
#include <ngl/VAOPrimitives.h>
Actor::Actor(GLuint _x , GLuint _z) : m_posX(_x),m_posZ(_z)
{

}

void Actor::setPos(GLuint _x, GLuint _z)
{
  m_posX=_x;
  m_posZ=_z;

}

void Actor::setMap(std::shared_ptr<Map> _map)
{
  m_map=_map;
}

void Actor::setParent(NGLScene *_parent)
{
  m_parent=_parent;
}


std::pair<GLint,GLint> Actor::getPos() const
{
  return  std::make_pair(m_posX,m_posZ);
}

void Actor::move(DIRECTION _d)
{
  auto moveValid=[this](DIRECTION _dir)
  {
    return true;
  };

  switch (_d)
  {
    case DIRECTION::NORTH : if(moveValid(_d)) m_posZ-=1; break;
    case DIRECTION::SOUTH : if(moveValid(_d)) m_posZ+=1; break;
    case DIRECTION::EAST  : if(moveValid(_d)) m_posX+=1; break;
    case DIRECTION::WEST  : if(moveValid(_d)) m_posX-=1; break;

  }
}

void Actor::draw() const
{
  ngl::ShaderLib *shader=ngl::ShaderLib::instance();
  ngl::Mat4 MVP;
  float halfZ=m_map->getImage()->height()/2;
  float halfX=m_map->getImage()->width()/2;
  ngl::Transformation tx;
  tx.setPosition(halfX-m_posX,0.0f,halfZ-m_posZ);
  MVP=tx.getMatrix()*
      m_parent->getMouseTX()
      *m_parent->getCamera().getVPMatrix();
  shader->setUniform("MVP",MVP);
  shader->setUniform("Colour",1.0f,0.0f,0.0f,1.0f);
  ngl::VAOPrimitives::instance()->draw("troll");
}

