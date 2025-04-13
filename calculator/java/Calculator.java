import java.util.ArrayList;
import java.util.Arrays;
import java.util.PriorityQueue;

abstract class Calculator {

	boolean bsEquals(int[] a1, int[] a2) {
		boolean eqLength = a1.length == a2.length;
		if (eqLength) {
			for (int i = 0; i < a1.length; i++) {
				if (a1[i] != a2[i]) {
					return false;
				}
			}
		}
		return true;
	}

	boolean blEquals(int[] a1, int[] a2) {
		boolean eqLength = a1.length == a2.length;
		if (eqLength) {
			for (int i = 0; i < a1.length; i++) {
				if (bsEquals(rotation(a1, i), a2)) {
					return true;
				}
			}
		}
		return false;
	}

	int[] decToBs(int n, int decimal) {
		int[] bitstring = new int[n];
		for (int i = 0; i < n; i++) {
			int powOf2 = (int) Math.pow(2, n - 1 - i);
			if (decimal >= powOf2) {
				decimal -= powOf2;
				bitstring[i] = 1;
			} else {
				bitstring[i] = 0;
			}
		}
		return bitstring;
	}

	int bsToDec(int[] bitstring) {
		int n = bitstring.length;
		int decimal = 0;
		for (int i = 0; i < n; i++)
			decimal += bitstring[n - 1 - i] * Math.pow(2, i);
		return decimal;
	}	

	boolean parity(int[] input) {
		int i = 0;
		for (int item : input) {
			i += item;
		}
		return (i % 2 == 0);
	}

	boolean symmetric(int[] input) {
		int[] reverse = reverse(input);
		return (blEquals(input, reverse));
	}

	boolean nonInvertible(int[] input) {
		return blEquals(input, inverse(input));
	}

	boolean cycleMember(int[] input) {
		int numBlCycle = numBlCycle(input);
		int[] link = input;
		for (int i = 0; i < numBlCycle; i++) {
			link = link(link);
		}
		return (blEquals(link, input));
	}

	int[] factor(int[] input) {
		int n = input.length;
		int nb = 0;
		boolean match = false;
		while (!match) {
			while (n % ++nb != 0) {}
			match = true;
			for (int i = 0; match && i < n / nb; i++) {
				for (int j = 0; match && j < nb; j++) {
					if (input[j] != input[nb * i + j]) {
						match = false;
					}
				}
			}
		}
		int[] fac = new int[nb];
		for (int i = 0; i < nb; i++) {
			fac[i] = input[i];
		}
		return fac;
	}

	int[] inverse(int[] input) {
		int n = input.length;
		int[] inverse = new int[n];
		for (int i = 0; i < n; i++) {
			inverse[i] = (input[i] + 1) % 2;
		}
		return inverse;
	}

	int[] reverse(int[] input) {
		int n = input.length;
		int[] reverse = new int[n];
		for (int i = 0; i < n; i++) {
			reverse[i] = input[n - 1 - i];
		}
		return reverse;
	}

	int[] link(int[] input) {
		int n = input.length;
		int[] link = new int[n];
		for (int i = 0; i < n - 1; i++) {	
			link[i] = (input[i] + input[i + 1]) % 2;
		}
		link[n - 1] = (input[n - 1] + input[0]) % 2;
		return link;
	}

	int[] sublink(int[] input) {
		int n = input.length;
		if (parity(input)) {
			int[] sublink = new int[n];
			sublink[0] = 0;
			for (int i = 0; i < n - 1; i++) {
				sublink[i + 1] = (sublink[i] + input[i]) % 2;
			}
			return sublink;
		} else {
			return null;
		}
	}

	int[] sublinkOfRoot(int[] input) {
		int[] sublink = sublink(input);
		return (cycleMember(sublink)) ? inverse(sublink) : sublink ;
	}

	int[] rotation(int[] input, int angle) {
		int n = input.length;
		int[] rotation = new int[n];
		for (int i = 0; i < n; i++) {
			rotation[((angle + i) % n)] = input[i];
		}
		return rotation;
	}

	int[][] rotations(int[] input) {
		int n = input.length;
		int[] fac = factor(input);
		int nb = fac.length;
		int[][] rotations = new int[nb][n];
		for (int j = 0; j < nb; j++) {
			for (int i = 0; i < n; i++) {
				rotations[j][((i + j) % n)] = input[i];
			}
		}
		return rotations;
	}
	
