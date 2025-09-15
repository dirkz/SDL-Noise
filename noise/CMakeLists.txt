add_library(noise OBJECT
	Noise.h
	Noise.cpp
 )

 if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET noise PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(noise PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../DirectXMath/Inc)

target_precompile_headers(noise PUBLIC stdafx.h)
