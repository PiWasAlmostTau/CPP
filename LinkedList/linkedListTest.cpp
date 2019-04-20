#include "LinkedList.h"
#include <fstream>

int main() {

	std::string filename = "testfile.txt";
	std::ifstream inputStream;
	inputStream.open(filename);

	if (!inputStream) {

		std::cout << "File " << filename << " could not be opened.\n";
		std::cout << "Program terminated.\n";
		std::cin.get();
		return -1;
	}

	LinkedList nameList;

	//read the data  
	char ch;
	std::string name;
	while (inputStream >> ch) {

		//Adding names
		if (ch == 'a') {
			inputStream >> name;

			if (nameList.insert(name))
				std::cout << "Added " << name << std::endl;
			else
				std::cout << "Can not add " << name << " again" << std::endl;
		}

		//Removing names
		else if (ch == 'r') {
			inputStream >> name;

			if (nameList.remove(name))
				std::cout << "Removed " << name << std::endl;
			else
				std::cout << name << "  is not in the list" << std::endl;
		}

		//Finding names
		else if (ch == 'f') {
			inputStream >> name;

			int pos = nameList.find(name);
			if (pos > 0)
				std::cout << name << " is at position " << pos << std::endl;
			else
				std::cout << name << " is not in the list" << std::endl;
		}

		//Print names
		else if (ch == 'P') {
			int numNames = nameList.count();
			std::cout << "The list contains " << numNames << " name" << (numNames == 1 ? ": " : "s: ");
			nameList.print();
		}

		//Remove all
		else if (ch == 'R'){
			std::cout << "Removing all entries" << std::endl;
			nameList.removeAll();
		}
	}



	//close the file  
	inputStream.close();

	std::cin.get();
	return 0;
}