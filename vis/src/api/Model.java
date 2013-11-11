package api;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Observable;

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
    }
        
    private List<LabelData> _data = new ArrayList<>();
    private Parser _parser;
    private String _file;
    
    private void update()
    {
        setChanged();
        notifyObservers();
    }
    
    public void addLabelData(LabelData data)
    {
        _data.add(data);
        update(); 
    }
    
    public void addLabelData(List<LabelData> data)
    {
        for(LabelData ld : data)
        {
            _data.add(ld);
        }
        update();
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
}
