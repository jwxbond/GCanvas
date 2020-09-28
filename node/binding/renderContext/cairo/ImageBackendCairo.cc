#include "ImageBackendCairo.h"
#include <assert.h>

ImageBackendCairo::ImageBackendCairo(int w, int h) : name("image"), width(w), height(h)
{
  createSurface();
}
ImageBackendCairo *ImageBackendCairo::createImageBackendCairo(int w, int h)
{

  return new ImageBackendCairo(w, h);
}

// This returns an approximate value only, suitable for Nan::AdjustExternalMemory.
// The formats that don't map to intrinsic types (RGB30, A1) round up.
int32_t ImageBackendCairo::approxBytesPerPixel()
{
  switch (format)
  {
  case CAIRO_FORMAT_ARGB32:
  case CAIRO_FORMAT_RGB24:
    return 4;
#ifdef CAIRO_FORMAT_RGB30
  case CAIRO_FORMAT_RGB30:
    return 3;
#endif
  case CAIRO_FORMAT_RGB16_565:
    return 2;
  case CAIRO_FORMAT_A8:
  case CAIRO_FORMAT_A1:
    return 1;
  default:
    return 0;
  }
}

cairo_surface_t *ImageBackendCairo::createSurface()
{
  assert(!surface);
  surface = cairo_image_surface_create(format, width, height);
  assert(surface);
  // Nan::AdjustExternalMemory(approxBytesPerPixel() * width * height);
  return surface;
}

void ImageBackendCairo::destroySurface()
{
  if (surface)
  {
    cairo_surface_destroy(surface);
    surface = nullptr;
    // Nan::AdjustExternalMemory(-approxBytesPerPixel() * width * height);
  }
}

cairo_format_t ImageBackendCairo::getFormat()
{
  return format;
}

void ImageBackendCairo::setFormat(cairo_format_t _format)
{
  this->format = _format;
}
