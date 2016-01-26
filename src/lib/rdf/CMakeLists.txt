remake_add_library(
  rdf
    *.cpp
    ../lib_info.cpp
  LINK
    ${Boost_LIBRARIES}
)

remake_add_directories(
  test
  COMPONENT test
)
