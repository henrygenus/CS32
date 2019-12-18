/*   #include "Set.h"
 #include <iostream>
 #include <cassert>
 using namespace std;
 
 // To test a Set of unsigned long, leave the #define line commented out;
 // to test a Set of string, remove the "//".
 
 // #define TEST_WITH_STRING
 
 #ifdef TEST_WITH_STRING
 
 const ItemType DUMMY_VALUE = "hello";
 const ItemType V1 = "abc";
 
 #else // assume unsigned long
 
 const ItemType DUMMY_VALUE = 9876543;
 const ItemType V1 = 123456789;
 
 #endif
 
 int main()
 {
 Set s;
 assert(s.empty());
 ItemType v = DUMMY_VALUE;
 assert( !s.get(0, v)  &&  v == DUMMY_VALUE); // v unchanged by get failure
 s.insert(V1);
 assert(s.size() == 1);
 assert(m.get(0, v)  &&  v == V1);
 cout << "Passed all tests" << endl;
 }
*/
