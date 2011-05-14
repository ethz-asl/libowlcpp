/** @file "/owl_cpp/lib/parse.hpp"
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef PARSE_HPP_
#define PARSE_HPP_
#include <iosfwd>
#include <string>
#include "raptor_triple.hpp"
#include "owl_cpp/exception.hpp"

typedef std::basic_string<unsigned char> ustring_t;

typedef struct raptor_parser_s raptor_parser;
typedef struct raptor_world_s raptor_world;

namespace owl_cpp{

/**
*******************************************************************************/
class Rdf_parser {
   typedef void (*handle_statement_fun_t)(void *, const void*);
   typedef bool (*stop_parsing_fun_t)(const void *);
public:
   //Raptor prefers unsigned chars
   typedef std::basic_string<unsigned char> string_t;

   struct Err : public base_exception {
      typedef b::error_info<struct errinfo_line_num_, unsigned> line_num_t;
   };
   static Rdf_parser rdfxml(const string_t&);

   ~Rdf_parser();

   template<class Sink> void operator()(
         std::istream& stream,
         Sink& sink,
         const unsigned base
   ) {
	   parse(stream, &sink, handle_statement<Sink>, stop_parsing<Sink>, base);
   }

private:
   raptor_world* world_;
   raptor_parser* parser_;
   const string_t base_uri_;

   Rdf_parser(const char*, const string_t&);

   void parse(
         std::istream&, void*,
         handle_statement_fun_t,
         stop_parsing_fun_t,
         const unsigned base
   );

   template<class Sink> static void handle_statement(void* sink, const void* rs) {
      const triple_t triple = convert_statement(rs);
	   reinterpret_cast<Sink*>(sink)->insert(triple);
   }

   template<class Sink> static bool stop_parsing(const void* sink) {
      return reinterpret_cast<const Sink*>(sink)->stop_parsing();
   }

   static triple_t convert_statement(const void* rs);
};


}//namespace owl_cpp

#endif /* PARSE_HPP_ */
