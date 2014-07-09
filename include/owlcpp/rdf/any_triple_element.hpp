/** @file "/owlcpp/include/owlcpp/rdf/any_triple_element.hpp" 
part of owlcpp project.
@n @n Distributed under the Boost Software License, Version 1.0; see doc/license.txt.
@n Copyright Mikhail K Levin 2012
*******************************************************************************/
#ifndef ANY_TRIPLE_ELEMENT_HPP_
#define ANY_TRIPLE_ELEMENT_HPP_

namespace owlcpp{

class Node_id;
class Doc_id;

/** Equals to any node or document ID.
Use for searching triples to indicate the elements of a triple that should be
ignored.
*******************************************************************************/
struct any{};

inline bool operator==(any const&, Node_id const&) {return true;}
inline bool operator==(Node_id const&, any const&) {return true;}
inline bool operator==(any const&, Doc_id const&) {return true;}
inline bool operator==(Doc_id const&, any const&) {return true;}
inline bool operator<(any const&, Node_id const&) {return false;}
inline bool operator<(Node_id const&, any const&) {return false;}
inline bool operator<(any const&, Doc_id const&) {return false;}
inline bool operator<(Doc_id const&, any const&) {return false;}

}//namespace owlcpp
#endif /* ANY_TRIPLE_ELEMENT_HPP_ */
