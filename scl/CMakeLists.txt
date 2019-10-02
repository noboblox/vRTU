add_library( scl STATIC 
             source/dataattribute.cpp
             source/enumtype.cpp
             source/dataobject.cpp
	     source/iecdataelement.cpp
             source/objectreference.cpp
	     source/datatemplatecontainer.cpp
             source/basicvalue.cpp
             source/basicvaluefactory.cpp)

target_include_directories( scl
                            PUBLIC
                            ${CMAKE_CURRENT_SOURCE_DIR}/include )

target_compile_features( scl 
                         PUBLIC
                         cxx_std_14 )