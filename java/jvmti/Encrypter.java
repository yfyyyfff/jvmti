package jvmti;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class Encrypter {

	public static void main(String[] args) {
		File file = new File("bin/jvmti/Encryptee.class");
		String absPath = file.getAbsolutePath();
		if (!file.exists()) {
			System.err.println("file not exist!");
			return;
		}
		try {
			FileInputStream is = new FileInputStream(file);
			BufferedInputStream bis = new BufferedInputStream(is);
			int length = bis.available();
			byte[] bytes = new byte[length];
			bis.read(bytes);
			bis.close();
			for (int i = 0; i < length; i++) {
				bytes[i] = (byte) (~bytes[i]);
			}
			FileOutputStream os = new FileOutputStream(file.getAbsolutePath() + "2");
			BufferedOutputStream bos = new BufferedOutputStream(os);
			bos.write(bytes);
			bos.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
