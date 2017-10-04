package jvmti;

import java.util.ArrayList;
import java.util.List;

public class EarlyReturn {
	

	List<Integer> list = new ArrayList<>();

	public static void main(String[] args) {
		System.out.println("EarlyReturn.main start");
		EarlyReturn return1 = new EarlyReturn();
		Integer int0 = new Integer(0x55);
		Integer int1 = new Integer(0x55);
		int0 = Integer.valueOf(0x55);
		int1 = Integer.valueOf(0x55);
		System.out.println("int0 == int1?  " + (int0 == int1));
		int r = -1;
		try {
			r = return1.earlyInt(return1.list);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
		System.out.println(return1.list);
		System.out.println("EarlyReturn.main end, r=" + r);

	}

	int earlyInt(List<Integer> list) throws InterruptedException {
		int i = 0;
		String string = "ababa";
		String s2 = new String(string);
		for (; i < 3; i++) {
			list.add(i);
			Thread.sleep(100);
		}
		return i;
	}

}
