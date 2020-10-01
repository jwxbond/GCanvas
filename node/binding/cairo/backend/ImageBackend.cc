#include "ImageBackend.h"
#include <exception>
#include <assert.h>

namespace cairocanvas
{

Napi::FunctionReference ImageBackend::constructor;

void ImageBackend::init(Napi::Env env)
{
  Napi::HandleScope scope(env);
  Napi::Function func = DefineClass(env, "ImageBackend", {});
  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

Napi::Object ImageBackend::NewInstance(Napi::Env env, int width, int height)
{
  Napi::Object obj = constructor.New({ 
      Napi::String::New(env,  "ImageBackend"),
      Napi::Number::New(env, width),
      Napi::Number::New(env, height) });
  obj.Set("name",  Napi::String::New(env, "ImageBackend"));
  return obj;
}


ImageBackend::ImageBackend(const Napi::CallbackInfo &info)
  : Backend(info)
{
}

// This returns an approximate value only, suitable for Nan::AdjustExternalMemory.
// The formats that don't map to intrinsic types (RGB30, A1) round up.
int32_t ImageBackend::approxBytesPerPixel() {
  switch (format) {
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

cairo_surface_t* ImageBackend::createSurface() {
  assert(!surface);
  surface = cairo_image_surface_create(format, width, height);
  assert(surface);

  //TODO
  // Napi::MemoryManagement::AdjustExternalMemory(env, approxBytesPerPixel() * width * height);
  return surface;
}

void ImageBackend::destroySurface() {
  if (surface) {
    cairo_surface_destroy(surface);
    surface = nullptr;
    //TODO
    // Napi::MemoryManagement::AdjustExternalMemory(env, -approxBytesPerPixel() * width * height);
  }
}

cairo_format_t ImageBackend::getFormat() {
  return format;
}

void ImageBackend::setFormat(cairo_format_t _format) {
	this->format = _format;
}
}
