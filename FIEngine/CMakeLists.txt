# CMakeList.txt : CMake project for FIEngine, include source and define
# project specific logic here.
#

# Yes, I know that it's not recommended. For now, I don't care. It's a problem of a future me
file(GLOB_RECURSE files CONFIGURE_DEPENDS 
		"*.cpp" 
		"*.h" 
		"*.vert"
		"*.frag"
)
add_executable(FIEngine ${files})

target_include_directories(FIEngine
		PUBLIC
			"Public"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FIEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(FIEngine
	PRIVATE
		glfw
		glad
		glm
		spdlog
		stb
		assimp
		json
		stduuid
		entt
		easy_profiler
)

add_custom_command(TARGET FIEngine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:easy_profiler> $<TARGET_FILE_DIR:FIEngine>)

#add_custom_command(
#	TARGET FIEngine POST_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory
#		"${CMAKE_CURRENT_SOURCE_DIR}/Resources"
#		"$<TARGET_FILE_DIR:FIEngine>/Resources"
#)

# Debug enable/disable variable
add_compile_definitions(DEBUG_ENABLED=1)
add_compile_definitions(RESOURCES_FOLDER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
add_compile_definitions(SAVED_FOLDER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/Saved")
