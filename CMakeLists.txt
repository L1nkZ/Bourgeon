cmake_minimum_required(VERSION 3.4)

project(Bourgeon
  LANGUAGES CXX
  VERSION 0.1.0
)

# Prevent 64-bit build
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR
    "This project cannot be built for the x64 platform, Ragnarok "
    "only provides a 32-bit client."
  )
endif()

# Remove Debug configuration since it would generate binaries with a different ABI
# compared to the game client
set(CMAKE_CONFIGURATION_TYPES Release RelWithDebInfo CACHE STRING INTERNAL FORCE)

add_subdirectory("src")
add_subdirectory("thirdparty")