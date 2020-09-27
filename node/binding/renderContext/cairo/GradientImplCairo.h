/**
 * 
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef GRADIENTIMPLCAIRO_H
#define GRADIENTIMPLCAIRO_H
#include <iostream>
#include <cairo.h>
#include "GradientBase.h"

namespace NodeBinding 
{

class GradientImplCairo : public GradientBase
{
public:
    GradientImplCairo(double x0, double y0, double x1, double y1);
    GradientImplCairo(double x0, double y0, double r0, double x1, double y1, double r1);
    void addColorStop(double pos, std::string color);


private:
    virtual ~GradientImplCairo();
    cairo_pattern_t *_pattern;
};

}   

#endif
