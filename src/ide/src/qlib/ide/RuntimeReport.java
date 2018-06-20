/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

/**
 *
 * @author halse
 */
public class RuntimeReport {
    
    public String output;
    public String errors;
    
    public String toString(){
        StringBuilder sb = new StringBuilder();
        
        if(output.trim().length() != 0){
            sb.append("\n//-----------------------");
            sb.append("\nProgram Execution Success");
            sb.append("\n//-----------------------\n");
            sb.append(output);
        }
        if(errors.trim().length() != 0){
            sb.append("\n//-----------------------");
            sb.append("\nErrors");
            sb.append("\n//-----------------------\n");
            sb.append(errors);
        }
        
        return sb.toString();
    }
    
}
