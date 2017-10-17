package com.maciejwozny;

import org.apache.commons.codec.binary.Base64;
import org.apache.commons.codec.binary.Hex;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.Writer;

/**
 * Created by maciek on 26.10.16.
 */

public class DecryptorThread extends Thread
{
    public final String[] hex = Main.hex;
    private String ID = Main.getChar();
    private String encrypted;
    private IvParameterSpec iv;
    private Cipher cipher;
    private String suffixKey;
    private int n = 0;

    static boolean isASCII(String s)
    {
        int nonASCII = 0;
        int maxBlednych = 20;
        int length = Integer.min(maxBlednych, s.length());
        for (int i = 0; i < length; i++) {
            if (s.charAt(i) > 127)
                ++nonASCII;
        }
        for (int i = length; i < s.length(); ++i)
        {
            if (s.charAt(i) > 127)
                ++nonASCII;
            if (nonASCII > maxBlednych)
                return false;
        }
        return true;
    }

    void init(String initVector, String suffixKey, String encrypted) throws Exception
    {
        this.encrypted = encrypted;
        this.suffixKey = ID + suffixKey;
        iv = new IvParameterSpec(Hex.decodeHex(initVector.toCharArray()));
        cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
    }

    @Override
    public void run()
    {
        runSevenChars(Main.initVector1, Main.suffixKey1, Main.encrypted1);

//        final String[] hex = { "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
//        for (String letter : hex) {
//            System.out.println("ID = " + ID + ", letter = " + letter);
//            runSevenChars(Main.initVector2, letter + Main.suffixKey2, Main.encrypted2);
//        }

    }

    public void runSevenChars(String initVector, String suffixKey, String encrypted)
    {
        try {
            init(initVector, suffixKey, encrypted);
            for (String first : hex){
                for (String second : hex){
                    for (String third : hex){
                        for (String fourth : hex) {
                            for (String fifth : hex) {
                                for(String sixth : hex) {
                                    for(String seventh : hex) {
                                        try {
                                            String key = first + second + third + fourth + fifth + sixth + seventh + this.suffixKey;
                                            SecretKeySpec secretKey = new SecretKeySpec(Hex.decodeHex(key.toCharArray()), "AES");
                                            cipher.init(Cipher.DECRYPT_MODE, secretKey, iv);
                                            byte[] original = cipher.doFinal(Base64.decodeBase64(encrypted));

                                            String result = new String(original);

                                            if (isASCII(result)) {
                                                System.out.println(key + " " + result);
                                                Main.addResult(key + " " + result);
                                                Writer output;
                                                output = new BufferedWriter(new FileWriter("/home/maciek/output" + Integer.toString(n) + ".log", true));
                                                output.append(key + " " + result + "\n");
                                                output.close();
                                            }
                                        }
                                        catch (Exception e) {}
                                    }
                                }
                            }
                        }
                    }
                    System.out.println("ID = " + ID + ", first chars = " + first + second);
                }
            }
        }
        catch (Exception e)
        {
            System.err.println(e.getMessage() + "\n" + e.getLocalizedMessage());
        }

    }
}