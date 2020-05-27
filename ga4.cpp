#include <iostream>
#include <string>
#include <fstream>
#include "ArgumentManager.h"
using namespace std;

bool isPrime(int n)
{
	// Corner cases 
	if (n <= 1)  return false;
	if (n <= 3)  return true;

	// This is checked so that we can skip  
	// middle five numbers in below loop 
	if (n % 2 == 0 || n % 3 == 0) return false;

	for (int i = 5; i*i <= n; i = i + 6)
		if (n%i == 0 || n % (i + 2) == 0)
			return false;

	return true;
}

int nextprime(int x) {
	//if(x == 1)
	do {
		x++;
	} while (!isPrime(x));
	return x;
}

int prevprime(int x) {
	if (x == 2)
		return 1;
	do {
		x--;
	} while (!isPrime(x));
	return x;
}

int convertn(char letter) {
	if (letter == 'a')
		return 1;
	else {
		int x = int(letter);
		return (x + 1) - 97;
	}
}

int convert(string line) {
	int sum = 0;
	for (size_t i = 0; i < line.length(); i++) {
		if (line[i] == ' ') {
			sum += 27;
		}
		else
		sum += convertn(line[i]);
	}
	return sum;
}

int hashn(int key, int size) {
	return key % size;
}

int hash2(int key, int size) {
	int prev = prevprime(size);
	return prev - (key%prev);
}
int probe(string HT[], int key, int size) {
	int i = 0;
	int index = hashn(key, size);
	int index2 = hash2(key, size);
	while (HT[(index + i*index2) % size] != "") {
		i++;
	}
	return (index + i*index2) % size;
}

void insert(string A[], int key, int size, string book) {
	int index = hashn(key, size);
	if (A[index] != "")
		index = probe(A, key, size);
	A[index] = book;
}

void search(string H[],int size, string book,ofstream &outf) {
	int key = convert(book);
	int index = hashn(key, size);
	int index2 = hash2(key, size);
	int i = 0;

	while (H[(index + i*index2) % size] != book && H[(index + i * index2) % size] != "")
		i++;

	int x = (index + i*index2) % size;

	if (H[(index + i * index2) % size] == book)
		outf << book << " found at : " << x << " after " << i << " iteration" << endl;
	else
		outf << book << " does not exist; needed " << i << " iterations" << endl;
}

void display(string A[], int size, ofstream &outf) {
	outf << "Current values in the hash table:" << endl;
	for (int i = 0; i < size; i++) {
		if(A[i] != "")
		outf << A[i] << " at: " << i << endl;
	}
}


int main(int argc, char* argv[]) {
	ArgumentManager am(argc, argv);
	const string inputx = am.get("books");
	const string commandx = am.get("commands");
	const string outputx = am.get("output");

	ifstream in_file;
	ifstream command_file;

	in_file.open(inputx);
	string line;
	int count = 0;

	while (getline(in_file, line)) {
		if (line.empty())
			continue;
		else {
			count++;
		}
	}
	in_file.close();

	int size;
	if (count == 1) {
		size = 2;
	}
	else {
		size = nextprime(count);
	}
	//Declare array of dynamic size
	string *arr = new string[size]{};


	in_file.open(inputx);
	int key;
	while (getline(in_file, line)) {
		if (line.empty())
			continue;
		else {
			key = convert(line);
			insert(arr, key, size, line);
			//cout << line << endl;
		}
	}

	ofstream output_file;
	output_file.open(outputx);

	output_file << "Number of books : " << count << ", Table size : "<< size << ", Prime : " << prevprime(size) << endl;
	output_file << "\n";

	command_file.open(commandx);
	//cout << prevprime(size);

	while (getline(command_file, line)) {
		if (line.substr(0, 4) == "Find") {
			string word = line.substr(5);
			search(arr, size, word, output_file);
		}
		else if (line.substr(0, 7) == "Display"){
			display(arr, size, output_file);
		}
	}

	in_file.close();
	command_file.close();
	output_file.close();
	//cin.get();
	return 0;
}