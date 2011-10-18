import java.util.*;
import java.math.*;
import java.io.*;

class modelo {
	static final double EPS = 1.e-10;
	static final boolean DBG = true;

	private static int cmp(double x, double y = 0, double tol = EPS) {
		return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
	}

	public static void main(String[] argv) {
		Scanner s = new Scanner(System.in);
	}
}
