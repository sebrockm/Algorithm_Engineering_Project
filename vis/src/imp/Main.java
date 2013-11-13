package imp;

import imp.Parser.RawData;

import java.awt.Color;

import api.Button;
import api.InputController;
import api.Model;
import api.Parser;
import api.Renderer;
import api.Renderer.ColorCodes;

public class Main 
{   
    public class Factory
    {
        public Renderer createRenderer()
        {
            return new SwingRenderer();
        }
        
        public Model createModel()
        {
            return new LabelModel(null);
        }
        
        public InputController createController()
        {
            return new Controlls();
        }
        
        public Parser createParser()
        {
            return new RawData();
        }
    }
    
    static class _colors implements ColorCodes
    {
        Color label = new Color(67/255f,128/255f,134/255f,0.5f);
        Color text = new Color(1f,1f,1f,1f);
        Color labelText = new Color(245f/255f,160f/255f,34f/255f,1f);
        Color dot = new Color(196f/255f,101f/255f,45f/255,1f);
        Color back = new Color(83f/255f,84/255f,138f/255f,1f);
        Color border = new Color(92f/255f,146/255f,181f/255f,1f);
        Color noLabel = new Color(160f/255f,77/255f,163f/255f,1f);
        Color raster = new Color(194f/255f,168/255f,116f/255f,1f);
        Color coordsColor = new Color(222f/255f,222/255f,222/255f,1f);
        Color button1 = new Color(192f/255f,80f/255f,77f/255f,1f);
        Color button = new Color(125/255f,60/255f,74/255f,1f);
        Color button2 = new Color(155/255f,187/255f,89f/255f,1f);
        
        @Override
        public Color getLabelColor() {
            return label;
        }

        @Override
        public Color getCSColor() {
            return text;
        }
        
        @Override
        public Color getLabelTextColor() {
            return labelText;
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
        
        public Color getButtonColor()
        {
            return button;
        }
    }
    
    public static void main(String[] args)
    {
//        try {
//            // Set cross-platform Java L&F (also called "Metal")
//        UIManager.setLookAndFeel(
//            UIManager.getSystemLookAndFeelClassName());
//        } 
//        catch (UnsupportedLookAndFeelException e) {
//           // handle exception
//        }
//        catch (ClassNotFoundException e) {
//           // handle exception
//        }
//        catch (InstantiationException e) {
//           // handle exception
//        }
//        catch (IllegalAccessException e) {
//           // handle exception
//        }
        
        Main.Factory f = new Main().new Factory();
        
        Model model = f.createModel();
        model.setParser(new RawData());
               
        final Renderer renderer = f.createRenderer();
                
        renderer.setColorCodes(new _colors());
        
        if(!renderer.init("PFLP - Visualizer", 1024, 768))
        {
            System.err.println("Failed to create renderer");
            System.exit(-1);
        }
                
        model.addObserver(renderer);
        
        final InputController c = f.createController();
        c.setModel(model);
        c.setRenderer(renderer);
        renderer.addInputController(c);
        
        model.generateData();
        
        int leftPadding = 10;
        renderer.addButton(new Button(leftPadding, 20, 100, 20, "Draw LabelText") {
            public void onButtonPressed() 
            {
                getRenderer().toggleDrawLabelText();
            }; 
        });
        
        renderer.addButton(new Button(100 + 2*leftPadding, 20, 100, 20, "Draw Dots") {
            public void onButtonPressed() 
            {
                getRenderer().toggleDrawDots();
            }; 
        });
        
        renderer.addButton(new Button(200 + 3*leftPadding, 20, 100, 20, "Load") {
            public void onButtonPressed() 
            {
                c.loadFile();
            }; 
        });
        
        renderer.addButton(new Button(300 + 4*leftPadding, 20, 100, 20, "Solve") {
            public void onButtonPressed() 
            {
                c.callCppProgProj();
            }; 
        });
        
        renderer.addButton(new Button(400 + 5*leftPadding, 20, 100, 20, "Evaluate") {
            public void onButtonPressed() 
            {
                c.callCppProgEval();
            }; 
        });
        
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
