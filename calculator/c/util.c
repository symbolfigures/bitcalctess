#include "util.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// BITS ----------------------------------------------------------


void setBit(unsigned char* array, int index) {
	// make bit = 1
	array[index / 8] |= (1 << (index % 8));
}

void clearBit(unsigned char* array, int index) {
	// make bit = 0
	array[index / 8] &= ~(1 << (index % 8));
}

void xorBit(unsigned char* array, int index) {
	// flip 0 to 1 or 1 to 0
	array[index / 8] ^= (1 << (index % 8));
}

bool getBit(unsigned char* array, int index) {
	// get a bit from the array
	return (array[index / 8] >> (index % 8)) & 1;
}


// ARRAYS ----------------------------------------------------------


unsigned char* initArray(int n) {
	// make an array
	// round up to full byte
	int num_bytes = (n + 7) / 8;  // a.k.a. buffer size
	unsigned char* array = malloc(num_bytes);
	if (array == NULL) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	// initialize bit array to all 0s
	memset(array, 0, num_bytes);
	return array;
}

unsigned char* copyArray(int n, unsigned char* array) {
	// copy array to new array with different memory
	unsigned char* newArray = initArray(n);
	for (int i = 0; i < n; i++) {
		if (getBit(array, i)) {
			setBit(newArray, i);
		}
	}
	return newArray;
}

void printArray(int n, unsigned char* array) {
	// print array
	if (array == NULL) {
		printf("NULL\n");
		return;
	}
	printf("[");
	for (int i = 0; i < n; i++) {
		printf(" %d", getBit(array, i));
	}
	printf(" ]\n");
}


// CONVERSION ----------------------------------------------------------


unsigned char* charsToBitArray(int n, char ca[]) {
	// convert char array to bit array
	unsigned char* newArray = initArray(n);
	for (int i = 0; i < n; i++) {
		if (ca[i] == '1') {
			setBit(newArray, i);
		}
	}
	return newArray;
}

unsigned char* decToBs(int n, int dec) {
	// convert decimal to bitstring
	unsigned char* bs = initArray(n);
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

int bsToDec(int n, unsigned char* bs) {
	// convert bitstring to decimal
	int dec = 0;
	for (int i = 0; i < n; i++) {
		dec += getBit(bs, n - 1 - i) * (1 << i);
	}
	return dec;
}


// ARRAY LIST ----------------------------------------------------------


ArrayList* createArrayList() {
	// create ArrayList
	// initialize as "ArrayList *list = createArrayList();"
	ArrayList *list = (ArrayList *)malloc(sizeof(ArrayList));
	if (!list) {
		perror("failed to create ArrayList");
		exit(EXIT_FAILURE);
	}
	list->size = 0;
	list->capacity = 1; // initial capacity is 1
	list->arrays = (unsigned char **)malloc(list->capacity * sizeof(unsigned char *));
	if (!list->arrays) {
		perror("failed to allocate memory for arrays");
		free(list);
		exit(EXIT_FAILURE);
	}
	return list;
}

void addArray(ArrayList *list, unsigned char *array) {
	// add unsigned char array to the ArrayList
	if (list->size >= list->capacity) { // resize as needed
		list->capacity *= 2;
		list->arrays = (unsigned char **)realloc(list->arrays, list->capacity * sizeof(unsigned char *));
		if (!list->arrays) {
			perror("failed to resize arrays");
			exit(EXIT_FAILURE);
		}
	}
	list->arrays[list->size++] = array; // add array and increment size
}

unsigned char* getArray(ArrayList *list, int index) {
	// return unsigned char array at index
	if (index >= list->size) {
		fprintf(stderr, "Index out of bounds\n");
		return NULL;
	}
	return list->arrays[index];
}

void trimArrayList(ArrayList *list, int index) {
	// trim array list.
	// if index is positive, trim elements from list[start] to list[index]
	// if index is negative, trim elements from list[end] to list[index]
	if (list->size == 0) {
		return;
	}
	int start, end;
	if (index >= 0) {
		if (index >= list->size) {
			printf("positive index was too high. nothing was trimmed\n");
			return;
		}
		// if index is last element, then the whole list is trimmed
		start = index + 1;
		end = list->size - 1;
	} else {
		index = list->size - 1 + index;
		if (index < 0 || index >= list->size) {
			printf("negative index was too low. nothing was trimmed\n");
			return;
		}
		start = 0;
		end = index;
	}
	int newSize = end - start + 1;
	unsigned char **newArrays = (unsigned char**)malloc(newSize * sizeof(unsigned char *));
	for (int i = 0; i < newSize; i++) {
		newArrays[i] = list->arrays[start + i];
	}
	free(list->arrays);
	list->arrays = newArrays;
	list->size = newSize;
	list->capacity = newSize;
}

unsigned char** arrayListToArray(ArrayList *list) {
	unsigned char **array = malloc((list->size + 1) * sizeof(unsigned char *));
	if (array == NULL) {
		return NULL;
	}
	for (int i = 0; i < list->size; i++) {
		array[i] = list->arrays[i];
	}
	array[list->size] = NULL; // includes NULL at the end to measure its size
	return array;
}

void freeArrayList(ArrayList*  list) {
	// free ArrayList
	// given an array is initialized and then added to the ArrayList,
	// freeArrayList frees that memory which was allocated for the array
	// when the array was initialized
	for (int i = 0; i < list->size; i++) {
		free(list->arrays[i]); // each array
	}
	free(list->arrays); // array of pointers
	free(list); // ArrayList structure
	list = NULL;
}


// MATH ----------------------------------------------------------


int pow2(int n) {
	int ans = 1;
	for (int i = 0; i < n; ++i) {
		ans *= 2;
	}
	return(ans);
}






















