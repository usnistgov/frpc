set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
include_directories (${CMAKE_SOURCE_DIR}/src/include)

# Configure to put executable in top level bin directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Get library implementation name
set (FRPC_IMPL_LIB $ENV{FRPC_IMPL_LIB})
# Get challenge identifier
set (FRPC_CHALLENGE $ENV{FRPC_CHALLENGE})

if (${FRPC_CHALLENGE} STREQUAL "11")
	# Build executable link to dependent libraries
	add_executable (validate11 util.cpp validate11.cpp)
	target_link_libraries (validate11 ${FRPC_IMPL_LIB})
endif()

if (${FRPC_CHALLENGE} STREQUAL "1N")
	# Build executable link to dependent libraries
	add_executable (validate1N util.cpp validate1N.cpp)
	target_link_libraries (validate1N ${FRPC_IMPL_LIB})
endif()
