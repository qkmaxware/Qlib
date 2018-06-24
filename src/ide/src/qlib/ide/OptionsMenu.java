/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

import java.awt.BorderLayout;
import java.util.LinkedList;
import java.util.Map.Entry;
import java.util.Vector;
import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;

/**
 * JFrame with a table for modifying key-value pairs in an INI file
 * @author halse
 */
public class OptionsMenu extends JFrame{
    
    public OptionsMenu(IniIO ini, String config_path){
        super();
        
        this.setTitle("Options: " + config_path);
        
        Vector data = new Vector();
            for(Entry<String,String> row : ini){
                Vector d = new Vector();
                d.add(row.getKey());
                d.add(row.getValue());
                data.addElement(d);
            }
        Vector columns = new Vector();
            columns.add("Key");
            columns.add("Value");
            
        JTable table = new JTable(data, columns);
         
        Box lower = Box.createHorizontalBox();
         
        JButton plus = new JButton("+");
        plus.addActionListener((evt) -> {
            Vector d = new Vector();
            d.add("<key " + data.size() + ">");
            d.add("<value>");
            data.add(d);
                    
            table.invalidate();
            table.revalidate();
        });
        lower.add(plus);
        
        JButton minus = new JButton("-");
        minus.addActionListener((evt) -> {
            int[] row = table.getSelectedRows();
            LinkedList<Object> toRemove = new LinkedList<Object>();
            for(int i = 0; i < row.length; i++){
                toRemove.add(data.get(row[i]));
            }
            data.removeAll(toRemove);
            
            table.invalidate();
            table.revalidate();
        });
        lower.add(minus);
        
        lower.add(Box.createHorizontalGlue());
         
        JButton save = new JButton("Save");
        save.addActionListener((evt) -> {
            ini.clear();
            for(Object o : data){
                String key = String.valueOf(((Vector)o).get(0));
                String value = String.valueOf(((Vector)o).get(1));
                
                ini.set(key, value);
            }
            IniIO.write(ini, config_path);
            this.setVisible(false);
        });
        lower.add(save);

        JPanel panel = new JPanel(new BorderLayout());
        panel.add(new JScrollPane(table), BorderLayout.CENTER);
        panel.add(lower, BorderLayout.SOUTH);

        this.add(panel);
    }
    
}
