// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#pragma once

#include <cairo.h>
// #include "CanvasError.h"
#include <functional>
#include <napi.h>
#include <stdint.h> // node < 7 uses libstdc++ on macOS which lacks complete c++11

#include <curl/curl.h>
#include <vector>
#include "NodeBindingUtil.h"
#include "lodepng.h"
#include "ImageCahced.h"
#include "ImageWorker.h"

namespace cairocanvas
{

struct ImageCallbackSet{
    Napi::FunctionReference mOnErrorCallback;
    Napi::FunctionReference mOnLoadCallback;
};
extern std::shared_ptr<ImageCached> findCacheByUrl(const std::string &url);

class Image: public Napi::ObjectWrap<Image> {
  public:
    void init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(const Napi::CallbackInfo &info);

    Image(const Napi::CallbackInfo &info);
    virtual ~Image();

    int getWidth();
    int getHeight();
    std::vector<unsigned char> &getPixels();
    void setTextureId(int textureId);
    int getTextureId();
    std::string getUrl(){
        return this->src;
    }

  public:
    cairo_surface_t* getSurface();
    void setSurface(cairo_surface_t* surface); 

  private:
    //cairo data
    cairo_surface_t *_surface;
    uint8_t *_data = nullptr;
    int _data_len;
    int width, height;
    void clearData();

  private:
    static Napi::FunctionReference constructor;
    std::string src;
    ImageCallbackSet *mCallbackSet;
    NodeBinding::ImageWorker *mDownloadImageWorker = nullptr;
    std::vector<unsigned char> emptyPixels;
    std::shared_ptr<ImageCached> mImageMemCached;
    Napi::Value getSrc(const Napi::CallbackInfo &info);
    void setSrc(const Napi::CallbackInfo &info, const Napi::Value &value);
    Napi::Value getOnLoadCallback(const Napi::CallbackInfo &info);
    void setOnLoadCallback(const Napi::CallbackInfo &info, const Napi::Value &value);
    Napi::Value getOnErrorCallback(const Napi::CallbackInfo &info);
    void setOnErrorCallback(const Napi::CallbackInfo &info, const Napi::Value &value);
    Napi::Value getWidth(const Napi::CallbackInfo &info);
    Napi::Value getHeight(const Napi::CallbackInfo &info);
    int mTextureId=-1;
};

}