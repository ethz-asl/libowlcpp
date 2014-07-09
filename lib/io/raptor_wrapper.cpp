/** @file "/owlcpp/lib/io/raptor_wrapper.cpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef OWLCPP_IO_SOURCE
#define OWLCPP_IO_SOURCE
#endif
#include "owlcpp/io/raptor_wrapper.hpp"
#include <istream>
#include "boost/lexical_cast.hpp"
#include "raptor2.h"

namespace owlcpp { namespace{

Raptor_wrapper::Err make_exception(char const* msg, raptor_locator* locator) {
   Raptor_wrapper::Err e;
   if( msg ) e << Raptor_wrapper::Err::msg_t(msg);
   if( locator ) {
      const int n = raptor_locator_line(locator);
      if( n >= 0 ) e << Raptor_wrapper::Err::int1_t(n);
      const char* file = raptor_locator_file(locator);
      if( file ) e << Raptor_wrapper::Err::str1_t(file);
   }
   return e;
}

/**@brief error message handler
*******************************************************************************/
void handle_error(void*, raptor_log_message* msg) {
   Raptor_wrapper::Err e = make_exception(msg->text, msg->locator);
   BOOST_THROW_EXCEPTION(e);
}
}//namespace anonymous

/*
*******************************************************************************/
Raptor_wrapper::Raptor_wrapper()
: world_(raptor_new_world(), &raptor_free_world),
  parser_(raptor_new_parser(world_.get(), "rdfxml"), &raptor_free_parser),
  abort_requested_(false)
{
   if( ! world_ ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("raptor world initialization failure")
   );

   raptor_world_set_log_handler(world_.get(), 0, &handle_error);
   raptor_world_set_generate_bnodeid_parameters(world_.get(), blank_prefix_char(), 1);

   if( ! parser_ ) BOOST_THROW_EXCEPTION(
            Err()
            << Err::msg_t("raptor parser initialization failure")
   );

   raptor_parser_set_option(parser_.get(), RAPTOR_OPTION_STRICT, 0, 1);
}

/*
*******************************************************************************/
void Raptor_wrapper::abort_parse() {
   abort_requested_ = true;
   raptor_parser_parse_abort(parser_.get());
}

/*
*******************************************************************************/
void Raptor_wrapper::setup(void* sink, handle_statement_fun_t hs_fun) {
   raptor_parser_set_statement_handler(
         parser_.get(),
         sink,
         reinterpret_cast<raptor_statement_handler>(hs_fun)
   );
}

/*
*******************************************************************************/
void Raptor_wrapper::parse(std::string const& fn) {
   boost::shared_ptr<unsigned char> uri_str(
            raptor_uri_filename_to_uri_string(fn.c_str()),
            &raptor_free_memory
   );

   boost::shared_ptr<raptor_uri> uri(
            raptor_new_uri(world_.get(), uri_str.get()),
            &raptor_free_uri
   );

/*
   boost::shared_ptr<unsigned char> base_uri_str(
            raptor_uri_filename_to_uri_string("/"),
            &raptor_free_memory
   );

   boost::shared_ptr<raptor_uri> base_uri(
            raptor_new_uri(world_.get(), base_uri_str.get()),
            &raptor_free_uri
   );
*/

   try{
      /*const int i =*/ raptor_parser_parse_file(parser_.get(), uri.get(), 0/*base_uri.get()*/);
      //exceptions should originate from Raptor log handler
//      if( i != 0 && ! abort_requested_ ) BOOST_THROW_EXCEPTION(
//               Err()
//               << Err::msg_t("unknown error")
//      );
   } catch(Err&) {
      Err e = make_exception("RDF error", raptor_parser_get_locator(parser_.get()));
      e << Err::nested_t(boost::current_exception());
      BOOST_THROW_EXCEPTION(e);
   }
}

/*
*******************************************************************************/
void Raptor_wrapper::parse(std::istream& stream) {
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
         if( i != 0 && ! abort_requested_ ) BOOST_THROW_EXCEPTION(
                  Err()
                  << Err::msg_t("unknown parsing error")
                  << Err::int1_t(ln)
         );
      } catch(std::exception const&) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("RDF parsing error")
               << Err::int1_t(ln)
               << Err::nested_t(boost::current_exception())
         );
      }
   }
}
}//namespace owlcpp
