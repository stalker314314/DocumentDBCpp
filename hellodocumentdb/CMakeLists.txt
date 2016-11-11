include_directories(${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
include_directories(${DOCUMENTDBCPP_INCLUDE_DIRS})

# THE ORDER OF FILES IS VERY /VERY/ IMPORTANT
if(UNIX)
  set(SOURCES
     hellodocumentdb.cpp
    )
endif()

add_executable(${DOCUMENTDBCPP_HELLODOCUMENTDB} ${SOURCES})

target_link_libraries(${DOCUMENTDBCPP_HELLODOCUMENTDB} ${DOCUMENTDBCPP_LIBRARIES})
