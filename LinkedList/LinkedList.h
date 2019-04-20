#pragma once
#include <iostream>
#include <string>


struct Node {
	std::string nodeString;
	Node* next;
};

class LinkedList
{
public:

	/*--------------------------------------------------------------
	Description- Default constructor, creates a linked list with a
				 head value of NULL
	--------------------------------------------------------------*/
	LinkedList();

	/*--------------------------------------------------------------
	Description- Destructor, deletes strings and deallocates memory
	--------------------------------------------------------------*/
	~LinkedList();

	/*--------------------------------------------------------------
	Description- Inserts a string into the list in alphabetic order,
				 no duplicates allowed
	@param stringToInsert - string to insert into list
	@return bool value, true if successfully inserted, false if duplicate value
	--------------------------------------------------------------*/
	bool insert(std::string stringToInsert);

	/*--------------------------------------------------------------
	Description- Finds if a given string is in the list
	@param checkStr - string to look for in the list
	@return bool value, true if in list, false if not
	--------------------------------------------------------------*/
	bool isInList(std::string checkStr);

	/*--------------------------------------------------------------
	Description- Removes a given string from the list
	@param  remStr - The string to find and remove
	@return bool value, true if found and removed, false if not found
	--------------------------------------------------------------*/
	bool remove(std::string remStr);

	/*--------------------------------------------------------------
	Description- Counts the amount of items in the list
	@return The number of items in the list
	--------------------------------------------------------------*/
	int count() const;

	/*--------------------------------------------------------------
	Description- Finds the position of a given string
	@param  findStr - The string to look for in the list
	@return The number of the location of the string, 0 if not found
	--------------------------------------------------------------*/
	int find(std::string findStr);

	/*--------------------------------------------------------------
	Description- Removes all strings from linked list and deallocates memory
	--------------------------------------------------------------*/
	void removeAll();

	/*--------------------------------------------------------------
	Description- Duplicates a linked list of strings
	@return pointer to a new linked list
	--------------------------------------------------------------*/
	LinkedList* duplicate();

	/*--------------------------------------------------------------
	Description- Prints all strings in the list
	--------------------------------------------------------------*/
	void print() const;

private:
	Node* head;
};

