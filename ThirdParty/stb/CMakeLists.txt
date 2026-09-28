cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(stb)

add_library(stb STATIC
	"stb_image.h"
	"stb_image.cpp"
)

target_include_directories(stb
	PUBLIC
		${stb_SOURCE_DIR}
)