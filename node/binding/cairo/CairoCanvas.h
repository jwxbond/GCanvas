// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#pragma once

#include "backend/CairoImageBackend.h"

#include <cairo.h>
#include <napi.h>
#include <pango/pangocairo.h>
#include <vector>



#ifndef CANVAS_MAX_STATES
#define CANVAS_MAX_STATES 64
#endif


namespace cairocanvas
{
/*
 * FontFace describes a font file in terms of one PangoFontDescription that
 * will resolve to it and one that the user describes it as (like @font-face)
 */
class FontFace {
  public:
    PangoFontDescription *sys_desc = nullptr;
    PangoFontDescription *user_desc = nullptr;
};

/*
 * Canvas.
 */
class Canvas: public Napi::ObjectWrap<Canvas> {
  public:
    static void Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value arg, Napi::Value arg2);
    static PangoWeight GetWeightFromCSSString(const char *weight);
    static PangoStyle GetStyleFromCSSString(const char *style);
    static PangoFontDescription *ResolveFontDescription(const PangoFontDescription *desc);

    Napi::ObjectReference mRef;
    Canvas(const Napi::CallbackInfo &info);
    virtual ~Canvas();


    inline CairoImageBackend* backend() { return _backend; }
    inline cairo_surface_t* surface(){ return backend()->getSurface(); }
    cairo_t* createCairoContext();

    inline uint8_t *data(){ return cairo_image_surface_get_data(surface()); }
    inline int stride(){ return cairo_image_surface_get_stride(surface()); }
    inline int nBytes(){ return getHeight() * stride(); }

    inline int getWidth() { return backend()->getWidth(); }
    inline int getHeight() { return backend()->getHeight(); }

    Canvas(CairoImageBackend* backend);
    
 private:
    static Napi::FunctionReference constructor;
    Napi::Value getWidth(const Napi::CallbackInfo &info);
    Napi::Value getHeight(const Napi::CallbackInfo &info);
    Napi::Value getContext(const Napi::CallbackInfo &info);
    Napi::Value createPNGStreamSync(const Napi::CallbackInfo &info);
    Napi::Value createJPGStreamSync(const Napi::CallbackInfo &info);
    Napi::Value ToBuffer(const Napi::CallbackInfo &info);
    Napi::Buffer<unsigned char> getPNGBuffer(const Napi::CallbackInfo &info, unsigned long &size);
    Napi::Buffer<unsigned char> getJPGBuffer(const Napi::CallbackInfo &info, unsigned long &size);
    Napi::Buffer<unsigned char> getRawDataBuffer(const Napi::CallbackInfo &info, unsigned long &size);

    Napi::ObjectReference mContext2dRef;
    Napi::ObjectReference mContextWebGLRef;

    void setWidth(const Napi::CallbackInfo &info, const Napi::Value &value);
    void setHeight(const Napi::CallbackInfo &info, const Napi::Value &value);
    void createPNG(const Napi::CallbackInfo &info);
    void createJPEG(const Napi::CallbackInfo &info);

    int mWidth = 0;
    int mHeight = 0;
    unsigned char *mDataRaw;

  private:
    CairoImageBackend* _backend;
};

}