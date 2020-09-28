/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef IMAGEDATAIMPLGCANVAS_H
#define IMAGEDATAIMPLGCANVAS_H

#include "ImageDataBase.h"

// #include <GCanvas.hpp>
// #include <gcanvas/GCanvas2dContext.h>

#include <iostream>
#include <memory>
#include <functional>
#include <unordered_map>


namespace NodeBinding
{
     class ImageDataImplGCanvas : public ImageDataBase  
    {
    public:
        virtual ~ImageDataImplGCanvas();
        // virtual void*GetData();
        
    protected:
        int mTextureId;
    };
} // namespace NodeBinding

#endif