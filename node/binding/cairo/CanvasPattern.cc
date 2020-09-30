// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#include "CanvasPattern.h"

#include "Canvas.h"
#include "Image.h"



const cairo_user_data_key_t *pattern_repeat_key;

Nan::Persistent<FunctionTemplate> Pattern::constructor;


void Pattern::init(Napi::Env env)
{
  Napi::Function func = DefineClass(env, "Pattern", {});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}


Napi::Object Pattern::NewInstance(const Napi::CallbackInfo &info)
{
  Napi::Object obj = constructor.New(info);
  obj.Set("name",  Napi::String::New(env, "Pattern"));
  return obj;
}


// /*
//  * Initialize CanvasPattern.
//  */

// void
// Pattern::Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target) {
//   Nan::HandleScope scope;

//   // Constructor
//   Local<FunctionTemplate> ctor = Nan::New<FunctionTemplate>(Pattern::New);
//   constructor.Reset(ctor);
//   ctor->InstanceTemplate()->SetInternalFieldCount(1);
//   ctor->SetClassName(Nan::New("CanvasPattern").ToLocalChecked());

//   ctor->InstanceTemplate()->SetInternalFieldCount(1);
//   ctor->SetClassName(Nan::New("CanvasPattern").ToLocalChecked());

//   // Prototype
//   Local<Context> ctx = Nan::GetCurrentContext();
//   Nan::Set(target,
//            Nan::New("CanvasPattern").ToLocalChecked(),
//            ctor->GetFunction(ctx).ToLocalChecked());
// }

// /*
//  * Initialize a new CanvasPattern.
//  */

// NAN_METHOD(Pattern::New) {
//   if (!info.IsConstructCall()) {
//     return Nan::ThrowTypeError("Class constructors cannot be invoked without 'new'");
//   }

//   cairo_surface_t *surface;

//   Local<Object> obj = Nan::To<Object>(info[0]).ToLocalChecked();

//   // Image
//   if (Nan::New(Image::constructor)->HasInstance(obj)) {
//     Image *img = Napi::ObjectWrap::Unwrap<Image>(obj);
//     if (!img->isComplete()) {
//       return Nan::ThrowError("Image given has not completed loading");
//     }
//     surface = img->surface();

//   // Canvas
//   } else if (Nan::New(Canvas::constructor)->HasInstance(obj)) {
//     Canvas *canvas = Napi::ObjectWrap::Unwrap<Canvas>(obj);
//     surface = canvas->surface();
//   // Invalid
//   } else {
//     return Nan::ThrowTypeError("Image or Canvas expected");
//   }
//   repeat_type_t repeat = REPEAT;
//   if (0 == strcmp("no-repeat", *Nan::Utf8String(info[1]))) {
//     repeat = NO_REPEAT;
//   } else if (0 == strcmp("repeat-x", *Nan::Utf8String(info[1]))) {
//     repeat = REPEAT_X;
//   } else if (0 == strcmp("repeat-y", *Nan::Utf8String(info[1]))) {
//     repeat = REPEAT_Y;
//   }
//   Pattern *pattern = new Pattern(surface, repeat);
//   pattern->Wrap(info.This());
//   info.GetReturnValue().Set(info.This());
// }

/*
 * Initialize pattern.
 */

Pattern::Pattern(const Napi::CallbackInfo &info) 
{
  Napi::Env env = info.Env();

  if (!info[0].IsObject() || !info[1].IsString())
  {
    napi_throw_error(env, "", "Pattern parameter invalid");
    return;
  }
  
  //TODO image or canvas check

  cairo_surface_t *surface;
  Napi::Object object = info[0].As<Napi::Object>();
  Napi::Value name = object.Get("name");
if (name.IsString())
{
  std::string namePropetry = name.As<Napi::String>().Utf8Value();

  // Image
  if (namePropetry == "image")
  {
      Image *image = Napi::ObjectWrap<Image>::Unwrap(object);
      
  }

  if (Nan::New(Image::constructor)->HasInstance(obj)) {
    Image *img = Napi::ObjectWrap::Unwrap<Image>(obj);
    if (!img->isComplete()) {
      return Nan::ThrowError("Image given has not completed loading");
    }
    surface = img->surface();

  // Canvas
  } else if (Nan::New(Canvas::constructor)->HasInstance(obj)) {
    Canvas *canvas = Napi::ObjectWrap::Unwrap<Canvas>(obj);
    surface = canvas->surface();
  // Invalid
  } else {
    return Nan::ThrowTypeError("Image or Canvas expected");
  }
  repeat_type_t repeat = REPEAT;
  if (0 == strcmp("no-repeat", *Nan::Utf8String(info[1]))) {
    repeat = NO_REPEAT;
  } else if (0 == strcmp("repeat-x", *Nan::Utf8String(info[1]))) {
    repeat = REPEAT_X;
  } else if (0 == strcmp("repeat-y", *Nan::Utf8String(info[1]))) {
    repeat = REPEAT_Y;
  }
  Pattern *pattern = new Pattern(surface, repeat);
  pattern->Wrap(info.This());
  info.GetReturnValue().Set(info.This());
}

Pattern::Pattern(cairo_surface_t *surface, repeat_type_t repeat) 
{
  _pattern = cairo_pattern_create_for_surface(surface);
  _repeat = repeat;
  cairo_pattern_set_user_data(_pattern, pattern_repeat_key, &_repeat, NULL);
}

repeat_type_t Pattern::get_repeat_type_for_cairo_pattern(cairo_pattern_t *pattern) {
  void *ud = cairo_pattern_get_user_data(pattern, pattern_repeat_key);
  return *reinterpret_cast<repeat_type_t*>(ud);
}

/*
 * Destroy the pattern.
 */

Pattern::~Pattern() {
  cairo_pattern_destroy(_pattern);
}
