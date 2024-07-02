# the minimum version of CMake.
cmake_minimum_required(VERSION 3.4.1)
project(aidexx_sdk_hmos)

set(NATIVERENDER_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED PACKAGE_FIND_FILE)
    include(${PACKAGE_FIND_FILE})
endif()

include_directories(${NATIVERENDER_ROOT_PATH}
                    ${NATIVERENDER_ROOT_PATH}/include)

add_library(blecommon SHARED
            api/object_wrap.cpp
            napi_init.cpp
            api/napi_ble_adapter.cpp
            controller/bgm/bgmcontroller.cpp
            controller/cgm/cgmcontroller.cpp
            controller/cgm/aidexxcontroller.cpp
            controller/pump/pumpcontroller.cpp
            controller/blecontroller.cpp
            devcomm/devcommclass.cpp
            parser/cgm/cgmbroadcastparser.cpp
            parser/cgm/cgmdefaultparamparser.cpp
            parser/cgm/cgmdevicecheckparser.cpp
            parser/cgm/cgmdeviceparser.cpp
            parser/cgm/cgmhistoriesparser.cpp
            parser/cgm/cgmhistoryparser.cpp
            parser/pump/pumphistoryparser.cpp
            parser/pump/pumpbroadcastparser.cpp
            parser/pump/pumpbolusprofileparser.cpp
            parser/pump/pumpbasalprofileparser.cpp
            util/memutils.cpp
            util/byteutils.cpp
            util/ctimer.cpp
            util/inputbytestream.cpp
            ble.cpp
            util/md5.cpp
            util/encryptor/aesencryptor.cpp
            devcomm/CLibrary/lib_aes.c
            devcomm/CLibrary/driver/drv.c
            devcomm/CLibrary/driver/drv_uart.c
            devcomm/CLibrary/aes.c
            devcomm/CLibrary/devcomm.c
            devcomm/CLibrary/lib_checksum.c
            devcomm/CLibrary/lib_frame.c
            devcomm/CLibrary/lib_queue.c
            devcomm/CLibrary/task_comm.c
            parser/pump/pumpdeviceparser.cpp
            parser/streamparser.cpp
            parser/numberparser.cpp
            parser/cgm/aidexxbroadcastparser.cpp
            parser/cgm/aidexxcalibrationsparser.cpp
            parser/cgm/aidexxdatetimeparser.cpp
            parser/cgm/aidexxdefaultparamsparser.cpp
            parser/cgm/aidexxscanresponseparser.cpp
            parser/cgm/aidexxhistoriesparser.cpp
            parser/bgm/bgmdeviceparser.cpp
            parser/cgm/aidexxinstanthistoryparser.cpp
            parser/cgm/aidexxdeviceparser.cpp
            parser/bgm/bgmhistoryparser.cpp
            parser/bgm/bgmstateparser.cpp
            parser/pump/pumpbasalprofileparser.cpp
            parser/pump/pumpbolusprofileparser.cpp)
target_link_libraries(blecommon PUBLIC libace_napi.z.so)