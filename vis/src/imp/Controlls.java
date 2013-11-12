package imp;

import imp.Parser.ProjectedData;
import imp.Parser.RawData;

import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.io.IOException;

import util.IO;
import api.InputController;

public class Controlls extends InputController 
{       
    @Override
    public void keyTyped(KeyEvent e) 
    {
        
    }

    @Override
    public void keyPressed(KeyEvent e) 
    {
        if(e.getKeyChar() == 'p')
        {
            getModel().setParser(new ProjectedData());
        }
        else if(e.getKeyChar() == 'o')
        {
            getModel().setParser(new RawData());
        }
        
        if(e.getKeyCode() == KeyEvent.VK_ESCAPE)
        {
            System.exit(0);
        }
        
        if(e.getKeyChar() == 'r')
        {
            getRenderer().resetViewPortSettings();
        }
        else if(e.getKeyChar() == 'w')
        {
            try {
                IO.writeLabelData(getModel());
            } catch (IOException e1) 
            {
                e1.printStackTrace();
            }
        }
        else if(e.getKeyChar() == 'c')
        {
        	callCppProgProj();
        }
        else if(e.getKeyChar() == 'd')
        {
            getRenderer().toggleDrawLabelText();
        }
        else if(e.getKeyChar() == 'e')
        {
        	callCppProgEval();
        }
        
        else if(e.getKeyChar() == 'l')
        {
            loadFile();
        }
    }

    @Override
    public void keyReleased(KeyEvent e) 
    {
    }

    @Override
    public void mouseClicked(MouseEvent e) 
    {

    }

    @Override
    public void mousePressed(MouseEvent e) 
    {
        
    }

    @Override
    public void mouseReleased(MouseEvent e) 
    {
        _lastX = -1;
        _lastY = -1;
    }

    @Override
    public void mouseEntered(MouseEvent e) {
 
    }

    @Override
    public void mouseExited(MouseEvent e) {
 
    }

    @Override
    public void mouseWheelMoved(MouseWheelEvent e) 
    {
        getRenderer().zoom(e.getWheelRotation() * 1e-1f);
    }

    int _lastX = -1;
    int _lastY = -1;
    @Override
    public void mouseDragged(MouseEvent e) 
    {
        int deltax = e.getX() - _lastX;
        int deltay = e.getY() - _lastY;
        if(_lastX == -1)
        {
            deltax = 0;
            deltay = 0;
        }

        getRenderer().translate(deltax, deltay);
        
        _lastX = e.getX();
        _lastY = e.getY();
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        // TODO Auto-generated method stub
        
    }
}
