#pragma once

#include <cairo.h>
#include <exception>
#include <string>

class ImageBackendCairo
{
public:
  ImageBackendCairo(int w, int h);
  static ImageBackendCairo *createImageBackendCairo(int w, int h);

  cairo_surface_t* createSurface();
  cairo_surface_t* recreateSurface();

  cairo_surface_t* getSurface();
  void destroySurface();

  inline std::string getName() {return name; }
  inline  int getWidth(){ return width; }
  void setWidth(int w);
  inline int getHeight() { return height; }

  cairo_format_t getFormat();
  void setFormat(cairo_format_t format);
  int32_t approxBytesPerPixel();
  const static cairo_format_t DEFAULT_FORMAT = CAIRO_FORMAT_ARGB32;

private:
  cairo_format_t format = DEFAULT_FORMAT;
  const std::string name;
  const char *error = NULL;

protected:
  int width;
  int height;
  cairo_surface_t *surface = nullptr;
};
