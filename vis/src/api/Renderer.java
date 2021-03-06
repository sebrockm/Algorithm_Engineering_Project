package api;

import java.awt.Canvas;
import java.awt.Color;
import java.util.Iterator;
import java.util.Observable;
import java.util.Observer;

import api.Model.LabelData;

public abstract class Renderer implements Observer 
{
    protected ColorCodes _codes;
    protected Model _model;
    
    public abstract void draw();
    
    public void setColorCodes(ColorCodes codes)
    {
        _codes = codes;
    }
    
    public abstract void addLabelData(LabelData data);
    
    public abstract Canvas getScreen();
    
    public abstract void clearScreen();
    
    public abstract boolean init(String title, int w, int h);
    
    public abstract void addInputController(InputController c);
    
    public abstract void zoom(float dfactor);
    
    public abstract void resetViewPortSettings();
    
    public abstract int getW();
    
    public abstract int getH();
    
    public abstract float getZoom();
    
    public abstract int getTranslationX();
    
    public abstract int getTranslationY();
    
    public abstract void toggleDrawLabelText();
    
    public abstract void toggleDrawDots();
    
    public abstract void addButton(Button b);
    
    public abstract void translate(int dx, int dy);
    
    public abstract void setSplashScreen(String text);
    
    public abstract void showPopUp(String message, boolean error);
    
    public void update(Observable o, Object arg)
    {
        clearScreen();
        Model m = (Model)o;
        _model = m;
        for(Iterator<LabelData> iter = m.getLabels(); iter.hasNext(); )
        {
            addLabelData(iter.next());
        }
    }
    
    public static interface ColorCodes
    {
        Color getLabelColor();
        
        Color getCSColor();
        
        Color getLabelTextColor();
        
        Color getDotColor();
        
        Color getBackgroundColor();
        
        Color getBorderColor();
        
        Color getNoLabelDotColor();
        
        Color getRasterColor();
        
        Color getCoordinatesColor();
        
        Color getButtonColor();
    }
}
