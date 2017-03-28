#ifndef ACTOR_H_
#define ACTOR_H_
#include <ngl/Vec3.h>
#include "Map.h"
#include <memory>
#include <utility>
class NGLScene;
class Actor
{
  public :
    enum class DIRECTION : char {NORTH,SOUTH,EAST,WEST};
    Actor()=default;
    Actor(GLuint _x, GLuint _z );
    void setPos(GLuint _x, GLuint _z );
    std::pair<GLint,GLint> getPos() const;
    void move(DIRECTION _d);
    void draw() const;
    void setMap(std::shared_ptr<Map> _map);
    void setParent(NGLScene *_parent);
  private :

    GLuint m_posX;
    GLuint m_posZ;
    std::shared_ptr<Map> m_map;
    const NGLScene *m_parent;

};


#endif
