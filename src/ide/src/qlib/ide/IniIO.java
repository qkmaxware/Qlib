/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

import java.io.File;
import java.io.FileWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map.Entry;

/**
 * Class for storing INI type config information
 * @author Colin Halseth
 */
public class IniIO implements Iterable<Entry<String,String>>{
    
    /**
     * Static reference
     */
    public static IniIO DEFAULT;
    
    /**
     * List of settings
     */
    private HashMap<String,String> options = new HashMap<String,String>();
    
    /**
     * Create an empty config file
     */
    private IniIO(){}
    
    /**
     * Create a config from a template of key value pairs
     * @param KeyValuePairs
     * @return 
     */
    public static IniIO template(String[] KeyValuePairs){
        IniIO io = new IniIO();
        for(String line : KeyValuePairs){
            String[] opts = line.split(":");
            if(opts.length >= 2)
                io.options.put(opts[0].trim().toLowerCase(), opts[1].trim().toLowerCase());
        }
        return io;
    }
    
    /**
     * Read a config from a given file
     * @param filename
     * @return 
     */
    public static IniIO read(String filename){
        File f = new File(filename);
        if(!f.exists()){
            //New INI file with defaults
            return DEFAULT;
        }
        
        //Read INI
        try{
            IniIO ini = new IniIO();
            for(String line : Files.readAllLines(Paths.get(f.getAbsolutePath()))){
                if(line.startsWith("#") || !line.matches(".+:.+")){ 
                    continue;
                }
                String[] opts = line.split(":", 2);
                if(opts.length >= 2)
                    ini.options.put(opts[0].trim().toLowerCase(), opts[1].trim().toLowerCase());
            }
            return ini;
        }catch(Exception e){
            //Return with default options
            return DEFAULT;
        }
    }
    
    /**
     * Copy-and-replace options from 'b' and 'a' into a new config
     * @param a
     * @param b
     * @return 
     */
    public static IniIO merge(IniIO a, IniIO b){
        IniIO c = new IniIO();
        for(String key : a.options.keySet()){
            c.options.put(key, a.options.get(key));
        }
        
        for(String key : b.options.keySet()){
            c.options.put(key, b.options.get(key));
        }
        
        return c;
    }
   
    /**
     * Read config from file and automatically copy without replacement with a default template ensuring that all default options exist
     * @param fname
     * @param updateTemplate
     * @return 
     */
    public static IniIO readAndUpdate(String fname, IniIO updateTemplate){
        IniIO n = IniIO.read(fname);
        
        try{
            FileWriter writer = new FileWriter(fname, true);
            
            //If file did not exist, make the default file
            if(n == DEFAULT){
               for(String key : n.options.keySet()){
                   writer.write(key+": "+n.options.get(key)+System.lineSeparator());
               }    
            }
            
            //Append any new ini options to the file
            for(String key : updateTemplate.options.keySet()){
                if(!n.options.containsKey(key)){
                    n.options.put(key, updateTemplate.options.get(key));
                    writer.write(key+": "+updateTemplate.options.get(key)+System.lineSeparator());  
                }
            }
            
            writer.close();
        }catch(Exception e){
            System.out.println("Failed to update ini");
        }
        
        return n;
    }
    
    /**
     * Get a boolean value from a named property
     * @param prop
     * @return 
     */
    public boolean isSet(String prop){
        prop = prop.toLowerCase();
        if(this.options.containsKey(prop)){
            return Boolean.parseBoolean(this.options.get(prop));
        }
        return false;
    }
    
    /**
     * Get a string from a named property
     * @param prop
     * @return 
     */
    public String getString(String prop){
        prop = prop.toLowerCase();
        if(this.options.containsKey(prop)){
            return String.valueOf(this.options.get(prop));
        }
        return null;
    }
    
    /**
     * Parse an integer from a named property
     * @param prop
     * @return 
     */
    public Integer getInt(String prop){
        prop = prop.toLowerCase();
        if(this.options.containsKey(prop)){
            return Integer.parseInt(this.options.get(prop));
        }
        return -1;
    }
    
    /**
     * Set a named property
     * @param prop
     * @param value 
     */
    public void set(String prop, Object value){
        this.options.put(prop, String.valueOf(value));
    }
    
    /**
     * Check if a property exists in the config
     * @param prop
     * @return 
     */
    public boolean exists(String prop){
        prop = prop.toLowerCase();
        if(this.options.containsKey(prop)){
            return true;
        }
        return false;
    }
    
    /**
     * Write ini config options to a file
     * @param settings
     * @param filename 
     */
    public static void write(IniIO settings, String filename){
        try{
            FileWriter writer = new FileWriter(filename);
            for(String key : settings.options.keySet()){
                writer.write(key+": "+settings.options.get(key)+System.lineSeparator());
            }
            writer.close();
        }catch(Exception e){
            System.out.println("Failed to write INI file");
        }
    }

    /**
     * Remove all config options
     */
    public void clear(){
        options.clear();
    }
    
    /**
     * Iterate over all key value pairs
     * @return 
     */
    @Override
    public Iterator<Entry<String, String>> iterator() {
        return options.entrySet().iterator();
    }
    
}