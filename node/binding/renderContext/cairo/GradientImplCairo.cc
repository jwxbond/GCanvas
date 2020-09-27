/**
 * 
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */


#include "GradientImplCairo.h"
#include "color.h"

namespace NodeBinding 
{

// GradientImplCairo::GradientImplCairo(double x0, double y0, double x1, double y1);
//     GradientImplCairo(double x0, double y0, double r0, double x1, double y1, double r1);
//     virtual ~GradientImplCairo() = default;
//     void addColorStop(double pos, std::string color);


GradientImplCairo::GradientImplCairo(double x0, double y0, double x1, double y1):
    GradientBase::GradientBase(x0, y0, x1, y1)
{
    _pattern = cairo_pattern_create_linear(x0, y0, x1, y1);
}
GradientImplCairo::GradientImplCairo(double x0, double y0, double r0, double x1, double y1, double r1):
     GradientBase::GradientBase(x0, y0, r0, x1, y1, r1)
{
    _pattern = cairo_pattern_create_radial(x0, y0, r0, x1, y1, r1);
}
GradientImplCairo::~GradientImplCairo()
{
    cairo_pattern_destroy(_pattern);
}
void GradientImplCairo::addColorStop(double pos, std::string color)
{
    short ok;
    uint32_t rgba = rgba_from_string(*str, &ok);
    if (ok)
    {
        rgba_t color = rgba_create(rgba);
        cairo_pattern_add_color_stop_rgba(_pattern, pos, color.r, color.g, color.b, color.a);
    }
}

}
