//
//#include <iostream>
//#include <cassert>
//#include "Set.h"
//
//void Test();
//
//int main()
//{
//    Test();
//}
//
//void Test()
//{
//    // ItemType is std::string
//    
//    // empty tests
//    Set ss;
//    assert(ss.size() == 0);                 // test size
//    assert(ss.empty());                     // test empty
//    assert(!ss.erase("roti"));              // nothing to remove
//    assert(ss.insert("temp")
//           && ss.erase("temp"));
//    std::cerr << "Empty Tests Passed." << std::endl;
//    //      << "Printing Sets:\n" << std::endl;
//    //    std::cerr << "Set ss: "; ss.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//    
//    //standard tests
//    Set s;
//    s.insert("ccc");
//    s.insert("aaa");
//    s.insert("bbb");
//    ItemType x = "xxx";
//    assert(!s.get(3, x)  &&  x == "xxx");       // x is unchanged
//    assert(s.get(1, x) &&  x == "bbb");         // "bbb" is greater than 1 item
//    std::cerr << "Standard Tests Passed." << std::endl;
//    //      << "Printing Sets:\n" << std::endl;;
//    //    std::cerr << "Set s: "; s.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//    
//    //swap function
//    Set t;
//    t.swap(s);
//    assert(s.size() == 0);                          // test size
//    assert(s.empty());                              // test empty
//    assert(!s.contains("ccc"));                     // t emptied
//    x = "xxx";
//    assert(!t.get(3, x)  &&  x == "xxx");           // x is unchanged
//    assert(t.get(1, x) &&  x == "bbb");             // "bbb" is greater than 1 item
//    std::cerr << "Swap Tests Passed." << std::endl;
//    //      << "Printing Sets:\n" << std::endl;
//    //    std::cerr << "Set s: "; s.dump();
//    //    std::cerr << "Set t: "; t.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//    
//    //copy constructor
//    Set tt(t);
//    x = "xxx";
//    assert(!tt.get(3, x)  &&  x == "xxx");         // x is unchanged
//    assert(tt.get(1, x) &&  x == "bbb");           // "bbb" is greater than 1 item
//    std::cerr << "Copy Tests Passed." << std::endl;
//    //       << "Printing Sets:" << std::endl;
//    //    std::cerr << "Set tt: "; tt.dump();
//    //    std::cerr << "Set t: "; t.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//    
//    //assignment operator
//    tt = ss;
//    assert(tt.size() == 0);                        // test size
//    assert(tt.empty());                            // test empty
//    assert(!tt.contains(""));                      // tt emptied
//    std::cerr << "Assignment Tests Passed." << std::endl;
//    //       << "Printing Sets:\n" << std::endl;
//    //    std::cerr << "Set tt: "; tt.dump();
//    //    std::cerr << "Set ss: ";   ss.dump();
//    //    std::cerr << "Set t: "; t.dump();
//    //    std::cerr << "Set s: "; s.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//    
//    //subtract
//    Set a;
//    assert(a.insert("alpha")
//           && a.insert("beta"));                    //a == alpha,beta
//    Set b = a;
//    assert(b.erase("alpha")
//           && b.contains("beta")
//           && !b.contains("alpha"));                //b = beta
//    Set result;
//    subtract(a, b, result);                         //a-b = alpha
//    assert(!result.contains("beta")
//           && result.contains("alpha"));
//    std::cerr << "Subtract Tests Passed." << std::endl;
//    //       << "Printing Sets:\n" << std::endl;
//    //    std::cerr << "Set a: "; a.dump();
//    //    std::cerr << "Set b: ";   b.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//
//    //unite
//    Set c;
//    assert(c.insert("beta")
//           && c.insert("delta"));                   //c=beta, delta
//    Set d = c;
//    assert(d.erase("delta")
//           && d.insert("charlie"));
//    assert(d.contains("beta")
//           && !d.contains("delta"));                //d=charlie, beta
//    unite(c, d, result);                            //c+d = beta, charlie, delta
//    assert(result.contains("beta")
//           && result.contains("delta")
//           && result.contains("charlie")
//           && !result.contains("alpha"));
//    std::cerr << "Unite Tests Passed." << std::endl;
//    //       << "Printing Sets:\n" << std::endl;
//    //    std::cerr << "Set c: "; c.dump();
//    //    std::cerr << "Set d: ";   d.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//    
//    //edge cases for unite
//    Set e;
//    Set f = a;
//    unite(e, f, result);                            //test empty s1
//    assert(result.contains("alpha")
//           && result.contains("beta"));             // e + f = alpha,beta
//    result = tt;                                    //set to empty
//    unite(f, e, result);                            //test empty s2
//    assert(result.contains("alpha")
//           && result.contains("beta"));             // e + f = alpha,beta
//    std::cerr << "Unite Edge Tests Passed." << std::endl;
//    //       << "Printing Sets:\n" << std::endl;
//    //    std::cerr << "Set e: "; e.dump();
//    //    std::cerr << "Set f: "; f.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//
//    //edge cases for unite
//    Set g;
//    Set h = a;
//    subtract(g, h, result);                            //test empty s1
//    assert(!result.contains("alpha")
//           && !result.contains("beta"));             // e + f = alpha,beta
//    result = tt;                                     //set to empty
//    subtract(h, g, result);                          //test empty s2
//    assert(result.contains("alpha")
//           && result.contains("beta"));              // e + f = alpha,beta
//    std::cerr << "Subtract Edge Tests Passed." << std::endl;
//    //       << "Printing Sets:\n" << std::endl;
//    //    std::cerr << "Set g: "; g.dump();
//    //    std::cerr << "Set h: "; h.dump();
//    //    std::cerr << std::string(20,'-') << std::endl;
//
//    std::cerr << "All Tests Passed." << std::endl;
//}
//
