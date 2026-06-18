cmake_minimum_required(VERSION 3.10)
project(Jubjub VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/defines.cpp
    src/logger.cpp
    src/playerListInitializer.cpp
    src/server.cpp
)

set(HEADERS
    include/defines.h
    include/json.hpp
    include/kissnet.hpp
    include/logger.h
    include/NetIF.hpp
    include/playerListInitializer.h
    include/server.h
)

add_library(Jubjub SHARED ${SOURCES})

set_target_properties(Jubjub PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
)

target_include_directories(Jubjub
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

include(GNUInstallDirs)

install(TARGETS Jubjub
    LIBRARY DESTINATION lib
)

install(FILES 
    include/server.h
    include/kissnet.hpp
    include/playerListInitializer.h
    include/defines.h
    include/json.hpp
    DESTINATION include/jubjub
)

# Configure pkg-config file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/jubjub.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/jubjub.pc
    @ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/jubjub.pc
    DESTINATION lib/pkgconfig
)

