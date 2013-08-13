
find_package(Boost 1.54 REQUIRED system regex)

include_directories(../include ${Boost_INCLUDE_DIR})

link_directories(${Boost_LIBRARY_DIR})

add_executable(avhttpd main.cpp)

target_link_libraries(avhttpd ${Boost_LIBRARIES})

install(TARGETS avhttpd RUNTIME DESTINATION bin)
