#pragma once

#include <cairo.h>
#include <exception>
#include <napi.h>
#include <string>
#include "CairoCanvas.h"

namespace cairocanvas
{
class Canvas;

class Backend : public Napi::ObjectWrap<Backend>
{
  private:
    std::string name;
    const char* error = NULL;
    static Napi::FunctionReference constructor;

  protected:
    int width;
    int height;
    cairo_surface_t* surface = nullptr;
    Canvas* canvas = nullptr;

    // static void init(const Nan::FunctionCallbackInfo<v8::Value> &info);
    // static Backend *construct(int width, int height){ return nullptr; }
  
  public:
    Backend(const Napi::CallbackInfo &info);
    static void init(Napi::Env env);
    static Napi::Object NewInstance(Napi::Env env, int width, int height);

    virtual ~Backend();

    void setCanvas(Canvas* canvas);

    virtual cairo_surface_t* createSurface();
    virtual cairo_surface_t* recreateSurface();

    cairo_surface_t* getSurface();
    virtual void destroySurface();

    std::string getName();

    int getWidth();
    virtual void setWidth(int width);

    int getHeight();
    virtual void setHeight(int height);

    // Overridden by ImageBackend. SVG and PDF thus always return INVALID.
    virtual cairo_format_t getFormat() {
      return CAIRO_FORMAT_INVALID;
    }

    bool isSurfaceValid();
    inline const char* getError(){ return error; }
};


class BackendOperationNotAvailable: public std::exception
{
  private:
    Backend* backend;
    std::string operation_name;

  public:
    BackendOperationNotAvailable(Backend* backend, std::string operation_name);
    ~BackendOperationNotAvailable() throw();

    const char* what() const throw();
};

}
