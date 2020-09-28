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

#include "Context2DBase.h"
#include "ImageDataBase.h"
#include "ImageBase.h"

#include <GCanvas.hpp>
#include <gcanvas/GCanvas2dContext.h>

#include <iostream>
#include <memory>
#include <functional>
#include <unordered_map>


namespace NodeBinding
{
    class Context2DImplGCanvas : public Context2DBase  
    {
    public:
        Context2DImplGCanvas();
        Context2DImplGCanvas(int width, int height);
        Context2DImplGCanvas(int width, int height, int ratio);
        virtual ~Context2DImplGCanvas();

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
        virtual float GetLineWidth();
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
        int mWidth;
        int mHeight;
        float mRatio;

        StyleType mFillStyleType;
        StyleType mStrokeStyleType;

        // std::shared_ptr<gcanvas::GCanvas> mGCanvas;
        // std::shared_ptr<GCanvasContext> mGCanvasContext;

        gcanvas::GCanvas *mGCanvas;
        GCanvasContext *mGCanvasContext;
    };
} // namespace NodeBinding

#endif