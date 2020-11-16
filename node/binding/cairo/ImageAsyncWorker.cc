#include "ImageAsyncWorker.h"
#include <iostream>
namespace cairocanvas
{

    ImageAsyncWorker::ImageAsyncWorker(Napi::Env env, std::string url, std::shared_ptr<ImageCached>& image, ImageDownloadCallback callback) 
      : Napi::AsyncWorker(env), 
        url(url),
        mImageMemCached(image),
        cb(callback)
    {

    }

    ImageAsyncWorker::~ImageAsyncWorker()
    {
        std::cout << "ImageAsyncWorker destructor .." << std::endl;
    }

    void ImageAsyncWorker::OnOK()
    {
        NodeBinding::cachedImage(this->url,this->mImageMemCached);
        if (cb != nullptr)
        {
            cb(Env(), (uint8_t *)this->mImageMemCached->buffer, this->mImageMemCached->bufferSize, "");
        }
    }

    void ImageAsyncWorker::OnError(const Napi::Error &e)
    {
        if (cb != nullptr)
        {
            cb(Env(), nullptr, 0, e.Message());
        }
    }


    void ImageAsyncWorker::Execute()
    {
        std::shared_ptr<ImageCached> cacheRet= NodeBinding::findCacheByUrl(url);
        // 命中缓存,直接返回
        if(cacheRet){
            this->mImageMemCached=cacheRet;
            return;
        }
        bool isHttpProtocol=url.rfind("http", 0) == 0;
        bool isHttpsProtocol=url.rfind("https", 0) == 0;
        if ( isHttpProtocol|| isHttpsProtocol)
        {
            content.size = NodeBinding::downloadImage(url, &content);
            if ((int)content.size <= 0)
            {
                content.memory = nullptr;
                this->SetError(std::move("Image Download Fail"));
                return;
            }
        }
        else
        { //本地文件
            content.size = NodeBinding::readImageFromLocalFile(url, &content);
            if ((int)content.size <= 0)
            {
                content.memory = nullptr;
                this->SetError(std::move("Image Read Fail"));
                return;
            }
        }

        //callback with content.memory & context.size
        if( content.memory && content.size > 0 )
        {
            //callback & decode
            mImageMemCached->bufferSize = content.size;
            mImageMemCached->buffer = new char[mImageMemCached->bufferSize];
            memcpy(mImageMemCached->buffer,content.memory, content.size);
        }
        else
        {
            this->SetError(std::move("Image Content Size is 0 "));
        }

        free(content.memory);
        content.memory = nullptr;
    }
} // namespace NodeBinding