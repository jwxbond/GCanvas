/**
 * 
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */

#ifndef CONTEXT2DBASE_H
#define CONTEXT2DBASE_H
#include <iostream>
#include <unordered_map>
#include "ImageDataBase.h"
#include "ImageBase.h"
#include "PatternBase.h"
#include "GradientBase.h"


namespace NodeBinding 
{

typedef enum {
    STYLE_TYPE_COLOR,
    STYLE_TYPE_PATTERN,
    STYLE_TYPE_GRADIENT_LINEAR,
    STYLE_TYPE_GRADIENT_RADIAL
} StyleType;

class Context2DBase {

public:
    Context2DBase(){};
    Context2DBase(const Context2DBase&){};
    virtual ~Context2DBase() {}

    virtual void resize( int w, int h ) = 0;

    // 2D method
    virtual void Arc(float x, float y, float r, float startAngle, float endAngle, int anticlosewise=0 ) = 0;
    virtual void ArcTo(float x1, float y1, float x2, float y2, float radius) = 0;
    virtual void BeginPath() = 0;
    virtual void BezierCurveTo(float x1, float y1, float x2, float y2, float x, float y) = 0;
    virtual void ClearRect(float x, float y, float w, float h) = 0;
    virtual void Clip(std::string rule) = 0;
    virtual void ClosePath() = 0;
    virtual ImageDataBase* CreateImageData() = 0;
    virtual GradientBase* CreateLinearGradient() = 0;
    virtual PatternBase* CreatePattern() = 0;
    virtual GradientBase* CreateRadialGradient() = 0;
    virtual void DrawImage(ImageBase *image, float dx, float dy) = 0;
    virtual void DrawImage(ImageBase *image, float dx, float dy, float dw, float dh) = 0;
    virtual void DrawImage(ImageBase *image, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh) = 0;
    virtual void Fill(std::string rule) = 0;
    virtual void FillRect(float x, float y, float w, float h) = 0;
    virtual void FillText(const char * text, float x, float y, float maxWidth) = 0;
    virtual ImageDataBase* GetImageData() = 0;
    virtual void* GetLineDash() = 0;
    virtual void LineTo(float x, float y) = 0;
    virtual float MeasureText(const char * text) = 0;
    virtual void MoveTo(float x, float y) = 0;
    virtual void PutImageData(ImageDataBase *data, int x,  int y, int w, int h, int dx, int dy, int dw, int dh ) = 0;
    virtual void QuadraticCurveTo(float cpx, float cpy, float x, float y) = 0;
    virtual void Rect(float x, float y, float w, float h) = 0;
    virtual void Restore() = 0;
    virtual void Rotate(float angle) = 0;
    virtual void Save() = 0;
    virtual void Scale(float sx, float sy) = 0;
    virtual void SetLineDash() = 0;
    virtual void SetTransform(float a, float b, float c, float d, float tx, float ty) = 0;
    virtual void Stroke() = 0;
    virtual void StrokeText(const char * text, float x, float y, float maxWidth) = 0;
    virtual void Transform(float a, float b, float c, float d, float tx, float ty) = 0;
    virtual void ResetTransform() = 0;
    virtual void Translate(float tx, float ty) = 0;
    // 2D property
    virtual std::string GetFillColor() = 0;
    virtual void SetFillColor(std::string v)  = 0;
    virtual StyleType GetFillStyleType() = 0;
    virtual void* GetFillStyle() = 0;
    virtual void SetFillStyle(const void* v, StyleType stype) = 0;
    virtual StyleType GetStrokeStyleType() = 0;
    virtual void* GetStrokeStyle() = 0;
    virtual void SetStrokeStyle(const void* v, StyleType stype) = 0;
    virtual float GetGlobalAlpha() = 0;
    virtual void SetGlobalAlpha(float alpha) = 0;
    virtual std::string  GetGlobalCompositeOperation() = 0;
    virtual void SetGlobalCompositeOperation(std::string v) = 0;
    virtual std::string GetLineCap() = 0;
    virtual void SetLineCap(std::string  v) = 0;
    virtual float GetLineDashOffset() = 0;
    virtual void SetLineDashOffset(float v ) = 0;
    virtual std::string GetLineJoin() = 0;
    virtual void SetLineJoin( std::string v) = 0;
    virtual float GetLineWidth() = 0;
    virtual void SetLineWidth(float v) = 0;
    virtual float GetMiterLimit() = 0;
    virtual void SetMiterLimit(float v) = 0;
    virtual float GetShadowBlur() = 0;
    virtual void SetShadowBlur(float v) = 0;
    virtual std::string GetShadowColor() = 0;
    virtual void SetShadowColor(std::string v) = 0;
    virtual float GetShadowOffsetX() = 0;
    virtual void SetShadowOffsetX(float v) = 0;
    virtual float GetShadowOffsetY() = 0;
    virtual void SetShadowOffsetY(float v) = 0;
    virtual std::string GetFont() = 0;
    virtual void SetFont(const std::string &v)  = 0;
    virtual std::string GetTextAlign() = 0;
    virtual void SetTextAlign(std::string v) = 0;
    virtual std::string GetTextBaseline() = 0;
    virtual void SetTextBaseline(std::string v) = 0;
    virtual void* GetCanvas() = 0;
};

}   

#endif
