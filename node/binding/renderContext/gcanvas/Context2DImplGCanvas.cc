/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */


#include "Context2DImplGCanvas.h"

namespace NodeBinding
{


Context2DImplGCanvas::Context2DImplGCanvas()
    : mWidth(100), mHeight(100), mRatio(1.0)
{
    SetupContext2D();
}
 
 Context2DImplGCanvas::Context2DImplGCanvas(int width, int height)
    :mWidth(width), mHeight(height), mRatio(1.0)
 {
    SetupContext2D();
 }

Context2DImplGCanvas::Context2DImplGCanvas(int width, int height, int ratio)
        :mWidth(width), mHeight(height), mRatio(ratio)
{
    SetupContext2D();
}

Context2DImplGCanvas::~Context2DImplGCanvas()
{
    if( mGCanvasContext ) {
        delete mGCanvasContext;
        mGCanvasContext = nullptr;
    }

    if( mGCanvas ){
        delete mGCanvas;
        mGCanvas = nullptr;
    }
}

void Context2DImplGCanvas::SetupContext2D()
{
    GCanvasConfig config = {true, false};
    // mGCanvas = std::make_shared<gcanvas::GCanvas>( "context2d-gcanvas", config, nullptr );
    mGCanvas = new gcanvas::GCanvas( "context2d-gcanvas", config, nullptr );
    if( !mGCanvas ) {
        return;
    }

    mGCanvas->CreateContext();
    mGCanvasContext = mGCanvas->GetGCanvasContext();
    if (!mGCanvasContext ){
        return;
    }

    mGCanvasContext->SetClearColor(gcanvas::StrValueToColorRGBA("transparent"));
    mGCanvasContext->ClearScreen();
    mGCanvasContext->SetDevicePixelRatio(mRatio);
    mGCanvas->OnSurfaceChanged(0, 0, mWidth, mHeight);
}


//implement Context2DBase API
void Context2DImplGCanvas::resize( int w, int h )
{
    //TODO resize
}

//==========================================================
//  2D method
//==========================================================
void Context2DImplGCanvas::arc(float x, float y, float r, float startAngle, float endAngle, int anticlosewise)
{
    mGCanvasContext->Arc(x, y, r, startAngle, endAngle, anticlosewise);
}
void Context2DImplGCanvas::arcTo(float x1, float y1, float x2, float y2, float radius)
{
    mGCanvasContext->ArcTo(x1, y1, x2, y2, radius);
}
void Context2DImplGCanvas::beginPath()
{
    mGCanvasContext->BeginPath();
}
void Context2DImplGCanvas::bezierCurveTo(float x1, float y1, float x2, float y2, float x, float y)
{
    mGCanvasContext->BezierCurveTo(x1, y1, x2, y2, x, y);
}
void Context2DImplGCanvas::clearRect(float x, float y, float w, float h)
{
    mGCanvasContext->ClearRect(x, y, w, h);
}
void Context2DImplGCanvas::clip(std::string rule)
{
    GFillRule fillRule = FILL_RULE_NONZERO;
    if (rule == "evenodd")
    {
        fillRule = FILL_RULE_EVENODD;
    }
    mGCanvasContext->Clip(fillRule);
}
void Context2DImplGCanvas::closePath()
{
    mGCanvasContext->ClosePath();
}
ImageDataBase* Context2DImplGCanvas::createImageData()
{
    // ImageDataBase *imageData = 
    return nullptr;
}
GradientBase* Context2DImplGCanvas::createLinearGradient()
{
    //TODO
    return nullptr;
}
PatternBase* Context2DImplGCanvas::createPattern()
{
    //TODO
    return nullptr;
}
GradientBase* Context2DImplGCanvas::createRadialGradient()
{
    //TODO
    return nullptr;
}

void Context2DImplGCanvas::drawImage(ImageBase *image, float dx, float dy)
{
    if (!image) { return; }
    float sx = 0, sy = 0;
    float sw = image->width(), sh = image->height();
    float dw = sw, dh = sh;
    Context2DImplGCanvas::drawImage(image, sx, sy, sw, sh, dx, dy, dw, dh);
}
void Context2DImplGCanvas::drawImage(ImageBase *image, float dx, float dy, float dw, float dh)
{
    if (!image) { return; }
    float sx = 0, sy = 0;
    float sw = image->width(), sh = image->height();
    Context2DImplGCanvas::drawImage(image, sx, sy, sw, sh, dx, dy, dw, dh);
}
void Context2DImplGCanvas::drawImage(ImageBase *image, float sx, float sy, float sw, float sh, float dx, float dy, float dw, float dh)
{
    if (!image) { return; }
    mGCanvasContext->DrawImage(image->textureId(), image->width(), image->height(), sx, sy, sw, sh, dx, dy, dw, dh);
}
void Context2DImplGCanvas::fill(std::string rule)
{
    GFillRule fillRule = FILL_RULE_NONZERO;
    if (rule == "evenodd")
    {
        fillRule = FILL_RULE_EVENODD;
    }
    mGCanvasContext->Fill(fillRule);
}
void Context2DImplGCanvas::fillRect(float x, float y, float w, float h)
{
    mGCanvasContext->FillRect(x, y, w, h);

}
void Context2DImplGCanvas::fillText(const char * text, float x, float y, float maxWidth)
{
    mGCanvasContext->DrawText(text, x, y, maxWidth);
}
ImageDataBase* Context2DImplGCanvas::GetImageData()
{
    //TODO
}
void* Context2DImplGCanvas::GetLineDash()
{
    //TODO
}
void Context2DImplGCanvas::lineTo(float x, float y)
{
    mGCanvasContext->LineTo(x, y);
}
float Context2DImplGCanvas::measureText(const char * text)
{
    float width = mGCanvasContext->MeasureTextWidth(text);
    return width;
}
void Context2DImplGCanvas::moveTo(float x, float y)
{
    mGCanvasContext->MoveTo(x, y);
}
void Context2DImplGCanvas::putImageData(ImageDataBase *imageData, int x,  int y, int w, int h, int dx, int dy, int dw, int dh )
{
    //TOOD
    // mGCanvasContext->PutImageData( (const unsigned char *)imageData->GetData(), imageData->GetWidth(), imageData->GetHeight(), x, y, dx);
}
void Context2DImplGCanvas::quadraticCurveTo(float cpx, float cpy, float x, float y)
{
    mGCanvasContext->QuadraticCurveTo(cpx, cpy, x, y );
}
void Context2DImplGCanvas::rect(float x, float y, float w, float h)
{
    mGCanvasContext->Rect(x, y, w, h);
}
void Context2DImplGCanvas::restore()
{
    mGCanvasContext->Restore();
}
void Context2DImplGCanvas::rotate(float angle)
{
    mGCanvasContext->Rotate(angle);
}
void Context2DImplGCanvas::save()
{
    mGCanvasContext->Save();
}
void Context2DImplGCanvas::scale(float sx, float sy)
{
    mGCanvasContext->Scale(sx, sy);
}
void Context2DImplGCanvas::SetLineDash()
{
    //TODO
    
}
void Context2DImplGCanvas::SetTransform(float a, float b, float c, float d, float tx, float ty)
{
    mGCanvasContext->SetTransform(a, b, c, d, tx, ty);
}
void Context2DImplGCanvas::stroke()
{
    mGCanvasContext->Stroke();
}
void Context2DImplGCanvas::strokeRect(float x, float y, float w, float h)
{
    mGCanvasContext->StrokeRect(x, y, w, h);
}
void Context2DImplGCanvas::strokeText(const char * text, float x, float y, float maxWidth)
{
    mGCanvasContext->StrokeText(text, x, y, maxWidth);
}
void Context2DImplGCanvas::transform(float a, float b, float c, float d, float tx, float ty)
{
    mGCanvasContext->Transfrom(a, b, c, d, tx, ty); //TODO rename
}
void Context2DImplGCanvas::resetTransform()
{
    mGCanvasContext->ResetTransform();
}
void Context2DImplGCanvas::translate(float tx, float ty)
{
    mGCanvasContext->Translate(tx, ty);
}


//==========================================================
//  2D property
//==========================================================
std::string Context2DImplGCanvas::GetFillColor()
{
    std::string color = nullptr;
    GCanvasState *state = mGCanvasContext->GetCurrentState();
    if ( state)
    {
        color =  gcanvas::ColorToString( state->mFillColor );
    }
    return color;
}
void Context2DImplGCanvas::SetFillColor(std::string v) 
{
    mGCanvasContext->SetFillStyle(v.c_str());
}

StyleType Context2DImplGCanvas::GetFillStyleType()
{
    return mFillStyleType;
}    
void* Context2DImplGCanvas::GetFillStyle()
{
    void* style = nullptr;
    GCanvasState *state = mGCanvasContext->GetCurrentState();
    if ( state)
    {
        style = state->mFillStyle;
    }
    return style;
}
void Context2DImplGCanvas::SetFillStyle(const void* v, StyleType stype)
{
    mFillStyleType = stype;
    GCanvasState *state = mGCanvasContext->GetCurrentState();
    if ( state)
    {
        state->mFillStyle = (GFillStyle*)v;
    }
}
StyleType Context2DImplGCanvas::GetStrokeStyleType()
{
    return mStrokeStyleType;
}
void* Context2DImplGCanvas::GetStrokeStyle()
{
    GCanvasState *state = mGCanvasContext->GetCurrentState();
    if ( state)
    {
        return state->mStrokeStyle;
    }
    return nullptr;
}
void Context2DImplGCanvas::SetStrokeStyle(const void* v, StyleType stype)
{
     mStrokeStyleType = stype;
    GCanvasState *state = mGCanvasContext->GetCurrentState();
    if ( state)
    {
        state->mStrokeStyle = (GFillStyle*)v;
    }
}
float Context2DImplGCanvas::GetGlobalAlpha()
{
    return mGCanvasContext->GlobalAlpha();
}
void Context2DImplGCanvas::SetGlobalAlpha(float alpha)
{
    mGCanvasContext->SetGlobalAlpha(alpha);
}
std::string  Context2DImplGCanvas::GetGlobalCompositeOperation()
{
    GCompositeOperation op =  mGCanvasContext->GlobalCompositeOperation();
    switch (op)
    {
        case COMPOSITE_OP_SOURCE_ATOP: return "source-atop";
        case COMPOSITE_OP_SOURCE_OUT: return "source-out";
        case COMPOSITE_OP_SOURCE_IN:  return "source-in";
        case COMPOSITE_OP_DESTINATION_OVER: return "destination-over";
        case COMPOSITE_OP_DESTINATION_ATOP: return "destination-atop";
        case COMPOSITE_OP_DESTINATION_OUT: return "destination-out";
        case COMPOSITE_OP_LIGHTER: return "lighter";
        case COMPOSITE_OP_COPY: return "copy";
        case COMPOSITE_OP_XOR: return "xor";
        default: return "source-over";
    }
}
void Context2DImplGCanvas::SetGlobalCompositeOperation(std::string v)
{
    GCompositeOperation op =  COMPOSITE_OP_SOURCE_OVER;

    if( v == "source-over" ) op = COMPOSITE_OP_SOURCE_OVER;
    else if( v ==  "source-atop") op = COMPOSITE_OP_SOURCE_ATOP;
    else if( v ==  "source-in") op =  COMPOSITE_OP_SOURCE_IN;
    else if( v ==   "source-out") op = COMPOSITE_OP_SOURCE_OUT;
    else if(  v == "destination-over") op = COMPOSITE_OP_DESTINATION_OVER;
    else if( v == "destination-atop" ) op = COMPOSITE_OP_DESTINATION_ATOP;
    else if( v == "destination-out" ) op = COMPOSITE_OP_DESTINATION_OUT;
    else if( v == "lighter" ) op = COMPOSITE_OP_LIGHTER;
    else if( v == "copy" ) op = COMPOSITE_OP_COPY;
    else if( v == "xor" ) op = COMPOSITE_OP_XOR;

    mGCanvasContext->SetGlobalCompositeOperation(op);
}
std::string Context2DImplGCanvas::GetLineCap()
{
    GLineCap cap =  mGCanvasContext->LineCap();
    if( cap == LINE_CAP_ROUND) return "round";
    else if( cap == LINE_CAP_SQUARE ) return "square";
    else return "butt";
}
void Context2DImplGCanvas::SetLineCap(std::string  v)
{
    mGCanvasContext->SetLineCap(v.c_str());
}
float Context2DImplGCanvas::GetLineDashOffset()
{
    float v = 0.0;
    GCanvasState* state = mGCanvasContext->GetCurrentState();
    if (state)
    {
        v = state->mLineDashOffset;
    }
    return v;
}
void Context2DImplGCanvas::SetLineDashOffset(float v)
{
    GCanvasState* state = mGCanvasContext->GetCurrentState();
    if (state)
    {
        state->mLineDashOffset = v;
    }
}
 std::string Context2DImplGCanvas::GetLineJoin()
{
    GLineJoin join =  mGCanvasContext->LineJoin();
    if( join == LINE_JOIN_BEVEL) return "bevel";
    else if( join == LINE_JOIN_ROUND ) return "round";
    else return "miter";
}
void Context2DImplGCanvas::SetLineJoin( std::string v)
{
    mGCanvasContext->SetLineJoin(  v.c_str() );
}
float Context2DImplGCanvas::GetLineWidth()
{
    return mGCanvasContext->LineWidth();
}
void Context2DImplGCanvas::SetLineWidth(float v)
{
    mGCanvasContext->SetLineWidth(v);
}
float Context2DImplGCanvas::GetMiterLimit()
{
    return mGCanvasContext->MiterLimit();
}
void Context2DImplGCanvas::SetMiterLimit(float v)
{
    mGCanvasContext->SetMiterLimit(v);
}
float Context2DImplGCanvas::GetShadowBlur()
{
    float v = 0.0;
    GCanvasState* state = mGCanvasContext->GetCurrentState();
    if (state)
    {
        v =  state->mShadowBlur;
    }
    return v;
}
void Context2DImplGCanvas::SetShadowBlur(float v)
{
    mGCanvasContext->SetShadowBlur(v);
}
std::string Context2DImplGCanvas::GetShadowColor()
{
    std::string color = nullptr;
    GCanvasState *state = mGCanvasContext->GetCurrentState();
    if (state)
    {
        color = gcanvas::ColorToString(state->mShadowColor);
    }
    return color;
}
void Context2DImplGCanvas::SetShadowColor(std::string v)
{
    GCanvasState *state = mGCanvasContext->GetCurrentState();
    if (state)
    {
        state->mShadowColor = gcanvas::StrValueToColorRGBA(v.c_str());
    }
}
float Context2DImplGCanvas::GetShadowOffsetX()
{
    float v = 0.0;
    GCanvasState* state = mGCanvasContext->GetCurrentState();
    if (state)
    {
        v =  state->mShadowOffsetX;
    }
    return v;
}
void Context2DImplGCanvas::SetShadowOffsetX(float v)
{
    GCanvasState* state = mGCanvasContext->GetCurrentState();
    if( state )
    {
        state->mShadowOffsetX = v;
    }

}
float Context2DImplGCanvas::GetShadowOffsetY()
{
    float v = 0.0;
    GCanvasState* state = mGCanvasContext->GetCurrentState();
    if (state)
    {
        v =  state->mShadowOffsetY;
    }
    return v;
}
void Context2DImplGCanvas::SetShadowOffsetY(float v)
{
    GCanvasState* state = mGCanvasContext->GetCurrentState();
    if( state )
    {
        state->mShadowOffsetY = v;
    }
}

std::string Context2DImplGCanvas::GetFont()
{
    std::string font = nullptr;
    GCanvasState* state = mGCanvasContext->GetCurrentState();
    if (state)
    {
        font = state->mFont->GetOriginFontName();
    }
    return font;
}
void Context2DImplGCanvas::SetFont(const std::string &v)
{
    if (!v.empty())
    {
        mGCanvasContext->SetFont(v.c_str());
    }
}

std::string Context2DImplGCanvas::GetTextAlign()
{
    GTextAlign align = mGCanvasContext->TextAlign();
    switch (align) 
    {
        case TEXT_ALIGN_START: return "start";
        case TEXT_ALIGN_END: return "end";
        case TEXT_ALIGN_LEFT: return "left";
        case TEXT_ALIGN_CENTER: return "center";
        case TEXT_ALIGN_RIGHT: return "right";
        default: return "start";
    }    
}

void Context2DImplGCanvas::SetTextAlign(std::string v)
{
    GTextAlign align = mGCanvasContext->TextAlign();
    if (v == "start") align = TEXT_ALIGN_START;
    else if (v == "end") align = TEXT_ALIGN_END;
    else if (v == "left") align = TEXT_ALIGN_LEFT;
    else if (v == "center") align = TEXT_ALIGN_CENTER;
    else if (  v == "right" ) align = TEXT_ALIGN_RIGHT;
    mGCanvasContext->SetTextAlign(align);
}
std::string Context2DImplGCanvas::GetTextBaseline()
{
    GTextBaseline baseline = mGCanvasContext->TextBaseline();
    switch (baseline) {
        case TEXT_BASELINE_ALPHABETIC: return "alphabetic";
        case TEXT_BASELINE_MIDDLE: return "middle";
        case TEXT_BASELINE_TOP: return "top";
        case TEXT_BASELINE_HANGING: return "hanging";
        case TEXT_BASELINE_BOTTOM: return "bottom";
        case TEXT_BASELINE_IDEOGRAPHIC: return "ideographic";
        default: return "alphabetic";
    }
    
}
void Context2DImplGCanvas::SetTextBaseline(std::string v)
{
    GTextBaseline baseline = mGCanvasContext->TextBaseline();
    if (v == "alphabetic") baseline = TEXT_BASELINE_ALPHABETIC;
    else if (v == "middle") baseline = TEXT_BASELINE_MIDDLE;
    else if (v == "top") baseline = TEXT_BASELINE_TOP;
    else if (v == "hanging") baseline = TEXT_BASELINE_HANGING;
    else if (v == "bottom") baseline = TEXT_BASELINE_BOTTOM;
    else if (v == "ideographic") baseline = TEXT_BASELINE_IDEOGRAPHIC;
    mGCanvasContext->SetTextBaseline(baseline);
}

void* Context2DImplGCanvas::GetCanvas()
{

}

} // namespace NodeBinding
