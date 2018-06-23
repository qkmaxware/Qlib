/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Map;

/**
 * Class for sending files to the QLib runtime executable
 * @author halse
 */
public class QLibRuntime {
    
    /**
     * Reference to the QLib runtime file
     */
    private File QlibPath;
    
    /**
     * Create a Qlib runtime reference to the given file
     * @param QlibPath 
     */
    public QLibRuntime(File QlibPath){
        this.QlibPath = QlibPath;
    }
    
    /**
     * Invoke the QLib runtime and pass in the given file
     * @param f
     * @return
     * @throws IOException 
     */
    public RuntimeReport dispatch(File f) throws IOException{
        //Build process
        //ProcessBuilder pb = new ProcessBuilder(QlibPath.getName());
        
        //Set env variables
        //Map<String, String> env = pb.environment();
        
        //Set working directory
        //File workingFolder = new File(QlibPath.getParent());
        //pb.directory(workingFolder);
        
        //Process proc = pb.start();
        
        Process proc = Runtime.getRuntime().exec(new String[]{QlibPath.getAbsolutePath(), f.getAbsolutePath()});
        
        BufferedReader stdInput = new BufferedReader(new 
        InputStreamReader(proc.getInputStream()));

        BufferedReader stdError = new BufferedReader(new 
        InputStreamReader(proc.getErrorStream()));

        RuntimeReport rpt = new RuntimeReport();
        
        StringBuilder sb = new StringBuilder();
        String s;
        while ((s = stdInput.readLine()) != null)
        {
            sb.append(s);
            sb.append(System.lineSeparator());
        }
        rpt.output = sb.toString();
        sb = new StringBuilder();

        // read any errors from the attempted command
        while ((s = stdError.readLine()) != null)
        {
            sb.append(s);
            sb.append(System.lineSeparator());
        }
        rpt.errors = sb.toString();
        
        return rpt;
    }
    
}
