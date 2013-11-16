package test;

import imp.LabelModel;

import java.io.IOException;
import java.util.StringTokenizer;

import api.Model.LabelData;

import util.IO;

public class DeutscheStaedte 
{
	
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
		int secondW = 1;
		int secondH = 1;
		try {
			String text = IO.readTextFile("Tabelle_5a.txt");
			StringTokenizer t = new StringTokenizer(text, "-");
			while(t.hasMoreElements())
			{
				StringTokenizer tt = new StringTokenizer(t.nextToken(), " \n");
				tt.nextToken();
				String g = tt.nextToken();
				LabelData d = m.createData();
//				int i = 0;
//				while(g.length() > i && isNumber(g.charAt(i++)));
//				g = g.substring(i-1, g.length());
				
				d.setText(g);
				
				String grad = tt.nextToken();
				grad = grad.replaceAll("\\W", " ");
				grad = grad.substring(0, grad.length()-1);

                String min = tt.nextToken();
                min = min.replaceAll("\\W", " ");
                min = min.substring(0, min.length()-1);
                
                String sec = tt.nextToken();
                sec = sec.replaceAll("\\W", " ");
                sec = sec.substring(0, sec.length()-1);
                
				int gi = Integer.parseInt(grad);
				int mi = Integer.parseInt(min);
				int si = Integer.parseInt(sec);
				d.setY(3600 * gi * secondW + 60 * mi * secondW + si * secondW);
				d.setY(d.getY()/100);
				d.setW(28);
				d.setAnchorX(d.getX());

				System.out.println(tt.nextToken());
				
                grad = tt.nextToken();
                grad = grad.replaceAll("\\W", " ");
                grad = grad.substring(0, grad.length()-1);

                min = tt.nextToken();
                min = min.replaceAll("\\W", " ");
                min = min.substring(0, min.length()-1);
                
                sec = tt.nextToken();
                sec = sec.replaceAll("\\W", " ");
                sec = sec.substring(0, sec.length()-1);
                gi = Integer.parseInt(grad);
                mi = Integer.parseInt(min);
                si = Integer.parseInt(sec);
                d.setX(3600 * gi * secondH + 60 * mi * secondH + si * secondH);
                d.setX(d.getX()/100);
                d.setH(20);
                d.setAnchorX(d.getY());
                
                m.addLabelData(d);
			}
			
			IO.writeLabelData(m);
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
}
