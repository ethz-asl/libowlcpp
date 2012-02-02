/** @file "/owlcpp/include/owlcpp/io/parser_triple.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef PARSER_TRIPLE_HPP_
#define PARSER_TRIPLE_HPP_
#include <iosfwd>
#include <string>
#include "boost/assert.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "owlcpp/io/config.hpp"
#include "owlcpp/io/exception.hpp"

typedef std::basic_string<unsigned char> ustring_t;

typedef struct raptor_parser_s raptor_parser;
typedef struct raptor_world_s raptor_world;
typedef struct raptor_uri_s raptor_uri;

namespace owlcpp{

/**
*******************************************************************************/
class OWLCPP_IO_DECL Parser_triple {
   typedef void (*handle_statement_fun_t)(void *, const void*);
   typedef bool (*stop_parsing_fun_t)(const void *);
public:
   struct Err : public Input_err {};

   Parser_triple();

   template<class Sink> void operator()(
         std::istream& stream,
         Sink& sink
   ) {
      setup(&sink, &handle_statement<Sink>);
      parse(stream);
      sink.finalize();
   }

   template<class Sink> void operator()(
         std::string const& file_name,
         Sink& sink
   ) {
      setup(&sink, &handle_statement<Sink>);
      parse(file_name);
      sink.finalize();
   }

   const boost::function<void()> abort_call() {
      return boost::bind(&Parser_triple::abort_parse, this);
   }

private:
   boost::shared_ptr<raptor_world> world_;
   boost::shared_ptr<raptor_parser> parser_;
   bool abort_requested_;

   void abort_parse();

   void setup(void* sink, handle_statement_fun_t hs_fun);

   void parse(std::istream&);

   void parse(std::string const&);

   template<class Sink> static void handle_statement(void* data, const void* rs) {
      static_cast<Sink*>(data)->insert(rs);
   }

};

}//namespace owlcpp
#endif /* PARSER_TRIPLE_HPP_ */
