/** @file "/owlcpp/lib/io/parser_triple.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/parser_triple.hpp"
#include <iostream>
#include "boost/lexical_cast.hpp"
#include "boost/bind.hpp"
#include "raptor2.h"

namespace owlcpp { namespace{
/**@brief error message handler
*******************************************************************************/
void handle_error(void* data, raptor_log_message* msg) {
   Parser_triple::Err e;
   e << Parser_triple::Err::msg_t(msg->text);
   if( msg->locator ) {
      const int n = raptor_locator_line(msg->locator);
      if( n >= 0 ) e << Parser_triple::Err::int1_t(n);
      const char* file = raptor_locator_file(msg->locator);
      if( file ) e << Parser_triple::Err::str1_t(file);
   }
   BOOST_THROW_EXCEPTION(e);
}
}//namespace anonymous

/*
*******************************************************************************/
Parser_triple::Parser_triple()
: world_(raptor_new_world(), &raptor_free_world),
  parser_(raptor_new_parser(world_.get(), "rdfxml"), &raptor_free_parser),
  data_(),
  abort_requested_(false)
{
   if( ! world_ ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("raptor world initialization failure")
   );

   raptor_world_set_log_handler(world_.get(), 0, &handle_error);

   if( ! parser_ ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("raptor parser initialization failure")
   );

   raptor_parser_set_option(parser_.get(), RAPTOR_OPTION_STRICT, 0, 1);
}

/*
*******************************************************************************/
void Parser_triple::abort_parse() {
   abort_requested_ = true;
   raptor_parser_parse_abort(parser_.get());
}

/*
*******************************************************************************/
void Parser_triple::setup(void* sink, handle_statement_fun_t hs_fun) {
   data_.first = sink;
   data_.second = boost::bind(&Parser_triple::abort_parse, this);

   raptor_parser_set_statement_handler(
         parser_.get(),
         &data_,
         reinterpret_cast<raptor_statement_handler>(hs_fun)
   );

}

/*
*******************************************************************************/
void Parser_triple::parse(std::string const& fn) {
   boost::shared_ptr<unsigned char> uri_str(
            raptor_uri_filename_to_uri_string(fn.c_str()),
            &raptor_free_memory
   );
   boost::shared_ptr<raptor_uri> uri(
            raptor_new_uri(world_.get(), uri_str.get()),
            &raptor_free_uri
   );
   boost::shared_ptr<raptor_uri> base_uri(
            raptor_uri_copy(uri.get()),
            &raptor_free_uri
   );
   try{
      const int i = raptor_parser_parse_file(parser_.get(), uri.get(), 0);
      //exceptions should originate from Raptor log handler
      if( i != 0 ) BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("unknown error")
      );
   } catch(Err& e) {
      BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("RDF error")
            << Err::str1_t(fn)
//               << Err::str1_t(str)
            << Err::nested_t(boost::copy_exception(e))
      );
   }
}

/*
*******************************************************************************/
void Parser_triple::parse(std::istream& stream) {
   if( ! stream.good() )
      BOOST_THROW_EXCEPTION( Err() << Err::msg_t("read error") );

   char const* uri_str = "from_stream";
   boost::shared_ptr<raptor_uri> uri(
            raptor_new_uri(
                     world_.get(),
                     reinterpret_cast<unsigned char const*>(uri_str)
            ),
            &raptor_free_uri
   );

   raptor_parser_parse_start( parser_.get(), uri.get() );

   std::string str;
   for(unsigned ln = 1; stream.good() && ! abort_requested_; ++ln) {
      std::getline(stream, str);
      //raptor does not like empty chunks(lines) in the middle of document
      str += '\n';
      try{
         const int i = raptor_parser_parse_chunk(
               parser_.get(),
               reinterpret_cast<const unsigned char*>(str.data()),
               stream ? str.size() : 0,
                     stream ? 0 : 1
         );
         //exceptions should originate from Raptor log handler
         if( i != 0 ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unknown error")
         );
      } catch(Err& e) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("RDF error")
               << Err::int1_t(ln)
//               << Err::str1_t(str)
               << Err::nested_t(boost::copy_exception(e))
         );
      }
   }
}
}//namespace owlcpp
