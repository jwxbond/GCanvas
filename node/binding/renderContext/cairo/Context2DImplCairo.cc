/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#include "Context2DImplCairo.h"
#include <algorithm>
// #include "backend/ImageBackend.h"
#include <cairo-pdf.h>
// #include "Canvas.h"
// #include "CanvasGradient.h"
#include "PatternImplCairo.h"
#include <cmath>
#include <cstdlib>
// #include "Image.h"
// #include "ImageData.h"
#include <limits>
#include <map>
#include "Point.h"
#include <string>
// #include "Util.h"
#include <vector>

namespace NodeBinding
{

    /*
 * Text baselines.
 */

    enum
    {
        TEXT_BASELINE_ALPHABETIC,
        TEXT_BASELINE_TOP,
        TEXT_BASELINE_BOTTOM,
        TEXT_BASELINE_MIDDLE,
        TEXT_BASELINE_IDEOGRAPHIC,
        TEXT_BASELINE_HANGING
    };

    Context2DImplCairo::Context2DImplCairo()
        : mWidth(100), mHeight(100), mRatio(1.0)
    {
        SetupContext2D();
    }

    Context2DImplCairo::Context2DImplCairo(int width, int height)
        : mWidth(width), mHeight(height), mRatio(1.0)
    {
        SetupContext2D();
    }

    Context2DImplCairo::Context2DImplCairo(int width, int height, int ratio)
        : mWidth(width), mHeight(height), mRatio(ratio)
    {
        SetupContext2D();
    }

    Context2DImplCairo::~Context2DImplCairo()
    {
        // if( mGCanvasContext ) {
        //     delete mGCanvasContext;
        //     mGCanvasContext = nullptr;
        // }Pattern

        // if( mGCanvas ){
        //     delete mGCanvas;
        //     mGCanvas = nullptr;
        // }
    }

    void Context2DImplCairo::SetupContext2D()
    {
        // GCanvasConfig config = {true, false};
        // // mGCanvas = std::make_shared<gcanvas::GCanvas>( "context2d-gcanvas", config, nullptr );
        // mGCanvas = new gcanvas::GCanvas( "context2d-gcanvas", config, nullptr );
        // if( !mGCanvas ) {
        //     return;
        // }

        // mGCanvas->CreateContext();
        // mGCanvasContext = mGCanvas->GetGCanvasContext();
        // if (!mGCanvasContext ){
        //     return;
        // }

        // mGCanvasContext->SetClearColor(gcanvas::StrValueToColorRGBA("transparent"));
        // mGCanvasContext->ClearScreen();
        // mGCanvasContext->SetDevicePixelRatio(mRatio);
        // mGCanvas->OnSurfaceChanged(0, 0, mWidth, mHeight);
    }

    //implement Context2DBase API
    void Context2DImplCairo::resize(int w, int h)
    {
        //TODO resize
    }

    //==========================================================
    //  protected
    //==========================================================

    /*
 * Create temporary surface for gradient or pattern transparency
 */
    cairo_pattern_t *
    create_transparent_gradient(cairo_pattern_t *source, float alpha)
    {
        double x0;
        double y0;
        double x1;
        double y1;
        double r0;
        double r1;
        int count;
        int i;
        double offset;
        double r;
        double g;
        double b;
        double a;
        cairo_pattern_t *newGradient;
        cairo_pattern_type_t type = cairo_pattern_get_type(source);
        cairo_pattern_get_color_stop_count(source, &count);
        if (type == CAIRO_PATTERN_TYPE_LINEAR)
        {
            cairo_pattern_get_linear_points(source, &x0, &y0, &x1, &y1);
            newGradient = cairo_pattern_create_linear(x0, y0, x1, y1);
        }
        else if (type == CAIRO_PATTERN_TYPE_RADIAL)
        {
            cairo_pattern_get_radial_circles(source, &x0, &y0, &r0, &x1, &y1, &r1);
            newGradient = cairo_pattern_create_radial(x0, y0, r0, x1, y1, r1);
        }
        else
        {
            // Nan::ThrowError("Unexpected gradient type");
            return NULL;
        }
        for (i = 0; i < count; i++)
        {
            cairo_pattern_get_color_stop_rgba(source, i, &offset, &r, &g, &b, &a);
            cairo_pattern_add_color_stop_rgba(newGradient, offset, r, g, b, a * alpha);
        }
        return newGradient;
    }

