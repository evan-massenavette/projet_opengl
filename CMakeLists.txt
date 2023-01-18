
# Project info
cmake_minimum_required(VERSION 3.0.0)
project(HGL VERSION 0.1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Packaging
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Tests
include(CTest)
enable_testing()

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

###############################
# Add libs and their includes #
###############################

set(LIBS_DIR "${CMAKE_SOURCE_DIR}/lib")

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_DIR "${LIBS_DIR}/glfw")
add_subdirectory(${GLFW_DIR})
target_include_directories(HGL PRIVATE "${GLFW_DIR}/include")

# GLAD
set(GLAD_DIR "${LIBS_DIR}/glad")
add_library(GLAD "${GLAD_DIR}/src/glad.c")
target_include_directories(GLAD PRIVATE "${GLAD_DIR}/include")
target_include_directories(HGL PRIVATE "${GLAD_DIR}/include")

# GLM
set(GLM_DIR "${LIBS_DIR}/glm")
target_include_directories(HGL PRIVATE ${GLM_DIR})

# stb_image
set(STB_DIR "${LIBS_DIR}/stb")
target_include_directories(HGL PRIVATE ${STB_DIR})

# tinyobjloader
set(TINYOBJLOADER_DIR "${LIBS_DIR}/tinyobjloader")
target_include_directories(HGL PRIVATE ${TINYOBJLOADER_DIR})

# Link libraries
set(LIBS glfw GLAD)
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Copy shaders
set(SHADERS_DIR "${PROJECT_SOURCE_DIR}/src/shaders")
set(SHADERS_DEST_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders")
if(EXISTS ${SHADERS_DIR})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${SHADERS_DIR}
		${SHADERS_DEST_DIR})
endif()

# Copy textures
set(TEXTURES_DIR "${PROJECT_SOURCE_DIR}/src/textures")
set(TEXTURES_DEST_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/textures")
if(EXISTS ${SHADERS_DIR})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${TEXTURES_DIR}
		${TEXTURES_DEST_DIR})
endif()

# Copy textures
set(MODELS_DIR "${PROJECT_SOURCE_DIR}/src/models")
set(MODELS_DEST_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/models")
if(EXISTS ${SHADERS_DIR})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		${MODELS_DIR}
		${MODELS_DEST_DIR})
endif()

# Copy dlls
if(WIN32)
	set(DLLS_DIR "${PROJECT_SOURCE_DIR}/dlls")
	set(DLLS_DEST_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/dlls")
	if(EXISTS ${DLLS_DIR})
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${DLLS_DIR}
			${DLLS_DEST_DIR})
	endif()
endif()
