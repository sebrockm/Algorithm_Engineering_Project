package imp;

import java.awt.BasicStroke;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics2D;
import java.awt.image.BufferStrategy;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import api.InputController;
import api.Model.LabelData;
import api.Renderer;

public class SwingRenderer extends Renderer
{
    private List<LabelData> _data = new ArrayList<>();
    private JFrame _frame;
    private Canvas _window;
    BufferStrategy _strategy;
    private Random _r;
    
    private float _zoom = 0.75f;
    private int _tx = 0, _ty = 0;
    
    private Color nextColor()
    {
        Color color = new Color(
                _codes.getLabelColor().getRed() / 255f, 
                _codes.getLabelColor().getGreen() / 255f, 
                _codes.getLabelColor().getBlue() / 255f, 
                0.5f + 0.5f*_r.nextFloat() * _codes.getLabelColor().getAlpha() / 255f);
        return color;
    }
    
    private void drawOval(int x, int y, int w, int h, Color color)
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
        int[] c = _model.getScreenCoords(x, y, this);
        g2d.setColor(color);
        int z = 10;//Math.max(5, (int)(15/_zoom));
        g2d.fillOval(c[0] - z/2, c[1] - z/2, z, z);
        g2d.drawOval(c[0] - z/2, c[1] - z/2, z, z);
    }
    
    private void drawTextDirect(String text, int x, int y)
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
        g2d.setColor(_codes.getTextColor());
        g2d.drawString(text, x, y + g2d.getFontMetrics().getHeight());
    }
    
    private void drawText(String text, int x, int y)
    {
        int[] c = _model.getScreenCoords(x, y, this);
        drawTextDirect(text, c[0], c[1]);
    }
    
    private void drawRect(int x, int y, int w, int h)
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();

        g2d.setColor(nextColor());
        
        int[] c = _model.getScreenCoords(x, y, w, h, this);
        g2d.fillRect(c[0], c[1], c[2], c[3]);
        g2d.drawRect(c[0], c[1], c[2], c[3]);
        
        g2d.setColor(_codes.getBorderColor());
        g2d.setStroke(new BasicStroke(4f));
        g2d.drawRect(c[0], c[1], c[2], c[3]);
        g2d.setStroke(new BasicStroke(1f));
    }
    
    private void printBackground()
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
        g2d.setBackground(_codes.getBackgroundColor());
        g2d.clearRect(0, 0, _frame.getWidth(), _frame.getHeight());
    }
    
    public void clearScreen()
    {
        synchronized(this) 
        {
            _data.clear();        
        }
    }
    
    private void drawLine(int axis, int c)
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
        c = _model.getScreenCoords(c, c, this)[axis];
        if((axis & 1) > 0)
        {
            g2d.setColor(_codes.getRasterColor());
            g2d.drawLine(0, c, _window.getWidth(), c);
        }
        else
        {
            g2d.setColor(_codes.getRasterColor());
            g2d.drawLine(c, 0, c, _window.getHeight());
        }
    }
    
    private void drawCSText(int axis, int v0)
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
        int[] c = _model.getScreenCoords(v0, v0, this);
        int other0 = c[axis];
        if((axis & 1) > 0)
        {
            g2d.setColor(_codes.getCoordinatesColor());
            drawTextDirect(String.valueOf(v0), 0, other0 - 17);  
        }
        else
        {
            g2d.setColor(_codes.getCoordinatesColor());
            drawTextDirect(String.valueOf(v0), other0 - 15, 0);
        }
    }
    
    private void drawRaster()
    {
        int rastersx = Math.min(30, Math.max(_model.getAxisAlignedBB().getW(), 10));
        int dx = Math.max(1, _model.getAxisAlignedBB().getW()/rastersx);
        int rastersy = Math.min(30, Math.max(_model.getAxisAlignedBB().getH(), 10));
        int dy = Math.max(1, _model.getAxisAlignedBB().getH()/rastersy);

        for(int i = _model.getAxisAlignedBB().getMinX()-1; i <= _model.getAxisAlignedBB().getMaxX()+1; i+=dx)
        {
            drawLine(0, i);
        }
        for(int i = _model.getAxisAlignedBB().getMinY()-1; i <= _model.getAxisAlignedBB().getMaxY()+1; i+=dy)
        {
            drawLine(1, i);
        }
    }
    
    private void drawCS()
    {
        int rastersx = Math.min(30, Math.max(_model.getAxisAlignedBB().getW(), 10));
        int dx = Math.max(1, _model.getAxisAlignedBB().getW()/rastersx);
        int rastersy = Math.min(30, Math.max(_model.getAxisAlignedBB().getH(), 10));
        int dy = Math.max(1, _model.getAxisAlignedBB().getH()/rastersy);
        
        for(int i = _model.getAxisAlignedBB().getMinX()-1; i <= _model.getAxisAlignedBB().getMaxX()+1; i+=dx)
        {
            drawCSText(0, i);
        }
        for(int i = _model.getAxisAlignedBB().getMinY()-1; i <= _model.getAxisAlignedBB().getMaxY()+1; i+=dy)
        {
            drawCSText(1, i);
        }
    }
        
    @Override
    public void draw() 
    {
        _r = new Random(0);
        printBackground();
        synchronized(this) 
        {
            drawRaster();
            for(LabelData d : _data)
            {
                if(d.isVisible())
                {
                    drawRect(d.getAnchorX(), d.getAnchorY(), d.getW(), d.getH());
                }
            }
            for(LabelData d : _data)
            {
                if(d.isVisible())
                {
                    drawText(d.getText(), d.getAnchorX(), d.getAnchorY());
                    drawOval(d.getX(), d.getY(), 10, 10, _codes.getDotColor());
                }
                else
                {
                    drawOval(d.getX(), d.getY(), 10, 10, _codes.getNoLabelDotColor());   
                }
            }
            drawCS();
        }
        _strategy.show();
    }

    @Override
    public void addLabelData(LabelData data) 
    {
        synchronized(this) 
        {
            _data.add(data);
        }
    }

    @Override
    public boolean init(final String title, final int w, final int h) 
    {
        SwingUtilities.invokeLater(new Runnable() {
            
            @Override
            public void run() 
            {           
                _frame = new JFrame();
                
                _window = new Canvas();
                _window.setBackground(Color.WHITE);
                _window.setPreferredSize(new Dimension(w, h));
                _window.setIgnoreRepaint(true);
                
                _frame.add(_window);        

                _frame.setTitle(title);
                _frame.setPreferredSize(new Dimension(w, h));
                _frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                _frame.pack();
                _frame.setLocationRelativeTo(null);
                _frame.setVisible(true);
                
                _window.createBufferStrategy(2);
                _strategy = _window.getBufferStrategy();
                
                synchronized(SwingRenderer.this) 
                {
                    SwingRenderer.this.notifyAll();   
                }
            }
        });
        synchronized(this) 
        {
            try {
                wait();
            } catch (InterruptedException e) 
            {
                e.printStackTrace();
            } 
        }
        return true;
    }

    @Override
    public void addInputController(InputController c) 
    {
        _window.addKeyListener(c);
        _window.addMouseListener(c);
        _window.addMouseWheelListener(c);
        _window.addMouseMotionListener(c);
    }

    @Override
    public void zoom(float dfactor) 
    {
        _zoom += dfactor;
        _zoom = Math.max(0.05f, _zoom);
    }

    @Override
    public void translate(int dx, int dy) 
    {
        _tx += dx;
        _ty += dy;
    }

    @Override
    public void resetViewPortSettings() 
    {
        _ty = _tx = 0;
        _zoom = 0.75f;
    }

    @Override
    public int getW() 
    {
        return _window.getWidth();
    }

    @Override
    public int getH() 
    {
        return _window.getHeight();
    }

    @Override
    public float getZoom() 
    {
        return _zoom;
    }

    @Override
    public int getTranslationX()
    {
        return _tx;
    }

    @Override
    public int getTranslationY() 
    {
        return _ty;
    }
}
