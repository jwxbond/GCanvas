// Copyright (c) 2010 LearnBoost <tj@learnboost.com>

#pragma once

#include <napi.h>

class ImageData: public Napi::ObjectWrap<ImageData> {
  public:
    // static Nan::Persistent<v8::FunctionTemplate> constructor;
    // static void Initialize(Nan::ADDON_REGISTER_FUNCTION_ARGS_TYPE target);
    // static NAN_METHOD(New);
    // static NAN_GETTER(GetWidth);
    // static NAN_GETTER(GetHeight);

    static void init(Napi::Env env);
    static Napi::Object NewInstance(Napi::Env env, const Napi::Value width, const Napi::Value height);
    static Napi::Object NewInstance(Napi::Env env, uint8_t *data, int width, int height);
    
    ImageData(const Napi::CallbackInfo &info);

   std::vector<u_int8_t> &getPixles();
    inline int width() { return _width; }
    inline int height() { return _height; }
    inline uint8_t *data() { return _data; }


  private:
    int _width;
    int _height;
    uint8_t *_data;


  private:
    static Napi::FunctionReference constructor;
    Napi::ObjectReference mImageDataRef;
    std::vector<u_int8_t> pixels;
    Napi::Value getData(const Napi::CallbackInfo &info);
    // void setData(const Napi::CallbackInfo &info, const Napi::Value &value);
    Napi::Value getWidth(const Napi::CallbackInfo &info);
    Napi::Value getHeight(const Napi::CallbackInfo &info);
    bool hasImageDataWrite = false;
};
