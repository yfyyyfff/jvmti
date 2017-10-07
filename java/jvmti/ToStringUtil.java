package jvmti;

import java.util.Arrays;

public class ToStringUtil {

	public static String array2String(Object paramVal) {
		if (paramVal != null && paramVal.getClass().isArray()) {
			if (paramVal instanceof boolean[])
				return Arrays.toString((boolean[]) paramVal);
			else if (paramVal instanceof byte[])
				return Arrays.toString((byte[]) paramVal);
			else if (paramVal instanceof char[])
				return Arrays.toString((char[]) paramVal);
			else if (paramVal instanceof short[])
				return Arrays.toString((short[]) paramVal);
			else if (paramVal instanceof int[])
				return Arrays.toString((int[]) paramVal);
			else if (paramVal instanceof long[])
				return Arrays.toString((long[]) paramVal);
			else if (paramVal instanceof float[])
				return Arrays.toString((float[]) paramVal);
			else if (paramVal instanceof double[])
				return Arrays.toString((double[]) paramVal);
			else
				return Arrays.toString((Object[]) paramVal);
		}
		return String.valueOf(paramVal);
	}
}
