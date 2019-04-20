#include "LinkedList.h"
//-------------------------------------------------------------------------
LinkedList::LinkedList() { head = NULL; }

//-------------------------------------------------------------------------
LinkedList::~LinkedList() { 
	removeAll();
}

//-------------------------------------------------------------------------
bool LinkedList::insert(std::string stringToInsert) {
	Node* newNode = new Node;
	newNode->nodeString = stringToInsert;
	newNode->next = NULL;

	//Empty list
	if (head == NULL) {
		head = newNode;
		return true;
	}

	if (isInList(stringToInsert)) {
		return false;
	}
	
	//Insert as first item
	if (newNode->nodeString < head->nodeString) {
		newNode->next = head;
		head = newNode;
		return true;
	}

	//Insert in middle or end
	Node* current = head->next;
	Node* trail = head;
	while (current != NULL && current->nodeString < newNode->nodeString) {

		trail = current;
		current = current->next;
	}
	newNode->next = trail->next;
	trail->next = newNode;

	return true;
}

//-------------------------------------------------------------------------
bool LinkedList::isInList(std::string checkStr) {
	Node* current = head;
	while (current != NULL) {
		if (current->nodeString == checkStr)
			return true;
		current = current->next;
	}
	return false;
}

//-------------------------------------------------------------------------
bool LinkedList::remove(std::string remStr) {

	if (head == NULL)
		return false;

	if (head->nodeString == remStr) {
		Node* temp = head->next;
		delete head;
		head = temp;
		return true;
	}

	Node* current = head->next;
	Node* trail = head;

	while (current != NULL) {
		if (current->nodeString == remStr) {
			trail->next = current->next;
			delete current;
			return true;
		}
		trail = current;
		current = current->next;
	}

	return false;
}

//-------------------------------------------------------------------------
int LinkedList::count() const {

	Node* current = head;
	int amount = 0;
	while (current != NULL) {
		amount++;
		current = current->next;
	}
	return amount;
}

//-------------------------------------------------------------------------
int LinkedList::find(std::string findStr) {
	int position = 1;
	Node* current = head;
	while (current != NULL) {
		if (current->nodeString == findStr)
			return position;
		position++;
		current = current->next;
	}
	return 0;
}

//-------------------------------------------------------------------------
void LinkedList::removeAll() {
	Node* current = head;

	while (current != NULL) {
		head = current->next;
		delete current;
		current = head;
	}
}

//-------------------------------------------------------------------------
LinkedList* LinkedList::duplicate() {
	LinkedList* newList = new LinkedList;

	//If current list is empty
	if (head == NULL)
		return newList;

	Node* newCurrent = new Node;
	newList->head = newCurrent;
	
	Node* current = this->head;

	while (current != NULL) {
		newCurrent->nodeString = current->nodeString;
		if (current->next != NULL) {
			newCurrent->next = new Node;
			newCurrent = newCurrent->next;
		}
		else
			newCurrent->next = NULL;

		current = current->next;
	}
	return newList;
}

//-------------------------------------------------------------------------
void LinkedList::print() const {
	if (head == NULL) {
		//std::cout << "The list is empty." << std::endl;
		return;
	}

	Node* current = head;
	while (current != NULL) {
		std::string end = (current->next != NULL) ? ", " : "";
		std::cout << current->nodeString << end;
		current = current->next;
	}
	std::cout << std::endl;
}