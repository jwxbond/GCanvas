/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#include "Image.h"
#include "Canvas.h"
#include "NodeBindingUtil.h"
#include <iostream>
#include <stdlib.h>
namespace NodeBinding
{
    Napi::FunctionReference Image::constructor;
    Image::Image(const Napi::CallbackInfo &info) : Napi::ObjectWrap<Image>(info)
    {
        mCallbackSet = new ImageCallbackSet();
    }

    Image::~Image()
    {
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

    void Image::Init(Napi::Env env, Napi::Object exports)
    {
        Napi::HandleScope scope(env);

        Napi::Function func =
            DefineClass(env,
                        "Image",
                        {
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
        obj.Set("name", Napi::String::New(env, "image"));
        return obj;
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
            mDownloadImageWorker = new ImageWorker(info.Env(), mImageMemCached,
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

    void Image::setTextureId(int textureId)
    {
        mTextureId = textureId;
    }
    int Image::getTextureId()
    {
        return mTextureId;
    }

}; // namespace NodeBinding