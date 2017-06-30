# Minimum CMake version required
cmake_minimum_required(VERSION 2.8)

# Project name
project(p2pspgui C CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set ( CMAKE_BUILD_TYPE Release )

list(APPEND BOOST_REQUIRED_COMPONENTS
  system
  filesystem
  chrono
  serialization
  timer
  thread
  log
  log_setup
  date_time
  program_options
)

find_package(Boost COMPONENTS
  ${BOOST_REQUIRED_COMPONENTS}
REQUIRED)

#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

#Find P2PSP Library
FIND_LIBRARY(P2PSP_LIBRARY p2psp ../core/bin/ ../core/src/)

#Find LIBVLC Library
FIND_LIBRARY(VLC_LIBRARY vlc)

set(P2PSP_INCLUDE_DIR ../core/src/)

set(P2PSPGUI_ICONS /icons)

include_directories (
	${PROJECT_SOURCE_DIR}  
	${P2PSP_INCLUDE_DIR}
  	${Boost_INCLUDE_DIR}
	${CMAKE_SOURCE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR}
	${P2PSPGUI_ICONS}
	)

link_directories(${Boost_LIBRARY_DIR})

# Initialize CXXFLAGS
set(CMAKE_CXX_FLAGS                "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
#set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g -D__D__ -D__D_CHURN__ -D__D_LOST_CHUNKS__")
#set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os")
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os")
#set(CMAKE_CXX_FLAGS_RELEASE        "-O4")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11")
endif ()

set ( p2pspgui_SRCS
	addchannels.cpp	
	add_dialog.cpp
	helper.cpp
	main.cpp
	peer.cpp
	peerthread.cpp
	rundialog.cpp
	rundialogstorage.cpp
	SimplePlayer.cpp
	logger.cpp
	)

set ( p2pspgui_UIS
	add_dialog.ui
	#dialog.ui
	helper.ui
	rundialog.ui
	SimplePlayer.ui
	)

set ( p2pspgui_MOCS
	add_dialog.h
	#dialog.h
	helper.h
	rundialog.h
	SimplePlayer.h
	)

set ( p2pspgui_RSCS
	icons.qrc
	)

QT5_ADD_RESOURCES(RSCS ${p2pspgui_RSCS})
QT5_WRAP_UI(UIS ${p2pspgui_UIS})
QT5_WRAP_CPP(MOCS ${p2pspgui_MOCS})

add_library (MyProjectLib SHARED 
	${UIS} 
	${MOCS}
	${RSCS}
	)
target_link_libraries (MyProjectLib 
	Qt5::Widgets 
	Qt5::Network
	${VLC_LIBRARY}    
	${P2PSP_LIBRARY}
	${Boost_LIBRARIES}
	)

add_executable(p2pspgui ${p2pspgui_SRCS} ${RSCS})
target_link_libraries (p2pspgui MyProjectLib )

# Enable C++11 for CMake 3.1.0 or later
set_property(TARGET p2pspgui PROPERTY CXX_STANDARD 11)
set_property(TARGET p2pspgui PROPERTY CXX_STANDARD_REQUIRED ON)
