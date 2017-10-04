package jvmti;

import java.util.ArrayList;
import java.util.List;

public class EarlyReturn {

	List<Integer> list = new ArrayList<>();

	public static void main(String[] args) {
		System.out.println("EarlyReturn.main start");
		EarlyReturn return1 = new EarlyReturn();
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
		for (; i < 30; i++) {
			list.add(i);
			Thread.sleep(100);
		}
		return i;
	}

}