	// minimum value rotation
	int[] mvr(int[] input) {
		int minVal = -1;
		int angle = 0;
		int[][] rotations = rotations(input);
		for(int i = 0; i < rotations.length; i++) {
			int val = bsToDec(rotations[i]);
			if(minVal == -1) minVal = val;
			if(val < minVal) {
				minVal = val;
				angle = i;
			}
		}
		return rotation(input, angle);
	}

	int[] root(int[] sample) {
		int n = sample.length;
		int[] link = sample;
		int[] root = new int[n];
		boolean cycle = false;
		ArrayList<int[]> chain = new ArrayList<>();
		while (!cycle) {
			chain.add(link);
			link = link(link);
			for (int i = 0; !cycle && i < chain.size(); i++) {
				if (blEquals(link, chain.get(i))) {
					cycle = true;
					root = link;
				}
			}
		}
		chain.clear();
		return root;
	}

	int[] signature(int[] sample) {
		int n = sample.length;
		int numLevelsTree = numLevelsTree(sample);
		int[] signature = new int[numLevelsTree + 1];
		signature[0] = 2;
		signature[numLevelsTree] = 0;
		int[] pointer = sublinkOfRoot(root(sample));
		for (int i = 1; i < numLevelsTree; i++) {
			if (nonInvertible(sublink(pointer))) {
				signature[i] = 1;
			} else {
				signature[i] = 2;
			}
			pointer = sublink(pointer);
		}
		return signature;
	}

	int numBlCycle(int[] sample) {
		int n = sample.length;
		int[] root = root(sample);
		int[] link = root;
		int numBlCycle = 0;
		boolean cycle = false;
		while (!cycle) {
			link = link(link);
			numBlCycle++;
			if (blEquals(root, link)) {
				cycle = true;
			}
		}
		return numBlCycle;
	}

	int numLevelsTree(int[] sample) {
		int[] root = root(sample);
		int[] sublink = inverse(root);
		int numLevelsTree = 0;
		while (sublink != null) {
			numLevelsTree++;
			sublink = sublink(sublink);
		}
		return numLevelsTree;
	}

	int numBlTree(int[] sample) {
		int n = sample.length;
		int[] root = root(sample);
		int[] pointer = inverse(root);
		int numLevelsTree = numLevelsTree(root);
		int numBlLevel = 1;	
		int numBlTree = numBlLevel;
		int[] sublink, inverse, rotation;
		for (int level = 1; level < numLevelsTree; level++) {
			numBlTree += numBlLevel;
			sublink = sublink(pointer);
			inverse = inverse(sublink);
			if (!blEquals(sublink, inverse)) {
				numBlLevel *= 2;
			}
			pointer = sublink(pointer);
		}
		numBlTree += numBlLevel;
		return numBlTree;
	}

	int numBlChain(int[] sample) {
		return numBlTree(sample) * numBlCycle(sample);
	}

	int numBsTree(int[] sample) {
		int n = sample.length;
		int[] root = root(sample);
		int[] pointer = inverse(root);
		int numLevelsTree = numLevelsTree(root);
		int numBlLevel = 1;	
		int numBsTree = factor(pointer).length;
		int[] sublink, inverse, rotation;
		for (int level = 1; level < numLevelsTree; level++) {
			numBsTree += factor(pointer).length * numBlLevel;
			sublink = sublink(pointer);
			inverse = inverse(sublink);
			if (!blEquals(sublink, inverse)) {
				numBlLevel *= 2;
			}
			pointer = sublink(pointer);
		}
		numBsTree += factor(pointer).length * numBlLevel;
		return numBsTree;
	}

	int numBsChain(int[] sample) {
		return numBsTree(sample) * numBlCycle(sample);
	}

	int numFacsTree(int[] sample) {
		int n = sample.length;
		int numFacsTree = numBlTree(sample);
		int[] signature = signature(sample);
		int numLevelsTree = signature.length - 1;
		int numBlLevel = 1;
		int level = 0;
		int[] pointer = root(sample);
		if (factor(pointer).length == n) {
			return numFacsTree;	
		}
		numFacsTree -= numBlLevel;
		level++;
		pointer = sublinkOfRoot(pointer);
		if (factor(pointer).length == n) {
			return numFacsTree;	
		}
		numFacsTree -= numBlLevel;
		while (level < numLevelsTree) {
			level++;
			if (signature[level - 1] == 2) {
				numBlLevel *= 2;
			}
			pointer = sublink(pointer);
			if (factor(pointer).length == n) {
				return numFacsTree;	
			}
			numFacsTree -= numBlLevel;
		}
		return numFacsTree;
	}

	int numFacsChain(int[] sample) {
		int numBlCycle = numBlCycle(sample);
		return numFacsTree(sample) * numBlCycle;
	}

