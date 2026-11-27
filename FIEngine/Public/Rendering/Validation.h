#pragma once

#include "Utilities/Logger.h"

#if DEBUG_ENABLED
#define CHECK_OGL_ERR(Call) \
    { \
        Call; \
        GLenum error = glGetError(); \
        switch (error) \
        { \
        case GL_NO_ERROR:  \
            /*LOG_INFO("{}: GL_NO_ERROR", #Call);*/  \
            break; \
        case GL_INVALID_ENUM:  \
            LOG_ERROR("{}: GL_INVALID_ENUM", #Call);  \
            break; \
        case GL_INVALID_VALUE:  \
            LOG_ERROR("{}: GL_INVALID_VALUE", #Call);  \
            break; \
        case GL_INVALID_OPERATION:  \
            LOG_ERROR("{}: GL_INVALID_OPERATION", #Call);  \
            break; \
        case GL_INVALID_FRAMEBUFFER_OPERATION:  \
            LOG_ERROR("{}: GL_INVALID_FRAMEBUFFER_OPERATION", #Call);  \
            break; \
        default:  \
            LOG_ERROR("{}: UNKNOWN_ERROR", #Call);  \
            break; \
        } \
        assert(error == GL_NO_ERROR); \
    }
#else
#define CHECK_OGL_ERR(Call) Call;
#endif

