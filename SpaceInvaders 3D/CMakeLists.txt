project(sdl2template)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
CONAN_BASIC_SETUP()

ADD_EXECUTABLE(openGLW2 src/main.cpp src/Sprite.cpp src/Sprite.h src/MainWorld.cpp src/MainWorld.h src/Mesh.h src/Mesh.cpp src/PersistantData.h src/PersistantData.cpp src/ShaderProg.h src/ShaderProg.cpp)
TARGET_LINK_LIBRARIES(openGLW2 ${CONAN_LIBS})

set (CMAKE_CXX_STANDARD 11)
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "openGLW2")

FILE(COPY assets/ DESTINATION bin/assets)
