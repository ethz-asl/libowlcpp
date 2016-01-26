remake_find_package(
  Boost REQUIRED
  COMPONENTS
    system
    filesystem
    program_options
    unit_test_framework
  RESULT_VAR Boost_FOUND
)

remake_find_package(
  raptor2 CONFIG
)

remake_find_package(
  libfact++ CONFIG
)

remake_include(
  ${CMAKE_SOURCE_DIR}/src/lib
  ${CMAKE_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
  ${RAPTOR2_INCLUDE_DIRS}
  ${LIBFACT++_INCLUDE_DIRS}
)

add_definitions(
  -DOWLCPP_BUILD=${REMAKE_PROJECT_REVISION}
  -DOWLCPP_NAME=\"${REMAKE_PROJECT_NAME}\"
  -DOWLCPP_DESCRIPTION=\"${REMAKE_PROJECT_SUMMARY}\"
  -DOWLCPP_VERSION_1=${REMAKE_PROJECT_MAJOR}
  -DOWLCPP_VERSION_2=${REMAKE_PROJECT_MINOR}
  -DOWLCPP_VERSION_3=${REMAKE_PROJECT_PATCH}
  -DOWLCPP_VERSION_EXTRA=${REMAKE_PROJECT_REVISION}
)

remake_add_directories(
  lib
)
remake_add_directories(
  bin
  COMPONENT utils
)

remake_add_headers(RECURSE FROM ${CMAKE_SOURCE_DIR}/include/owlcpp)
remake_pkg_config_generate(fact++ raptor2)
