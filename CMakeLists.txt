cmake_minimum_required(VERSION 3.12)

project(DBLParse VERSION 3.1.0
        DESCRIPTION "An application base on DBLP data"
        HOMEPAGE_URL "https://tootal.xyz/DBLParse/"
        LANGUAGES C CXX)

add_subdirectory(bignumber)
add_subdirectory(pivoter)
add_subdirectory(src)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_SET_DESTDIR ON)
set(CPACK_INSTALL_PREFIX "${PEOJECT_SOURCE_DIR}/dist")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_RPM_PACKAGE_GROUP "tootal")
set(CPACK_PACKAGE_VENDOR "tootal")
set(CPACK_RPM_PACKAGE_LICENSE "GPL v3")
include(CPACK)
