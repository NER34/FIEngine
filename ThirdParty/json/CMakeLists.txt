cmake_minimum_required(VERSION 3.16...3.28 FATAL_ERROR)

project(json LANGUAGES CXX)

add_library(json INTERFACE)

target_include_directories(json
	INTERFACE
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
)
