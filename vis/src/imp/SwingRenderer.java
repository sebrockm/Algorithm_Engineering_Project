package imp;

import java.awt.BasicStroke;
import java.awt.Canvas;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics2D;
import java.awt.image.BufferStrategy;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.SwingUtilities;

import api.Button;
import api.InputController;
import api.Model.LabelData;
import api.Renderer;

public class SwingRenderer extends Renderer
{
    private List<LabelData> _data = new ArrayList<LabelData>();
    private JFrame _frame;
    private Canvas _window;
    BufferStrategy _strategy;
    private Random _r;
    
    private float _zoom = 0.75f;
    private float _tx = 0, _ty = 0;
    private boolean _drawLabelText = true;
    private boolean _drawDots = true;
    private List<Button> _gui = new ArrayList<Button>();
    private String _splashScreen = "";
    private int _labeled = 0;
    
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
        int[] c = _model.getScreenCoords(x, y, w, h, this);
        g2d.setColor(color);
        int z = Math.max(4, Math.min(10, c[2]/15));
        g2d.fillOval(c[0] - z/2, c[1] - z/2, z, z);
        g2d.drawOval(c[0] - z/2, c[1] - z/2, z, z);
    }
    
    private void drawTextDirect(String text, int x, int y, Color c)
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
        g2d.setColor(c);
        g2d.drawString(text, x, y + g2d.getFontMetrics().getHeight());
    }
    
    private void drawText(String text, int x, int y)
    {
        int[] c = _model.getScreenCoords(x, y, this);
        drawTextDirect(text, c[0], c[1], _codes.getLabelTextColor());
    }
    
    private void drawRect(int x, int y, int w, int h)
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();

        g2d.setColor(nextColor());
        
        int[] c = _model.getScreenCoords(x, y, w, h, this);
        g2d.fillRect(c[0], c[1], c[2], c[3]);
        g2d.drawRect(c[0], c[1], c[2], c[3]);
        
        g2d.setColor(_codes.getBorderColor());
        
        int s = Math.max(1, Math.min(4, _model.getScreenWidths(10, 10, this)[0]/200));
        g2d.setStroke(new BasicStroke(s));
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
        _labeled = 0;
    }
    
    private void drawGui()
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
        for(Button b : _gui)
        {
            b.draw(g2d, _codes.getButtonColor(), _codes.getCoordinatesColor());
        }
        
        String nodes = String.format("Nodes: %d", _data.size());
        String labeled = String.format("Labeled: %d", _labeled);
        
        String max = nodes.length() > labeled.length() ? nodes : labeled;
        
        int w = 10 + _strategy.getDrawGraphics().getFontMetrics().charsWidth(max.toCharArray(), 0, max.length());
        int h = 35; int x = 40; int y = 10;
        
        g2d.setColor(_codes.getBackgroundColor());
        g2d.clearRect(x, y, w, h);
        g2d.fillRect(x, y, w, h);
        g2d.setColor(Color.WHITE);
        g2d.drawRect(x, y, w, h);
        
        g2d.drawString(nodes, 5 + x, 25);
        g2d.drawString(labeled, 5 + x, 25 + _strategy.getDrawGraphics().getFontMetrics().getHeight());
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
    
    private void drawCSText(int axis, int v0, int dx, int dy)
    {
        Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
        int[] c = _model.getScreenCoords(v0, v0, this);
        int other0 = c[axis];
        if((axis & 1) > 0)
        {
            g2d.setColor(_codes.getCoordinatesColor());
            drawTextDirect(String.valueOf(v0), 2, other0 - _strategy.getDrawGraphics().getFontMetrics().getHeight() - 2, _codes.getCSColor());  
        }
        else
        {
            g2d.setColor(_codes.getCoordinatesColor());
            drawTextDirect(String.valueOf(v0), other0 + 2, getH() - _strategy.getDrawGraphics().getFontMetrics().getHeight() - 2, _codes.getCSColor());
        }
    }
    
    private void drawRaster()
    {
        int rastersx = Math.min(25, Math.max(_model.getAxisAlignedBB().getW(), 10));
        int dx = Math.max(1, _model.getAxisAlignedBB().getW()/rastersx);
        int rastersy = Math.min(25, Math.max(_model.getAxisAlignedBB().getH(), 10));
        int dy = Math.max(1, _model.getAxisAlignedBB().getH()/rastersy);
        
        for(int i = _model.getAxisAlignedBB().getMaxX()-dx; ;i-=dx)
        {
            drawLine(0, i);
            if(_model.getScreenCoords(i, 0, this)[0] < 0)
            {
                break;
            }
        }
        
        for(int i = _model.getAxisAlignedBB().getMaxX(); ;i+=dx)
        {
            drawLine(0, i);
            if(_model.getScreenCoords(i, 0, this)[0] > getW())
            {
                break;
            }
        }
        
        for(int i = _model.getAxisAlignedBB().getMaxY()-dy; ;i-=dy)
        {
            drawLine(1, i);
            if(_model.getScreenCoords(0, i, this)[1] > getH())
            {
                break;
            }
        }
        
        for(int i = _model.getAxisAlignedBB().getMaxY(); ;i+=dy)
        {
            drawLine(1, i);
            if(_model.getScreenCoords(0, i, this)[1] < 0)
            {
                break;
            }
        }
    }
    
//    private boolean modelInWindowScreen(int offx, int offy)
//    {
//        AxisAlignedBB aabb = getModelScreenBox(offx, offy);
//        
//        return aabb.getW() <= getW() && aabb.getH() <= getH() && aabb.getMinX() >= 0 && aabb.getMaxY() <= getH();
//    }
    
