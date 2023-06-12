cmake_minimum_required(VERSION 2.8)
project(SwarmsMetacontroller)



if(ARGOS_BUILD_FOR_SIMULATOR)
	include(SimLibrary.cmake)
elseif(ARGOS_BUILD_FOR_EPUCK)
	include(RealLibrary.cmake)
endif(ARGOS_BUILD_FOR_SIMULATOR)

