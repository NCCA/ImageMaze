#ifndef MAP_H_
#define MAP_H_
#include <ngl/Image.h>
#include <memory>
#include <string>

class Map
{
  public :
    explicit Map(const std::string &_filename,const ngl::Mat4 *_view, ngl::Mat4 *_project);
    void draw(const ngl::Mat4 &_mouse);
    ngl::Image * getImage(){return m_image.get();}
  private :
    std::unique_ptr<ngl::Image> m_image;
    const ngl::Mat4 *m_view;
    const ngl::Mat4 *m_project;
};

#endif
