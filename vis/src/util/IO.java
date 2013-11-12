package util;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;

import api.Model;
import api.Model.LabelData;

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
        br.close();
        return sb.toString();
    }
    
    public static void writeTextFile(String file, String text) throws IOException
    {
    	BufferedWriter br = null;
        br = new BufferedWriter(new FileWriter(file));
        br.write(text);
        br.flush();
        br.close();
    }
    
    public static void writeLabelData(Model m) throws IOException
    {
        int c = 0;
        String format = "%d %d %d %d %s %d %d %d\n";
        String text = "";
        for(Iterator<LabelData> iter = m.getLabels(); iter.hasNext(); )
        {
            LabelData data = iter.next();
            text += String.format(format, data.getX(), data.getY(), data.getW(), data.getH(), data.getText(), data.isVisible() ? 1 : 0, data.getAnchorX(), data.getAnchorY());
            c++;
        }
        BufferedWriter br = null;
        text = String.format("%d\n", c) + text;
        br = new BufferedWriter(new FileWriter(m.getFile()));
        br.write(text);
        br.flush();
        br.close();
    }
}