    cairo_pattern_t *
    create_transparent_pattern(cairo_pattern_t *source, float alpha)
    {
        cairo_surface_t *surface;
        cairo_pattern_get_surface(source, &surface);
        int width = cairo_image_surface_get_width(surface);
        int height = cairo_image_surface_get_height(surface);
        cairo_surface_t *mask_surface = cairo_image_surface_create(
            CAIRO_FORMAT_ARGB32,
            width,
            height);
        cairo_t *mask_context = cairo_create(mask_surface);
        if (cairo_status(mask_context) != CAIRO_STATUS_SUCCESS)
        {
            // Nan::ThrowError("Failed to initialize context");
            return NULL;
        }
        cairo_set_source(mask_context, source);
        cairo_paint_with_alpha(mask_context, alpha);
        cairo_destroy(mask_context);
        cairo_pattern_t *newPattern = cairo_pattern_create_for_surface(mask_surface);
        cairo_surface_destroy(mask_surface);
        return newPattern;
    }

    /*
 * Gets the baseline adjustment in device pixels
 */
    inline double getBaselineAdjustment(PangoLayout *layout, short baseline)
    {
        PangoRectangle logical_rect;
        pango_layout_line_get_extents(pango_layout_get_line(layout, 0), NULL, &logical_rect);

        double scale = 1.0 / PANGO_SCALE;
        double ascent = scale * pango_layout_get_baseline(layout);
        double descent = scale * logical_rect.height - ascent;

        switch (baseline)
        {
        case TEXT_BASELINE_ALPHABETIC:
            return ascent;
        case TEXT_BASELINE_MIDDLE:
            return (ascent + descent) / 2.0;
        case TEXT_BASELINE_BOTTOM:
            return ascent + descent;
        default:
            return 0;
        }
    }

    bool Context2DImplCairo::hasShadow()
    {
        return state->shadow.a && (state->shadowBlur || state->shadowOffsetX || state->shadowOffsetY);
    }

    void Context2DImplCairo::setSourceRGBA(rgba_t color)
    {
        setSourceRGBA(_context, color);
    }
    void Context2DImplCairo::setSourceRGBA(cairo_t *ctx, rgba_t color)
    {
        cairo_set_source_rgba(ctx, color.r, color.g, color.b, color.a * state->globalAlpha);
    }

    void Context2DImplCairo::setTextPath(double x, double y)
    {
        PangoRectangle logical_rect;

        switch (state->textAlignment)
        {
        // center
        case 0:
            pango_layout_get_pixel_extents(_layout, NULL, &logical_rect);
            x -= logical_rect.width / 2;
            break;
        // right
        case 1:
            pango_layout_get_pixel_extents(_layout, NULL, &logical_rect);
            x -= logical_rect.width;
            break;
        }

        y -= getBaselineAdjustment(_layout, state->textBaseline);

        cairo_move_to(_context, x, y);
        if (state->textDrawingMode == TEXT_DRAW_PATHS)
        {
            pango_cairo_layout_path(_context, _layout);
        }
        else if (state->textDrawingMode == TEXT_DRAW_GLYPHS)
        {
            pango_cairo_show_layout(_context, _layout);
        }
    }

    void Context2DImplCairo::blur(cairo_surface_t *surface, int radius)
    {
        //TODO
    }
    void Context2DImplCairo::shadow(void(fn)(cairo_t *cr))
    {
        //TODO
    }
    void Context2DImplCairo::shadowStart()
    {
        //TODO
    }
    void Context2DImplCairo::shadowApply()
    {
        //TODO
    }
    void Context2DImplCairo::savePath()
    {
        _path = cairo_copy_path_flat(_context);
        cairo_new_path(_context);
    }

    void Context2DImplCairo::restorePath()
    {
        cairo_new_path(_context);
        cairo_append_path(_context, _path);
        cairo_path_destroy(_path);
    }

    // void saveState();
    // void restoreState();