//    private AxisAlignedBB getModelScreenBox(int offx, int offy)
//    {
//        AxisAlignedBB aabb = new AxisAlignedBB();
//        int[] min = _model.getScreenCoords(_model.getAxisAlignedBB().getMinX() - offx, _model.getAxisAlignedBB().getMinY() - offy, this);
//        int[] max = _model.getScreenCoords(_model.getAxisAlignedBB().getMaxX() - offx, _model.getAxisAlignedBB().getMaxY() + offy, this);
//        aabb.addPoint(min[0], min[1]);
//        aabb.addPoint(max[0], max[1]);
//        return aabb;
//    }
    
    private void drawCS()
    {
        int rastersx = Math.min(25, Math.max(_model.getAxisAlignedBB().getW(), 10));
        int dx = Math.max(1, _model.getAxisAlignedBB().getW()/rastersx);
        int rastersy = Math.min(25, Math.max(_model.getAxisAlignedBB().getH(), 10));
        int dy = Math.max(1, _model.getAxisAlignedBB().getH()/rastersy);

//        for(int i = _model.getAxisAlignedBB().getMinX(); i <= _model.getAxisAlignedBB().getMaxX()+dx; i+=dx)
//        {
//            drawCSText(0, i, dx, dy);
//        }
//        for(int i = _model.getAxisAlignedBB().getMinY()-dy; i <= _model.getAxisAlignedBB().getMaxY()+dy; i+=dy)
//        {
//            drawCSText(1, i, dx, dy);
//        }
        
        for(int i = _model.getAxisAlignedBB().getMaxX()-dx; ;i-=dx)
        {
            drawCSText(0, i, dx, dy);
            if(_model.getScreenCoords(i, 0, this)[0] < 0)
            {
                break;
            }
        }
        
        for(int i = _model.getAxisAlignedBB().getMaxX(); ;i+=dx)
        {
            drawCSText(0, i, dx, dy);
            if(_model.getScreenCoords(i, 0, this)[0] > getW())
            {
                break;
            }
        }
        
        for(int i = _model.getAxisAlignedBB().getMaxY()-dy; ;i-=dy)
        {
            if(_model.getScreenCoords(0, i, this)[1] + _strategy.getDrawGraphics().getFontMetrics().getHeight() > getH())
            {
                break;
            }
            drawCSText(1, i, dx, dy);
        }
        
        for(int i = _model.getAxisAlignedBB().getMaxY(); ;i+=dy)
        {
            drawCSText(1, i, dx, dy);
            if(_model.getScreenCoords(0, i, this)[1] < 0)
            {
                break;
            }
        }
    }
        
    @Override
    public void draw() 
    {
        _r = new Random(0);
        printBackground();
        
        if(_model != null)
        {        
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
                        if(_drawLabelText)
                        {
                            drawText(d.getText(), d.getAnchorX(), d.getAnchorY());    
                        }
                        if(_drawDots)
                        drawOval(d.getX(), d.getY(), 10, 10, _codes.getDotColor());
                    }
                    else
                    {
                        if(_drawDots)
                        drawOval(d.getX(), d.getY(), 10, 10, _codes.getNoLabelDotColor());   
                    }
                }
                drawCS();
            }
        }
       
        if(_splashScreen.length() > 0)
        {
            Graphics2D g2d = (Graphics2D)_strategy.getDrawGraphics();
            Font s = g2d.getFont();
            g2d.setFont(new Font("Arial", Font.BOLD, 100));
            int l = g2d.getFontMetrics().charsWidth(_splashScreen.toCharArray(), 0, _splashScreen.length());
            int y = getH()/2 + g2d.getFontMetrics().getHeight()/3;
            int x = getW()/2 - l/2;
            g2d.setColor(_codes.getCSColor());
            g2d.drawString(_splashScreen, x, y);
            g2d.setFont(s);
        }
        
        drawGui();
        
        _strategy.show();
    }

    @Override
    public void addLabelData(LabelData data) 
    {
        synchronized(this) 
        {
            _data.add(data);
            if(data.isVisible())
            {
                _labeled++;
            }
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
        float sign = 1 + Math.signum(dfactor);
        _zoom = _zoom * (1-sign) * 0.9f + (1/0.9f) * _zoom * sign;
        _zoom = Math.max(0.05f, _zoom);
    }

    @Override
    public void translate(int dx, int dy) 
    {
        float _dx = dx/_zoom;
        float _dy = dy/_zoom;
        
        _tx += Math.abs(_dx) > 0.2f ? _dx : Math.signum(dx) * 0.2f; 
        _ty += Math.abs(_dy) > 0.2f ? _dy : Math.signum(dy) * 0.2f; 
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
        return (int)_tx;
    }

    @Override
    public int getTranslationY() 
    {
        return (int)_ty;
    }
    
    public void addButton(Button b)
    {
        _gui.add(b);
        addInputController(b);
        b.setRenderer(this);
        b.setModel(_model);
    }

    @Override
    public void toggleDrawLabelText() 
    {
        _drawLabelText = !_drawLabelText;
    }

    @Override
    public void toggleDrawDots() 
    {
        _drawDots = !_drawDots;
    }

    @Override
    public void setSplashScreen(String text) 
    {
        _splashScreen = text;
    }

    @Override
    public void showPopUp(String message, boolean error) 
    {
        JOptionPane.showMessageDialog(_frame, message, error ? "Error" : "Info", error ? JOptionPane.ERROR_MESSAGE : JOptionPane.INFORMATION_MESSAGE);
    }

    @Override
    public Canvas getScreen() 
    {
    	return _window;
    }
}
