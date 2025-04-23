#include <iostream>
#include <vector>
#include "calc.h"
#include "util.h"
using namespace std;


bool bsEqual(int n, vector<unsigned char> a, vector<unsigned char> b) {
	// are bitstrings equal
    for (int i = 0; i < n; i++) {
		// compare bits
		if (getBit(a, i) ^ getBit(b, i)) { 
			return false;
		}
	}
	return true;
}


bool blEqual(int n, vector<unsigned char> a, vector<unsigned char> b) {
	// are bitloops equal
	for (int t = 0; t < n; t++) {
		if (bsEqual(n, a, b)) {
			return true;
		}
		rotate1(n, b);
	}
	return false;
}


void rotate1(int n, vector<unsigned char>& bs) {
	// rotate 1 bit around
	// save rightmost bit to wrap around
	unsigned char last_bit = (bs[(n - 1) / 8] >> ((n - 1) % 8)) & 1;
	// for each bit starting with the rightmost
	for (int i = n - 1; i > 0; i--) {
		// if left bit is set
		if (bs[(i - 1) / 8] & (1 << ((i - 1) % 8))) {
			// then set the bit
			bs[i / 8] |= (1 << (i % 8)); // 1 == 0b00000001
		// else it is clear
		} else {
			// clear the bit
			bs[i / 8] &= ~(1 << (i % 8));
		}
	}
	// if the rightmost bit is set
	if (last_bit) {
		// set the leftmost bit
		bs[0] |= (1 << 0);
	// else it is clear
	} else {
		// clear the bit
		bs[0] &= ~(1 << 0);
	}
}


//void rotateT(int n, vector<unsigned char>& array, int t);
void rotateT(int n, vector<unsigned char>& array, int t) {
	// rotate T bits around
	t = t % n;
	if (t < 0) {
		t += n;
	}
	vector<unsigned char> temp = initArray(n);
	for (int i = 0; i < n; i++) {
		int j = (i + t) % n;
		if (array[i / 8] & (1 << (i % 8))) {
			temp[j / 8] |= (1 << (j % 8));
		}
	}
	for (int i = 0; i < (n + 7) / 8; i++) {
		array[i] = temp[i];
	}
}


vector<unsigned char> minValRot(int n, vector<unsigned char> bs) {
	int min = 1 << n;
	for (int i = 0; i < n; i++) {
		rotate1(n, bs);
		int dec = bsToDec(n, bs);
		if (dec < min) {
			min = dec;
		}
	}
	return decToBs(n, min);
}


bool parity(int n, vector<unsigned char> bs) {
	// is bitstring "even" or "odd"
	// returns 0 (false) if there are an even number of 1s
	// returns 1 (true) if there are an odd number of 1s
	bool p = 0;
	for (int i = 0; i < n; i++) {
		bool b = getBit(bs, i);
		p = p ^ b;
	}
	return p;
}


vector<unsigned char> inverse(int n, vector<unsigned char> bs) {
	// flip 1s and 0s in a bitstring
	for (int i = 0; i < n; i++) {
		xorBit(bs, i);
	}
	return bs;
}


//bool isInverse(int n, vector<unsigned char> bs);
bool isInverse(int n, vector<unsigned char> bs) {
	// is a bitloop equal to its inverse
	vector<unsigned char> inv = inverse(n, bs);
	bool b = blEqual(n, bs, inv);
	return b;
}


//vector<unsigned char> reverse(int n, vector<unsigned char> bs);
vector<unsigned char> reverse(int n, vector<unsigned char> bs) {
	// returns bits in reverse order
	vector<unsigned char> rev = initArray(n);
	for (int i = 0; i < n; i++) {
		if (getBit(bs, n - 1 - i)) {
			setBit(rev, i);
		}
	}
	return rev;
}


//bool isReverse(int n, vector<unsigned char> bs);
bool isReverse(int n, vector<unsigned char> bs) {
	// is a bitstring equal to its reverse
	vector<unsigned char> rev = copyArray(n, bs);
	rev = reverse(n, rev);
	bool b = bsEqual(n, bs, rev);
	return b;
}


//bool isSymmetric(int n, vector<unsigned char> bs);
bool isSymmetric(int n, vector<unsigned char> bs) {
	// is a bitloop equal to its reverse
	// does the circle of bits have an axis of symmetry
	vector<unsigned char> rev = copyArray(n, bs);
	rev = reverse(n, rev);
	bool b = blEqual(n, bs, rev);
	return b;
}


