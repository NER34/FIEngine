#pragma once

#include "Utilities/Logger.h"

#if DEBUG_ENABLED
#define CHECK_OGL_ERR(...) \
    { \
        GLenum error = glGetError(); \
        switch (error) \
        { \
        case GL_NO_ERROR:  \
            LOG_INFO("{}: GL_NO_ERROR", __VA_ARGS__);  \
            break; \
        case GL_INVALID_ENUM:  \
            LOG_ERROR("{}: GL_INVALID_ENUM", __VA_ARGS__);  \
            break; \
        case GL_INVALID_VALUE:  \
            LOG_ERROR("{}: GL_INVALID_VALUE", __VA_ARGS__);  \
            break; \
        case GL_INVALID_OPERATION:  \
            LOG_ERROR("{}: GL_INVALID_OPERATION", __VA_ARGS__);  \
            break; \
        case GL_INVALID_FRAMEBUFFER_OPERATION:  \
            LOG_ERROR("{}: GL_INVALID_FRAMEBUFFER_OPERATION", __VA_ARGS__);  \
            break; \
        default:  \
            LOG_ERROR("{}: UNKNOWN_ERROR", __VA_ARGS__);  \
            break; \
        } \
        assert(error == GL_NO_ERROR); \
    }
#else
#define CHECK_OGL_ERR(...) {}
#endif

