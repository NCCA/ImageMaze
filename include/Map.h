#ifndef MAP_H_
#define MAP_H_
#include <ngl/Camera.h>
#include <ngl/Image.h>
#include <memory>
#include <string>

class Map
{
  public :
    explicit Map(const std::string &_filename,const ngl::Camera *cam);
    void draw(const ngl::Mat4 &_mouse);
    ngl::Image * getImage(){return m_image.get();}
  private :
    std::unique_ptr<ngl::Image> m_image;
    const ngl::Camera *m_cam;
};

#endif
