/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef PATTERNIMPLGCANVAS_H
#define PATTERNIMPLGCANVAS_H

#include "PatternBase.h"

// #include <GCanvas.hpp>
// #include <gcanvas/GCanvas2dContext.h>

#include <iostream>
#include <memory>
#include <functional>
#include <unordered_map>


namespace NodeBinding
{
     class PatternImplGCanvas : public PatternBase  
    {
    public:
        PatternImplGCanvas(void* image, PatternRepeatType type);
        virtual ~PatternImplGCanvas();
        
        void *mImage;
        PatternRepeatType mRepeatType;
        int mWidth;
        int mHeight;
    };
} // namespace NodeBinding

#endif