    void Context2DImplCairo::setFillRule(std::string value)
    {
        // if( value.empty() ) return;

        cairo_fill_rule_t rule = CAIRO_FILL_RULE_WINDING;
        if (value == "evenodd")
        {
            rule = CAIRO_FILL_RULE_EVEN_ODD;
        }
        cairo_set_fill_rule(_context, rule);
    }
    void Context2DImplCairo::doFill(bool preserve)
    {
        cairo_pattern_t *new_pattern;
        if (state->fillPattern)
        {
            if (state->globalAlpha < 1)
            {
                new_pattern = create_transparent_pattern(state->fillPattern, state->globalAlpha);
                if (new_pattern == NULL)
                {
                    // failed to allocate; Nan::ThrowError has already been called, so return from this fn.
                    return;
                }
                cairo_set_source(_context, new_pattern);
                cairo_pattern_destroy(new_pattern);
            }
            else
            {
                cairo_set_source(_context, state->fillPattern);
            }
            repeat_type_t repeat = PatternImplCairo::get_repeat_type_for_cairo_pattern(state->fillPattern);
            if (NO_REPEAT == repeat)
            {
                cairo_pattern_set_extend(cairo_get_source(_context), CAIRO_EXTEND_NONE);
            }
            else
            {
                cairo_pattern_set_extend(cairo_get_source(_context), CAIRO_EXTEND_REPEAT);
            }
        }
        else if (state->fillGradient)
        {
            if (state->globalAlpha < 1)
            {
                new_pattern = create_transparent_gradient(state->fillGradient, state->globalAlpha);
                if (new_pattern == NULL)
                {
                    // failed to recognize gradient; Nan::ThrowError has already been called, so return from this fn.
                    return;
                }
                cairo_pattern_set_filter(new_pattern, state->patternQuality);
                cairo_set_source(_context, new_pattern);
                cairo_pattern_destroy(new_pattern);
            }
            else
            {
                cairo_pattern_set_filter(state->fillGradient, state->patternQuality);
                cairo_set_source(_context, state->fillGradient);
            }
        }
        else
        {
            setSourceRGBA(state->fill);
        }
        if (preserve)
        {
            hasShadow()
                ? shadow(cairo_fill_preserve)
                : cairo_fill_preserve(_context);
        }
        else
        {
            hasShadow()
                ? shadow(cairo_fill)
                : cairo_fill(_context);
        }
    }
    void Context2DImplCairo::doStroke(bool preserve)
    {
        cairo_pattern_t *new_pattern;
        if (state->strokePattern)
        {
            if (state->globalAlpha < 1)
            {
                new_pattern = create_transparent_pattern(state->strokePattern, state->globalAlpha);
                if (new_pattern == NULL)
                {
                    // failed to allocate; Nan::ThrowError has already been called, so return from this fn.
                    return;
                }
                cairo_set_source(_context, new_pattern);
                cairo_pattern_destroy(new_pattern);
            }
            else
            {
                cairo_set_source(_context, state->strokePattern);
            }
            repeat_type_t repeat = PatternImplCairo::get_repeat_type_for_cairo_pattern(state->strokePattern);
            if (NO_REPEAT == repeat)
            {
                cairo_pattern_set_extend(cairo_get_source(_context), CAIRO_EXTEND_NONE);
            }
            else
            {
                cairo_pattern_set_extend(cairo_get_source(_context), CAIRO_EXTEND_REPEAT);
            }
        }
        else if (state->strokeGradient)
        {
            if (state->globalAlpha < 1)
            {
                new_pattern = create_transparent_gradient(state->strokeGradient, state->globalAlpha);
                if (new_pattern == NULL)
                {
                    // failed to recognize gradient; Nan::ThrowError has already been called, so return from this fn.
                    return;
                }
                cairo_pattern_set_filter(new_pattern, state->patternQuality);
                cairo_set_source(_context, new_pattern);
                cairo_pattern_destroy(new_pattern);
            }
            else
            {
                cairo_pattern_set_filter(state->strokeGradient, state->patternQuality);
                cairo_set_source(_context, state->strokeGradient);
            }
        }
        else
        {
            setSourceRGBA(state->stroke);
        }

        if (preserve)
        {
            hasShadow()
                ? shadow(cairo_stroke_preserve)
                : cairo_stroke_preserve(_context);
        }
        else
        {
            hasShadow()
                ? shadow(cairo_stroke)
                : cairo_stroke(_context);
        }
    }

    // void setFontFromState();
    // void resetState(bool init = false);

