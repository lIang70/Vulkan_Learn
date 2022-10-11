CMAKE_MINIMUM_REQUIRED(VERSION 3.8.0)

PROJECT(01_begin)

# Add the source code to the project's executable。
ADD_EXECUTABLE(01_opengl_test src/opengl_test.cc)
# Set properties: output path
SET_TARGET_PROPERTIES(01_opengl_test    PROPERTIES 
                                        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin)
TARGET_LINK_LIBRARIES(01_opengl_test PRIVATE OpenGL::OpenGL GLU glut)


# Add the source code to the project's executable。
ADD_EXECUTABLE(01_opengl_window src/opengl_window.cc src/glad.c)
# Set properties: output path
SET_TARGET_PROPERTIES(01_opengl_window  PROPERTIES 
                                        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin)
TARGET_LINK_LIBRARIES(01_opengl_window PRIVATE OpenGL::GL glfw ${CMAKE_DL_LIBS})


FILE(GLOB_RECURSE HEADER_SOURCE "src/header/*.cc")

# Add the source code to the project's executable。
ADD_EXECUTABLE(01_opengl_triangle ${HEADER_SOURCE} src/opengl_triangle.cc src/glad.c)
# Set properties: output path
SET_TARGET_PROPERTIES(01_opengl_triangle    PROPERTIES 
                                            RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin
                                            RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin
                                            RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin
                                            RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin)
TARGET_LINK_LIBRARIES(01_opengl_triangle PRIVATE OpenGL::GL glfw ${CMAKE_DL_LIBS})


# Add the source code to the project's executable。
ADD_EXECUTABLE(01_opengl_texture ${HEADER_SOURCE} src/opengl_texture.cc src/glad.c)
# Set properties: output path
SET_TARGET_PROPERTIES(01_opengl_texture PROPERTIES 
                                        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin)
TARGET_LINK_LIBRARIES(01_opengl_texture PRIVATE OpenGL::GL glfw ${CMAKE_DL_LIBS})


# Add the source code to the project's executable。
ADD_EXECUTABLE(01_opengl_coordinate_systems  ${HEADER_SOURCE} src/opengl_coordinate_systems.cc src/glad.c)
# Set properties: output path
SET_TARGET_PROPERTIES(01_opengl_coordinate_systems  PROPERTIES 
                                                    RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin
                                                    RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin
                                                    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin
                                                    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin)
TARGET_LINK_LIBRARIES(01_opengl_coordinate_systems PRIVATE OpenGL::GL glfw ${CMAKE_DL_LIBS})

# Add the source code to the project's executable。
ADD_EXECUTABLE(01_opengl_camera  ${HEADER_SOURCE} src/opengl_camera.cc src/glad.c)
# Set properties: output path
SET_TARGET_PROPERTIES(01_opengl_camera  PROPERTIES 
                                        RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin
                                        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/bin)
TARGET_LINK_LIBRARIES(01_opengl_camera PRIVATE OpenGL::GL glfw ${CMAKE_DL_LIBS})