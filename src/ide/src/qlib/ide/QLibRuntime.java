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
 *
 * @author halse
 */
public class QLibRuntime {
    
    private File QlibPath;
    
    public QLibRuntime(File QlibPath){
        this.QlibPath = QlibPath;
    }
    
    public RuntimeReport dispatch(File f) throws IOException{
        //Build process
        ProcessBuilder pb = new ProcessBuilder(QlibPath.getName());
        
        //Set env variables
        Map<String, String> env = pb.environment();
        
        //Set working directory
        File workingFolder = new File(QlibPath.getParent());
        pb.directory(workingFolder);
        
        Process proc = pb.start();
        
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
        }
        rpt.output = sb.toString();
        sb = new StringBuilder();

        // read any errors from the attempted command
        while ((s = stdError.readLine()) != null)
        {
            sb.append(s);
        }
        rpt.errors = sb.toString();
        
        return rpt;
    }
    
}
