remake_add_library(
  io
  LINK
    rdf
    ${Boost_LIBRARIES}
    ${RAPTOR2_LIBRARIES}
)

remake_add_directories(
  test
  COMPONENT test
)
