package jvmti;

public class TestValueOf {

	public static void main(String[] args) {
		Integer v1 = Integer.valueOf(55);
		Integer v2 = Integer.valueOf(55);
		//bacause of Integer cache, v1 and v2 are the same object
		System.out.println("v1==v2 ? " + (v1 == v2));
	}

}
