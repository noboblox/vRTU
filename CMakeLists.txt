cmake_minimum_required( VERSION 3.11 )

project( vrtu )
    
set( CMAKE_CXX_STANDARD 11 )

enable_testing()

add_library( vrtu STATIC
             util/watchdog.cpp
             telecontrol/iec104server.cpp
             telecontrol/basepropertylist.cpp
             telecontrol/iec104properties.cpp
             telecontrol/iec104data.cpp
             telecontrol/staticenums.cpp
             telecontrol/iec104quality.cpp
             telecontrol/iec104timestamp.cpp
           )

target_include_directories( vrtu PUBLIC
                            datamodel
                            telecontrol
                            util
                          )

target_include_directories( vrtu PRIVATE
                            external/lib60870/lib60870-C/src/inc/api
                            external/lib60870/lib60870-C/src/hal/inc
                          )
                          
target_link_libraries( vrtu
                       PRIVATE Boost::boost
                       PRIVATE Boost::date_time
                       PRIVATE lib60870
                     )

if(!WIN32)
  target_link_libraries( vrtu PRIVATE pthread)
endif()

add_subdirectory( external )
add_subdirectory( tests )
