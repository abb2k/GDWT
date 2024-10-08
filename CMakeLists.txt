cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(GDWT VERSION 1.0.0)

file(GLOB_RECURSE SOURCES 
    src/*.cpp
)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})

if(ANDROID)
    # Android specific settings
    set(OPENSSL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/openssl/android/include")
    set(OPENSSL_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/openssl/android/lib")
elseif(APPLE)
    # macOS specific settings
    set(OPENSSL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/openssl/macos/include")
    set(OPENSSL_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/openssl/macos/lib")
else()
    # Default (assuming Windows)
    set(OPENSSL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/openssl/current/include")
    set(OPENSSL_LIB_DIR "${PROJECT_SOURCE_DIR}/lib/openssl/current/lib/VC/x64/MD")
endif()

include_directories(${OPENSSL_INCLUDE_DIR})
if(ANDROID)
    target_link_libraries(${PROJECT_NAME} "${OPENSSL_LIB_DIR}/libcrypto.so" "${OPENSSL_LIB_DIR}/libssl.so")
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} "${OPENSSL_LIB_DIR}/libcrypto.dylib" "${OPENSSL_LIB_DIR}/libssl.dylib")
else()
    target_link_libraries(${PROJECT_NAME} "${OPENSSL_LIB_DIR}/libcrypto.lib" "${OPENSSL_LIB_DIR}/libssl.lib")
endif()