    //==========================================================
    //  2D method
    //==========================================================
    void Context2DImplCairo::arc(float x, float y, float r, float startAngle, float endAngle, int anticlosewise)
    {
        if (anticlosewise && M_PI * 2 != endAngle)
        {
            cairo_arc_negative(_context, x, y, r, startAngle, endAngle);
        }
        else
        {
            cairo_arc(_context, x, y, r, startAngle, endAngle);
        }
    }
    void Context2DImplCairo::arcTo(float x1, float y1, float x2, float y2, float radius)
    {
        // Current path point
        double x, y;
        cairo_get_current_point(_context, &x, &y);
        Point<float> p0(x, y);

        // Point (x0,y0)
        Point<float> p1(x1, y1);

        // Point (x1,y1)
        Point<float> p2(x2, y2);

        if ((p1.x == p0.x && p1.y == p0.y) || (p1.x == p2.x && p1.y == p2.y) || radius == 0.f)
        {
            cairo_line_to(_context, p1.x, p1.y);
            return;
        }

        Point<float> p1p0((p0.x - p1.x), (p0.y - p1.y));
        Point<float> p1p2((p2.x - p1.x), (p2.y - p1.y));
        float p1p0_length = sqrtf(p1p0.x * p1p0.x + p1p0.y * p1p0.y);
        float p1p2_length = sqrtf(p1p2.x * p1p2.x + p1p2.y * p1p2.y);

        double cos_phi = (p1p0.x * p1p2.x + p1p0.y * p1p2.y) / (p1p0_length * p1p2_length);
        // all points on a line logic
        if (-1 == cos_phi)
        {
            cairo_line_to(_context, p1.x, p1.y);
            return;
        }

        if (1 == cos_phi)
        {
            // add infinite far away point
            unsigned int max_length = 65535;
            double factor_max = max_length / p1p0_length;
            Point<float> ep((p0.x + factor_max * p1p0.x), (p0.y + factor_max * p1p0.y));
            cairo_line_to(_context, ep.x, ep.y);
            return;
        }

        float tangent = radius / tan(acos(cos_phi) / 2);
        float factor_p1p0 = tangent / p1p0_length;
        Point<float> t_p1p0((p1.x + factor_p1p0 * p1p0.x), (p1.y + factor_p1p0 * p1p0.y));

        Point<float> orth_p1p0(p1p0.y, -p1p0.x);
        float orth_p1p0_length = sqrt(orth_p1p0.x * orth_p1p0.x + orth_p1p0.y * orth_p1p0.y);
        float factor_ra = radius / orth_p1p0_length;

        double cos_alpha = (orth_p1p0.x * p1p2.x + orth_p1p0.y * p1p2.y) / (orth_p1p0_length * p1p2_length);
        if (cos_alpha < 0.f)
            orth_p1p0 = Point<float>(-orth_p1p0.x, -orth_p1p0.y);

        Point<float> p((t_p1p0.x + factor_ra * orth_p1p0.x), (t_p1p0.y + factor_ra * orth_p1p0.y));

        orth_p1p0 = Point<float>(-orth_p1p0.x, -orth_p1p0.y);
        float sa = acos(orth_p1p0.x / orth_p1p0_length);
        if (orth_p1p0.y < 0.f)
            sa = 2 * M_PI - sa;

        bool anticlockwise = false;

        float factor_p1p2 = tangent / p1p2_length;
        Point<float> t_p1p2((p1.x + factor_p1p2 * p1p2.x), (p1.y + factor_p1p2 * p1p2.y));
        Point<float> orth_p1p2((t_p1p2.x - p.x), (t_p1p2.y - p.y));
        float orth_p1p2_length = sqrtf(orth_p1p2.x * orth_p1p2.x + orth_p1p2.y * orth_p1p2.y);
        float ea = acos(orth_p1p2.x / orth_p1p2_length);

        if (orth_p1p2.y < 0)
            ea = 2 * M_PI - ea;
        if ((sa > ea) && ((sa - ea) < M_PI))
            anticlockwise = true;
        if ((sa < ea) && ((ea - sa) > M_PI))
            anticlockwise = true;

        cairo_line_to(_context, t_p1p0.x, t_p1p0.y);

        if (anticlockwise && M_PI * 2 != radius)
        {
            cairo_arc_negative(_context, p.x, p.y, radius, sa, ea);
        }
        else
        {
            cairo_arc(_context, p.x, p.y, radius, sa, ea);
        }
    }
    void Context2DImplCairo::beginPath()
    {
        cairo_new_path(_context);
    }
    void Context2DImplCairo::bezierCurveTo(float x1, float y1, float x2, float y2, float x, float y)
    {
        cairo_curve_to(_context, x1, y1, x2, y2, x, y);
    }
    void Context2DImplCairo::clearRect(float x, float y, float w, float h)
    {
        if (0 == w || 0 == h)
            return;

        cairo_save(_context);
        savePath();
        cairo_rectangle(_context, x, y, w, h);
        cairo_set_operator(_context, CAIRO_OPERATOR_CLEAR);
        cairo_fill(_context);
        restorePath();
        cairo_restore(_context);
    }
    void Context2DImplCairo::clip(std::string rule)
    {
        setFillRule(rule);
        cairo_clip_preserve(_context);
    }
    void Context2DImplCairo::closePath()
    {
        cairo_close_path(_context);
    }
    ImageDataBase *Context2DImplCairo::createImageData()
    {
        //TODO
        return nullptr;
    }
    GradientBase *Context2DImplCairo::createLinearGradient()
    {
        // GradientBase *gradient = new GradientBase();
        return nullptr;
    }
    PatternBase *Context2DImplCairo::createPattern()
    {
        //TODO
        return nullptr;
    }
    GradientBase *Context2DImplCairo::createRadialGradient()
    {
        //TODO
        return nullptr;
    }

