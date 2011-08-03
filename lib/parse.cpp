/** @file "/owl_cpp/lib/parse.cpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/

#include "parse.hpp"
#include <iostream>
#include "boost/lexical_cast.hpp"
namespace b = boost;
#include "raptor2.h"

namespace owl_cpp { namespace{
/**@brief error message handler
*******************************************************************************/
void handle_error(void* data, raptor_log_message* msg) {
   BOOST_THROW_EXCEPTION(
         Rdf_parser::Err()
         << Rdf_parser::Err::msg_t(msg->text)
   );
}
}//namespace anonymous

/*
*******************************************************************************/
Rdf_parser::Rdf_parser(const char* type, const string_t& base_uri)
: world_(0), parser_(0), base_uri_(base_uri) {
   world_ = raptor_new_world();
   if( ! world_ )
      BOOST_THROW_EXCEPTION(Err()<<Err::msg_t("raptor initialization failure"));

   raptor_world_set_log_handler(world_, 0, &handle_error);

   parser_ = raptor_new_parser(world_, type);
   if( ! parser_ )
      BOOST_THROW_EXCEPTION(Err()<<Err::msg_t("raptor initialization failure"));

   raptor_parser_set_option(parser_, RAPTOR_OPTION_STRICT, 0, 1);
}

/**
*******************************************************************************/
Rdf_parser::~Rdf_parser() {
   if( parser_ ) raptor_free_parser(parser_);
   if( world_ ) raptor_free_world(world_);
}

/*
*******************************************************************************/
Rdf_parser Rdf_parser::rdfxml(const string_t& base_uri) {
   return Rdf_parser("rdfxml", base_uri);
}

/*
*******************************************************************************/
void Rdf_parser::parse(
      std::istream& stream,
      void* sink,
      handle_statement_fun_t handle_statement_fun,
      stop_parsing_fun_t stop_parsing_fun,
      const unsigned base
) {
   if( stream.fail() )
      BOOST_THROW_EXCEPTION( Err() << Err::msg_t("read error") );

   raptor_parser_set_statement_handler(
         parser_,
         sink,
         reinterpret_cast<raptor_statement_handler>(handle_statement_fun)
   );

   const std::string base_str = b::lexical_cast<std::string>(base) + "_";
   char base_c[10];
   std::strcpy(base_c, base_str.c_str());
   raptor_world_set_generate_bnodeid_parameters(world_, base_c, 0);

   raptor_uri* uri = raptor_new_uri(world_, base_uri_.c_str());
   raptor_parser_parse_start( parser_, uri );
   std::string str;
   for(unsigned ln = 1; stream; ++ln) {
      if( stop_parsing_fun(sink) ) {
         raptor_parser_parse_abort(parser_);
         return;
      }
      std::getline(stream, str);
      //raptor does not like empty chunks(lines) in the middle of document
      str += '\n';
      try{
         const int i = raptor_parser_parse_chunk(
               parser_,
               reinterpret_cast<const unsigned char*>(str.data()),
               stream ? str.size() : 0,
                     stream ? 0 : 1
         );
         //exceptions should originate from Raptor log handler
         if( i != 0 ) BOOST_THROW_EXCEPTION(Err()<<Err::msg_t("unknown error"));
      } catch(Err& e) {
         BOOST_THROW_EXCEPTION(
               Err()
               << Err::msg_t("RDF error")
               << Err::line_num_t(ln)
//               << Err::str1_t(str)
               << Err::nested_t(b::copy_exception(e))
         );
      }
   }
}

namespace{
/**
*******************************************************************************/
inline Term convert_term(
      const raptor_term* rt
) {
   switch (rt->type) {
   case RAPTOR_TERM_TYPE_URI:
      return Term(Resource, raptor_uri_as_string(rt->value.uri));
   case RAPTOR_TERM_TYPE_LITERAL:
      return Term(Literal, rt->value.literal.string);
   case RAPTOR_TERM_TYPE_BLANK:
      return Term(Anonymous, rt->value.blank.string);
   case RAPTOR_TERM_TYPE_UNKNOWN:
   default:
      return Term(Unknown, "");
   }
}
}//namespace anonymous

/*
*******************************************************************************/
triple_t Rdf_parser::convert_statement(const void* rs) {
   const raptor_statement* stat = reinterpret_cast<const raptor_statement*>(rs);
   triple_t triple(
         convert_term(stat->subject),
         convert_term(stat->predicate),
         convert_term(stat->object)
   );
   return triple;
}

/**
*******************************************************************************/
}//namespace owl_cpp
