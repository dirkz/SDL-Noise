# CMakeList.txt : CMake project for Noise, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SDL-Noise")

set(MAIN_SOURCES main.cpp AppState.h AppState.cpp)

# Add source to this project's executable.
if (WIN32)
	add_executable (${PROJECT_NAME} WIN32 ${MAIN_SOURCES})
else()
	add_executable (${PROJECT_NAME} ${MAIN_SOURCES})
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

option(SDL_STATIC "Build SDL as a static library" ON)

add_subdirectory(SDL-Hpp EXCLUDE_FROM_ALL)
add_subdirectory(noise)

target_precompile_headers(${PROJECT_NAME} REUSE_FROM noise)

target_link_libraries(${PROJECT_NAME} SDL-Hpp noise)

