/** @file "/owlcpp/include/owlcpp/io/raptor_wrapper.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef RAPTOR_WRAPPER_HPP_
#define RAPTOR_WRAPPER_HPP_
#include <iosfwd>
#include <string>
#include "boost/assert.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "boost/shared_ptr.hpp"
#include "owlcpp/io/config.hpp"
#include "owlcpp/io/exception.hpp"

typedef struct raptor_parser_s raptor_parser;
typedef struct raptor_world_s raptor_world;
typedef struct raptor_uri_s raptor_uri;

namespace owlcpp{

/**@brief 
*******************************************************************************/
class OWLCPP_IO_DECL Raptor_wrapper {
   typedef void (*handle_statement_fun_t)(void *, const void*);
   typedef bool (*stop_parsing_fun_t)(const void *);

   static char* blank_prefix_char() {
      static char pref[] = "b";
      return pref;
   }
public:
   struct Err : public Input_err {};

   static std::string const& blank_prefix() {
      static const std::string pref(blank_prefix_char());
      return pref;
   }

   Raptor_wrapper();

   template<class Sink> void operator()(std::istream& stream, Sink& sink) {
      setup(&sink, &handle_statement<Sink>);
      parse(stream);
   }

   template<class Sink> void operator()(std::string const& file, Sink& sink) {
      setup(&sink, &handle_statement<Sink>);
      parse(file);
   }

   const boost::function<void()> abort_call() {
      return boost::bind(&Raptor_wrapper::abort_parse, this);
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
#endif /* RAPTOR_WRAPPER_HPP_ */
