package util;

public class AxisAlignedBB {
    
    int[] minMax = new int[4];
    
    public void reset()
    {
        for(int i = 0; i < 4; ++i)
        {
            minMax[i] = 0;
        }
    }
    
    public void addPoint(int x, int y)
    {
        if(x < getMinX())
        {
            minMax[0] = x;
        } else if(x > getMaxX())
        {
            minMax[1] = x;
        }
        
        if(y < getMinY())
        {
            minMax[2] = y;
        } else if(y > getMaxY())
        {
            minMax[3] = y;
        }
    }
    
    public int getMinX()
    {
        return minMax[0];
    }
    
    public int getMaxX()
    {
        return minMax[1];
    }
    
    public int getMinY()
    {
        return minMax[2];
    }
    
    public int getMaxY()
    {
        return minMax[3];
    }
    
    public int getW()
    {
        return getMaxX() - getMinX();
    }
    
    public int getH()
    {
        return getMaxY() - getMinY();
    }
}
