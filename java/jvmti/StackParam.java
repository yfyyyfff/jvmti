package jvmti;

import java.util.ArrayList;
import java.util.List;

public class StackParam {

	private void throwException(int a) {
		throw new StackParamException();
	}

	private void occurException(int a, List<Integer> list) {
		int size = list.size();
		if (a < 0) {
			throwException(a);
		}
	}

	public static void main(String[] args) {
		StackParam paramStack = new StackParam();
		List<Integer> ls = new ArrayList<>();
		ls.add(555);
		ls.add(666);
		paramStack.occurException(-5, ls);
	}

}
