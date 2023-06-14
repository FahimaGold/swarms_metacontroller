# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})

# Headers
set(AUTOMODE_HEADERS
	AutoMoDe/src/core/AutoMoDeController.h
	AutoMoDe/src/core/AutoMoDeFiniteStateMachine.h
	AutoMoDe/src/core/AutoMoDeFsmBuilder.h
	AutoMoDe/src/core/AutoMoDeFsmHistory.h
	# Behaviours
	AutoMoDe/src/modules/AutoMoDeBehaviour.h
	AutoMoDe/src/modules/AutoMoDeBehaviourAntiPhototaxis.h
	AutoMoDe/src/modules/AutoMoDeBehaviourPhototaxis.h
	AutoMoDe/src/modules/AutoMoDeBehaviourAttraction.h
	AutoMoDe/src/modules/AutoMoDeBehaviourRepulsion.h
	AutoMoDe/src/modules/AutoMoDeBehaviourStop.h
	AutoMoDe/src/modules/AutoMoDeBehaviourExploration.h
	# Conditions
	AutoMoDe/src/modules/AutoMoDeCondition.h
	AutoMoDe/src/modules/AutoMoDeConditionBlackFloor.h
	AutoMoDe/src/modules/AutoMoDeConditionWhiteFloor.h
	AutoMoDe/src/modules/AutoMoDeConditionGrayFloor.h
	AutoMoDe/src/modules/AutoMoDeConditionNeighborsCount.h
	AutoMoDe/src/modules/AutoMoDeConditionInvertedNeighborsCount.h
	AutoMoDe/src/modules/AutoMoDeConditionFixedProbability.h
	# SwarmsMetactontroller
	core/SwarmsMetacontroller.h
	)

# Sources
set(AUTOMODE_SOURCES
AutoMoDe/src/core/AutoMoDeController.cpp
AutoMoDe/src/core/AutoMoDeFiniteStateMachine.cpp
AutoMoDe/src/core/AutoMoDeFsmBuilder.cpp
AutoMoDe/src/core/AutoMoDeFsmHistory.cpp
	# Behaviours
	AutoMoDe/src/modules/AutoMoDeBehaviour.cpp
	AutoMoDe/src/modules/AutoMoDeBehaviourAntiPhototaxis.cpp
	AutoMoDe/src/modules/AutoMoDeBehaviourPhototaxis.cpp
	AutoMoDe/src/modules/AutoMoDeBehaviourAttraction.cpp
	AutoMoDe/src/modules/AutoMoDeBehaviourRepulsion.cpp
	AutoMoDe/src/modules/AutoMoDeBehaviourStop.cpp
	AutoMoDe/src/modules/AutoMoDeBehaviourExploration.cpp
	# Conditions
	AutoMoDe/src/modules/AutoMoDeCondition.cpp
	AutoMoDe/src/modules/AutoMoDeConditionBlackFloor.cpp
	AutoMoDe/src/modules/AutoMoDeConditionWhiteFloor.cpp
	AutoMoDe/src/modules/AutoMoDeConditionGrayFloor.cpp
	AutoMoDe/src/modules/AutoMoDeConditionNeighborsCount.cpp
	AutoMoDe/src/modules/AutoMoDeConditionInvertedNeighborsCount.cpp
	AutoMoDe/src/modules/AutoMoDeConditionFixedProbability.cpp
	# SwarmsMetactontroller
	core/SwarmsMetacontroller.cpp)


add_library(metacontroller SHARED ${AUTOMODE_HEADERS} ${AUTOMODE_SOURCES})
target_link_libraries(metacontroller argos3plugin_${ARGOS_BUILD_FOR}_epuck)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(metacontroller_main SwarmsOpMain.cpp)
target_link_libraries(metacontroller_main metacontroller argos3core_${ARGOS_BUILD_FOR} argos3plugin_${ARGOS_BUILD_FOR}_epuck argos3_demiurge_loop_functions argos3_demiurge_epuck_dao)
