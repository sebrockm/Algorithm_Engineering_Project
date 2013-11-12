package api;

import imp.Parser.ProjectedData;

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
    
    private static final String PROGRAM_PATH = "ae -in %s -out %s";
    
    public void loadFile()
    {
        JFileChooser chooser = new JFileChooser();
        chooser.setCurrentDirectory(new File("./tests/"));
        if(chooser.showOpenDialog(null) == JFileChooser.APPROVE_OPTION)
        {
            getModel().setFile(chooser.getSelectedFile().getPath());
            getRenderer().setSplashScreen("Loading...");
            getModel().generateData();
            getRenderer().setSplashScreen("");
        }
    }
    
    public void callCppProgProj()
    {
        getRenderer().setSplashScreen("Solving...");
        String outFile = getModel().getFile() + "p";
        String inputFile = getModel().getFile();
        if(_callCppProg(String.format(PROGRAM_PATH, inputFile, outFile)) > -1)
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
        if(_callCppProg(String.format("ae -eval %s", inputFile)) == -1)
        {
            getRenderer().showPopUp("Evaluating Failed", true);
        }
        getRenderer().setSplashScreen("");
        getRenderer().showPopUp("Evaluating Succeded", false);
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
