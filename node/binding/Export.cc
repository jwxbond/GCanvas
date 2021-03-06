/**
 * Created by G-Canvas Open Source Team.
 * Copyright (c) 2017, Alibaba, Inc. All rights reserved.
 *
 * This source code is licensed under the Apache Licence 2.0.
 * For the full copyright and license information, please view
 * the LICENSE file in the root directory of this source tree.
 */
#include <napi.h>
#include <iostream>


#include "CairoCanvas.h"
#include "CairoCanvasGradient.h"
#include "CairoCanvasPattern.h"
#include "CairoCanvasRenderingContext2d.h"
#include "CairoImage.h"
#include "CairoImageData.h"

#ifdef __APPLE__

#else
#include "Canvas.h"
#include "Image.h"
#include "TextMetrics.h"
#include "./webgl/WebGLShader.h"
#include "./webgl/WebGLBuffer.h"
#include "./webgl/WebGLProgram.h"
#include "./webgl/WebGLTexture.h"
#include "./webgl/WebGLFrameBuffer.h"
#include "./webgl/WebGLActiveInfo.h"
#include "./webgl/WebGLUniformLocation.h"
#include "./webgl/WebGLRenderBuffer.h"

#endif

static bool useCairo = true;  //TODO check use cairo 



Napi::Object createCanvas(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  if (info.Length() < 2)
  {
    Napi::TypeError::New(env, "canvas need width & height")
        .ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }
  
  if( info.Length() >=3 && info[2].IsBoolean() )
  {
    useCairo = info[2].As<Napi::Boolean>().ToBoolean();
  }

  #ifdef __APPLE__
    printf("createCanvas use cairo in MacOS\n");
    return cairocanvas::Canvas::NewInstance(env, info[0], info[1]);
  #else
    if( useCairo ){
      printf("createCanvas  use cairo \n");
      return cairocanvas::Canvas::NewInstance(env, info[0], info[1]);
    } else {
    printf("createCanvas  use gcanvas \n");
      return NodeBinding::Canvas::NewInstance(env, info[0], info[1]);
    }
  #endif 
}

Napi::Object createImage(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  #ifdef __APPLE__
    return cairocanvas::Image::NewInstance(env);
  #else
    //TODO check useCairo in Unix
    if( useCairo ) {
      return cairocanvas::Image::NewInstance(env);
    }else{
      return NodeBinding::Image::NewInstance(env);
    }
  #endif
}

void registerParseFont(const Napi::CallbackInfo &info )
{
  std::cout << "createCanvas registerParseFont" << std::endl;


  Napi::Env env = info.Env();
  if ( info.Length() == 1 && info[0].IsFunction() )
  {
    cairocanvas::Context2d::registerParseFont(info[0].As<Napi::Function>());
  }
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
  //export  all binding class

  //2d Cairo 
  cairocanvas::Canvas::Init(env, exports);
  cairocanvas::Context2d::Init(env, exports);
  cairocanvas::Pattern::Init(env);
  cairocanvas::Gradient::Init(env);
  cairocanvas::Image::Init(env, exports);
  cairocanvas::ImageData::Init(env);

#ifndef __APPLE__
  //2d gcanvas
  NodeBinding::Canvas::Init(env, exports);
  NodeBinding::Image::Init(env, exports);
  NodeBinding::Context2D::Init(env);
  NodeBinding::ContextWebGL::Init(env);
  NodeBinding::Gradient::Init(env);
  NodeBinding::ImageData::Init(env);
  NodeBinding::TextMetrics::Init(env);
  NodeBinding::Pattern::Init(env);

  //webgl
  NodeBinding::WebGLShader::Init(env);
  NodeBinding::WebGLProgram::Init(env);
  NodeBinding::WebGLBuffer ::Init(env);
  NodeBinding::WebGLTexture::Init(env);
  NodeBinding::WebGLFrameBuffer::Init(env);
  NodeBinding::WebGLRenderBuffer::Init(env);
  NodeBinding::WebGLActiveInfo::Init(env);
  NodeBinding::WebGLUniformLocation::Init(env); 
#endif

  //export functions
  exports.Set(Napi::String::New(env, "registerParseFont"), Napi::Function::New(env, registerParseFont));
  exports.Set(Napi::String::New(env, "createCanvas"), Napi::Function::New(env, createCanvas));
  exports.Set(Napi::String::New(env, "createImage"), Napi::Function::New(env, createImage));
  return exports;
}

NODE_API_MODULE(canvas, Init)
