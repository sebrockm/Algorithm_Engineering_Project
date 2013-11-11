package imp;

import imp.Parser.ProjectedData;

import java.awt.Color;

import api.InputController;
import api.Model;
import api.Parser;
import api.Renderer;
import api.Renderer.ColorCodes;

public class Main 
{
    public final static String FILE = "test.txt";
    
    public class Factory
    {
        public Renderer createRenderer()
        {
            return new SwingRenderer();
        }
        
        public Model createModel()
        {
            return new LabelModel(FILE);
        }
        
        public InputController createController()
        {
            return new Controlls();
        }
        
        public Parser createParser()
        {
            return new ProjectedData();
        }
    }
    
    static class _colors implements ColorCodes
    {
        Color label = new Color(67/255f,128/255f,134/255f,0.5f);
        Color text = new Color(1f,1f,1f,1f);
        Color dot = new Color(196f/255f,101f/255f,45f/255,1f);
        Color back = new Color(83f/255f,84/255f,138f/255f,1f);
        Color border = new Color(92f/255f,146/255f,181f/255f,1f);
        Color noLabel = new Color(160f/255f,77/255f,163f/255f,1f);
        Color raster = new Color(194f/255f,168/255f,116f/255f,1f);
        Color coordsColor = new Color(222f/255f,222/255f,222/255f,1f);
        
        @Override
        public Color getLabelColor() {
            return label;
        }

        @Override
        public Color getTextColor() {
            return text;
        }

        @Override
        public Color getDotColor() {
            return dot;
        }

        @Override
        public Color getBackgroundColor() {
            return back;
        }

        @Override
        public Color getBorderColor()
        {
            return border;
        }

        @Override
        public Color getNoLabelDotColor() 
        {
            return noLabel;
        }
        
        public Color getRasterColor()
        {
            return raster;
        }
        
        public Color getCoordinatesColor()
        {
            return coordsColor;
        }
    }
    
    public static void main(String[] args)
    {
        Main.Factory f = new Main().new Factory();
        
        Model model = f.createModel();
        
        model.setParser(f.createParser());
       
        Renderer renderer = f.createRenderer();
                
        renderer.setColorCodes(new _colors());
        
        if(!renderer.init("Visualizer", 1024, 768))
        {
            System.err.println("Failed to create renderer");
            System.exit(-1);
        }
        
        model.addObserver(renderer);
        
        if(!model.generateData())
        {
            System.err.println("Failed to create model");
            System.exit(-1);
        }
        
        InputController c = f.createController();
        c.setModel(model);
        c.setRenderer(renderer);
        renderer.addInputController(c);
        
        while(true)
        {
            renderer.draw();
            try {
                Thread.sleep(16);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
}
