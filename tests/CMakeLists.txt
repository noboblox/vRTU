add_executable( test_vrtu
                test_datamodel.cpp
                test_telecontrol.cpp   )
                
target_link_libraries( test_vrtu
                       PRIVATE vrtu
                       PRIVATE Boost::boost 
                       PRIVATE Boost::system 
                       PRIVATE Boost::unit_test_framework
                       PRIVATE lib60870
                    )

if(!WIN32)
  target_link_libraries( vrtu PRIVATE pthread)
endif()

target_include_directories( test_vrtu PRIVATE
                            ${vrtu_SOURCE_DIR}/external/lib60870/lib60870-C/src/inc/api
                            ${vrtu_SOURCE_DIR}/external/lib60870/lib60870-C/src/hal/inc
                          )
                  
add_test( vrtu test_vrtu ) 
