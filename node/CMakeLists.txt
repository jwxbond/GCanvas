cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0042 NEW)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -fno-unwind-tables -fexceptions")

if(CORE_DIR)
message("set core_dir")
set(CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${CORE_DIR}")
else()
message("not core_dir default local build")
set(CORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../core/")
endif()
message(STATUS ${CORE_DIR})
project(canvas)
 
 # CMake JS
include_directories(${CMAKE_JS_INC})


MESSAGE(STATUS "operation system is ${CMAKE_SYSTEM}")

 # CMake JS
include_directories(${CMAKE_JS_INC})

IF (APPLE)
        MESSAGE(STSTUS "############# APPLE ######################")
        # include_directories("/usr/local/include")
        include_directories("/usr/local/include/cairo")
        include_directories("/usr/local/include/pango-1.0")
        include_directories("/usr/local/include/glib-2.0")
        include_directories("/usr/local/include/harfbuzz")
        # include_directories("/usr/local/opt/jpeg/include")
        include_directories("/usr/local/opt/jpeg-turbo/include")

        include_directories("/usr/local/lib/glib-2.0/include")  #fix glibconfig.h not found
        include_directories("/usr/local/opt/curl-openssl/include")

        link_directories("/usr/local/opt/jpeg-turbo/lib")

        # link_directories("/usr/local/lib")
        link_directories("/usr/local/opt/glib/lib")
        link_directories("/usr/local/opt/cairo/lib")
        link_directories("/usr/local/opt/pango/lib")
        link_directories("/usr/local/opt/libffi/lib")
        link_directories("/usr/local/opt/curl-openssl/lib")

        find_package(PNG REQUIRED)
        if(PNG_FOUND)
                message(STATUS "PNG library status:" )
                message(STATUS "                    include path:${PNG_INCLUDE_DIR}")
                message(STATUS "                    library path:${PNG_LIBRARIES}")	
                include_directories(${PNG_INCLUDE_DIR})
        endif()

ELSEIF (UNIX) 
        MESSAGE(STSTUS "############# UNIX ######################")

        include_directories("/usr/include")
        include_directories("/usr/include/cairo")
        include_directories("/usr/include/pango-1.0")
        include_directories("/usr/include/glib-2.0")
        include_directories("/usr/lib/x86_64-linux-gnu/glib-2.0/include") #fix glibconfig.h not found

        link_directories("/usr/lib/x86_64-linux-gnu")
ENDIF()


include_directories("./binding")
include_directories("./binding/util")
include_directories("./binding/cairo")
include_directories("./binding/renderContext")
include_directories("./binding/renderContext/gcanvas")
include_directories("./binding/renderContext/cairo")
include_directories("./third_party")
include_directories("./third_party/font/freetype2/")
include_directories("./image/")

include_directories("${CORE_DIR}")
include_directories("${CORE_DIR}src/")
include_directories("${CORE_DIR}src/webgl/")
include_directories("${CORE_DIR}src/gcanvas")
include_directories("${CORE_DIR}src/gcanvas/GL")
include_directories("${CORE_DIR}src/support")
include_directories("${CORE_DIR}src/platform/Android")
include_directories("${CORE_DIR}src/platform/Linux")


IF(APPLE)
set(SOURCE_FILES 
        ./binding/Export.cc
        ./binding/util/NodeBindingUtil.cc
        ./third_party/lodepng.cc
        ./binding/cairo/backend/CairoImageBackend.cc
        ./binding/cairo/CairoCanvas.cc
        ./binding/cairo/CairoCanvasGradient.cc
        ./binding/cairo/CairoCanvasPattern.cc
        ./binding/cairo/CairoCanvasRenderingContext2d.cc
        ./binding/cairo/CairoImage.cc
        ./binding/cairo/CairoImageData.cc
        ./binding/cairo/color.cc
        ./binding/cairo/closure.cc
        ./binding/cairo/ImageAsyncWorker.cc
)
ELSEIF(UNIX)
set(SOURCE_FILES 
        ./binding/Export.cc
        ./binding/Canvas.cc
        ./binding/CanvasRenderingContext2D.cc
        ./binding/CanvasRenderingContextWebGL.cc
        ./binding/CanvasGradient.cc
        ./binding/ImageData.cc
        ./binding/TextMetrics.cc
        ./binding/Image.cc
        ./binding/ImageWorker.cc
        ./binding/CanvasPattern.cc
        ./binding/webgl/WebGLShader.cc
        ./binding/webgl/WebGLBuffer.cc
        ./binding/webgl/WebGLProgram.cc
        ./binding/webgl/WebGLTexture.cc
        ./binding/webgl/WebGLFrameBuffer.cc
        ./binding/webgl/WebGLRenderBuffer.cc
        ./binding/webgl/WebGLActiveInfo.cc
        ./binding/webgl/WebGLUniformLocation.cc
        ./binding/renderContext/GRenderContext.cc
        
        ./binding/util/NodeBindingUtil.cc
        ./third_party/lodepng.cc

        ./binding/cairo/backend/CairoImageBackend.cc
        ./binding/cairo/CairoCanvas.cc
        ./binding/cairo/CairoCanvasGradient.cc
        ./binding/cairo/CairoCanvasPattern.cc
        ./binding/cairo/CairoCanvasRenderingContext2d.cc
        ./binding/cairo/CairoImage.cc
        ./binding/cairo/CairoImageData.cc
        ./binding/cairo/color.cc
        ./binding/cairo/closure.cc
        ./binding/cairo/ImageAsyncWorker.cc

        ${CORE_DIR}src/GCanvas.cpp        
        ${CORE_DIR}src/GCanvasManager.cpp

        # gcanvas srcs
        ${CORE_DIR}src/gcanvas/GCanvas2dContext.cpp
        ${CORE_DIR}src/gcanvas/GCanvasState.cpp
        ${CORE_DIR}src/gcanvas/GConvert.cpp
        ${CORE_DIR}src/gcanvas/GFontManager.cpp
        ${CORE_DIR}src/gcanvas/GFontStyle.cpp
        ${CORE_DIR}src/gcanvas/GPath2D.cpp
        ${CORE_DIR}src/gcanvas/GL/GFrameBufferObject.cpp
        ${CORE_DIR}src/gcanvas/GGlyphCache.cpp
        ${CORE_DIR}src/gcanvas/GPath.cpp
        ${CORE_DIR}src/gcanvas/GL/GShader.cpp
        ${CORE_DIR}src/gcanvas/GL/GShaderManager.cpp
        ${CORE_DIR}src/gcanvas/GStrSeparator.cpp
        ${CORE_DIR}src/gcanvas/GL/GTexture.cpp
        ${CORE_DIR}src/gcanvas/GTreemap.cpp
        ${CORE_DIR}src/gcanvas/GPathStroker.cpp
 
        # # platform srcs
      
        ${CORE_DIR}src/platform/Linux/GFontManagerLinux.cpp
        ${CORE_DIR}src/platform/Linux/GFontLinux.cpp
        ${CORE_DIR}src/platform/Linux/GFontCacheLinux.cpp
        # ${CORE_DIR}src/platform/Linux/GSystemFontInformation.cpp
        # ${CORE_DIR}src/platform/Linux/GFontFamily.cpp
        #linux can reuse the android GCanvas2DContextImpl/GFrameBufferObjectImpl
        ${CORE_DIR}src/platform/Android/GCanvas2DContextImpl.cpp
        ${CORE_DIR}src/platform/Android/GFrameBufferObjectImpl.cpp
        ${CORE_DIR}src/platform/Android/GFreeTypeWrap.cpp
        # support
        ${CORE_DIR}src/support/CharacterSet.cpp
        ${CORE_DIR}src/support/Encode.cpp
        ${CORE_DIR}src/support/FileUtils.cpp
        ${CORE_DIR}/src/gcanvas/GL/GLUtil.cpp
        ${CORE_DIR}src/support/Log.cpp
        ${CORE_DIR}src/support/Util.cpp
        ${CORE_DIR}src/support/Value.cpp
)

ENDIF()




add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")


if( APPLE )
  target_link_libraries(${PROJECT_NAME} 
                        ${CMAKE_JS_LIB}
                        "-framework GLUT"
                        "-framework OpenGL"
                        # ${JPEG_LIBRARIES}
                        ${PNG_LIBRARIES}
                        jpeg
                        ffi
                        curl
                        glib-2.0
                        cairo
                        pango-1.0
                        pangocairo-1.0)

elseif(UNIX)
        target_link_libraries(${PROJECT_NAME} 
                        ${CMAKE_JS_LIB}
                        GL
                        GLESv2
                        EGL
                        atomic
                        glfw
                        freetype
                        boost_system
                        jpeg
                        curl
                        glib-2.0
                        cairo
                        pango-1.0
                        pangocairo-1.0)


                #       ${Cairo_LIBRARIES}
endif()

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

# define NPI_VERSION
add_definitions(-DNAPI_VERSION=3)
