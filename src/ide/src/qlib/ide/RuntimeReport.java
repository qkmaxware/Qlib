/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

/**
 * Report generated from the QLibRuntime class
 * @author halse
 */
public class RuntimeReport {
    
    /**
     * Terminal output string
     */
    public String output;
    /**
     * Terminal error string;
     */
    public String errors;
    
    /**
     * Print report
     * @return 
     */
    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        if(output.trim().length() != 0){
            sb.append("\n// Program Execution Success ----------------------- \n");
            sb.append(output);
        }
        if(errors.trim().length() != 0){
            sb.append("\n// Errors -----------------------\n");
            sb.append(errors);
        }
        
        return sb.toString();
    }
    
}
