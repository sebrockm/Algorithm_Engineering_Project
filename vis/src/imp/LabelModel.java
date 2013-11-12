package imp;

import api.Model;

public class LabelModel extends Model
{
    public static class _LabelData implements LabelData
    {
        public int x, y, w, h;
        public int ax, ay;
        public String text;
        boolean isVisible;
        
        _LabelData()
        {
            x = 0;
            y = 0;
            w = 0;
            h = 0;
            ax = 0;
            ay = 0;
        }
        
        @Override
        public int getX() 
        {
            return x;
        }

        @Override
        public int getY() 
        {
            return y;
        }

        @Override
        public int getW() 
        {
            return w;
        }

        @Override
        public int getH() 
        {
            return h;
        }
        
        public String getText()
        {
            return text;
        }

        @Override
        public int getAnchorX() 
        {
            return ax;
        }

        @Override
        public int getAnchorY() 
        {
            return ay;
        }
        
        public boolean isVisible()
        {
            return isVisible;
        }
    }
    
    public LabelModel(String file)
    {
        setFile(file);
    }
    
    
}
