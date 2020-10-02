// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#include "CairoCanvas.h"

#include <algorithm> // std::min
#include <assert.h>
#include "CairoCanvasRenderingContext2d.h"
#include <cstring>
#include <cctype>
#include <ctime>
#include <glib.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <unordered_set>
#include <vector>
// #include "backend/ImageBackend.h"
#include "backend/CairoImageBackend.h"
#include "NodeBindingUtil.h"


namespace cairocanvas
{
Napi::FunctionReference Canvas::constructor;

std::vector<FontFace> font_face_list;

 void Canvas::Init(Napi::Env env, Napi::Object exports)
{
    Napi::HandleScope scope(env);

    Napi::Function func =
        DefineClass(env,
            "Canvas",
            {
              // InstanceAccessor("type", &Canvas::getType, nullptr),
              // InstanceAccessor("stride", &Canvas::getStride, nullptr),
              InstanceAccessor("width", &Canvas::getWidth, nullptr),
              InstanceAccessor("height", &Canvas::getHeight, nullptr),
              InstanceMethod("getContext", &Canvas::getContext),
              InstanceMethod("createPNG", &Canvas::createPNG),
              InstanceMethod("createJPEG", &Canvas::createJPEG),
              InstanceMethod("createPNGStreamSync", &Canvas::createPNGStreamSync),
              InstanceMethod("createJPGStreamSync", &Canvas::createJPGStreamSync),
              InstanceMethod("toBuffer", &Canvas::ToBuffer),
            });
    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    return;
}

Napi::Object Canvas::NewInstance(Napi::Env env, Napi::Value arg, Napi::Value arg2)
{
    std::cout << "Canvas::NewInstance" << std::endl;
    Napi::Object obj = constructor.New({arg, arg2});
    obj.Set("name", Napi::String::New(env, "cairocanvas"));
    // Canvas *canvas = Napi::ObjectWrap<Canvas>::Unwrap(obj);
    // canvas->mRef = Napi::ObjectReference::New(obj);
    return obj;
}


Canvas::Canvas(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Canvas>(info), mDataRaw(nullptr)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  NodeBinding::checkArgs(info, 2);
  mWidth = info[0].As<Napi::Number>().Int32Value();
  mHeight = info[1].As<Napi::Number>().Int32Value();

_backend = new CairoImageBackend(mWidth, mHeight);

  if( !_backend->isSurfaceValid() )
  {
    delete _backend;
    return;
  }

  std::cout << "create CairoImageBackend Success width:" << mWidth << ", height:" << mHeight << std::endl;
_backend->setCanvas(this);
}

Canvas::~Canvas()
{

}

cairo_t* Canvas::createCairoContext()
{
  cairo_t* ret = cairo_create(surface());
  cairo_set_line_width(ret, 1); // Cairo defaults to 2
  return ret;
}

Napi::Value Canvas::getWidth(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), mWidth);
}

Napi::Value Canvas::getHeight(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), mHeight);
}

Napi::Value Canvas::getContext(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  NodeBinding::checkArgs(info, 1);
  std::string type = info[0].As<Napi::String>().Utf8Value();

  std::cout << "Canvas::getContext('" <<  type << "')" <<  std::endl;

  if (type == "2d")
  {
    if (mContext2dRef.IsEmpty())
    {
      Napi::Object obj = Context2d::NewInstance(info);
      Context2d *context2d = Napi::ObjectWrap<Context2d>::Unwrap(obj);
      context2d->setupContext2d( this );
      //save reference
      mContext2dRef = Napi::ObjectReference::New(obj);
      return obj;
    }
    else
    {
        return mContext2dRef.Value();
    }
  }
  else
  {
    napi_throw_error(env, "", "type is invalid");
    return Napi::Object::New(env);
  }
}

void Canvas::createPNG(const Napi::CallbackInfo &info)
{
  return;
}
void Canvas::createJPEG(const Napi::CallbackInfo &info)
{
  
}
Napi::Value Canvas::createJPGStreamSync(const Napi::CallbackInfo &info)
{
  NodeBinding::checkArgs(info, 2);
  unsigned long size = 0;
  Napi::Buffer<unsigned char> buffer = getJPGBuffer(info, size);
  if (size >= 0)
  {
      Napi::Function callback = info[0].As<Napi::Function>();
      //handlescope 表示作用域,一般调用callback函数时使用
      Napi::HandleScope scope(info.Env());
      callback.Call({info.Env().Null(),
                      buffer,
                      Napi::Number::New(info.Env(), size)});
  }
  else
  {
      Napi::Function callback = info[0].As<Napi::Function>();
      Napi::HandleScope scope(info.Env());
      callback.Call({Napi::String::New(Env(), "createJPGStreamFail"),
                      info.Env().Null(),
                      info.Env().Null()});
  }
  return info.Env().Undefined();
}

