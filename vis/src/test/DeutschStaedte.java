package test;

import imp.LabelModel;

import java.io.IOException;
import java.util.StringTokenizer;

import api.Model.LabelData;

import util.IO;

public class DeutschStaedte {
	
	
	static boolean isNumber(char c)
	{
		try
		{
			Integer.parseInt(""+c);
		} catch(Exception e)
		{
			return false;
		}
		
		return true;
	}
	
	public static void main(String[] a)
	{
		String fileName = "tests/staedte/d.dat";
		LabelModel m = new LabelModel(fileName);
		
		try {
			String text = IO.readTextFile("Tabelle_5a.txt");
			StringTokenizer t = new StringTokenizer(text, "-");
			
			while(t.hasMoreElements())
			{
				StringTokenizer tt = new StringTokenizer(t.nextToken(), " ");
				String g = tt.nextToken();
				LabelData d = m.createData();
				int i = 0;
				while(g.length() > i && isNumber(g.charAt(i++)));
				g = g.substring(i-1, g.length());
				System.out.println(g);
				d.setText(g);
			}
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
