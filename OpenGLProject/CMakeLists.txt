# CMakeList.txt : CMake project for OpenGLProject, include source and define
# project specific logic here.
#

# Yes, I know that it's not recommended. For now, I don't care. It's a problem of a future me
file(GLOB_RECURSE files CONFIGURE_DEPENDS 
		"*.cpp" 
		"*.h" 
		"*.vert"
		"*.frag"
)
add_executable(OpenGLProject ${files})

target_include_directories(OpenGLProject
		PUBLIC
			"Public"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGLProject PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
target_link_libraries(OpenGLProject
	PRIVATE
		glfw
		glad
		glm
		spdlog
		stb
		assimp
		json
		stduuid
)

add_custom_command(
	TARGET OpenGLProject POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/Resources"
		"$<TARGET_FILE_DIR:OpenGLProject>/Resources"
)

# Debug enable/disable variable
add_compile_definitions(DEBUG_ENABLED=1)
