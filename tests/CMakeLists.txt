add_executable( test_vrtu
                test_datamodel.cpp
                test_telecontrol.cpp   )
                
target_link_libraries( test_vrtu
                       PRIVATE vrtu
                       PRIVATE Boost::boost 
                       PRIVATE Boost::system 
                       PRIVATE Boost::unit_test_framework
                    )
                    
add_test( vrtu test_vrtu ) 
