cmake_minimum_required(VERSION 3.10)
project("LunchJet")

#
# momo for send video stream from camera
#

set(MOMO_TARBALL_URL "https://github.com/shiguredo/momo/releases/download/2021.2.2/momo-2021.2.2_ubuntu-18.04_armv8_jetson_nano.tar.gz")
string(REGEX REPLACE "^.*/" "" MOMO_TARBALL_NAME ${MOMO_TARBALL_URL})
string(REGEX REPLACE "\.tar\.gz" "/" MOMO_DIR ${MOMO_TARBALL_NAME})
string(APPEND MOMO_CMD ${MOMO_DIR} "momo")

add_custom_command(
  OUTPUT ${MOMO_TARBALL_NAME}
  COMMAND "wget" ${MOMO_TARBALL_URL})

add_custom_command(
    OUTPUT ${MOMO_CMD}
    MAIN_DEPENDENCY ${MOMO_TARBALL_NAME}
    COMMAND "tar" "-m" "-xvf" ${MOMO_TARBALL_NAME}
    COMMAND "chmod" "u+x" ${MOMO_CMD}
)

add_custom_target(check_momo_version ALL COMMAND ${MOMO_CMD} "--version" DEPENDS ${MOMO_CMD})

#
# build lunchjet sources
#
add_subdirectory(src/i2c)
add_subdirectory(src/include)
add_subdirectory(src/pca9685)
add_subdirectory(src/rc_car_driver)
add_subdirectory(src/user_input_device)
add_subdirectory(src/rc_car_controller)
add_subdirectory(src/rc_car_server)
add_subdirectory(src/video_input_device)
add_subdirectory(src/drive_detector)

#
# scripts
#
add_subdirectory(scripts)

#
# build lunchjet tests
#
add_subdirectory(test)

#
# install artifacts
#

install(DIRECTORY DESTINATION /etc/lunchjet)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${MOMO_DIR} DESTINATION "/opt/lunchjet/momo" USE_SOURCE_PERMISSIONS)

install(FILES "lunchjet_momo.service" DESTINATION "/usr/lib/systemd/system/")
install(FILES "lunchjet_virtual_camera.service" DESTINATION "/usr/lib/systemd/system/")
install(FILES "lunchjet.conf" DESTINATION "/etc/modprobe.d/")
install(FILES "lunchjet_load.conf" DESTINATION "/etc/modules-load.d/")

install(FILES lunchjet_template.conf DESTINATION /etc/lunchjet)

install(CODE "message(\"install has finished!\")")
install(CODE "message(\"you can enable LunchJet with\")")
install(CODE "message(\"    sudo systemctl enable --now lunchjet_momo.service\")")
install(CODE "message(\"    sudo systemctl enable --now lunchjet_virtual_camera.service\")")