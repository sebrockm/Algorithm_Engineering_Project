package test;

import imp.LabelModel;

import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

import util.IO;
import api.Model.LabelData;

public class InstanceGenerator 
{
	static class TestInstanceConfig
	{
		private int seq;
		private Map<String, String> properties = new HashMap<String, String>();
		
		public TestInstanceConfig(int seq)
		{
			this.seq = seq;
		}
		
		public void addProperty(String name, double value)
		{
			addProperty(name, String.valueOf(value));
		}
		
		public void addProperty(String name, int value)
		{
			addProperty(name, String.valueOf(value));
		}
		
		public void addProperty(String name, String value)
		{
			properties.put(name, value);
		}
		
		public String getPath()
		{
			return "tests/" + String.format("test_%d/", seq);
		}
		
		public void write()
		{
			File f = new File("tests/" + String.format("test_%d", seq));
			if(!f.exists())
			{
				f.mkdir();	
			}
				
			String text = "";
			for(String s : properties.keySet())
			{
				String v = properties.get(s);
				text += s;
				text += "=";
				text += v;
				text += "\n";
			}
			try
			{
				IO.writeTextFile("tests/" + String.format("test_%d/config.cfg", seq), text);
			} catch (IOException e) 
			{
				e.printStackTrace();
			}
		}
	}
	
	public static void createTest(int seed, int testSeq, int cluster, int elemsPerCluster, int radius, int w, int h, double labelPercentage)
	{
		Random r = new Random(seed);
				
		int medLabelSize = (int)((double)((4 * w * h) * labelPercentage) / (double)(cluster * elemsPerCluster));
		
		double aspect = 16.0/9.0;
				
		int labelHMean = Math.max(1, (int)Math.sqrt(medLabelSize/aspect));
		int vary = Math.max(labelHMean/2, 1);
		
		int labelWMean = (int)(medLabelSize/(double)labelHMean); 
		int varx = Math.max(labelWMean/2, 1);
		
		TestInstanceConfig config = new TestInstanceConfig(testSeq);
		config.addProperty("cluster", cluster);
		config.addProperty("elemsPerCluster", cluster);
		config.addProperty("radius", radius);
		config.addProperty("w", w);
		config.addProperty("h", h);
		config.addProperty("labelHMean", labelHMean);
		config.addProperty("labelWMean", labelWMean);
		config.addProperty("varx", varx);
		config.addProperty("vary", vary);
		config.write();
		
		String fileName = String.format(config.getPath() + "_test_s=%d.dat", seed);
		LabelModel m = new LabelModel(fileName);
		
		
		for(int i = 0; i < cluster; ++i)
		{
			int x = -w + 2*r.nextInt(w+1);
			int y = -h + 2*r.nextInt(h+1);
	
			for(int j = 0; j < elemsPerCluster; ++j)
			{
				double gx = r.nextGaussian();
				double gy = r.nextGaussian();
							
				LabelData ld = m.createData();
				ld.setX((int)(x + gx * radius));
				ld.setY((int)(y + gy * radius));
				ld.setAnchorX(ld.getX());
				ld.setAnchorY(ld.getY());
				ld.setW(Math.max(1, labelWMean - varx + r.nextInt(2 * varx + 1)));
				ld.setH(Math.max(1, labelHMean - vary + r.nextInt(2 * vary + 1)));
				ld.setText(String.format("%d/%d", ld.getX(), ld.getY()));
				ld.setVisible(true);
				m.addLabelData(ld);
			}
		}
		try 
		{
			IO.writeLabelData(m);
		} catch (IOException e) 
		{
			e.printStackTrace();
		}
	}
	
	public static void createTest(int seq, int radius, int cluster, int elemsPerCluster, double percent)
	{
		for(int i = 0; i < 10; ++i)
		{
			createTest(i, seq, cluster, elemsPerCluster, radius, 100, 100, percent);	
		}
	}
	
	public static void main(String[] args)
	{
		/*createTest(0, 50, 10, 1000, 1);
		createTest(1, 6, 1000, 10, 1);
		createTest(2, 12, 100, 100, 1);
		createTest(3, 12, 50, 50, 1);
		createTest(4, 12, 50, 50, 0.5);*/
		createTest(5, 10, 2000, 10, 0.5);
	}
}
