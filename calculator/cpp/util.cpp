#include <iostream>
#include <string>
#include <vector>
#include "util.h"
using namespace std;


// BITS ----------------------------------------------------------


void setBit(vector<unsigned char>& array, int index) {
	// make bit = 1
	array[index / 8] |= (1 << (index % 8));
}

void clearBit(vector<unsigned char>& array, int index) {
	// make bit = 0
	array[index / 8] &= ~(1 << (index % 8));
}

void xorBit(vector<unsigned char>& array, int index) {
	// flip 0 to 1 or 1 to 0
	array[index / 8] ^= (1 << (index % 8));
}

bool getBit(vector<unsigned char> array, int index) {
	// get a bit from the array
	return (array[index / 8] >> (index % 8)) & 1;
}


// ARRAYS ----------------------------------------------------------


vector<unsigned char> initArray(int n) {
	// make an array
	// round up to full byte
	int num_bytes = (n + 7) / 8;  // a.k.a. buffer size
	vector<unsigned char> array(num_bytes);
	// initialize bit array to all 0s
	fill(begin(array), end(array), 0);
	return array;
}


//vector<unsigned char> copyArray(int n, vector<unsigned char> array);
vector<unsigned char> copyArray(int n, vector<unsigned char> array) {
	// copy array to new array with different memory
	vector<unsigned char> copy = initArray(n);
	for (int i = 0; i < n; i++) {
		if (getBit(array, i)) {
			setBit(copy, i);
		}
	}
	return copy;
}

//void printArray(int n, vector<unsigned char> array);
void printArray(int n, vector<unsigned char> array) {
	// print array
	cout << "[";
	for (int i = 0; i < n; i++) {
		cout << " " << getBit(array, i);
	}
	cout << " ]\n";
}


string arrayToString(int n, vector<unsigned char> array) {
	// array to string
	string s = "[";
	for (int i = 0; i < n; i++) {
		s.append(" " + to_string(getBit(array, i)));
	}
	s.append(" ]\n");
	return s;
}


// CONVERSION ----------------------------------------------------------


//vector<unsigned char> charsToBitArray(int n, char ca[]);
vector<unsigned char> charsToBitArray(int n, vector<unsigned char> bs) {
	// convert char array to bit array
	vector<unsigned char> bitArray = initArray(n);
	for (int i = 0; i < n; i++) {
		if (bs[i] == '1') {
			setBit(bitArray, i);
		}
	}
	return bitArray;
}


vector<unsigned char> decToBs(int n, int dec) {
	// convert decimal to bitstring
	vector<unsigned char> bs = initArray(n);
	for (int i = 0; i < n; i++) {
		int powOf2 = 1 << (n - 1 - i);
		if (dec >= powOf2) {
			dec -= powOf2;
			setBit(bs, i);
		} else {
			clearBit(bs, i);
		}
	}
	return bs;
}

int bsToDec(int n, vector<unsigned char> bs) {
	// convert bitstring to decimal
	int dec = 0;
	for (int i = 0; i < n; i++) {
		dec += getBit(bs, n - 1 - i) * (1 << i);
	}
	return dec;
}


// MATH ----------------------------------------------------------


int pow2(int n) {
	int ans = 1;
	for (int i = 0; i < n; ++i) {
		ans *= 2;
	}
	return(ans);
}










