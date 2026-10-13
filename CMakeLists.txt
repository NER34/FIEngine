# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.30)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OpenGLProject")

# Include sub-projects.
add_subdirectory("ThirdParty/glfw")
add_subdirectory("ThirdParty/glad")
add_subdirectory("ThirdParty/glm")
add_subdirectory("ThirdParty/spdlog")
add_subdirectory("ThirdParty/stb")
add_subdirectory("ThirdParty/assimp")
add_subdirectory("ThirdParty/json")
add_subdirectory("ThirdParty/stduuid")
add_subdirectory("OpenGLProject")

set(VS_STARTUP_PROJECT OpenGLProject)

set_target_properties(OpenGLProject PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:OpenGLProject>"
)