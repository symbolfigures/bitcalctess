#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "calc.h"
#include "util.h"
using namespace std;


int main1() {

	int n = 10;
	vector<vector<unsigned char>> chs = chains(n);

	ofstream file;
	file.open ("output/" + to_string(n) + ".txt");
	file << "n: " << n << "\n";
	for (int i = 0; i < chs.size(); i++) {
		file << arrayToString(n, chs[i]);
	}
	file.close();

	return 0;
}

int main() {

	printf("\nEnter a bitstring: ");

	// load bit string into temp
	// n = number of bits
	// copy to ca[], which has n elements  
	char temp[31];
	int n = 0;
	int ch;
	while ((ch = getchar()) != '\n' && n < 30) {
		temp[n++] = ch;
	}
	vector<unsigned char> ca(temp, temp + 1 + 1);
	for (int i = 0; i <= n; i++) {
		ca[i] = temp[i];
	}

	vector<unsigned char> bs = charsToBitArray(n, ca);
	vector<unsigned char> bscpy = bs;

	printf("\nBitstring: ");
	printArray(n, bs);

	printf("\nProperties of the bitstring:\n");
	printf("# of bits: %d\n", n);
	printf("Parity: ");
	bool p = parity(n, bs);
	if (p) {
		printf("Odd\n");
	} else {
		printf("Even\n");
	}
	printf("Inverse: ");
	printArray(n, inverse(n, bs));
	bscpy = inverse(n, bs); // reset bs
	printf("Reverse: ");
	printArray(n, reverse(n, bs));
	printf("Pattern: ");
	printArray(n, pattern(n, bs));

	printf("\nProperties of the bitloop to which the bitstring belongs:");
	printf("\nSymmetry: ");
	bool s = isSymmetric(n, bs);
	if (s) {
		printf("Symmetric\n");
	} else {
		printf("Asymmetric\n");
	}
	printf("Link: ");
	printArray(n, link(n, bs));
	printf("Sublink: ");
	printArray(n, sublink(n, bs));
	minValRot(n, bs);
	printf("Min value rotation: ");
	printArray(n, bs);

	printf("\nProperties of the tree to which the bitloop belongs:");
	printf("\nRoot: ");
	printArray(n, root(n, bs));

	printf("\nProperties of the power set to which the chain belongs:");
	printf("\nSee \"output/%d.txt\"", n);
	printf("\n\n");

	vector<vector<unsigned char>> chs = chains(n);
	ofstream file;
	file.open ("output/" + to_string(n) + ".txt");
	file << "Chains in the powerset of n = "<< n << ":\n";
	for (int i = 0; i < chs.size(); i++) {
		file << arrayToString(n, chs[i]);
	}
	file.close();

	return 0;

}
















