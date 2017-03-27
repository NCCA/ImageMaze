#include "Map.h"
#include <iostream>
#include <ngl/VAOPrimitives.h>
#include <ngl/Transformation.h>
#include <ngl/Colour.h>
#include <ngl/ShaderLib.h>
#include <ngl/NGLStream.h>
Map::Map(const std::string &_fname, const ngl::Camera *_cam)
{
  m_cam=_cam;
  m_image.reset(new ngl::Image(_fname));
}

void Map::draw(const ngl::Mat4 &_mouse)
{
  // calculate extents from -ve to +ve in x and z for image
  float halfZ=m_image->height()/2;
  float halfX=m_image->width()/2;
  ngl::Transformation tx;
  ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
  ngl::ShaderLib *shader =ngl::ShaderLib::instance();
  shader->use(ngl::nglColourShader);
  shader->setUniform("Colour",1.0f,0.0f,0.0f,1.0f);
  GLuint imageX=0;
  GLuint imageZ=0;
  ngl::Colour pixel;
  for(float z=-halfZ; z<halfZ; ++z)
  {
    for(float x=-halfX; x<halfX; ++x)
    {
      pixel=m_image->getColour(imageX,imageZ);
      if(pixel.m_r==0)
      {
        tx.setPosition(x,0,z);
        shader->setUniform("MVP",tx.getMatrix()*_mouse*m_cam->getVPMatrix());
        prim->draw("cube");
      }
      ++imageX;
    }
    imageX=0;
    ++imageZ;
  }

}
