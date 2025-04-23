#include "calc.h"
#include "util.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>


bool bsEqual(int n, unsigned char* a, unsigned char* b) {
	// are bitstrings equal
    for (int i = 0; i < n; i++) {
		// compare whole bytes (chars) instead of bits
		if (a[i] != b[i]) {
			return false;
		}
	}
	return true;
}


bool blEqual(int n, unsigned char* a, unsigned char* b) {
	// are bitloops equal
	for (int t = 0; t < n; t++) {
		if (bsEqual(n, a, b)) {
			return true;
		}
		rotate1(n, b);
	}
	return false;
}


void rotate1(int n, unsigned char* bs) {
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


void rotateT(int n, unsigned char* array, int t) {
	// rotate T bits around
	t = t % n;
	if (t < 0) {
		t += n;
	}
	unsigned char* temp = initArray(n);
	for (int i = 0; i < n; i++) {
		int j = (i + t) % n;
		if (array[i / 8] & (1 << (i % 8))) {
			temp[j / 8] |= (1 << (j % 8));
		}
	}
	for (int i = 0; i < (n + 7) / 8; i++) {
		array[i] = temp[i];
	}
	free(temp);
}


void minValRot(int n, unsigned char* bs) {
	int min = 1 << n;
	for (int i = 0; i < n; i++) {
		rotate1(n, bs);
		int dec = bsToDec(n, bs);
		if (dec < min) {
			min = dec;
		}
	}
	free(bs);
	bs = decToBs(n, min);
}


bool parity(int n, unsigned char* bs) {
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


unsigned char* inverse(int n, unsigned char* bs) {
	// flip 1s and 0s in a bitstring
	for (int i = 0; i < n; i++) {
		xorBit(bs, i);
	}
	return bs;
}


bool isInverse(int n, unsigned char* bs) {
	// is a bitloop equal to its inverse
	unsigned char* inv = copyArray(n, bs);
	inv = inverse(n, inv);
	bool b = blEqual(n, bs, inv);
	free(inv);
	return b;
}


unsigned char* reverse(int n, unsigned char* bs) {
	// returns bits in reverse order
	unsigned char* rev = initArray(n);
	for (int i = 0; i < n; i++) {
		if (getBit(bs, n - 1 - i)) {
			setBit(rev, i);
		}
	}
	return rev;
}


bool isReverse(int n, unsigned char* bs) {
	// is a bitstring equal to its reverse
	unsigned char* rev = copyArray(n, bs);
	rev = reverse(n, rev);
	bool b = bsEqual(n, bs, rev);
	free(rev);
	return b;
}


bool isSymmetric(int n, unsigned char* bs) {
	// is a bitloop equal to its reverse
	// does the circle of bits have an axis of symmetry
	unsigned char* rev = copyArray(n, bs);
	rev = reverse(n, rev);
	bool b = blEqual(n, bs, rev);
	free(rev);
	return b;
}


unsigned char* link(int n, unsigned char* bs) {
	// return the link of a given bitloop
	unsigned char* lnk = initArray(n);
	for (int i = 0; i < n; i++) {
		if (getBit(bs, i) ^ getBit(bs, (i + 1) % n)) {
			setBit(lnk, i);
		}
	}
	return lnk;
}


unsigned char* sublink(int n, unsigned char* bs) {
	// return the sublink of a given bitloop
	if (parity(n, bs)) {
		return NULL;
	}
	unsigned char* slink = initArray(n);
	//setBit(slink, 0);
	for (int i = 0; i < n - 1; i++) {
		if (getBit(slink, i) ^ getBit(bs, i)) {
			setBit(slink, i + 1);
		}
	}
	return slink;
}


unsigned char* pattern(int n, unsigned char* bs, int* nP) {
	// returns the pattern of a given bitloop
	// to pass in nP (number of bits the patern has),
	// first initialize it as "int nP;",
	// and then pass it in as "&nP"'
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
	unsigned char* pat = initArray(div);
	for (int i = 0; i < div; i++) {
		if (getBit(bs, i)) {
			setBit(pat, i);
		}
	}
	*nP = div;
	return pat;
}


unsigned char* root(int n, unsigned char* bs) {
	unsigned char* lnk = copyArray(n, bs);
	unsigned char* rt = initArray(n);
	bool match = 0;
	ArrayList *treecycle = createArrayList();
	while (!match) {
		addArray(treecycle, lnk);
		lnk = link(n, lnk);
		for (int i = 0; !match && i < treecycle->size; i++) {
			if (blEqual(n, lnk, getArray(treecycle, i))) {
				match = 1;
				rt = lnk;
			}
		}
	}
	freeArrayList(treecycle);
	return rt;
}


ArrayList* powersetMinValRots(int n) {
	int cardinality = pow2(n);
	unsigned char **powerset = (unsigned char **)malloc(cardinality * sizeof(unsigned char *));
	if (powerset == NULL) {
		fprintf(stderr, "memory allocation failed for powerset\n");
		return NULL;
	}
	for (int i = 0; i < cardinality; i++) {
		powerset[i] = decToBs(n, i);
		minValRot(n, powerset[i]);
	}
	ArrayList* powersetMVR = createArrayList();
	addArray(powersetMVR, powerset[0]);
	int index = 0;
	for (int i = 1; i < cardinality; i++) {
		int bs1 = bsToDec(n, powerset[i]);
		int bs2 = bsToDec(n, getArray(powersetMVR, index));
		if (bs1 > bs2) {
			addArray(powersetMVR, powerset[i]);
			index++;
		}
	}
	free(powerset);
	return powersetMVR;
}


unsigned char* chain(int n, unsigned char* bs, int* cycleOrder) {
	// returns a single bitstring representing the whole chain
	//
	// like root(), but returns the minimum value root in the respective cycle
	// roots (which are bitloops) are each represented by their MVR
	// so it is the MVRs of these roots that are compared
	// 
	// initialize cycleOrder as "int cycleOrder;"
	// and then "...chain(n, bs, &cycleOrder)."
	// *cycleOrder is a pointer to the value declared in chains()
	unsigned char* lnk = copyArray(n, bs);
	bool match = 0;
	// treecycle is a series of successive links
	// it ends with the full cycle in the chain
	// it may start in a tree that leads into the cycle
	// that happens when the first bitloop is not a root
	ArrayList *treecycle = createArrayList();
	while (!match) {
		addArray(treecycle, lnk);
		lnk = link(n, lnk);
		for (int i = 0; !match && i < treecycle->size; i++) {
			if (blEqual(n, lnk, getArray(treecycle, i))) {
				match = 1;
				*cycleOrder = treecycle->size - i;
			}
		}
	}
	// check every node in the cycle, find the lowest value
	int minVal = (1 << n);
	for (int i = 0; i < *cycleOrder; i++) {
		minValRot(n, lnk);
		int val = bsToDec(n, lnk);
		if (val < minVal) {
			minVal = val;
		}
		lnk = link(n, lnk);
	}
	freeArrayList(treecycle);
	return decToBs(n, minVal);
}


ArrayList* chains(int n) {
	// returns every chain that partitions P_n
	// each chain is represented by the node in its cycle
	// which has the lowest min val rotation of any node in the cycle
	if (n < 1) {
		return NULL;
	}
	ArrayList* chs = createArrayList();

	// get list of bitloops
	// each bitloop is represented by the MVR of its rotation class
	ArrayList* psmvr = powersetMinValRots(n);

	// create another array with bools
	// this indicates if the element has already been worked,
	// that is, its respective chain was already added to chs
	unsigned char* psCheckbox = initArray(pow2(n));
	/*
	* maybe this for loop be done through multithreading
	* all threads would reference the same 2 arrays
	* - psmvr, psmvrCheckbox
	* its possible some elements would be worked multiple times
	* this becomes more of an issue as the cycle order increases
	*/
	for (int i = 0; i < psmvr->size; i++) {
		// skip element if it has already been checked
		// this needs to index by decimal value of piomvr_i
		if (getBit(psCheckbox, bsToDec(n, getArray(psmvr, i)))) {
			continue;
		}
		// identify respective chain, add to chs
		int cycleOrder;
		unsigned char* ch = chain(n, getArray(psmvr, i), &cycleOrder);
		addArray(chs, ch);
		// identify all bitloops in the chain (represented by MVR)
		ArrayList* chNodes = chainNodes(n, ch, &cycleOrder);
		// check them all off
		for (int j = 0; j < chNodes->size; j++) {
			// get index to check off
			// that is the decimal value of the chain node's MVR
			// psmvr and treeNodes store only MVRs, so they may be compared
			int k = bsToDec(n, getArray(chNodes, j));
			setBit(psCheckbox, k);
			//if (k == 7) printf("  after set: %d, i: %d\n", getBit(psCheckbox, k), i);
		}
	}
	// now all the MVRs in the powerset are checked off
	return chs;
}


ArrayList* chainNodes(int n, unsigned char* ch, int* cycleOrder) {
	// returns an array of every node in the chain
	ArrayList* chNodes = createArrayList();
	unsigned char* lnk = link(n, copyArray(n, ch));
	for (int i = 0; i < *cycleOrder; i++) {
		ArrayList* trNodes = treeNodes(n, lnk);
		for (int j = 0; j < trNodes->size; j++) {
			addArray(chNodes, getArray(trNodes, j));
		}
		free(trNodes);
		lnk = link(n, lnk);
	}
	return chNodes;
}


ArrayList* treeNodes(int n, unsigned char* rt) {
	// returns an array of every node in the tree
	ArrayList* trNodes = createArrayList();
	minValRot(n, rt);
	addArray(trNodes, rt);
	unsigned char* sublnk = sublinkOfRootOffCycle(n, rt);
	makeTree(n, trNodes, sublnk);
	return trNodes;
}


void makeTree(int n, ArrayList* tns, unsigned char* sublnk) {
	// treeNodes helper function, recursive
	minValRot(n, sublnk);
	addArray(tns, sublnk);
	sublnk = sublink(n, sublnk);
	if(sublnk == NULL) {
		return;
	}
	unsigned char* inv = inverse(n, copyArray(n, sublnk));
	minValRot(n, inv);
	makeTree(n, tns, sublnk);
	if (!blEqual(n, sublnk, inv)) {
		makeTree(n, tns, inv);
	}
}


unsigned char* sublinkOfRootOffCycle(int n, unsigned char* rt) {
	// the sublink of a given root that is not also a root
	// first take the root's two sublinks
	// then for each sublink, take successive sublinks until NULL
	// whichever leads to NULL first is the off-cycle sublink
	unsigned char* treeNode1 = sublink(n, rt);
	unsigned char* treeNode2 = inverse(n, copyArray(n, treeNode1));
	while (treeNode1 != NULL && treeNode2 != NULL) {
		treeNode1 = sublink(n, treeNode1);
		treeNode2 = sublink(n, treeNode2);
	}
	free(treeNode2);
	return treeNode1 == NULL ? sublink(n, rt) : inverse(n, sublink(n, rt));
}






























