include_directories(${Boost_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR})
include_directories(${DOCUMENTDBCPP_INCLUDE_DIRS})

# THE ORDER OF FILES IS VERY /VERY/ IMPORTANT
if(UNIX)
  set(SOURCES
     Attachment.cpp
     DocumentClient.cpp
     hmac_bcrypt_unix.cpp
     StoredProcedureIterator.cpp
     UserDefinedFunctionIterator.cpp
     AttachmentIterator.cpp
     Document.cpp
     Index.cpp
     Trigger.cpp
     Utils.cpp
     Collection.cpp
     DocumentDBConfiguration.cpp
     IndexingPolicy.cpp
     IndexPath.cpp
     TriggerIterator.cpp
     ConnectionHelper.cpp
     DocumentDBEntity.cpp
     Permission.cpp
     User.cpp
     Database.cpp
     DocumentIterator.cpp
     StoredProcedure.cpp
     UserDefinedFunction.cpp
    )
endif()

if ("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()
if (APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNINGS}")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

add_library(${DOCUMENTDBCPP_LIBRARY} ${SOURCES})

target_link_libraries(${DOCUMENTDBCPP_LIBRARIES})

# Portions specific to DocumentDbCpp binary versioning and installation.
if(UNIX)
  set_target_properties(${DOCUMENTDBCPP_LIBRARY} PROPERTIES
    SOVERSION ${DOCUMENTDBCPP_VERSION_MAJOR}
    VERSION ${DOCUMENTDBCPP_VERSION_MAJOR}.${DOCUMENTDBCPP_VERSION_MINOR})

  install(
    TARGETS ${DOCUMENTDBCPP_LIBRARY}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    )
endif()
