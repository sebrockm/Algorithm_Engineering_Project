package api;

import imp.Parser.ProjectedData;

import java.awt.event.KeyListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;
import java.io.IOException;

public abstract class InputController implements KeyListener, MouseListener, MouseWheelListener, MouseMotionListener
{
    private Model _model;
    private Renderer _renderer;
    
    private static final String PROG_NAME = "pflp_solver";
    private static final String SOLVE = PROG_NAME + " -in %s -out %s";
    private static final String EVAL = PROG_NAME + " -eval %s";
    
    public void loadFile()
    {
        getRenderer().openLoadScreen();
    }
    
    public void callCppProgProj()
    {
        getRenderer().setSplashScreen("Solving...");
        String outFile = getModel().getFile();
        String inputFile = getModel().getFile();
        if(_callCppProg(String.format(SOLVE, inputFile, outFile)) > -1)
        {
            getRenderer().setSplashScreen("Loading...");
            getModel().setFile(outFile);
            getModel().setParser(new ProjectedData());
        }
        else
        {
            getRenderer().showPopUp("Solving Failed", true);
        }
        getRenderer().setSplashScreen("");
    }
    
    public void callCppProgEval()
    {
        getRenderer().setSplashScreen("Evaluating...");
        String inputFile = getModel().getFile();
        if(_callCppProg(String.format(EVAL, inputFile)) == -1)
        {
            getRenderer().showPopUp("Evaluating Failed", true);
        }
        else
        {
            getRenderer().showPopUp("Evaluating Succeded", false);   
        }
        getRenderer().setSplashScreen("");
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
        } catch (InterruptedException e) 
        {
            e.printStackTrace();
        }
        return -1;
    }
    
    public void setModel(Model model)
    {
        _model = model;
    }
    
    public Model getModel()
    {
        return _model;
    }
    
    public void setRenderer(Renderer r)
    {
        _renderer = r;
    }
    
    public Renderer getRenderer()
    {
        return _renderer;
    }
}
