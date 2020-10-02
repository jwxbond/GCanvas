#pragma once

#include "Backend.h"
#include <napi.h>

namespace cairocanvas
{

class ImageBackend : public Backend
{
  private:
    static Napi::FunctionReference constructor;
    cairo_surface_t* createSurface();
    void destroySurface();
    cairo_format_t format = DEFAULT_FORMAT;

  public:
    static void init(Napi::Env env);
    static Napi::Object NewInstance(const Napi::CallbackInfo &info);
    static Napi::Object NewInstance(Napi::Env env, int width, int height);

    ImageBackend(const Napi::CallbackInfo &info);
    cairo_format_t getFormat();
    void setFormat(cairo_format_t format);

    int32_t approxBytesPerPixel();
    const static cairo_format_t DEFAULT_FORMAT = CAIRO_FORMAT_ARGB32;
};
}
