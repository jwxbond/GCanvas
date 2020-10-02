// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#include "CairoImage.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>


namespace cairocanvas
{

Napi::FunctionReference Image::constructor;

void Image::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(env, "Image", {
        InstanceAccessor("src", &Image::getSrc, &Image::setSrc),
        InstanceAccessor("width", &Image::getWidth, nullptr),
        InstanceAccessor("height", &Image::getHeight, nullptr),
        InstanceAccessor("onload", &Image::getOnLoadCallback, &Image::setOnLoadCallback),
        InstanceAccessor("onerror", &Image::getOnErrorCallback, &Image::setOnErrorCallback),
    });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Image", func);
}


Napi::Object Image::NewInstance(Napi::Env env)
{
  Napi::Object obj = constructor.New({});
  obj.Set("name", Napi::String::New(env, "cairoimage"));
  return obj;
}

Image::Image(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Image>(info)
{
    mCallbackSet = new ImageCallbackSet();
}

Image::~Image()
{
    clearData();
    
    if(mDownloadImageWorker) {
        delete mDownloadImageWorker;
        mDownloadImageWorker = nullptr;
    }

    if( mCallbackSet ) {
        delete mCallbackSet;
        mCallbackSet = nullptr;
    }
    mImageMemCached = nullptr;
}

Napi::Value Image::getSrc(const Napi::CallbackInfo &info)
{
    return Napi::String::New(info.Env(), src);
}

void Image::setSrc(const Napi::CallbackInfo &info, const Napi::Value &value)
  {
    NodeBinding::checkArgs(info, 1);
    src = value.As<Napi::String>().Utf8Value();
    mImageMemCached=std::make_shared<ImageCached>();
    if (!mDownloadImageWorker)
    {
        mDownloadImageWorker = new NodeBinding::ImageWorker(info.Env(), mImageMemCached,
                                  mImageMemCached->width,
                                  mImageMemCached->height);
    }
    if (mDownloadImageWorker)
    {
        mDownloadImageWorker->url = src;
        mDownloadImageWorker->setOnErrorCallback(mCallbackSet->mOnErrorCallback.Value());
        mDownloadImageWorker->setOnLoadCallback(mCallbackSet->mOnLoadCallback.Value());
        mDownloadImageWorker->Queue();
    }
}

Napi::Value Image::getOnLoadCallback(const Napi::CallbackInfo &info)
{
    return mCallbackSet->mOnLoadCallback.Value();
}
Napi::Value Image::getOnErrorCallback(const Napi::CallbackInfo &info)
{
    return mCallbackSet->mOnErrorCallback.Value();
}

void Image::setOnLoadCallback(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    NodeBinding::checkArgs(info, 1);
    mCallbackSet->mOnLoadCallback = Napi::Persistent(value.As<Napi::Function>());
}

void Image::setOnErrorCallback(const Napi::CallbackInfo &info, const Napi::Value &value)
{
    NodeBinding::checkArgs(info, 1);
    mCallbackSet->mOnErrorCallback = Napi::Persistent(value.As<Napi::Function>());
}

Napi::Value Image::getWidth(const Napi::CallbackInfo &info)
{
    if (mImageMemCached)
    {
        return Napi::Number::New(info.Env(), mImageMemCached->width);
    }
    else
    {
        return Napi::Number::New(info.Env(), 0);
    }
}
Napi::Value Image::getHeight(const Napi::CallbackInfo &info)
{
    if (mImageMemCached)
    {
        return Napi::Number::New(info.Env(), mImageMemCached->height);
    }
    else
    {
        return Napi::Number::New(info.Env(), 0);
    }
}

int Image::getWidth()
{
    return mImageMemCached != nullptr ? mImageMemCached->width : 0;
}
int Image::getHeight()
{
    return mImageMemCached != nullptr ? mImageMemCached->height : 0;
}

std::vector<unsigned char> &Image::getPixels()
{
    if (mImageMemCached)
    {
        return mImageMemCached->getPixels();
    }
    else
    {
        //引用没办法,只能给一个非局部的vector,稍微浪费一点内存
        return emptyPixels;
    }
}

 cairo_surface_t* Image::getSurface()
 {
    if( !_surface ) 
    {
        ImageCached *imageCached = mImageMemCached.get();
        if (imageCached) 
        {
            int width = imageCached->width;
            int height = imageCached->height;
            int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);

            _surface = cairo_image_surface_create_for_data( 
            imageCached->getPixels().data(),
            CAIRO_FORMAT_ARGB32,
            width, 
            height, 
            stride);
        }
    }
    return _surface;
}

void Image::setSurface(cairo_surface_t* surface)
{
    _surface = surface;
}

void Image::clearData()
{
    if (_surface) {
        cairo_surface_destroy(_surface);
        // Nan::AdjustExternalMemory(-_data_len);
        _data_len = 0;
        _surface = NULL;
    }

    delete[] _data;
    _data = nullptr;
    width = height = 0;
}

}