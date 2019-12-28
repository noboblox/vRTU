cmake_minimum_required( VERSION 3.11 )

project( vrtu )

enable_testing()

add_library( vrtu STATIC
             util/watchdog.cpp
             telecontrol/iec104server.cpp
             datamodel/basecontrol.cpp
             datamodel/baseelement.cpp
           )
    
set_property( TARGET vrtu PROPERTY CXX_STANDARD 14 )

target_include_directories( vrtu PUBLIC
                            datamodel
                            telecontrol
                            util
                            external/lib60870/lib60870-C/src/inc/api
                            external/lib60870/lib60870-C/src/hal/inc
                          )
                          
target_link_libraries( vrtu 
                       PRIVATE pthread
                       PRIVATE lib60870
                     )

add_subdirectory( external )
add_subdirectory( tests )
