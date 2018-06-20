/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

import java.awt.BorderLayout;
import java.awt.Color;
import java.io.File;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.LinkedList;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;

/**
 *
 * @author halse
 */
public class FileManager {
    
    private JTabbedPane tabs;
    
    private class Tab {
        String name;
        String filepath;
        JScrollPane panel;
        CodeEditor editor;
        boolean edited = false;
        
        public void setChanged(boolean changed, JTabbedPane parent){
            boolean updated = changed != edited;
            this.edited = changed;
            this.panel.setName(this.name + (this.edited ? "*" : ""));
            if(updated){
                int idx = parent.indexOfComponent(this.panel);
                if(idx >= 0){
                    parent.setTitleAt(idx, this.panel.getName());
                }
            }
        }
    }
    
    private LinkedList<Tab> openTabs = new LinkedList<Tab>();
    
    public FileManager(){
        tabs = new JTabbedPane();
    }
    
    public void empty(){
        open(null);
    }
    
    public void open(String file){
        try{
            Tab t = new Tab();
            t.name = null;
            
            try{
                t.name = new File(file).getName();
            }catch(Exception e){}
            
            t.filepath = file;
            
            CodeEditor editor = new CodeEditor();
            editor.setBackground(new Color(253, 247, 225));
            editor.addStyle("(?:^|\\n)\\s*.*?(?=(?:$|\\s))", new Color(99,125,160));
            editor.addStyle("\\/\\/.*?(?=(?:\\n|$))", new Color(111, 159, 147));
            editor.addStyle("\\/\\*(?:.|\\n)+?\\*\\/", new Color(111, 159, 147));
            
            JScrollPane editor_scroll = new JScrollPane(editor);
            editor_scroll.getVerticalScrollBar().setUnitIncrement(16);
            
            t.editor = editor;
            t.panel = editor_scroll;
            t.panel.setName(t.name);
            
            try{
                String content = String.join("\n",Files.readAllLines(Paths.get(file)));
                t.editor.setText(content);
            }catch(Exception e){}
            
            editor.addChangeListener((str) -> {
                t.setChanged(true, tabs);
            });
            
            tabs.add(t.panel);
            tabs.repaint();
            
            openTabs.add(t);
        }catch(Exception e){}
    }
    
    public void save(){
        try{
            int i = tabs.getSelectedIndex();
            if(i < 0)
                return;
            Tab openTab = openTabs.get(i);
            if(openTab.name == null){
                //Create a file chooser
                final JFileChooser fc = new JFileChooser();
                //In response to a button click:
                int returnVal = fc.showSaveDialog(null);
                if (returnVal == JFileChooser.APPROVE_OPTION) {
                    File file = fc.getSelectedFile();
                    try(PrintWriter pw = new PrintWriter(file.getAbsolutePath())){
                        pw.print(openTab.editor.getText());
                    }
                    openTab.name = file.getName();
                    openTab.filepath = file.getAbsolutePath();
                    JOptionPane.showMessageDialog(null, "File: " + openTab.name, "Saved", JOptionPane.INFORMATION_MESSAGE);
                    openTab.setChanged(false, tabs);
                }
            }else{
                try(PrintWriter pw = new PrintWriter(openTab.filepath)){
                    pw.print(openTab.editor.getText());
                }
                JOptionPane.showMessageDialog(null, "File: " + openTab.name, "Saved", JOptionPane.INFORMATION_MESSAGE);
                openTab.setChanged(false, tabs);
            }
        }catch(Exception e){
            JOptionPane.showMessageDialog(null, e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
        }
    }
    
    public void close(){
        int i = tabs.getSelectedIndex();
        if(i < 0)
            return;
        Tab openTab = openTabs.get(i);
        tabs.remove(openTab.panel);
        openTabs.remove(openTab);
        tabs.repaint();
    }
    
    public File getActive(){
        int i = tabs.getSelectedIndex();
        if(i < 0)
            return null;
        Tab openTab = openTabs.get(i);
        return new File(openTab.filepath);
    }
    
    public JTabbedPane getPanel(){
        return tabs;
    }
    
}