    void Context2DImplCairo::drawImage(ImageBase *image, float dx, float dy)
    {
        if (!image)
        {
            return;
        }
        float sx = 0, sy = 0;
        float sw = image->width(), sh = image->height();
        float dw = sw, dh = sh;
        Context2DImplCairo::drawImage(image, sx, sy, sw, sh, dx, dy, dw, dh);
    }
    void Context2DImplCairo::drawImage(ImageBase *image, float dx, float dy, float dw, float dh)
    {
        if (!image)
        {
            return;
        }
        float sx = 0, sy = 0;
        float sw = image->width(), sh = image->height();
        Context2DImplCairo::drawImage(image, sx, sy, sw, sh, dx, dy, dw, dh);
    }
    void Context2DImplCairo::drawImage(ImageBase *image, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh)
    {
        if (!image)
        {
            return;
        }
    }
    void Context2DImplCairo::fill(std::string rule)
    {
        setFillRule(rule);
    }
    void Context2DImplCairo::fillRect(float x, float y, float w, float h)
    {
        if (w == 0 || h == 0)
            return;
        savePath();
        cairo_rectangle(_context, x, y, w, h);
        fill("nonzero"); //TODO
        restorePath();
    }

    void Context2DImplCairo::fillText(const char *text, float x, float y, float maxWidth)
    {
        //TODO
    }
    ImageDataBase *Context2DImplCairo::GetImageData()
    {
        //TODO
    }
    void *Context2DImplCairo::GetLineDash()
    {
        //TODO
    }
    void Context2DImplCairo::lineTo(float x, float y)
    {
        cairo_line_to(_context, x, y);
    }
    float Context2DImplCairo::measureText(const char *text)
    {
        //TODO
    }
    void Context2DImplCairo::moveTo(float x, float y)
    {
        cairo_move_to(_context, x, y);
    }
    void Context2DImplCairo::putImageData(ImageDataBase *imageData, int x, int y, int w, int h, int dx, int dy, int dw, int dh)
    {
        //TOOD
        // mGCanvasContext->PutImageData( (const unsigned char *)imageData->GetData(), imageData->GetWidth(), imageData->GetHeight(), x, y, dx);
    }
    void Context2DImplCairo::quadraticCurveTo(float x1, float y1, float x2, float y2)
    {
        double x, y;
        cairo_get_current_point(_context, &x, &y);

        if (0 == x && 0 == y)
        {
            x = x1;
            y = y1;
        }
        cairo_curve_to(_context, x + 2.0 / 3.0 * (x1 - x), y + 2.0 / 3.0 * (y1 - y), x2 + 2.0 / 3.0 * (x1 - x2), y2 + 2.0 / 3.0 * (y1 - y2), x2, y2);
    }
    void Context2DImplCairo::rect(float x, float y, float w, float h)
    {
        if (w == 0)
        {
            cairo_move_to(_context, x, y);
            cairo_line_to(_context, x, y + h);
        }
        else if (h == 0)
        {
            cairo_move_to(_context, x, y);
            cairo_line_to(_context, x + w, y);
        }
        else
        {
            cairo_rectangle(_context, x, y, w, h);
        }
    }
    void Context2DImplCairo::restore()
    {
        if (stateno > 0)
        {
            cairo_restore(_context);
            pango_font_description_free(states[stateno]->fontDescription);
            free(states[stateno]);
            states[stateno] = NULL;
            state = states[--stateno];
            pango_layout_set_font_description(_layout, state->fontDescription);
        }
    }
    void Context2DImplCairo::rotate(float angle)
    {
        cairo_rotate(_context, angle);
    }
    void Context2DImplCairo::save()
    {
        if (stateno < CANVAS_MAX_STATES)
        {
            cairo_save(_context);
            states[++stateno] = (canvas_state_t *)malloc(sizeof(canvas_state_t));
            memcpy(states[stateno], state, sizeof(canvas_state_t));
            states[stateno]->fontDescription = pango_font_description_copy(states[stateno - 1]->fontDescription);
            state = states[stateno];
        }
    }
    void Context2DImplCairo::scale(float sx, float sy)
    {
        cairo_scale(_context, sx, sy);
    }
    void Context2DImplCairo::SetLineDash()
    {
        //TODO
    }
    void Context2DImplCairo::SetTransform(float a, float b, float c, float d, float tx, float ty)
    {
        cairo_identity_matrix(_context);
        transform(a, b, c, d, tx, ty);
    }
    void Context2DImplCairo::stroke()
    {
        doStroke();
    }
    void Context2DImplCairo::strokeRect(float x, float y, float w, float h)
    {
        if (0 == w && 0 == h)
            return;
        savePath();
        cairo_rectangle(_context, x, y, w, h);
        stroke();
        restorePath();
    }
    void Context2DImplCairo::strokeText(const char *text, float x, float y, float maxWidth)
    {
        // mGCanvasContext->StrokeText(text, x, y, maxWidth);
    }
    void Context2DImplCairo::transform(float a, float b, float c, float d, float tx, float ty)
    {
        cairo_matrix_t matrix;
        cairo_matrix_init(&matrix, a, b, c, d, tx, ty);
        cairo_transform(_context, &matrix);
    }
    void Context2DImplCairo::resetTransform()
    {
        cairo_identity_matrix(_context);
    }
    void Context2DImplCairo::translate(float tx, float ty)
    {
        cairo_translate(_context, tx, ty);
    }

