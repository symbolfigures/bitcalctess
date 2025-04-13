import java.util.Scanner;

public class I {

	public static void main(String[] args) {

		Scanner sc = new Scanner(System.in);
		System.out.println("Number of bits?");
		int n = sc.nextInt();
		Powerset ps = new Powerset(n);
		ps.print();

	}

}
