CMAKE_MINIMUM_REQUIRED(VERSION 3.8)

PROJECT(Vulkan_Learn VERSION 0.1)

MESSAGE(STATUS "CMAKE_C_COMPILER_ID = ${CMAKE_C_COMPILER_ID}")
MESSAGE(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")

MESSAGE(STATUS "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
MESSAGE(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")

MESSAGE(STATUS "CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM} ")

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 17)

OPTION(build_with_debuginfo "Build with DEBUG-INFO" ON)

IF(build_with_debuginfo)
    SET(CMAKE_BUILD_TYPE RelWithDebInfo)

    IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ggdb -pthread")
    ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /DEBUG /Zi /EHa")
    ENDIF()
ELSE()
    SET(CMAKE_BUILD_TYPE MinSizeRel)

    IF(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread")
    ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /EHa")
    ENDIF()
ENDIF()

# add 3rd lib
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Add OpenGL
IF (POLICY CMP0072)
    SET(OpenGL_GL_PREFERENCE LEGACY)
ENDIF(POLICY CMP0072)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLUT REQUIRED)
FIND_PACKAGE(glfw3 REQUIRED)

# Add Vulkan 
FIND_PACKAGE(Vulkan REQUIRED FATAL_ERROR)

ADD_SUBDIRECTORY(01_begin)