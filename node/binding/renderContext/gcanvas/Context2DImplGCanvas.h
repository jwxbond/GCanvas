/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef CONTEXT2DIMPLGCANVAS_H
#define CONTEXT2DIMPLGCANVAS_H
#include <iostream>

#include "Context2DBase.h"
// #include <GCanvas.hpp>
// #include "lodepng.h"
// #include <functional>
// #include <unordered_map>
// #include <unordered_map>
// #include "GConvert.h"
// #include "GWebGLRenderContext.hpp"
// #include "NodeBindingUtil.h"
// #include "Util.h"
// #include "GFrameBufferObject.h"
#include "gcanvas/GCanvas2dContext.h"

namespace NodeBinding
{
     class Context2DImplGCanvas : public Context2DBase  
    {
    public:
        Context2DImplGCanvas();
        Context2DImplGCanvas(int width, int height);
        Context2DImplGCanvas(int width, int height, int ratio);
        virtual ~Context2DImplGCanvas();
        
    protected:
        int mWidth, mHeight;
        GCanvasContext *mGCanvasContext;
    };
} // namespace NodeBinding

#endif