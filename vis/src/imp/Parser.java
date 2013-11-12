package imp;

import imp.LabelModel._LabelData;

import java.io.IOException;
import java.util.StringTokenizer;

import util.IO;

import api.Model;

public class Parser 
{
    
    public static class RawData implements api.Parser
    {
        @Override
        public boolean parse(Model m) 
        {            String t;
        try 
        {
            t = IO.readTextFile(m.getFile());
        } catch (IOException e) 
        {
            e.printStackTrace();
            return false;
        }
        StringTokenizer st = new StringTokenizer(t, "\n");
        st.nextToken();
        while(st.hasMoreElements())
        {
            StringTokenizer _st = new StringTokenizer(st.nextToken(), " ");
            if(_st.countTokens() < 5)
            {
                m.clear();
                return false;
            }
            _LabelData ld = new _LabelData();
            ld.x = Integer.parseInt(_st.nextToken());
            ld.y = Integer.parseInt(_st.nextToken());
            ld.w = Integer.parseInt(_st.nextToken());
            ld.h = Integer.parseInt(_st.nextToken());
            ld.text = _st.nextToken();
            _st.nextToken();
            ld.isVisible = true;
            ld.ax = ld.x;
            ld.ay = ld.y;
            m.addLabelData(ld);
        }
        return true;
        }
    }
    
    public static class ProjectedData implements api.Parser
    {
        @Override
        public boolean parse(Model m) 
        {
            String t;
            try 
            {
                t = IO.readTextFile(m.getFile());
            } catch (IOException e) 
            {
                e.printStackTrace();
                return false;
            }
            StringTokenizer st = new StringTokenizer(t, "\n");
            st.nextToken();
            while(st.hasMoreElements())
            {
                StringTokenizer _st = new StringTokenizer(st.nextToken(), " ");
                if(_st.countTokens() < 5)
                {
                    m.clear();
                    return false;
                }
                _LabelData ld = new _LabelData();
                ld.x = Integer.parseInt(_st.nextToken());
                ld.y = Integer.parseInt(_st.nextToken());
                ld.w = Integer.parseInt(_st.nextToken());
                ld.h = Integer.parseInt(_st.nextToken());
                ld.text = _st.nextToken();
                ld.isVisible = Integer.parseInt(_st.nextToken()) > 0;
                ld.ax = Integer.parseInt(_st.nextToken());
                ld.ay = Integer.parseInt(_st.nextToken());
                m.addLabelData(ld);
            }
            return true;
        }
    }
}
