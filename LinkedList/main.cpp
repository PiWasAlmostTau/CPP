////#include "LinkedList.h"
//
//int test() {
//	LinkedList list1;
//	list1.insert("Bob");
//	list1.print();
//	std::cout << list1.count() << std::endl;
//
//	list1.insert("Bill");
//	list1.print();
//	std::cout << list1.count() << std::endl;
//
//	list1.insert("Xander");
//	list1.print();
//	std::cout << list1.count() << std::endl;
//
//	list1.insert("John");
//	list1.print();
//	std::cout << list1.count() << std::endl;
//
//	list1.insert("Anthony");
//	list1.print();
//	std::cout << list1.count() << std::endl;
//
//	std::string test = "Anthony";
//	std::cout << test << " found at position " << list1.find(test) << std::endl;
//
//	std::cout << list1.remove("Billybob") << std::endl;
//	list1.print();
//	std::cout << list1.count() << std::endl;
//
//	LinkedList list2 = *list1.duplicate();
//
//	list2.print();
//	list2.removeAll();
//	list2.print();
//
//
//	std::cin.get();
//	return 0;
//}