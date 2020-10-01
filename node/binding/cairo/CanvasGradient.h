// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#pragma once

#include <napi.h>
#include <cairo.h>


namespace cairocanvas
{

class Gradient: public Napi::ObjectWrap<Gradient> {
  public:
    // static Nan::Persistent<v8::FunctionTemplate> constructor;
    // static void Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target);
    // static NAN_METHOD(New);
    // static NAN_METHOD(AddColorStop);

    static void init(Napi::Env env);
    static Napi::Object NewInstance(Napi::Env env, const Napi::CallbackInfo &info);
    static Napi::Object NewInstance(Napi::Env env, double x0, double y0, double x1, double y1);
    static Napi::Object NewInstance(Napi::Env env, double x0, double y0, double r0, double x1, double y1, double r1);

    Gradient(double x0, double y0, double x1, double y1);
    Gradient(double x0, double y0, double r0, double x1, double y1, double r1);
    inline cairo_pattern_t *pattern(){ return _pattern; }

  private:
    ~Gradient();
    cairo_pattern_t *_pattern;
};
}