	ArrayList<int[]> treeNodes(int[] root) {
		ArrayList<int[]> tns = new ArrayList<>();
		tns.add(root);
		int[] sor = sublinkOfRoot(root);
		makeTree(sor, tns);
		return tns;
	}
	void makeTree(int[] root, ArrayList<int[]> tns) {
		tns.add(root);
		int[] sub = sublink(root);
		if(sub == null) return;
		int[] inv = inverse(sub);
		makeTree(sub, tns);
		if(!blEquals(sub, inv)) makeTree(inv, tns);
	}	

	// powerset index of minimum value rotation
	int[] piomvr(int n) {
		int psSize = (int) Math.pow(2, n);
		boolean[] ps = new boolean[psSize];
		int psFill = 0;
		int numBl = 0;
		int psx = 0;
		while(psFill < psSize) {
			if(!ps[psx]){
				int[] bitstring = decToBs(n, psx);
				int facLen = factor(bitstring).length;
				psFill += facLen;
				numBl++;
				for(int agl = 1; agl < facLen; agl++) {
					bitstring = rotation(bitstring, 1);
					int decimal = bsToDec(bitstring);
					ps[decimal] = true;
				}
			}
			psx++;
		}
		int[] indices = new int[numBl];
		int idx = 0;
		for(psx = 0; psx < psSize; psx++) {
			if(!ps[psx]) {
				indices[idx] = psx;
				idx++;
			}
		}
		return indices;
	}	

	// chainReps v2
	ArrayList<int[]> chainReps(int n, int[] piomvr) {
		boolean[] piomx = new boolean[piomvr.length];
		for(int i = 0; i < piomx.length; i++) {
			// for each bitloop that hasn't been marked
			if(!piomx[i]) {
				int mvcn = Integer.MAX_VALUE;
				// get root of the bitloop
				int[] root = root(decToBs(n, piomvr[i]));
				int nbc = numBlCycle(root);
				// for every tree in the chain
				for(int j = 0; j < nbc; j++) {
					root = link(root);
					ArrayList<int[]> tns = treeNodes(root);
					// for every tree node in the tree
					for(int k = 0; k < tns.size(); k++) {
						int[] tn = tns.get(k);
						int mvr = bsToDec(mvr(tn));
						// find it in piomvr
						int x = Arrays.binarySearch(piomvr, mvr);
						// mark it off, and its reverse
						piomx[x] = true;
						if(mvr < mvcn) mvcn = mvr;
						if(!symmetric(tn)) {
							mvr = bsToDec(mvr(reverse(tn)));
							x = Arrays.binarySearch(piomvr, mvr);
							piomx[x] = true;
						}
					}
				}
				// un-mark the chain node with the minimum value
				int x = Arrays.binarySearch(piomvr, mvcn);
				piomx[x] = false;
			}
		}
		// assemble un-marked chain nodes as chainReps
		ArrayList<int[]> cr = new ArrayList<>();
		for(int i = 0; i < piomx.length; i++) {
			if(!piomx[i]) cr.add(decToBs(n, piomvr[i]));
		}
		return cr;
	}
/*
	// chainReps v1
	ArrayList<int[]> chainReps(int n) {
		int psFill = 0;
		int[] sample = new int[n];
		int psSize = (int) Math.pow(2, n);
		ArrayList<int[]> chainReps = new ArrayList<>();
		int dec = 0;
		while (psFill < psSize) {
			sample = nextBlPs(n, dec);
			dec = bsToDec(sample) + 1;
			int[] root = root(sample);
			boolean sym = symmetric(root);
			boolean match = false;
			for (int i = 0; !match && i < chainReps.size(); i++) {
				int[] link = chainReps.get(i);
				int numBlCycle = numBlCycle(link);
				for (int j = 0; !match && j < numBlCycle; j++) {
					if (blEquals(root, link)) { 
						match = true;
					}
					if (!sym && blEquals(reverse(root), link)) {
						match = true;
					}
					link = link(link);
				}
			}
			if (!match) {
				chainReps.add(mvr(root));
				if (sym) {	
					psFill += numBsChain(sample);
				} else {
					psFill += numBsChain(sample) * 2;
				}
			}
		}
		return chainReps;
	}
	int[] nextBlPs(int n, int dec) {
		int psSize = (int) Math.pow(2, n);
		int[] bs = new int[n];
		while(dec <= psSize) {
			bs = decToBs(n, dec);
			if (bsEquals(bs, mvr(bs))) {
				break;
			}
			dec++;
		}
		return bs;
	}
*/
}


















