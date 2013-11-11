package api;

import java.awt.event.KeyListener;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;

public abstract class InputController implements KeyListener, MouseListener, MouseWheelListener, MouseMotionListener
{
    private Model _model;
    private Renderer _renderer;
    
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
