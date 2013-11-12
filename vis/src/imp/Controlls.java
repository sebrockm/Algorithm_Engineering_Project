package imp;

import imp.Parser.ProjectedData;
import imp.Parser.RawData;

import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.io.File;
import java.io.IOException;

import javax.swing.JFileChooser;

import util.IO;
import api.InputController;

public class Controlls extends InputController 
{    
    private static final String PROGRAM_PATH = "ae -in %s -out %s";
    
    public void callCppProgProj()
    {
        String outFile = getModel().getFile() + "p";
        String inputFile = getModel().getFile();
        if(_callCppProg(String.format(PROGRAM_PATH, inputFile, outFile)) > -1)
        {
        	getModel().setFile(outFile);
        	getModel().setParser(new ProjectedData());
        }
    }
    
    public void callCppProgEval()
    {
        String inputFile = getModel().getFile();
        if(_callCppProg(String.format("ae -eval %s", inputFile)) == 0)
        {
        	System.out.println("Ok");
        }
    }
    
    public int _callCppProg(String args)
    {
        Runtime rt = Runtime.getRuntime();

        try {
            Process p = rt.exec(args);
            p.waitFor();
            int status = p.exitValue();
            return status;
        } catch (IOException e) 
        {
            e.printStackTrace();
        } catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
        return -1;
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
        else if(e.getKeyChar() == 'e')
        {
        	callCppProgEval();
        }
        
        else if(e.getKeyChar() == 'l')
        {
            JFileChooser chooser = new JFileChooser();
            chooser.setCurrentDirectory(new File("./tests/"));
            if(chooser.showOpenDialog(null) == JFileChooser.APPROVE_OPTION)
            {
            	getModel().setFile(chooser.getSelectedFile().getPath());
            	getModel().generateData();
            }
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
