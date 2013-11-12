package util;

public class AxisAlignedBB {
    
    int[] minMax = new int[4];
    
    
    public AxisAlignedBB()
    {
        reset();
    }
    
    public void reset()
    {
        minMax[0] = Integer.MAX_VALUE;
        minMax[1] = Integer.MIN_VALUE;
        minMax[2] = Integer.MAX_VALUE;
        minMax[3] = Integer.MIN_VALUE;
    }
    
    public void addPoint(int x, int y)
    {
        minMax[0] = Math.min(minMax[0], x);
        minMax[1] = Math.max(minMax[1], x);

        minMax[2] = Math.min(minMax[2], y);
        minMax[3] = Math.max(minMax[3], y);
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
    
    public String toString()
    {
        return String.format("[%d, %d] - [%d, %d]", getMinX(), getMinY(), getMaxX(), getMaxY());
    }
}
