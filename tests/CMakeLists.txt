add_executable( scl_tests
                source/test_scl_templates.cpp
                source/test_basic_values.cpp   )
add_test( test_scl scl_tests ) 

target_link_libraries( scl_tests PRIVATE scl )
target_link_libraries( scl_tests PRIVATE Boost::boost )
target_link_libraries( scl_tests PRIVATE Boost::system )
target_link_libraries( scl_tests PRIVATE Boost::unit_test_framework )