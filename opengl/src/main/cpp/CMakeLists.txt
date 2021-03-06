cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror")

# Export ANativeActivity_onCreate(),
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(native-activity SHARED
        common/esUtil_Android.c
        common/esUtil.c
        common/esShader.c
        #        lsn02/Hello_Triangle.c
        #        lsn06/E6_3.cpp
        #        lsn06/E6_3_vertex_buffer.cpp
        #        lsn09/Simple_Texture2D.c
        #        lsn09/02_MipMap2D.c
        #        lsn09/03_TextureWrap.c
        #        lsn09/04_Simple_TextureCubemap.c
        )

target_include_directories(native-activity PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue
        common)

# add lib dependencies
target_link_libraries(native-activity
        android
        native_app_glue
        EGL
        GLESv2
        GLESv1_CM
        log)
