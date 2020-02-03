cmake_minimum_required(VERSION 3.4)

project(snoretoast VERSION 0.7.0)

set(SNORETOAST_CALLBACK_GUID E32293C0-F85D-4EB9-8BC2-BEB9F607E5A4)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

option(BUILD_STATIC_RUNTIME "Whether link statically to the msvc runtime" ON)

include(GenerateExportHeader)

include(cmakerc/CMakeRC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

if (BUILD_STATIC_RUNTIME)
	#link runtime static
	if(MSVC)
		foreach(_bt DEBUG RELEASE RELWITHDEBINFO)
			string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_${_bt} ${CMAKE_CXX_FLAGS_${_bt}})
		endforeach(_bt DEBUG RELEASE RELWITHDEBINFO)
	endif(MSVC)
endif()

add_subdirectory(data)
add_subdirectory(src)

