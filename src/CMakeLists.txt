

INCLUDE_DIRECTORIES(/usr/local/include)

find_package( OpenCV REQUIRED )

add_executable(homesec_witty main.cpp session.cpp User.cpp)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
    find_package(Boost COMPONENTS system filesystem regex signals thread date_time REQUIRED)
    INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} ) 
endif()

TARGET_LINK_LIBRARIES(homesec_witty
    ${WT_PROJECT}
    ${WT_CONNECTOR}
    ${SSL_LIBRARIES}
    ${Boost_LIBRARIES}
    ${BOOST_WT_LIBRARIES}
    ${BOOST_WTHTTP_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${ASIO_LIBRARIES}
    ${OpenCV_LIBS}
    wt
    wtdbo
    wtdbosqlite3
    boost_system
    )


install(TARGETS homesec_witty RUNTIME DESTINATION bin)


