

INCLUDE_DIRECTORIES(/usr/local/include)

find_package( OpenCV REQUIRED )

add_executable(homesec_witty main.cpp session.cpp User.cpp)

TARGET_LINK_LIBRARIES(homesec_witty
    ${WT_PROJECT}
    ${WT_CONNECTOR}
    ${SSL_LIBRARIES}
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


