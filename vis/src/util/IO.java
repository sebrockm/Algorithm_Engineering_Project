package util;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class IO 
{
    public static String readTextFile(String file) throws IOException
    {
        BufferedReader br = null;
        br = new BufferedReader(new FileReader(file));
        StringBuilder sb = new StringBuilder();
        String line = br.readLine();
        while(line != null) 
        {
            sb.append(line);
            sb.append('\n');
            line = br.readLine();
        }
        return sb.toString();
    }
}
