add_library( scl STATIC 
             source/dataattribute.cpp
             source/enumtype.cpp)

target_include_directories( scl
                            PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_compile_features( scl 
                         PUBLIC
                         cxx_std_11 )