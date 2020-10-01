// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#include "CairoImageData.h"
#include "Util.h"

namespace cairocanvas
{
    
Napi::FunctionReference ImageData::constructor;
void ImageData::init(Napi::Env env)
{
   Napi::HandleScope scope(env);
    Napi::Function func = DefineClass(env, "ImageData", {
        InstanceAccessor("data", &ImageData::getData, nullptr),
        InstanceAccessor("width", &ImageData::getWidth, nullptr),
        InstanceAccessor("height", &ImageData::getHeight, nullptr),
    });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}


sNapi::Object ImageData::NewInstance(const Napi::CallbackInfo &info)
{
    Napi::Object obj = constructor.New(info);
    obj.Set("name", Napi::String::New(env, "imageData"));
    return obj;
}

ImageData::ImageData(const Napi::CallbackInfo &info) : Napi::ObjectWrap<ImageData>(info)
{
    _width = info[0].As<Napi::Number>().Int32Value();
    _height = info[1].As<Napi::Number>().Int32Value();
    pixels.resize(4 * _width * _height);
}

Napi::Value ImageData::getData(const Napi::CallbackInfo &info)
{
    hasImageDataWrite = true;
    if (mImageDataRef.IsEmpty())
    {
        Napi::Array ret = Napi::Array::New(info.Env(), pixels.size());

        if (!pixels.empty())
        {
            for (int i = 0; i < pixels.size(); i++)
            {
                ret.Set(i, Napi::Number::New(info.Env(), pixels[i]));
            }
        }
        mImageDataRef = Napi::ObjectReference::New(ret);
        return ret;
    }
    else
    {
        return mImageDataRef.Value();
    }
}

Napi::Value ImageData::getWidth(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(), _width);
}

Napi::Value ImageData::getHeight(const Napi::CallbackInfo &info)
{
    return Napi::Number::New(info.Env(),_height);
}

std::vector<u_int8_t> &ImageData::getPixles()
{
    if (!mImageDataRef.IsEmpty() && hasImageDataWrite)
    {
        Napi::Array ret = mImageDataRef.Value().As<Napi::Array>();
        for (int i = 0; i < pixels.size(); i++)
        {
            pixels[i] = ret.Get(i).As<Napi::Number>().Int32Value();
        }
        hasImageDataWrite = false;
    }
    return pixels;
}

}