#ifndef CALC_H
#define CALC_H

#include <iostream>
#include <vector>
using namespace std;

bool bsEqual(int n, vector<unsigned char> a, vector<unsigned char> b);
bool blEqual(int n, vector<unsigned char> a, vector<unsigned char> b);
void rotate1(int n, vector<unsigned char>& array);
void rotateT(int n, vector<unsigned char>& array, int t);
vector<unsigned char> minValRot(int n, vector<unsigned char> bs);
bool parity(int n, vector<unsigned char> bs);
vector<unsigned char> inverse(int n, vector<unsigned char> bs);
bool isInverse(int n, vector<unsigned char> bs);
vector<unsigned char> reverse(int n, vector<unsigned char> bs);
bool isReverse(int n, vector<unsigned char> bs);
bool isSymmetric(int n, vector<unsigned char> bs);
vector<unsigned char> link(int n, vector<unsigned char> bs);
vector<unsigned char> sublink(int n, vector<unsigned char> bs);
vector<unsigned char> pattern(int n, vector<unsigned char> bs);
vector<unsigned char> root(int n, vector<unsigned char> bs);
vector<vector<unsigned char>> powersetMinValRots(int n);
vector<unsigned char> chain(int n, vector<unsigned char> bs, int& cycleOrder);
vector<vector<unsigned char>> chains(int n);
vector<vector<unsigned char>> chainNodes(int n, vector<unsigned char> ch, int cycleOrder);
vector<vector<unsigned char>> treeNodes(int n, vector<unsigned char> rt);
void makeTree(int n, vector<vector<unsigned char>>& tns, vector<unsigned char> sublnk);
vector<unsigned char> sublinkOfRootOffCycle(int n, vector<unsigned char> rt);

#endif