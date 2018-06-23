/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.util.LinkedList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import javax.swing.JPanel;
import javax.swing.JTextPane;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.AttributeSet;
import javax.swing.text.BadLocationException;
import javax.swing.text.DefaultStyledDocument;
import javax.swing.text.StyleConstants;
import javax.swing.text.StyleContext;

/**
 * Class representing a code editor with line numbering and syntax highlighting
 * @author Colin Halseth
 */
public class CodeEditor extends JPanel {
    
    /**
     * Internal class representing text style for a given pattern
     */
    private class Style {
        public Pattern pattern;
        public Color font;
        public Color background;
        public AttributeSet[] attributes;
    }
    
    /**
     * Internal interface for listeners
     */
    public interface ChangeListener {
        void invoke(String change);
    }
    
    /**
     * Reference to the editor pane
     */
    private JTextPane editor;
    /**
     * Reference to the line number pane
     */
    private JTextPane numbers;
    /**
     * Reference to the editor's document
     */
    private DefaultStyledDocument doc;
    
    /**
     * List of style rules
     */
    private LinkedList<Style> styles = new LinkedList<Style>();
    /**
     * Blank attribute set when no style is chosen
     */
    private AttributeSet none;
    /**
     * List of listeners to call when the document is changed
     */
    private LinkedList<ChangeListener> onChangeListeners = new LinkedList<ChangeListener>();
    
    /**
     * Create a new code editor
     */
    public CodeEditor(){
        super();
        this.setLayout(new BorderLayout());
        
        editor = new JTextPane();
        editor.setOpaque(false);
        numbers = new JTextPane();
        
        editor.setAutoscrolls(false);
        numbers.setAutoscrolls(false);
        this.setAutoscrolls(false);
        
        this.add(editor, BorderLayout.CENTER);
        this.add(numbers, BorderLayout.WEST);
        
        StyleContext context = StyleContext.getDefaultStyleContext();
        none = context.addAttribute(context.getEmptySet(), StyleConstants.Foreground, Color.BLACK);
        
        doc = new DefaultStyledDocument (){
            @Override
            public void insertString (int offset, String str, AttributeSet a) throws BadLocationException {
                super.insertString(offset, str, a);
                
                style(this);
                updateNumbering();
                
                for(ChangeListener listener : onChangeListeners){
                    listener.invoke(str);
                }
            }
            
            @Override
            public void remove(int offset, int length) throws BadLocationException{
                super.remove(offset, length);
                
                style(this);
                updateNumbering();
                
                for(ChangeListener listener : onChangeListeners){
                    listener.invoke(null);
                }
            }
            
            @Override
            public void replace(int offset, int length, String str, AttributeSet a) throws BadLocationException{
                super.replace(offset, length, str, a);
                style(this);
                updateNumbering();
                
                for(ChangeListener listener : onChangeListeners){
                    listener.invoke(str);
                }
            }
        };
        editor.setDocument(doc);
        
        editor.getDocument().addDocumentListener(new DocumentListener(){
            @Override
            public void insertUpdate(DocumentEvent de) {
                updateNumbering();
            }

            @Override
            public void removeUpdate(DocumentEvent de) {
                updateNumbering();
            }

            @Override
            public void changedUpdate(DocumentEvent de) {
                updateNumbering();
            }
        
        });
        
        numbers.setEditable(false);
        numbers.setBackground(new Color(184,184,184));
        numbers.setPreferredSize(new Dimension(30,40));
        
        updateNumbering();
    }
   
    /**
     * Add a change listener
     * @param listener 
     */
    public void addChangeListener(ChangeListener listener){
        this.onChangeListeners.add(listener);
    }
    
    /**
     * Remove a change listener
     * @param listener 
     */
    public void removeChangeListener(ChangeListener listener){
        this.onChangeListeners.remove(listener);
    }
    
    /**
     * Set the editor background colour
     * @param c 
     */
    public void setBackgroundColour(Color c){
        this.editor.setBackground(c);
    } 
    
    /**
     * Set the editor font colour
     * @param c 
     */
    public void setFontColour(Color c){
        StyleContext context = StyleContext.getDefaultStyleContext();
        none = context.addAttribute(context.getEmptySet(), StyleConstants.Foreground, c);
    }
    
    /**
     * Set the editor text
     * @param text 
     */
    public void setText(String text){
        this.editor.setText(text);
    }
    
    /**
     * Get the editor text
     * @return text
     */
    public String getText(){
        return this.editor.getText();
    }
    
    /**
     * Update the line numbers
     */
    private void updateNumbering(){
        String t = editor.getText();
        StringBuffer buffer = new StringBuffer();
        int count = 1;
        for(int i = 0; i < t.length(); i++){
            if(t.charAt(i) == '\n'){
                buffer.append(count++).append("\n");
            }
        }
        buffer.append(count++).append("\n");
        
        numbers.setText(buffer.toString());
    }
    
    /**
     * Set the font used by the code editor
     * @param font 
     */
    public void setFont(Font font){
        if(editor != null)
            editor.setFont(font);
        if(numbers != null)
            numbers.setFont(font);
    }

    /**
     * Add style for regex
     * @param regex
     * @param text 
     */
    public void addStyle(String regex, Color text){
        addStyle(regex, text, this.editor.getBackground());
    }
    
    /**
     * Add style for regex
     * @param regex
     * @param text 
     * @param background 
     */
    public void addStyle(String regex, Color text, Color background){
        StyleContext context = StyleContext.getDefaultStyleContext();
        
        Style s = new Style();
        s.pattern = Pattern.compile(regex);
        s.font = text;
        s.background = background;
        s.attributes = new AttributeSet[1]; 
        
        s.attributes[0] = context.addAttribute(context.getEmptySet(), StyleConstants.Foreground, s.font);
        //.attributes[1] = context.addAttribute(context.getEmptySet(), StyleConstants.Background, s.background);
        
        this.styles.add(s);
    }
 
    /**
     * Style the document
     * @param doc 
     */
    private void style(DefaultStyledDocument doc){
        //Clear colours
        doc.setCharacterAttributes(0, doc.getLength(), none, true);

        try {
            String text = doc.getText(0, doc.getLength());
            //Set Styles
            for(Style s : styles){
                Matcher m = s.pattern.matcher(text);
                while(m.find()){
                    for(AttributeSet set : s.attributes){
                        doc.setCharacterAttributes(m.regionStart() + m.start(), m.end() - m.start(), set, false);
                    }
                }
            }
        }catch(Exception e){}
    }
    
    /**
     * Get the internal editor panel
     * @return JTextPane
     */
    public JTextPane getEditor(){
        return this.editor;
    }
}