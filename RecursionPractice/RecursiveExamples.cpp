#include <iostream>
#include <string>
#include <cmath>

/* Recursion practice */

// Check if a given strink is a palindrome (no spaces- could add support for that later though)
bool isPalindrome(std::string check) {
	if (check.length() <= 1)
		return true;

	if (check[0] == check[check.length()-1]) 
		return isPalindrome(check.substr(1, check.length() - 2));
	
	
	return false;
}

void printPalindrome(std::string check) {
	if (isPalindrome(check))
		std::cout << check << " is a palindrome." << std::endl;
	else
		std::cout << check << " is not a palindrome." << std::endl;
}

//Finds the sum of all numbers from 1 to n
int sum(int n) {
	if (n < 1)
		return 0;
	if (n == 1)
		return 1;
	else
		return (n + sum(n - 1));
}

//Sums up all numbers in an array
int sumArray(int a[], int n) {
	if (n == 1)
		return a[0];
	else
		return a[n-1] + sumArray(a, n - 1);

}

//Finds the lowest number in an array
int findMin(int a[], int n) {
	if (n == 1)
		return a[0];
	else {
		int recurse = findMin(a, n - 1);
		if (a[n - 1] < recurse)
			return a[n - 1];
		else return recurse;
	}
}

int main() {

	std::cout << sum(4) << std::endl;
	std::cout << sum(0) << std::endl;

	std::string test = "racecar";

	printPalindrome(test);

	int sum[5] = { 5,2,3,4,6 };
	std::cout << sumArray(sum, 5) << std::endl;

	int arr[4] = { 8,5,3,9 };
	std::cout << findMin(arr, 4) << std::endl;

	std::cin.get();
}