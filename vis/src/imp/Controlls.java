package imp;

import imp.LabelModel._LabelData;
import imp.Parser.ProjectedData;

import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.io.IOException;

import util.IO;
import api.InputController;
import api.Model.LabelData;

public class Controlls extends InputController 
{    
    private static final String PROGRAM_PATH = "prog.exe -%s -%s";
    
    public void callCppProg()
    {
        Runtime rt = Runtime.getRuntime();
        String outFile = getModel().getFile();
        String inputFile = getModel().getFile();
        try {
            Process p = rt.exec(String.format(PROGRAM_PATH, inputFile, outFile));
            if(p.exitValue() == 0)
            {
                getModel().generateData();
            }
        } catch (IOException e) 
        {
            e.printStackTrace();
        }
    }
    
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
            //getModel().setParser(new RawData());
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
            callCppProg();
        }
    }

    @Override
    public void keyReleased(KeyEvent e) 
    {
    }

    _LabelData newData = null;
    @Override
    public void mouseClicked(MouseEvent e) 
    {
        if(newData == null)
        {
            newData = new _LabelData();
            newData.isVisible = true;
            newData.x = e.getX();
            newData.y = e.getY();
        }
        else
        {
            int w = e.getX() - newData.x;
            int h = e.getY() - newData.y;
            newData.w = w;
            newData.h = h;
            newData.text = "test";
            getModel().addLabelData(newData);
            newData = null;
        }
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

        getRenderer().translate(deltax, -deltay);
        
        _lastX = e.getX();
        _lastY = e.getY();
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        // TODO Auto-generated method stub
        
    }
}
