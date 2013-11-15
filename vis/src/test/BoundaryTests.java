package test;

import imp.LabelModel;

import java.io.IOException;

import api.Model.LabelData;

import util.IO;

public class BoundaryTests {
	
	public static void allLabeled()
	{
		String fileName = "tests/extremes/allLabeled.dat";
		LabelModel m = new LabelModel(fileName);
		
		int lw = 10;
		int lh = 4;
		int xcm2 = 50;
		int ycm2 = 50;
		
		for(int i = 0; i < ycm2; ++i)
		{
			for(int j = 0; j < xcm2; ++j)
			{
				LabelData d = m.createData();
				
				int x = -xcm2 + (int)(lw*xcm2 * ((j*lw))/(float)(lw*xcm2));
				int y = -ycm2 + (int)(lh*ycm2 * ((i*lh))/(float)(lh*ycm2));
				
				d.setX(x);
				d.setY(y);
				d.setW(lw);
				d.setH(lh);
				d.setText(String.format("(%d, %d)", j, i));
				d.setAnchorX(x);
				d.setAnchorY(y);
				m.addLabelData(d);
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
	
	public static void only4()
	{
		String fileName = "tests/extremes/only4.dat";
		LabelModel m = new LabelModel(fileName);
		
		int lw = 10;
		int lh = 4;
		int xcm2 = 50;
		int ycm2 = 50;
		
		int flw = lw * xcm2;
		int flh = lh * ycm2;
		
		for(int i = 0; i < ycm2; ++i)
		{
			for(int j = 0; j < xcm2; ++j)
			{
				LabelData d = m.createData();
				
				int x = -lw + (int)(lw*xcm2 * ((j*lw))/(float)(lw*xcm2));
				int y = -lh + (int)(lh*ycm2 * ((i*lh))/(float)(lh*ycm2));
				
				d.setX(x);
				d.setY(y);
				d.setW(flw);
				d.setH(flh);
				d.setText(String.format("(%d,%d)", j, i));
				d.setAnchorX(x);
				d.setAnchorY(y);
				m.addLabelData(d);
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
	
	public static void main(String[] args)
	{
		only4();
	}
}
