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
#include "PatternBase.h"
#include "GradientBase.h"


namespace NodeBinding 
{
class Context2DBase {

    virtual void resize( int w, int h ) =  0;


    // 2D method
    virtual void arc(float x, float y, float r, float startAngle, float endAngle, int anticlosewise=0 ) = 0;
    virtual void arcTo(float x1, float y1, float x2, float y2, float radius) = 0;
    virtual void beginPath() = 0;
    virtual void bezierCurveTo(float x1, float y1, float x2, float y2, float x, float y) = 0;
    virtual void clearRect(float x, float y, float w, float h) = 0;
    virtual void clip(std::string rule) = 0;
    virtual void closePath() = 0;
    virtual ImageDataBase* createImageData() = 0;
    virtual GradientBase* createLinearGradient() = 0;
    virtual PatternBase* createPattern() = 0;
    virtual GradientBase* createRadialGradient() = 0;
    virtual void drawImage() = 0;
    virtual void fill(std::string rule) = 0;
    virtual void fillRect(float x, float y, float w, float h);
    virtual void fillText(const char * text, float x, float y, float maxWidth) = 0;
    virtual ImageDataBase* GetImageData() = 0;
    virtual void* GetLineDash() = 0;
    virtual void lineTo(float x, float y) = 0;
    virtual void* measureText(const char * text) = 0;
    virtual void moveTo(float x, float y) = 0;
    virtual void putImageData(ImageDataBase *data, int x,  int y, int w, int h, int dx, int dy, int dw, int dh ) = 0;
    virtual void quadraticCurveTo() = 0;
    virtual void rect(float x, float y, float w, float h) = 0;
    virtual void restore() = 0;
    virtual void rotate(float angle) = 0;
    virtual void save() = 0;
    virtual void scale(float sx, float sy) = 0;
    virtual void SetLineDash() = 0;
    virtual void SetTransform(float a, float b, float c, float d, float tx, float ty) = 0;
    virtual void stroke() = 0;
    virtual void strokeRect(float x, float y, float w, float h) = 0;
    virtual void strokeText(const char * text, float x, float y, float maxWidth) = 0;
    virtual void transform(float a, float b, float c, float d, float tx, float ty) = 0;
    virtual void resetTransform() = 0;
    virtual void translate(float tx, float ty) = 0;

    // 2D property
    virtual std::string GetFillColor() = 0;
    virtual void SetFillColor(std::string v)  = 0;

    virtual void* GetFillStyle() = 0;
    virtual void SetFillStyle(const void* v)  = 0;
    
    virtual void* GetStrokeStyle() = 0;
    virtual void SetStrokeStyle(const void* v) = 0;

    virtual float GetGlobalAlpha() = 0;
    virtual void SetGlobalAlpha(float alpha) = 0;

    virtual std::string  GetGlobalCompositeOperation() = 0;
    virtual void SetGlobalCompositeOperation(std::string v) = 0;
    
    virtual std::string GetLineCap() = 0;
    virtual void SetLineCap(std::string  v);

    virtual void* GetLineDashOffset() = 0;
    virtual void SetLineDashOffset(const void* v ) = 0;
    virtual  std::string GetLineJoin() = 0;
    virtual void SetLineJoin( std::string v) = 0;
    virtual float GetlineWidth() = 0;
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
    virtual void SetFont(const void* v)  = 0;
    virtual std::string GetTextAlign() = 0;
    virtual void SetTextAlign(std::string v) = 0;
    virtual std::string GetTextBaseline() = 0;
    virtual void SetTextBaseline(std::string v) = 0;

    virtual void* GetCanvas() = 0;
};

}   

#endif
