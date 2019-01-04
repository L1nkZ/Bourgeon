cmake_minimum_required(VERSION 3.0)

project(Bourgeon
  LANGUAGES CXX
  VERSION 0.1
)

# Remove Debug configuration since it would generate binaries with a different ABI
# compared to the game client
set(CMAKE_CONFIGURATION_TYPES Release RelWithDebInfo CACHE TYPE INTERNAL FORCE)

add_definitions(-DBOURGEON_VERSION="${PROJECT_VERSION}")
add_definitions(-DWITH_THREAD)

add_subdirectory(src)
add_subdirectory(thirdparty)
