set( Boost_USE_MULTITHREADED TRUE )

find_package( Boost REQUIRED COMPONENTS system unit_test_framework )

if ( Boost_FOUND )
    set_target_properties( Boost::boost
                           Boost::system
                           Boost::unit_test_framework
                           PROPERTIES IMPORTED_GLOBAL TRUE )
message (STATUS "boost include dir is: ${Boost_INCLUDE_DIRS}")
message (STATUS "boost binary dir is: ${Boost_LIBRARY_DIRS}")
endif ()
