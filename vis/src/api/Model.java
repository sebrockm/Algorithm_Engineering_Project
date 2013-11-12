package api;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Observable;

import util.AxisAlignedBB;

public abstract class Model extends Observable
{    
    public static interface LabelData
    {
        int getX();
        int getY();
        int getW();
        int getH();
        String getText();
        int getAnchorX();
        int getAnchorY();
        boolean isVisible();
        
        void setX(int x);
        void setY(int y);
        void setW(int w);
        void setH(int h);
        void setTextY(String text);
        void setAnchorX(int x);
        void setAnchorY(int y);
        void setVisible(boolean b);
    }
        
    private List<LabelData> _data = new ArrayList<>();
    private Parser _parser;
    private String _file;
    private AxisAlignedBB _aabb = new AxisAlignedBB();
    
    public int[] getScreenCoords(int x, int y, Renderer renderer)
    {
        int h = renderer.getH();
        int w = renderer.getW();
        float zoom = renderer.getZoom();
        int tx = renderer.getTranslationX();
        int ty = renderer.getTranslationY();

        int posx = x - _aabb.getMinX();
        int posy = y - _aabb.getMinY();
        
        posx = (int)((posx / (float)_aabb.getW()) * w) + tx;
        posy = (int)((posy / (float)_aabb.getH()) * h) - ty;
        
        float u = -0.5f + posx/(float)w;
        float v = -0.5f + posy/(float)h;
 
        u = 0.5f + zoom * u;
        v = 0.5f + zoom * v;
        
        posx = (int)(u * (float)w);
        posy = (int)((1-v) * (float)h);

        return new int[] { (int)posx, (int)posy };
    }
    
    public int[] getScreenCoords(int x, int y, int w, int h, Renderer renderer)
    {
        int[] c = new int[4];
        int[] _c = getScreenCoords(x, y, renderer);
        c[0] = _c[0];
        c[1] = _c[1];

        double u = (w / (double)_aabb.getW());
        double v = (h / (double)_aabb.getH());
        
        int sh = renderer.getH();
        int sw = renderer.getW();
        float zoom = renderer.getZoom();
        
        c[2] = (int)(zoom * u * sw);
        c[3] = (int)(zoom * v * sh);
        
        return c;
    }
    
    public int[] getModelCoords(int x, int y, Renderer renderer)
    {
        int h = renderer.getH();
        int w = renderer.getW();
        float zoom = renderer.getZoom();
        int tx = renderer.getTranslationX();
        int ty = renderer.getTranslationY();
        
        float u = -0.5f + (x - tx)/(float)w;
        float v = -0.5f + (y + ty)/(float)h;
        
        u = 0.5f + u/zoom;
        v = 0.5f + v/zoom;
        
        int posx = (int)(u * (float)_aabb.getW());
        int posy = (int)(v * (float)_aabb.getH());
        
        posx = posx + _aabb.getMinX();
        posy = posy + _aabb.getMinY();
        
        return new int[] { (int)posx, (int)-posy };
    }
    
    private void update()
    {
        setChanged();
        notifyObservers();
    }
    
    public AxisAlignedBB getAxisAlignedBB()
    {
        return _aabb;
    }
        
    public boolean addLabelData(LabelData data)
    {
    	if(_data.contains(data))
    	{
    		return false;
    	}
        _data.add(data);
        _aabb.addPoint(data.getX(), data.getY());
        _aabb.addPoint(data.getX() + data.getW(), data.getY() - data.getH());
        _aabb.addPoint(data.getAnchorX(), data.getAnchorY());
        update(); 
        
        return true;
    }

    public void setParser(Parser p)
    {
        _parser = p;
        generateData();
    }
    
    public Iterator<LabelData> getLabels()
    {
        return _data.listIterator();
    }
    
    public void clear()
    {
        _aabb.reset();
        _data.clear();
        update();
    }
    
    public void setFile(String file)
    {
        _file = file;
    }
    
    public String getFile()
    {
        return _file;
    }
    
    public Parser getParser()
    {
        return _parser;
    }
    
    public boolean generateData() 
    {
        clear();
        return _parser.parse(this);
    }
    
    public abstract LabelData createData();
}
