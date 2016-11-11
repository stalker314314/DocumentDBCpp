include_directories(${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
include_directories(${DOCUMENTDBCPP_INCLUDE_DIRS})

# THE ORDER OF FILES IS VERY /VERY/ IMPORTANT
if(UNIX)
  set(SOURCES
     test.cpp
    )
endif()

add_executable(${DOCUMENTDBCPP_LIBRARY_TEST} ${SOURCES})

target_link_libraries(${DOCUMENTDBCPP_LIBRARY_TEST} ${DOCUMENTDBCPP_LIBRARIES})

