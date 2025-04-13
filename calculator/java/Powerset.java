import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;

public class Powerset extends Calculator {

	int n;
	ArrayList<int[]> chainReps = new ArrayList<int[]>();
	ArrayList<int[]> symChainReps = new ArrayList<int[]>();
	ArrayList<int[]> asymChainReps = new ArrayList<int[]>();
	ArrayList<int[]> graphReps = new ArrayList<int[]>();
	StringBuilder sb1 = new StringBuilder();
	StringBuilder sb2 = new StringBuilder();

	Powerset (int n) {
		this.n = n;
		chainReps = chainReps(n, piomvr(n));	// chainReps v2
//		chainReps = chainReps(n);				// chainReps v1
		partitionChainReps();
		graphReps();
	}

	void partitionChainReps() {
		for (int[] item : chainReps) {
			if (symmetric(item)) {
				symChainReps.add(item);
			} else {
				asymChainReps.add(item);
			}
		}
	}

	void graphReps() {
		for (int i = 0; i < chainReps.size(); i++) {
			boolean match = false;
			int[] chainRep = chainReps.get(i);
			for (int j = 0; !match && j < graphReps.size(); j++) {
				int[] graphRep = graphReps.get(j);
				if (numBlCycle(chainRep) == numBlCycle(graphRep) &&
				(bsEquals(signature(chainRep), signature(graphRep)))) {
					match = true;
				}
			}
			if (!match) {
				graphReps.add(chainRep);
			}
		}
	}

	int numBsPowerset() {
		return (int) Math.pow(2.0d, n);
	}

	int numBlPowerset() {
		int numBlPowerset = 0;
		int[] chainRep;
		for (int i = 0; i < chainReps.size(); i++) {
			if (symmetric(chainReps.get(i))) {
				numBlPowerset += numBlChain(chainReps.get(i));
			} else {
				numBlPowerset += numBlChain(chainReps.get(i)) * 2;
			}			
		}
		return numBlPowerset;
	}

	int numFacsPowerset() {
		int numFacsPowerset = 0;
		int[] chainRep;
		for (int i = 0; i < chainReps.size(); i++) {
			if (symmetric(chainReps.get(i))) {
				numFacsPowerset += numFacsChain(chainReps.get(i));
			} else {
				numFacsPowerset += numFacsChain(chainReps.get(i)) * 2;
			}			
		}
		return numFacsPowerset;
	}

	int[] numChainsGraph(int[] sample) {
		int[] numChainsGraph = new int[2];
		for (int i = 0; i < chainReps.size(); i++) {
			if (numBlCycle(chainReps.get(i)) == numBlCycle(sample) &&
			(bsEquals(signature(chainReps.get(i)), signature(sample)))) {
				if (symmetric(chainReps.get(i))) {
					numChainsGraph[0]++;
				} else {
					numChainsGraph[1]++;
				}
			}
		}
		return numChainsGraph;
	}

	void print() {
		try {
			BufferedWriter writer = new BufferedWriter(
				new FileWriter(String.format("output/p%d.csv", n)));
			writer.append(String.format("P(%d)", n)+"\n");
			writer.append(printPowerset()+"\n");
			writer.append("Graphs\n");
			writer.append(printGraphs()+"\n");
			writer.append("Symmetric chains\n");
			writer.append(printChainReps(symChainReps)+"\n");
			writer.append("Asymmetric chains\n");
			writer.append(printChainReps(asymChainReps)+"\n");
			writer.close();
		} catch (IOException e) { e.printStackTrace(); }
	}
	
	String printPowerset() {
		sb1.append("bitstrings,bitloops,factors,sym-chains,asym-chains,graphs\n");
		sb1.append(
			String.format(
				"%d,%d,%d,%d,%d,%d",
				numBsPowerset(),
				numBlPowerset(),
				numFacsPowerset(),
				symChainReps.size(),
				asymChainReps.size(),
				graphReps.size()
			)+"\n"
		);
		String s = sb1.toString();
		sb1.delete(0, sb1.length());
		return s;
	}

	String printGraphs() {
		sb1.append("name,sym-chains,asym-chains,bitloops,cycle-order,tree-depth,signature\n");
		for (int i = 0; i < graphReps.size(); i++) {
			int[] item = graphReps.get(i);
			int[] numChainsGraph = numChainsGraph(item);
			sb1.append(
				printName(item)+","+ String.format(
					"%d,%d,%d,%d,%d,%s",
					numChainsGraph[0],
					numChainsGraph[1],
					numBlChain(item),
					numBlCycle(item),
					numLevelsTree(item),
					printSignature(item)
				)+"\n"
			);
		}
		String s = sb1.toString();
		sb1.delete(0, sb1.length());
		return s;
	}

	String printChainReps(ArrayList<int[]> chainReps) {
//		if (chainReps.size() == 0) return "-";
		sb1.append("sample,bitstrings,bitloops,factors,cycle-order,tree-depth,signature\n");
		for (int i = 0; i < chainReps.size(); i++) {
			int[] item = chainReps.get(i);
			sb1.append(
				printSample(item)+","+String.format(
					"%d,%d,%d,%d,%d,%s",
					numBsChain(item),
					numBlChain(item),
					numFacsChain(item),
					numBlCycle(item),
					numLevelsTree(item),
					printSignature(item)
				)+"\n"
			);
		}
		String s = sb1.toString();
		sb1.delete(0, sb1.length());
		return s;
	}

	String printSample(int[] sample) {
		for (int item : sample) sb2.append(String.format("%d", item));
		String s = sb2.toString();
		sb2.delete(0, sb2.length());
		return s;
	}

	String printSignature(int[] sample) {
		int[] signature = signature(sample);
		for (int item : signature) sb2.append(String.format("%d", item));
		String s = sb2.toString();
		sb2.delete(0, sb2.length());
		return s;
	}

	String printName(int[] sample) {
		sb2.append(String.format("G%d-%s", numBlCycle(sample), printSignature(sample)));
		String s = sb2.toString();
		sb2.delete(0, sb2.length());
		return s;
	}

}

























		






















