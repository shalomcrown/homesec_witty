# homesec_witty
Web application to allow capture and display of images remotely

Can be compiled for RaspberryPi or desktop Linux

No installation yet, so use the 'run.sh' in the package

Instructions for Raspbian:
     sudo apt-get install libwt-dev libwt-doc libopencv-dev libwtdbo-dev libwtext-dev libwthttp-dev libwtdbosqlite-dev \
        cmake gcc make autoconf automake libtool, flex, bison, gdb

     cmake  -DCMAKE_BUILD_TYPE=Debug  -G "Unix Makefiles"
     make
     ./run.sh
