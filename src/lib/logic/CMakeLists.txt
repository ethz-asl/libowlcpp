remake_add_library(
  logic
    *.cpp
    factpp/*.cpp
  LINK
    rdf
    ${Boost_LIBRARIES}
    ${LIBFACT++_LIBRARIES}
)

remake_add_directories(
  test
  COMPONENT test
)