vector<unsigned char> link(int n, vector<unsigned char> bs) {
	// return the link of a given bitloop
	vector<unsigned char> lnk = initArray(n);
	for (int i = 0; i < n; i++) {
		if (getBit(bs, i) ^ getBit(bs, (i + 1) % n)) {
			setBit(lnk, i);
		}
	}
	return lnk;
}


vector<unsigned char> sublink(int n, vector<unsigned char> bs) {
	// return the sublink of a given bitloop
	if (parity(n, bs)) {
		return {};
	}
	vector<unsigned char> slink = initArray(n);
	for (int i = 0; i < n - 1; i++) {
		if (getBit(slink, i) ^ getBit(bs, i)) {
			setBit(slink, i + 1);
		}
	}
	return slink;
}


//vector<unsigned char> pattern(int n, vector<unsigned char> bs);
vector<unsigned char> pattern(int n, vector<unsigned char> bs) {
	// returns the pattern of a given bitloop
	int div = 0; // divisor of n
	bool match = 0;
	while (!match) {
		while (n % ++div != 0) {} // div = next divisor
		match = 1;
		// i is the partition index within the bitstring
		for (int i = 0; match && i < n / div; i++) {
			// j is the bit within partition_i
			for (int j = 0; match && j < div; j++) {
				if (getBit(bs, j) != getBit(bs, div * i + j)) {
					match = 0;
				}
			}
		}
	}
	vector<unsigned char> pat = initArray(div);
	for (int i = 0; i < div; i++) {
		if (getBit(bs, i)) {
			setBit(pat, i);
		}
	}
	return pat;
}


//vector<unsigned char> root(int n, vector<unsigned char> bs);
vector<unsigned char> root(int n, vector<unsigned char> bs) {
	vector<unsigned char> lnk = copyArray(n, bs);
	vector<unsigned char> rt = initArray(n);
	bool match = 0;
	vector<vector<unsigned char>> treecycle;
	while (!match) {
		treecycle.push_back(lnk);
		lnk = link(n, lnk);
		for (int i = 0; !match && i < treecycle.size(); i++) {
			if (blEqual(n, lnk, treecycle[i])) {
				match = 1;
				rt = lnk;
			}
		}
	}
	return rt;
}


vector<vector<unsigned char>> powersetMinValRots(int n) {
	int cardinality = pow2(n);
	vector<vector<unsigned char>> powerset(cardinality, vector<unsigned char>((n + 7) / 8));
	for (int i = 0; i < cardinality; i++) {
		powerset[i] = minValRot(n, decToBs(n, i));
	}
	vector<vector<unsigned char>> powersetMVR;
	powersetMVR.push_back(powerset[0]);
	int index = 0;
	for (int i = 1; i < cardinality; i++) {
		int bs1 = bsToDec(n, powerset[i]);
		int bs2 = bsToDec(n, powersetMVR[index]);
		if (bs1 > bs2) {
			powersetMVR.push_back(powerset[i]);
			index++;
		}
	}
	return powersetMVR;
}


vector<unsigned char> chain(int n, vector<unsigned char> bs, int& cycleOrder) {
	// returns a single bitstring representing the whole chain
	//
	// like root(), but returns the minimum value root in the respective cycle
	// roots (which are bitloops) are each represented by their MVR
	// so it is the MVRs of these roots that are compared
	// 
	// initialize cycleOrder as "int cycleOrder;"
	// and then "...chain(n, bs, &cycleOrder)."
	// *cycleOrder is a pointer to the value declared in chains()
	vector<unsigned char> lnk = bs;
	bool match = 0;
	// treecycle is a series of successive links
	// it ends with the full cycle in the chain
	// it may start in a tree that leads into the cycle
	// that happens when the first bitloop is not a root
	vector<vector<unsigned char>> treecycle;
	while (!match) {
		treecycle.push_back(lnk);
		lnk = link(n, lnk);
		for (int i = 0; !match && i < treecycle.size(); i++) {
			if (blEqual(n, lnk, treecycle[i])) {
				match = 1;
				cycleOrder = treecycle.size() - i;
			}
		}
	}
	// check every node in the cycle, find the lowest value
	int minVal = (1 << n);
	for (int i = 0; i < cycleOrder; i++) {
		int val = bsToDec(n, minValRot(n, lnk));
		if (val < minVal) {
			minVal = val;
		}
		lnk = link(n, lnk);
	}
	return decToBs(n, minVal);
}


