// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#include "CanvasGradient.h"

#include "CairoCanvas.h"
#include "color.h"



Nan::Persistent<FunctionTemplate> Gradient::constructor;


void Gradient::init(Napi::Env env)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "CanvasGradient", {
    InstanceMethod("addColorStop", &Gradient::addColorStop),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}



Napi::Object Gradient::NewInstance(const Napi::CallbackInfo &info)

{
  Napi::Object obj = constructor.New(info);
  obj.Set("name",  Napi::String::New(env, "Gradient"));
  return obj;
}


Gradient::Gradient(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ImageData>(info)
{
  Napi::Env env = info.Env();

  if( info.Length() == 4 && 
      info[0].IsNumber() && 
      info[1].IsNumber() && 
      info[2].IsNumber() &&
      info[3].IsNumber() )
  {
    double args[4];
    args[0] = info[0].As<Napi::Number>().DoubleValue();
    args[1] = info[1].As<Napi::Number>().DoubleValue();
    args[2] = info[2].As<Napi::Number>().DoubleValue();
    args[3] = info[3].As<Napi::Number>().DoubleValue();

    setupGradient(args[0], args[1], args[2], args[3]);
  }
  else if( info.Length() == 6 )
  {
    double args[6];
    args[0] = info[0].As<Napi::Number>().DoubleValue();
    args[1] = info[1].As<Napi::Number>().DoubleValue();
    args[2] = info[2].As<Napi::Number>().DoubleValue();
    args[3] = info[3].As<Napi::Number>().DoubleValue();
    args[4] = info[4].As<Napi::Number>().DoubleValue();
    args[3] = info[5].As<Napi::Number>().DoubleValue();
    setupGradient(args[0], args[1], args[2], args[3], args[4], args[5]);
  }
}

/*
 * Add color stop.
 */


void Gradient::addColorStop(const Napi::CallbackInfo &info)
{
  if( info.Length() < 2)
  {
    return;
  }

  float offset = 0.0;
  if (info[0].IsNumber())
  {
    offset = info[0].As<Napi::Number>().FloatValue();
  }
  else if (info[0].IsString())
  {
    std::string tmp = info[0].As<Napi::String>().Utf8Value();
    offset = std::stof(tmp);
  }
  else
  {
    throwError(info, "offset is invalid");
    return;
  }

  if (!info[1]->IsString())
  {
    throwError(info, "color string is invalid");
    return;
  }

  short ok;
  Nan::Utf8String str(info[1]);
  uint32_t rgba = rgba_from_string(*str, &ok);
  if (ok) {
    rgba_t color = rgba_create(rgba);
    cairo_pattern_add_color_stop_rgba(
        grad->pattern()
      , Nan::To<double>(info[0]).FromMaybe(0)
      , color.r
      , color.g
      , color.b
      , color.a);
  } else {
    napi_throw_error(env, "", "parse color failed");
  }
}

/*
 * Initialize linear gradient.
 */

void Gradient::setupGradient(double x0, double y0, double x1, double y1) 
{
  _pattern = cairo_pattern_create_linear(x0, y0, x1, y1);
}

/*
 * Initialize radial gradient.
 */

void Gradient::setupGradient(double x0, double y0, double r0, double x1, double y1, double r1) 
{
  _pattern = cairo_pattern_create_radial(x0, y0, r0, x1, y1, r1);
}

/*
 * Destroy the pattern.
 */

Gradient::~Gradient() 
{
  cairo_pattern_destroy(_pattern);
}
