# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})
message("ARGOS_INCLUDE_DIRS: ${ARGOS_INCLUDE_DIRS}")

set(METACONTROLLER_HEADERS
	core/Observer.h
	core/LightObserver.h
	core/SwarmsMetacontroller.h)

	set(METACONTROLLER_SOURCES
	core/LightObserver.cpp
	core/SwarmsMetacontroller.cpp)

add_library(metacontroller SHARED ${METACONTROLLER_HEADERS} ${METACONTROLLER_SOURCES})

target_link_libraries(metacontroller argos3plugin_${ARGOS_BUILD_FOR}_epuck)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(metacontroller_main SwarmsOpMain.cpp)
target_link_libraries(metacontroller_main metacontroller /Users/mac/automode-thesis/swarms_metacontroller/src/AutoMoDe/build/src/libautomode.dylib
  argos3core_${ARGOS_BUILD_FOR} argos3plugin_${ARGOS_BUILD_FOR}_epuck argos3_demiurge_loop_functions argos3_demiurge_epuck_dao)