vector<vector<unsigned char>> chains(int n) {
	// returns every chain that partitions P_n
	// each chain is represented by the node in its cycle
	// which has the lowest min val rotation of any node in the cycle
	vector<vector<unsigned char>> chs;

	// get list of bitloops
	// each bitloop is represented by the MVR of its rotation class
	vector<vector<unsigned char>> psmvr = powersetMinValRots(n);
	for (int i = 0; i < psmvr.size(); i++) {
	}
	// create another array with bools
	// this indicates if the element has already been worked,
	// that is, its respective chain was already added to chs
	vector<unsigned char> psCheckbox = initArray(pow2(n));
	/*
	* maybe this for loop could be done through multithreading
	* all threads would reference the same 2 arrays
	* - psmvr, psmvrCheckbox
	* its possible some elements would be worked multiple times
	* this becomes more of an issue as the cycle order increases
	*/
	for (int i = 0; i < psmvr.size(); i++) {
		// skip element if it has already been checked
		// this needs to index by decimal value of piomvr_i
		if (getBit(psCheckbox, bsToDec(n, psmvr[i]))) {
			continue;
		}
		// identify respective chain, add to chs
		int cycleOrder;
		vector<unsigned char> ch = chain(n, psmvr[i], cycleOrder);
		chs.push_back(ch);
		// identify all bitloops in the chain (represented by MVR)
		vector<vector<unsigned char>> chNodes = chainNodes(n, ch, cycleOrder);
		// check them all off
		for (int j = 0; j < chNodes.size(); j++) {
			// get index to check off
			// that is the decimal value of the chain node's MVR
			// psmvr and treeNodes store only MVRs, so they may be compared
			int k = bsToDec(n, chNodes[j]);
			setBit(psCheckbox, k);
		}
	}
	// now all the MVRs in the powerset are checked off
	return chs;
}


vector<vector<unsigned char>> chainNodes(int n, vector<unsigned char> ch, int cycleOrder) {
	// returns an array of every node in the chain
	vector<vector<unsigned char>> chNodes;
	vector<unsigned char> lnk = link(n, ch);
	for (int i = 0; i < cycleOrder; i++) {
		vector<vector<unsigned char>> trNodes = treeNodes(n, lnk);
		for (int j = 0; j < trNodes.size(); j++) {
			chNodes.push_back(trNodes[j]);
		}
		lnk = link(n, lnk);
	}
	return chNodes;
}


vector<vector<unsigned char>> treeNodes(int n, vector<unsigned char> rt) {
	// returns an array of every node in the tree
	vector<vector<unsigned char>> trNodes;
	trNodes.push_back(minValRot(n, rt));
	vector<unsigned char> sublnk = sublinkOfRootOffCycle(n, rt);
	makeTree(n, trNodes, sublnk);
	return trNodes;
}


void makeTree(int n, vector<vector<unsigned char>>& tns, vector<unsigned char> sublnk) {
	// treeNodes helper function, recursive
	tns.push_back(minValRot(n, sublnk));
	sublnk = sublink(n, sublnk);
	if (!sublnk.empty()) {
		vector<unsigned char> inv = minValRot(n, inverse(n, sublnk));
		makeTree(n, tns, sublnk);
		if (!blEqual(n, sublnk, inv)) {
			makeTree(n, tns, inv);
		}
	}
}


vector<unsigned char> sublinkOfRootOffCycle(int n, vector<unsigned char> rt) {
	// the sublink of a given root that is not also a root
	// first take the root's two sublinks
	// then for each sublink, take successive sublinks until
	// sublink() returns {}, i.e. result.empty() == true
	// whichever leads to {} first is the off-cycle sublink
	vector<unsigned char> treeNode1 = sublink(n, rt);
	vector<unsigned char> treeNode2 = inverse(n, treeNode1);
	while (!treeNode1.empty() && !treeNode2.empty()) {
		treeNode1 = sublink(n, treeNode1);
		treeNode2 = sublink(n, treeNode2);
	}
	return treeNode1.empty() ? sublink(n, rt) : inverse(n, sublink(n, rt));
}
























































