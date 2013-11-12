package api;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;

public abstract class Button extends InputController 
{
    private int _x, _y, _w, _h;
    private String _label = "";
    
    public Button(int x, int y, int w, int h, String label)
    {
        _x = x;
        _y = y;
        _w = w;
        _h = h;
        _label = label;
    }
    
    public String getLabel()
    {
        return _label;
    }
    
    public int getX()
    {
        return getRenderer().getW() - _x - _w;
    }
    
    public int getY()
    {
        return _y;
    }
    
    public int getW()
    {
        return _w;
    }
    
    public int getH()
    {
        return _h;
    }
    
    public abstract void onButtonPressed();
    
    public void draw(Graphics2D g, Color button, Color text)
    {
        g.setColor(button);
        g.fillRoundRect(getX(), getY(), _w, _h, 20, 20);
        g.drawRoundRect(getX(), getY(), _w, _h, 20, 20);
        g.setColor(text);
        int l = g.getFontMetrics().charsWidth(_label.toCharArray(), 0, _label.length());
        int y = getY() + _h/2 + g.getFontMetrics().getHeight()/3;
        int x = getX() + _w/2 - l/2;
        g.drawString(_label, x, y);
    }
    
    private boolean isIn(int x, int y)
    {
        return x > getX() && getX() + _w > x && y > getY() && y < getY() + _h;
    }

    @Override
    public void mousePressed(MouseEvent e) 
    {
        if(isIn(e.getX(), e.getY()))
        {
            onButtonPressed(); 
        }
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void mouseReleased(MouseEvent e) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void mouseExited(MouseEvent e) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void mouseWheelMoved(MouseWheelEvent e) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void mouseDragged(MouseEvent e) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        // TODO Auto-generated method stub
        
    }
    
    @Override
    public void keyTyped(KeyEvent e) {
        // TODO Auto-generated method stub
        
    }

    @Override
    public void keyPressed(KeyEvent e) {

    }

    @Override
    public void keyReleased(KeyEvent e) {
        // TODO Auto-generated method stub
        
    } 
}
