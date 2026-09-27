add_library(glad STATIC
                 "${glad_SOURCE_DIR}/include/glad/glad.h"
                 "${glad_SOURCE_DIR}/include/KHR/khrplatform.h"
				 "glad.c"
)

target_include_directories(glad
	PUBLIC
		${glad_SOURCE_DIR}/include
)