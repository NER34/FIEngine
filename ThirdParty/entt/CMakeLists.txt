cmake_minimum_required(VERSION 3.16...3.28 FATAL_ERROR)

project(entt LANGUAGES CXX)

add_library(entt INTERFACE)

target_include_directories(entt
	INTERFACE
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
)
