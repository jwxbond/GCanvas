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


    // 2D method
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

}
void Context2DImplGCanvas::closePath()
{
    mGCanvasContext->ClosePath();

}
ImageDataBase* Context2DImplGCanvas::createImageData()
{
    return nullptr;
}
GradientBase* Context2DImplGCanvas::createLinearGradient()
{
    return nullptr;
}
PatternBase* Context2DImplGCanvas::createPattern()
{
    return nullptr;
}
GradientBase* Context2DImplGCanvas::createRadialGradient()
{
    return nullptr;
}
void Context2DImplGCanvas::drawImage()
{
}
void Context2DImplGCanvas::fill(std::string rule)
{
    
}
void Context2DImplGCanvas::fillRect(float x, float y, float w, float h)
{
    mGCanvasContext->FillRect(x, y, w, h);

}
void Context2DImplGCanvas::fillText(const char * text, float x, float y, float maxWidth)
{

}
ImageDataBase* Context2DImplGCanvas::GetImageData()
{

}
void* Context2DImplGCanvas::GetLineDash()
{

}
void Context2DImplGCanvas::lineTo(float x, float y)
{
    mGCanvasContext->LineTo(x, y);
}
void* Context2DImplGCanvas::measureText(const char * text)
{

}
void Context2DImplGCanvas::moveTo(float x, float y)
{
    mGCanvasContext->MoveTo(x, y);
}
void Context2DImplGCanvas::putImageData(ImageDataBase *data, int x,  int y, int w, int h, int dx, int dy, int dw, int dh )
{

}
void Context2DImplGCanvas::quadraticCurveTo()
{

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

    // 2D property
std::string Context2DImplGCanvas::GetFillColor()
{
    GCanvasState *state = mGCanvasContext->GetCurrentState();
    std::string color = nullptr;
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

void* Context2DImplGCanvas::GetFillStyle()
{

}
void Context2DImplGCanvas::SetFillStyle(const void* v) 
{

}
    
void* Context2DImplGCanvas::GetStrokeStyle()
{

}
void Context2DImplGCanvas::SetStrokeStyle(const void* v)
{

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

    default:
         return "source-over";
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
    else return "butter";
}
void Context2DImplGCanvas::SetLineCap(std::string  v)
{
    mGCanvasContext->SetLineCap(  v.c_str() );
}

void* Context2DImplGCanvas::GetLineDashOffset()
{

}
void Context2DImplGCanvas::SetLineDashOffset(const void* v)
{

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
float Context2DImplGCanvas::GetlineWidth()
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

}
void Context2DImplGCanvas::SetShadowColor(std::string v)
{

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

}

std::string Context2DImplGCanvas::GetFont()
{

}
void Context2DImplGCanvas::SetFont(const void* v)
{

}

std::string Context2DImplGCanvas::GetTextAlign()
{

}
void Context2DImplGCanvas::SetTextAlign(std::string v)
{

}
std::string Context2DImplGCanvas::GetTextBaseline()
{

}
void Context2DImplGCanvas::SetTextBaseline(std::string v)
{

}

void* Context2DImplGCanvas::GetCanvas()
{

}

} // namespace NodeBinding
