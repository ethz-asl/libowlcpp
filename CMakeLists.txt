cmake_minimum_required(VERSION 2.6)

include(ReMake)

remake_project(
  libowlcpp
  VERSION 0.3.3
  RELEASE stable
  SUMMARY "A C++ library for working with OWL2 ontologies."
  AUTHOR "Mikhail K Levin"
  CONTACT "mlevin@svarnetics.org"
  HOME "http://sourceforge.net/p/owl-cpp"
  LICENSE "Boost Software License"
  INSTALL /usr
  PREFIX owlcpp-
  HEADER_DESTINATION include/owlcpp
  EXTRA_CXX_FLAGS -std=c++0x
)

remake_doc(
  html
)
 
remake_pack_deb(
  DEPENDS libboost-system[0-9.]* libboost-filesystem[0-9.]*
    libboost-program-options[0-9.]* libfact[+][+] libraptor2-0
)
remake_pack_deb(
  COMPONENT utils
  DEPENDS libowlcpp
  DESCRIPTION "utilities"
)
remake_pack_deb(
  COMPONENT dev
  DEPENDS libowlcpp libboost-system-dev libboost-filesystem-dev
    libboost-program-options-dev libfact[+][+]-dev libraptor2-dev
  DESCRIPTION "development headers"
)
remake_pack_deb(
  ARCH all
  COMPONENT doc
  DESCRIPTION "documentation"
)

remake_distribute_deb(
  DISTRIBUTION trusty
  SECTION libs
  UPLOAD ppa:kralf/asl
  DEPENDS libboost-system-dev libboost-filesystem-dev
    libboost-program-options-dev libboost-test-dev libfact++-dev
    libraptor2-dev remake doxygen pkg-config
  PASS CMAKE_BUILD_TYPE LIBOWLCPP_GIT_REVISION
)
