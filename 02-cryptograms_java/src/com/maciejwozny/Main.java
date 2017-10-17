package com.maciejwozny;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import org.apache.commons.codec.binary.Base64;
import org.apache.commons.codec.binary.Hex;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;


public class Main {
    static final String zad1Key = "3a2da4920b76bf1952cf38b9f6f4e5da73b7ed6d9b3dfb18f5a1a0811713b747 ";
    static final String zad1Result = "Zamieszanie wokol wpisu Krystyny Jandy, w ktorym poskarzyla sie, ze nie rozpoznal jej sprzedawca w sklepie, uderzylo rykoszetem w dzialalnosc artystyczna tej wybitnej aktorki.";

//    public static final String[] hex = { "f" , "e" , "d", "c", "b", "a", "9", "8", "7", "6", "5", "4", "3", "2", "1", "0"};
    public static final String[] hex = { "0" , "1" , "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};
//    public static final String[] hex = { /*"0" , "1" ,*/ "2", "3", "4", /*"5", "6", "7", "8",*/ "9", "a", /*"b", "c", */"d" /*,  "e", "f" */};

    static int id = 0;

    static final String suffixKey1  = "0b76bf1952cf38b9f6f4e5da73b7ed6d9b3dfb18f5a1a0811713b747";
    static final String initVector1 = "b7b0f17b4dbaa830f9a8aed86fbbfb75"; // 16 bytes IV
    static final String encrypted1  = "3XBUTxXGSf2sBqoy2b5o+LAPXhx+dsRT/BVZb012Ns9UQnaJuhoEfdpgTcfkD0HiUv9Z/ky5o2tJBe8xdjOvwTYjAQkC6AKlCv48KbKnfnc1wi2A6PFrhJ8eXigN6qdSn75Ehf87rw5Z5PO5YRpvo5DvyBtzN0CH0/poN2I/vyD/pLF1HOBcKgU9reiBICxkNmmyaXh/bpBqhVospKy6tqhC/LbFpZNK4avLIRKR5PRKWY7Bw2LAkSp+R/vHlyVF";

    static final String suffixKey2  = "f380c5d89341cc33fbcb2fff4ba997d02807eed4103a5961679466a";
    static final String initVector2 = "8d93b005a6218b0cc5c677a04a3e491e"; // 16 bytes IV
    static final String encrypted2  = "W9S7zVk+Y8sZMS/sokKKR8ZUAZ9xyh2uISYJIw8nHAwoHK3YIuDBsFRGZIF4aH1hRKsY+L1oZMkgqaq2cjnpFqY3lXzBT/2kFYlEOllgm1F9mDSqFD+1kBJVF50IkJy+XS4ujNyYNI5EvHXutMKT+MzNf3G8bie4JZ2by0b084q+PZzQzhjhoZf0pi7PVXgJGZDJ9ptpXTuty08z3/VlIA==";

    private static IvParameterSpec iv;
    private static Cipher cipher;

    static synchronized final String getChar()
    {
        return hex[id++];
    }

    static List<String> resultList = new ArrayList<>();
    static synchronized void addResult(String txt)
    {
        resultList.add(txt);
    }

    static void init() throws Exception
    {
//        byte[] initVectorByte = Hex.decodeHex(initVector1.toCharArray());
        iv = new IvParameterSpec(Hex.decodeHex(initVector1.toCharArray()));
        cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
    }

    public static void main(String[] args) throws Exception {
        init();

        String timeStampStart = new SimpleDateFormat("HH:mm:ss").format(Calendar.getInstance().getTime());
        System.out.println("START: " + timeStampStart);

        List<DecryptorThread> threadList = new ArrayList<>();
        for (Object i : hex)
        {
            threadList.add(new DecryptorThread());
        }

        for (Thread thread : threadList)
        {
            thread.start();
        }

        for (Thread thread : threadList)
        {
            thread.join();
            System.out.println("DONE");
        }
        String timeStampEnd = new SimpleDateFormat("HH:mm:ss").format(Calendar.getInstance().getTime());
        System.out.println("DONE: " + timeStampStart + " - " + timeStampEnd);

        for (String result : resultList)
        {
            System.out.println(result);
        }

//
//        System.out.println(decrypt(key, initVector,
//                encrypt(key, iv,
//                        "Ala ma kota. Kot ma, ale... to jednak ona go posiada. Jednakże gdy przeczytamy to ponownie to...")));


    }

//=================================================================================

    private static boolean isASCII(String s)
    {
        for (int i = 0; i < s.length(); i++)
            if (s.charAt(i) > 127)
                return false;
        return true;
    }

    public static String encrypt(String key, IvParameterSpec iv, String value) {
        try {
            SecretKeySpec secretKey = new SecretKeySpec(Hex.decodeHex(key.toCharArray()), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.ENCRYPT_MODE, secretKey, iv);

            byte[] encrypted = cipher.doFinal(value.getBytes());
            System.out.println("encrypted string: "
                    + Base64.encodeBase64String(encrypted));

            return Base64.encodeBase64String(encrypted);
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        return null;
    }

    public static String decrypt(String key, String initVector, String encrypted) {
        try {
            IvParameterSpec iv = new IvParameterSpec(Hex.decodeHex(initVector.toCharArray()));
            SecretKeySpec secretKey = new SecretKeySpec(Hex.decodeHex(key.toCharArray()), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            cipher.init(Cipher.DECRYPT_MODE, secretKey, iv);

            byte[] original = cipher.doFinal(Base64.decodeBase64(encrypted));

            return new String(original);
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        return null;
    }



}
