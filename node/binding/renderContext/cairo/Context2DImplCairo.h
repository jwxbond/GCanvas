/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef CONTEXT2DIMPLCAIRO_H
#define CONTEXT2DIMPLCAIRO_H

#include "Context2DBase.h"
#include "ImageDataBase.h"
#include "ImageBase.h"

#include "cairo.h"
// #include "Canvas.h"
// #include "color.h"
// #include "nan.h"
#include <pango/pangocairo.h>

typedef enum {
  TEXT_DRAW_PATHS,
  TEXT_DRAW_GLYPHS
} canvas_draw_mode_t;

#include <iostream>
#include <memory>
#include <functional>
#include <unordered_map>

#ifndef CANVAS_MAX_STATES
#define CANVAS_MAX_STATES 64
#endif

namespace NodeBinding
{
    typedef enum {
        TEXT_DRAW_PATHS,
        TEXT_DRAW_GLYPHS
    } canvas_draw_mode_t;

    typedef struct {
        rgba_t fill;
        rgba_t stroke;
        cairo_filter_t patternQuality;
        cairo_pattern_t *fillPattern;
        cairo_pattern_t *strokePattern;
        cairo_pattern_t *fillGradient;
        cairo_pattern_t *strokeGradient;
        float globalAlpha;
        short textAlignment;
        short textBaseline;
        rgba_t shadow;
        int shadowBlur;
        double shadowOffsetX;
        double shadowOffsetY;
        canvas_draw_mode_t textDrawingMode;
        PangoFontDescription *fontDescription;
        bool imageSmoothingEnabled;
    } canvas_state_t;

    void state_assign_fontFamily(canvas_state_t *state, const char *str);

    typedef struct {
        float x;
        float y;
        float width;
        float height;
    } float_rectangle;


    class Context2DImplCairo : public Context2DBase  
    {
    public:
        Context2DImplCairo();
        Context2DImplCairo(int width, int height);
        Context2DImplCairo(int width, int height, int ratio);
        virtual ~Context2DImplCairo();

        void SetupContext2D();


        virtual void resize(int w, int h);

        // 2D method
        virtual void arc(float x, float y, float r, float startAngle, float endAngle, int anticlosewise=0);
        virtual void arcTo(float x1, float y1, float x2, float y2, float radius);
        virtual void beginPath();
        virtual void bezierCurveTo(float x1, float y1, float x2, float y2, float x, float y);
        virtual void clearRect(float x, float y, float w, float h);
        virtual void clip(std::string rule);
        virtual void closePath();
        virtual ImageDataBase* createImageData();
        virtual GradientBase* createLinearGradient();
        virtual PatternBase* createPattern();
        virtual GradientBase* createRadialGradient();
        virtual void drawImage(ImageBase *image, float dx, float dy);
        virtual void drawImage(ImageBase *image, float dx, float dy, float dw, float dh);
        virtual void drawImage(ImageBase *image, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh);
        virtual void fill(std::string rule);
        virtual void fillRect(float x, float y, float w, float h);
        virtual void fillText(const char * text, float x, float y, float maxWidth);
        virtual ImageDataBase* GetImageData();
        virtual void* GetLineDash();
        virtual void lineTo(float x, float y);
        virtual float measureText(const char * text);
        virtual void moveTo(float x, float y);
        virtual void putImageData(ImageDataBase *data, int x,  int y, int w, int h, int dx, int dy, int dw, int dh );
        virtual void quadraticCurveTo(float cpx, float cpy, float x, float y);
        virtual void rect(float x, float y, float w, float h);
        virtual void restore();
        virtual void rotate(float angle);
        virtual void save();
        virtual void scale(float sx, float sy);
        virtual void SetLineDash();
        virtual void SetTransform(float a, float b, float c, float d, float tx, float ty);
        virtual void stroke();
        virtual void strokeRect(float x, float y, float w, float h);
        virtual void strokeText(const char * text, float x, float y, float maxWidth);
        virtual void transform(float a, float b, float c, float d, float tx, float ty);
        virtual void resetTransform();
        virtual void translate(float tx, float ty);

        // 2D property
        virtual std::string GetFillColor();
        virtual void SetFillColor(std::string v);

        virtual StyleType GetFillStyleType();
        virtual void* GetFillStyle();
        virtual void SetFillStyle(const void* v, StyleType stype);

        virtual StyleType GetStrokeStyleType();
        virtual void* GetStrokeStyle();
        virtual void SetStrokeStyle(const void* v, StyleType stype);

        virtual float GetGlobalAlpha();
        virtual void SetGlobalAlpha(float alpha);

        virtual std::string  GetGlobalCompositeOperation();
        virtual void SetGlobalCompositeOperation(std::string v);
        
        virtual std::string GetLineCap();
        virtual void SetLineCap(std::string  v);

        virtual float GetLineDashOffset();
        virtual void SetLineDashOffset(float v);
        virtual  std::string GetLineJoin();
        virtual void SetLineJoin( std::string v);
        virtual float GetlineWidth();
        virtual void SetLineWidth(float v);
        virtual float GetMiterLimit();
        virtual void SetMiterLimit(float v);

        virtual float GetShadowBlur();
        virtual void SetShadowBlur(float v);
        virtual std::string GetShadowColor();
        virtual void SetShadowColor(std::string v);
        virtual float GetShadowOffsetX();
        virtual void SetShadowOffsetX(float v);
        virtual float GetShadowOffsetY();
        virtual void SetShadowOffsetY(float v);

        virtual std::string GetFont();
        virtual void SetFont(const std::string &v);
        virtual std::string GetTextAlign();
        virtual void SetTextAlign(std::string v);
        virtual std::string GetTextBaseline();
        virtual void SetTextBaseline(std::string v);

    
        
        virtual void* GetCanvas();

    protected:
        inline void setContext(cairo_t *ctx) { _context = ctx; }
        inline cairo_t *context(){ return _context; }
        inline Canvas *canvas(){ return _canvas; }
        inline bool hasShadow();
        void inline setSourceRGBA(rgba_t color);
        void inline setSourceRGBA(cairo_t *ctx, rgba_t color);
        void setTextPath(double x, double y);
        void blur(cairo_surface_t *surface, int radius);
        void shadow(void (fn)(cairo_t *cr));
        void shadowStart();
        void shadowApply();
        void savePath();
        void restorePath();
        void saveState();
        void restoreState();
        // void inline setFillRule(v8::Local<v8::Value> value);
        void doFill(bool preserve = false);
        void doStroke(bool preserve = false);
        void save();
        void restore();
        void setFontFromState();
        void resetState(bool init = false);
        inline PangoLayout *layout(){ return _layout; }


    public:
        short stateno;
        canvas_state_t *states[CANVAS_MAX_STATES];
        canvas_state_t *state;

    protected:
        
        void SetFillRule(std::string value);


        cairo_t *_context;
        cairo_path_t *_path;
        PangoLayout *_layout;


    private:
        

    };
} // namespace NodeBinding

#endif