Napi::Value Canvas::createPNGStreamSync(const Napi::CallbackInfo &info)
{
  NodeBinding::checkArgs(info, 2);
  unsigned long size = 0;
  Napi::Buffer<unsigned char> buffer = getPNGBuffer(info, size);
  if (size >= 0)
  {
      Napi::Function callback = info[0].As<Napi::Function>();
      //handlescope 表示作用域,一般调用callback函数时使用
      Napi::HandleScope scope(info.Env());
      callback.Call({info.Env().Null(),
                      buffer,
                      Napi::Number::New(info.Env(), size)});
  }
  else
  {
      Napi::Function callback = info[0].As<Napi::Function>();
      Napi::HandleScope scope(info.Env());
      callback.Call({Napi::String::New(Env(), "createPNGStreamFail"),
                      info.Env().Null(),
                      info.Env().Null()});
  }
  return info.Env().Undefined();
}
Napi::Buffer<unsigned char> Canvas::getPNGBuffer(const Napi::CallbackInfo &info, unsigned long &size)
{
  cairo_surface_t *s = surface();
  cairo_surface_flush(s);
  const unsigned char *data = cairo_image_surface_get_data(s);
  std::vector<unsigned char> dataVec;
  NodeBinding::encodePNGInBuffer(dataVec, (unsigned char*)data, getWidth(), getHeight());

  size = dataVec.size();

  if( !dataVec .empty())
  {
    return Napi::Buffer<unsigned char>::Copy(info.Env(), &dataVec[0], dataVec.size());
  }
  else
  {
    return Napi::Buffer<unsigned char>::New(info.Env(), nullptr, 0);
  }
}
Napi::Buffer<unsigned char> Canvas::getJPGBuffer(const Napi::CallbackInfo &info, unsigned long &size)
{
  // if (mRenderContext)
  // {
  //     mRenderContext->makeCurrent();
  //     mRenderContext->drawFrame();
  // }
  // unsigned char *dataJPGFormat = nullptr;
  // int ret = mRenderContext->getImagePixelJPG(&dataJPGFormat, size);
  // if (ret == 0)
  // {
  //     return Napi::Buffer<unsigned char>::Copy(info.Env(), dataJPGFormat, size);
  // }
  // else
  // {
  //     size = -1;
  //     return Napi::Buffer<unsigned char>::New(info.Env(), nullptr, 0);
  // }
}
Napi::Buffer<unsigned char> Canvas::getRawDataBuffer(const Napi::CallbackInfo &info, unsigned long &size)
{

//   if (mDataRaw == nullptr)
//   {
//       mDataRaw = new unsigned char[4 * mWidth * mHeight];
//   }
//   int ret = mRenderContext->readPixelAndSampleFromCurrentCtx(mDataRaw);
//   if (ret == 0)
//   {
//       return Napi::Buffer<unsigned char>::Copy(info.Env(), mDataRaw, 4 * mWidth * mHeight);
//   }
//   else
//   {
//       size = -1;
//       return Napi::Buffer<unsigned char>::Copy(info.Env(), nullptr, 0);
//   }
}
Napi::Value Canvas::ToBuffer(const Napi::CallbackInfo &info)
{
//   unsigned long size = 0;
//   //默认输出png 编码
//   if (info.Length() == 0)
//   {
//       return getPNGBuffer(info, size);
//   }
//   else
//   {
//       Napi::Buffer<unsigned char> ret;
//       if (info.Length() == 1)
//       {
//           std::string mimeType = info[0].As<Napi::String>().Utf8Value();
//           if (mimeType == "image/png")
//           {
//               ret = getPNGBuffer(info, size);
//           }
//           else if (mimeType == "image/jpeg")
//           {
//               ret = getJPGBuffer(info, size);
//           }
//           else if (mimeType == "raw")
//           {
//               ret = getRawDataBuffer(info, size);
//           }
//       }
//       if (size < 0)
//       {
//           return info.Env().Null();
//       }
//       else
//       {
//           return ret;
//       }
//   }
}

}