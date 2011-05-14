/** @file "/owl_cpp/include/owl_cpp/comparable_value.hpp" 
part of owl_cpp project.
Distributed under GNU Lesser General Public License; see doc/license.txt.
@date 2010 @author Mikhail K Levin
*******************************************************************************/
#ifndef COMPARABLE_VALUE_HPP_
#define COMPARABLE_VALUE_HPP_

namespace owl_cpp{

/** Use as a base for making type-specific IDs for large objects
*******************************************************************************/
template<class T> class Comparable_value {
public:
   typedef T value_type;
   bool operator==(const Comparable_value& t) const {return val_ == t.val_;}
   bool operator<(const Comparable_value& t) const {return val_< t.val_;}
   bool operator>(const Comparable_value& t) const {return val_ > t.val_;}
   bool operator!=(const Comparable_value& t) const {return val_ != t.val_;}
   T operator()() const {return val_;}

protected:
   Comparable_value() : val_() {}
   explicit Comparable_value(const T val) : val_(val) {}
   Comparable_value(const Comparable_value& cv) : val_(cv.val_) {}
   T& value() {return val_;}
private:
   T val_;
};

typedef Comparable_value<unsigned> unsigned_val_t;

}//namespace owl_cpp

#endif /* COMPARABLE_VALUE_HPP_ */
