package api;

import imp.Parser.ProjectedData;
import imp.Parser.RawData;

import java.awt.event.KeyListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;
import java.io.File;
import java.io.IOException;

import javax.swing.JFileChooser;

public abstract class InputController implements KeyListener, MouseListener, MouseWheelListener, MouseMotionListener
{
    private Model _model;
    private Renderer _renderer;
    
    private static final String PROG_NAME = "./pflp_solver";
    private static final String SOLVE = PROG_NAME + " -in %s -out %s";
    private static final String EVAL = PROG_NAME + " -eval %s";
    
    public void loadFile()
    {
        JFileChooser chooser = new JFileChooser();
        chooser.setCurrentDirectory(new File("."));
        if(chooser.showOpenDialog(getRenderer().getScreen()) == JFileChooser.APPROVE_OPTION)
        {
            _model.setFile(chooser.getSelectedFile().getPath());
            getRenderer().setSplashScreen("Loading...");
            _model.setParser(new RawData());
            _model.generateData();
            getRenderer().setSplashScreen("");
            getRenderer().resetViewPortSettings();
        }
    }
    
    public void callCppProgProj()
    {
        getRenderer().setSplashScreen("Solving...");
        String outFile = getModel().getFile();
        String inputFile = getModel().getFile();
        int returnvalue = _callCppProg(String.format(SOLVE, inputFile, outFile));
        if(returnvalue > -1)
        {
            getRenderer().showPopUp(String.format("Solving succeded in '%d' Seconds", returnvalue), false);
            getRenderer().setSplashScreen("Loading...");
            getModel().setFile(outFile);
            getModel().setParser(new ProjectedData());
        }
        else
        {
            getRenderer().showPopUp("Solving failed", true);
        }
        getRenderer().setSplashScreen("");
    }
    
    public void callCppProgEval()
    {
        getRenderer().setSplashScreen("Evaluating...");
        String inputFile = getModel().getFile();
        if(_callCppProg(String.format(EVAL, inputFile)) == -1)
        {
            getRenderer().showPopUp("Evaluating failed", true);
        }
        else
        {
            getRenderer().showPopUp("Evaluating succeded", false);   
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
