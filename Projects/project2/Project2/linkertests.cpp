/*
 //////////////////////////////////////////
 ///////////    Linker Tests    ///////////
 //////////////////////////////////////////
 
 #include "Set.h"
 #include <type_traits>
 
 #define CHECKTYPE(f, t) { auto p = static_cast<t>(f); (void)p; }
 
 static_assert(std::is_default_constructible<Set>::value,
 "Set must be default-constructible.");
 static_assert(std::is_copy_constructible<Set>::value,
 "Set must be copy-constructible.");
 
 void thisFunctionWillNeverBeCalled()
 {
 CHECKTYPE(&Set::operator=, Set& (Set::*)(const Set&));
 CHECKTYPE(&Set::empty,     bool (Set::*)() const);
 CHECKTYPE(&Set::size,      int  (Set::*)() const);
 CHECKTYPE(&Set::insert,    bool (Set::*)(const ItemType&));
 CHECKTYPE(&Set::erase,     bool (Set::*)(const ItemType&));
 CHECKTYPE(&Set::contains,  bool (Set::*)(const ItemType&) const);
 CHECKTYPE(&Set::get,       bool (Set::*)(int, ItemType&) const);
 CHECKTYPE(&Set::swap,      void (Set::*)(Set&));
 CHECKTYPE(unite,    void (*)(const Set&, const Set&, Set&));
 CHECKTYPE(subtract, void (*)(const Set&, const Set&, Set&));
 }
 
 int main()
 {}
 */
