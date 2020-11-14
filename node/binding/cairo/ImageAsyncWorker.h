/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#ifndef IMAGE_WORKER_H
#define IMAGE_WORKER_H
#include <napi.h>
#include "ImageCahced.h"
#include "NodeBindingUtil.h"

namespace cairocanvas
{

typedef std::function<void (Napi::Env env, uint8_t *data, size_t size, std::string msg )> ImageDownloadCallback;

// //使用asyncWorker来进行node中的异步调用
// void cachedImage(const std::string url, std::shared_ptr<ImageCached> imageCached);

class ImageAsyncWorker : public Napi::AsyncWorker
{
public:

    ImageAsyncWorker(Napi::Env env, std::string url, std::shared_ptr<ImageCached>& image, ImageDownloadCallback callback); 
    ~ImageAsyncWorker();

    void Execute();
    void OnOK();
    void OnError(const Napi::Error &e);

    std::string url;

private:
    std::shared_ptr<ImageCached> &mImageMemCached;
    NodeBinding::ImageContent content;
    ImageDownloadCallback cb;
};
}
#endif