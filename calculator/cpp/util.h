#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

void setBit(vector<unsigned char>& array, int index);
void clearBit(vector<unsigned char>& array, int index);
void xorBit(vector<unsigned char>& array, int index);
bool getBit(vector<unsigned char> array, int index);

vector<unsigned char> initArray(int n);
vector<unsigned char> copyArray(int n, vector<unsigned char> array);
void printArray(int n, vector<unsigned char> array);
string arrayToString(int n, vector<unsigned char> array);

vector<unsigned char> charsToBitArray(int n, vector<unsigned char> bs);
vector<unsigned char> decToBs(int n, int dec);
int bsToDec(int n, vector<unsigned char> bs);

int pow2(int n);

#endif