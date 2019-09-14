add_executable( scl_tests source/test_scl_templates.cpp )

target_link_libraries( scl_tests PRIVATE scl )
target_link_libraries( scl_tests PRIVATE Boost::boost )
target_link_libraries( scl_tests PRIVATE Boost::system )
target_link_libraries( scl_tests PRIVATE Boost::unit_test_framework )