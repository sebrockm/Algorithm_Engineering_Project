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

		@Override
		public void setX(int x) {
			this.x = x;
			
		}

		@Override
		public void setY(int y) {
			this.y = y;
		}

		@Override
		public void setW(int w) {
			this.w = w;
		}

		@Override
		public void setH(int h) {
			this.h = h;
		}

		@Override
		public void setTextY(String text) {
			this.text = text;
			
		}

		@Override
		public void setAnchorX(int x) {
			ax = x;
		}

		@Override
		public void setAnchorY(int y) {
			ay = y;
			
		}

		@Override
		public void setVisible(boolean b) 
		{
			isVisible = b;
		}
		
		public boolean equals(Object other)
        {
			LabelData o = (LabelData)(other);
			
			return (getX() == o.getX() && getY() == o.getY());  
        }
    }
    
    public LabelModel(String file)
    {
        setFile(file);
        _aabb.addPoint(5, 5);
        _aabb.addPoint(-5, -5);
    }
    
    public LabelData createData()
    {
    	return new _LabelData();
    }
    
    
}
