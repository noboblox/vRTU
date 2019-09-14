add_executable( scl_tests source/test_scl_templates.cpp )

target_link_library( scl_tests PRIVATE scl )