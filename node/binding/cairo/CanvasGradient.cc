// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#include "CanvasGradient.h"

#include "Canvas.h"
#include "color.h"



Nan::Persistent<FunctionTemplate> Gradient::constructor;


 void Gradient::init(Napi::Env env)
 {
   Napi::HandleScope scope(env);

    Napi::Function func =
        DefineClass(env,
                    "CanvasGradient",
                    {
                        InstanceMethod("addColorStop", &Gradient::addColorStop),
                    });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
 }

Napi::Object Gradient::NewInstance(Napi::Env env, int width, int height)
{ 
     Napi::Object obj = constructor.New({ 
        Napi::String::New(env,  "backend"),
        Napi::Number::New(env, width),
        Napi::Number::New(env, height) });
  obj.Set("name",  Napi::String::New(env, "backend"));
  return obj;
}

Napi::Object Gradient::NewInstance(Napi::Env env, const Napi::CallbackInfo &info)
{
  
}
  
Napi::Object Gradient::NewInstance(Napi::Env env, double x0, double y0, double x1, double y1)
{

}

Napi::Object Gradient::NewInstance(Napi::Env env, double x0, double y0, double r0, double x1, double y1, double r1)
{

}
/*
 * Initialize a new CanvasGradient.
 */

// NAN_METHOD(Gradient::New) {
//   if (!info.IsConstructCall()) {
//     return Nan::ThrowTypeError("Class constructors cannot be invoked without 'new'");
//   }

//   // Linear
//   if (4 == info.Length()) {
//     Gradient *grad = new Gradient(
//         Nan::To<double>(info[0]).FromMaybe(0)
//       , Nan::To<double>(info[1]).FromMaybe(0)
//       , Nan::To<double>(info[2]).FromMaybe(0)
//       , Nan::To<double>(info[3]).FromMaybe(0));
//     grad->Wrap(info.This());
//     info.GetReturnValue().Set(info.This());
//     return;
//   }

//   // Radial
//   if (6 == info.Length()) {
//     Gradient *grad = new Gradient(
//         Nan::To<double>(info[0]).FromMaybe(0)
//       , Nan::To<double>(info[1]).FromMaybe(0)
//       , Nan::To<double>(info[2]).FromMaybe(0)
//       , Nan::To<double>(info[3]).FromMaybe(0)
//       , Nan::To<double>(info[4]).FromMaybe(0)
//       , Nan::To<double>(info[5]).FromMaybe(0));
//     grad->Wrap(info.This());
//     info.GetReturnValue().Set(info.This());
//     return;
//   }

//   return Nan::ThrowTypeError("invalid arguments");
// }

/*
 * Add color stop.
 */


void Gradient::addColorStop(const Napi::CallbackInfo &info)
{
    NodeBinding::checkArgs(info, 2);
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
    }


  if (!info[1]->IsString())
  {
    throwError(info, "color string is invalid");
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
      return Nan::ThrowTypeError("parse color failed");
    }
}

/*
 * Initialize linear gradient.
 */

Gradient::Gradient(double x0, double y0, double x1, double y1) {
  _pattern = cairo_pattern_create_linear(x0, y0, x1, y1);
}

/*
 * Initialize radial gradient.
 */

Gradient::Gradient(double x0, double y0, double r0, double x1, double y1, double r1) {
  _pattern = cairo_pattern_create_radial(x0, y0, r0, x1, y1, r1);
}

/*
 * Destroy the pattern.
 */

Gradient::~Gradient() {
  cairo_pattern_destroy(_pattern);
}
