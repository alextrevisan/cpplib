project(cpplib)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(LIB_SUFFIX 64)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/;${CMAKE_MODULE_PATH}")

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}")

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_SEARCH_END_STATIC 1)
FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
  SET(requiredlibs ${requiredlibs} ${CURL_LIBRARIES} )
  target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})
  add_definitions(-DCURL_STATICLIB)
  target_link_libraries(${PROJECT_NAME} ws2_32)
  target_link_libraries(${PROJECT_NAME} wldap32)
ELSE(CURL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the CURL library and development files.")
ENDIF(CURL_FOUND)

FIND_PACKAGE(OPENSSL)
IF(OPENSSL_FOUND)
  INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
  SET(requiredlibs ${requiredlibs} ${OPENSSL_LIBRARIES} )
  target_link_libraries(${PROJECT_NAME} ${OPENSSL_LIBRARIES})
ELSE(OPENSSL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find the OpenSSL library and development files.")
ENDIF(OPENSSL_FOUND)

IF( ${CMAKE_COMPILER_IS_GNUCXX} )
  SET(CMAKE_EXE_LINKER_FLAGS "-static -lpthread -static-libgcc -static-libstdc++")
ENDIF( ${CMAKE_COMPILER_IS_GNUCXX} )

SET(CMAKE_CXX_FLAGS "-std=c++1y")