    //==========================================================
    //  2D property
    //==========================================================
    std::string Context2DImplCairo::GetFillColor()
    {
    }
    void Context2DImplCairo::SetFillColor(std::string v)
    {
    }

    StyleType Context2DImplCairo::GetFillStyleType()
    {
    }
    void *Context2DImplCairo::GetFillStyle()
    {
    }
    void Context2DImplCairo::SetFillStyle(const void *v, StyleType stype)
    {
    }
    StyleType Context2DImplCairo::GetStrokeStyleType()
    {
    }
    void *Context2DImplCairo::GetStrokeStyle()
    {
    }
    void Context2DImplCairo::SetStrokeStyle(const void *v, StyleType stype)
    {
        if (stype == STYLE_TYPE_COLOR)
        {
        }
        else if (stype == STYLE_TYPE_PATTERN)
        {
        }
        else if (stype == STYLE_TYPE_GRADIENT_LINEAR)
        {
        }
        else if (stype == STYLE_TYPE_GRADIENT_RADIAL)
        {
        }
    }
    float Context2DImplCairo::GetGlobalAlpha()
    {
        return state->globalAlpha;
    }
    void Context2DImplCairo::SetGlobalAlpha(float alpha)
    {
        state->globalAlpha = alpha;
    }
    std::string Context2DImplCairo::GetGlobalCompositeOperation()
    {
        std::string op = "source-over";
        cairo_operator_t opType = cairo_get_operator(_context);
        switch (opType)
        {
        // composite modes:
        case CAIRO_OPERATOR_CLEAR:
            op = "clear";
            break;
        case CAIRO_OPERATOR_SOURCE:
            op = "copy";
            break;
        case CAIRO_OPERATOR_DEST:
            op = "destination";
            break;
        case CAIRO_OPERATOR_OVER:
            op = "source-over";
            break;
        case CAIRO_OPERATOR_DEST_OVER:
            op = "destination-over";
            break;
        case CAIRO_OPERATOR_IN:
            op = "source-in";
            break;
        case CAIRO_OPERATOR_DEST_IN:
            op = "destination-in";
            break;
        case CAIRO_OPERATOR_OUT:
            op = "source-out";
            break;
        case CAIRO_OPERATOR_DEST_OUT:
            op = "destination-out";
            break;
        case CAIRO_OPERATOR_ATOP:
            op = "source-atop";
            break;
        case CAIRO_OPERATOR_DEST_ATOP:
            op = "destination-atop";
            break;
        case CAIRO_OPERATOR_XOR:
            op = "xor";
            break;
        case CAIRO_OPERATOR_ADD:
            op = "lighter";
            break;
        // blend modes:
        // Note: "source-over" and "normal" are synonyms. Chrome and FF both report
        // "source-over" after setting gCO to "normal".
        // case CAIRO_OPERATOR_OVER: op = "normal";
        case CAIRO_OPERATOR_MULTIPLY:
            op = "multiply";
            break;
        case CAIRO_OPERATOR_SCREEN:
            op = "screen";
            break;
        case CAIRO_OPERATOR_OVERLAY:
            op = "overlay";
            break;
        case CAIRO_OPERATOR_DARKEN:
            op = "darken";
            break;
        case CAIRO_OPERATOR_LIGHTEN:
            op = "lighten";
            break;
        case CAIRO_OPERATOR_COLOR_DODGE:
            op = "color-dodge";
            break;
        case CAIRO_OPERATOR_COLOR_BURN:
            op = "color-burn";
            break;
        case CAIRO_OPERATOR_HARD_LIGHT:
            op = "hard-light";
            break;
        case CAIRO_OPERATOR_SOFT_LIGHT:
            op = "soft-light";
            break;
        case CAIRO_OPERATOR_DIFFERENCE:
            op = "difference";
            break;
        case CAIRO_OPERATOR_EXCLUSION:
            op = "exclusion";
            break;
        case CAIRO_OPERATOR_HSL_HUE:
            op = "hue";
            break;
        case CAIRO_OPERATOR_HSL_SATURATION:
            op = "saturation";
            break;
        case CAIRO_OPERATOR_HSL_COLOR:
            op = "color";
            break;
        case CAIRO_OPERATOR_HSL_LUMINOSITY:
            op = "luminosity";
            break;
        // non-standard:
        case CAIRO_OPERATOR_SATURATE:
            op = "saturate";
            break;
        }
        return op;
    }
    void Context2DImplCairo::SetGlobalCompositeOperation(std::string v)
    {
        const std::map<std::string, cairo_operator_t> blendmodes = {
            // composite modes:
            {"clear", CAIRO_OPERATOR_CLEAR},
            {"copy", CAIRO_OPERATOR_SOURCE},
            {"destination", CAIRO_OPERATOR_DEST}, // this seems to have been omitted from the spec
            {"source-over", CAIRO_OPERATOR_OVER},
            {"destination-over", CAIRO_OPERATOR_DEST_OVER},
            {"source-in", CAIRO_OPERATOR_IN},
            {"destination-in", CAIRO_OPERATOR_DEST_IN},
            {"source-out", CAIRO_OPERATOR_OUT},
            {"destination-out", CAIRO_OPERATOR_DEST_OUT},
            {"source-atop", CAIRO_OPERATOR_ATOP},
            {"destination-atop", CAIRO_OPERATOR_DEST_ATOP},
            {"xor", CAIRO_OPERATOR_XOR},
            {"lighter", CAIRO_OPERATOR_ADD},
            // blend modes:
            {"normal", CAIRO_OPERATOR_OVER},
            {"multiply", CAIRO_OPERATOR_MULTIPLY},
            {"screen", CAIRO_OPERATOR_SCREEN},
            {"overlay", CAIRO_OPERATOR_OVERLAY},
            {"darken", CAIRO_OPERATOR_DARKEN},
            {"lighten", CAIRO_OPERATOR_LIGHTEN},
            {"color-dodge", CAIRO_OPERATOR_COLOR_DODGE},
            {"color-burn", CAIRO_OPERATOR_COLOR_BURN},
            {"hard-light", CAIRO_OPERATOR_HARD_LIGHT},
            {"soft-light", CAIRO_OPERATOR_SOFT_LIGHT},
            {"difference", CAIRO_OPERATOR_DIFFERENCE},
            {"exclusion", CAIRO_OPERATOR_EXCLUSION},
            {"hue", CAIRO_OPERATOR_HSL_HUE},
            {"saturation", CAIRO_OPERATOR_HSL_SATURATION},
            {"color", CAIRO_OPERATOR_HSL_COLOR},
            {"luminosity", CAIRO_OPERATOR_HSL_LUMINOSITY},
            // non-standard:
            {"saturate", CAIRO_OPERATOR_SATURATE}};

        auto op = blendmodes.find(v);
        if (op != blendmodes.end())
        {
            cairo_set_operator(_context, op->second);
        }
    }
std::string Context2DImplCairo::GetLineCap()
{
    cairo_line_cap_t cap = cairo_get_line_cap(_context);
    if( cap == CAIRO_LINE_CAP_ROUND) return "round";
    else if( cap == CAIRO_LINE_CAP_SQUARE ) return "square";
    else return "butt";
}
void Context2DImplCairo::SetLineCap(std::string  v)
{
    cairo_line_cap_t cap = CAIRO_LINE_CAP_BUTT;
    if ( v == "round" ) cap = CAIRO_LINE_CAP_ROUND;
    else if( v == "square" ) cap = CAIRO_LINE_CAP_SQUARE;
    else cap = CAIRO_LINE_CAP_BUTT;
    cairo_set_line_cap(_context, cap);
}
float Context2DImplCairo::GetLineDashOffset()
{
    double offset;
    cairo_get_dash(_context, NULL, &offset);
    return offset;
}
void Context2DImplCairo::SetLineDashOffset(float v)
{
    int dashes = cairo_get_dash_count(_context);
    std::vector<double> a(dashes);
    cairo_get_dash(_context, a.data(), NULL);
    cairo_set_dash(_context, a.data(), dashes, v);
}
std::string Context2DImplCairo::GetLineJoin()
{
    cairo_line_join_t join =  cairo_get_line_join(_context);
    switch (join)
    {
        case CAIRO_LINE_JOIN_BEVEL: return "bevel";
        case CAIRO_LINE_JOIN_ROUND: return "round";
        default: return "miter";
    }
}
void Context2DImplCairo::SetLineJoin( std::string v)
{
    cairo_line_join_t join = CAIRO_LINE_JOIN_MITER;
    if ( v == "round" ) join = CAIRO_LINE_JOIN_ROUND;
    else if( v == "bevel" ) join = CAIRO_LINE_JOIN_BEVEL;
    else join = CAIRO_LINE_JOIN_MITER;
    cairo_set_line_join(_context, join);
}
float Context2DImplCairo::GetLineWidth()
{
    return cairo_get_line_width(_context);
}
void Context2DImplCairo::SetLineWidth(float v)
{
    cairo_set_line_width(_context, v);
}
float Context2DImplCairo::GetMiterLimit()
{
    return cairo_get_miter_limit(_context);
}
void Context2DImplCairo::SetMiterLimit(float v)
{
    cairo_set_miter_limit(_context, v);
}
float Context2DImplCairo::GetShadowBlur()
{
    float v = 0.0;
    if (state)
    {
        v =  state->shadowBlur;
    }
    return v;
}
void Context2DImplCairo::SetShadowBlur(float v)
{
    if( state )
    {
        state->shadowBlur = v;
    }
}
std::string Context2DImplCairo::GetShadowColor()
{
    std::string color = nullptr;
    // GCanvasState *state = mGCanvasContext->GetCurrentState();
    // if (state)
    // {
    //     color = gcanvas::ColorToString(state->mShadowColor);
    // }
    return color;
}
void Context2DImplCairo::SetShadowColor(std::string v)
{
    // GCanvasState *state = mGCanvasContext->GetCurrentState();
    // if (state)
    // {
    //     state->mShadowColor = gcanvas::StrValueToColorRGBA(v.c_str());
    // }
}
float Context2DImplCairo::GetShadowOffsetX()
{
    float v = 0.0;
    if (state)
    {
        v =  state->shadowOffsetX;
    }
    return v;
}
void Context2DImplCairo::SetShadowOffsetX(float v)
{
    if( state )
    {
        state->shadowOffsetX = v;
    }
}
float Context2DImplCairo::GetShadowOffsetY()
{
    float v = 0.0;
    if (state)
    {
        v =  state->shadowOffsetY;
    }
    return v;
}
void Context2DImplCairo::SetShadowOffsetY(float v)
{
    if( state )
    {
        state->shadowOffsetY = v;
    }
}
std::string Context2DImplCairo::GetFont()
{
    //TOOD
}
void Context2DImplCairo::SetFont(const std::string &v)
{
    //TODO
}

std::string Context2DImplCairo::GetTextAlign()
{
    //TODO
}

void Context2DImplCairo::SetTextAlign(std::string v)
{
    // GTextAlign align = mGCanvasContext->TextAlign;
    // if (v == "start") align = TEXT_ALIGN_START;
    // else if (v == "end") align = TEXT_ALIGN_END;
    // else if (v == "left") align = TEXT_ALIGN_LEFT;
    // else if (v == "center") align = TEXT_ALIGN_CENTER;
    // else if (  v == "right" ) align = TEXT_ALIGN_RIGHT;
    // mGCanvasContext->SetTextAlign(align);
}
std::string Context2DImplCairo::GetTextBaseline()
{
    // short mode = state->textBaseline;
    // switch (mode) {
    //     case 0: return "alphabetic";
    //     case 1: return "top";
    //     case 2: return "bottom";
    //     case 3: return "middle";
    //     case 5: return "hanging";
    //     case 4: return "ideographic";
    //     default: return "alphabetic";
    // }
}
void Context2DImplCairo::SetTextBaseline(std::string v)
{
    const std::map<std::string, int32_t> modes = {
        {"alphabetic", 0},
        {"top", 1},
        {"bottom", 2},
        {"middle", 3},
        {"ideographic", 4},
        {"hanging", 5}
    };

    auto iter = modes.find(v);
    if( iter == modes.end() ) return;

    state->textBaseline = iter->second;
}

void* Context2DImplCairo::GetCanvas()
{

}

} // namespace NodeBinding
