/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

import java.awt.Dimension;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import javax.swing.DefaultListModel;
import javax.swing.JList;
import javax.swing.JScrollPane;
import javax.swing.JWindow;
import javax.swing.ListSelectionModel;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.Caret;
import javax.swing.text.JTextComponent;
import javax.swing.text.Utilities;

/**
 * Class for auto complete word suggestions used with JTextComponent
 * @author chalseth
 */
public class AutoComplete {
    
    /**
     * Parent text component
     */
    private final JTextComponent textComponent;
    /**
     * Document listener for the textComponent
     */
    private final DocumentListener docListener = new DocumentListener() {
        @Override
        public void insertUpdate(DocumentEvent de) {
            checkForAndShowSuggestions();
        }

        @Override
        public void removeUpdate(DocumentEvent de) {
            checkForAndShowSuggestions();
        }

        @Override
        public void changedUpdate(DocumentEvent de) {
            checkForAndShowSuggestions();
        }
    };
    /**
     * List of words to suggest
     */
    private final ArrayList<String> suggestions = new ArrayList<String>();
    /**
     * Number of letters until suggestions get shown
     */
    private int lettersUntilShown = 1;
    /**
     * The currently typed word
     */
    private String typedWord;
    /**
     * Start index of the typed word
     */
    private int startPosition;
    /**
     * End index of the typed word
     */
    private int endPosition;
    /**
     * Reference to the suggestion window
     */
    private final JWindow window = new JWindow();
    /**
     * Reference to the list of suggestions
     */
    private JList list = new JList();
    /**
     * Reference to the list's data model
     */
    private DefaultListModel model = new DefaultListModel();
    /**
     * Width of suggestion window
     */
    private int width = 430;
    /**
     * Maximum height of the suggestion window
     */
    private int maxHeight = 160;
    
    /**
     * Create an auto complete suggestion dialog for the given text component
     * @param textComponent 
     */
    public AutoComplete(JTextComponent textComponent) {
        this.textComponent = textComponent;
        this.textComponent.getDocument().addDocumentListener(docListener);
        this.textComponent.addComponentListener(new ComponentAdapter() {
            @Override
            public void componentHidden(ComponentEvent e) {
                hideSuggestions();
            }
        });
        this.textComponent.addFocusListener(new FocusAdapter() {
            @Override
            public void focusLost(FocusEvent e) {
                hideSuggestions();
            }
        });

        list.setModel(model);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        list.setLayoutOrientation(JList.VERTICAL);
        list.setVisibleRowCount(-1);
        //Click element insert
        list.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent mouseEvent) {
                JList<String> theList = (JList) mouseEvent.getSource();
                if (mouseEvent.getClickCount() == 2) {
                    int index = theList.locationToIndex(mouseEvent.getPoint());
                    if (index >= 0) {
                        Object o = theList.getModel().getElementAt(index);
                        insertSuggestion(o);
                    }
                }
            }
        });
        //Keyboard element insert
        textComponent.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent ke) {
                if (ke.getKeyCode() == KeyEvent.VK_TAB && isVisible()) {
                    int s = list.getSelectedIndex() + 1;
                    if (s >= model.getSize()) {
                        s = 0;
                    }
                    list.setSelectedIndex(s);
                    ke.consume();
                }
                if (ke.getKeyCode() == KeyEvent.VK_ENTER && isVisible()) {
                    int s = list.getSelectedIndex();
                    insertSuggestion(model.get(s));
                    ke.consume();
                }
            }
        });

        JScrollPane scroller = new JScrollPane(list);
        window.add(scroller);
        window.setAlwaysOnTop(true);
        window.setSize(new Dimension(width, maxHeight));
    }

    /**
     * Check if the suggestion box is visible
     * @return 
     */
    public boolean isVisible() {
        return window.isVisible();
    }

    /**
     * Add word to the dictionary
     * @param s 
     */
    public void addWord(String s) {
        suggestions.add(s);
    }

    /**
     * Remove word from the dictionary
     * @param s 
     */
    public void removeWord(String s) {
        suggestions.remove(s);
    }

    /**
     * Insert word, replacing the currently typed word
     * @param o 
     */
    private void insertSuggestion(Object o) {
        String k = String.valueOf(o);
        String s = textComponent.getText();
        String n = s.substring(0, startPosition) + k + s.substring(endPosition);
        textComponent.setText(n);
        int cpos = Math.min(startPosition + k.length(), n.length());
        textComponent.setCaretPosition(cpos);
        hideSuggestions();
    }

    /**
     * Show the suggestion box
     */
    private void showSuggestions() {
        if (typedWord == null) {
            hideSuggestions();
            return;
        }

        model.clear();

        for (int i = 0; i < suggestions.size(); i++) {
            String q = suggestions.get(i);
            if (q.startsWith(typedWord)) {
                model.addElement(q);
            }
        }

        if (model.size() > 0) {
            try {
                Caret caret = textComponent.getCaret();
                Point p = caret.getMagicCaretPosition();
                p.x += textComponent.getLocationOnScreen().x;
                p.y += textComponent.getLocationOnScreen().y;
                Rectangle r = textComponent.getUI().modelToView(textComponent, textComponent.getCaretPosition());
                window.setLocation(p.x, p.y + textComponent.getGraphics().getFontMetrics().getHeight());
                int size = Math.min(maxHeight, 32 * model.size());
                window.setSize(new Dimension(width, size));
                window.setVisible(true);
                list.setSelectedIndex(0);
            } catch (Exception ex) {
                hideSuggestions();
            }
        }else{
            hideSuggestions();
        }
    }

    /**
     * Set the number of characters until the suggestion box is shown
     * @param characters 
     */
    public void setCharactersUntilSuggestions(int characters) {
        this.lettersUntilShown = characters;
    }

    /**
     * Hide the suggestion box
     */
    private void hideSuggestions() {
        window.setVisible(false);
    }

    /**
     * Get the text between 2 indexes
     * @param start
     * @param length
     * @return 
     */
    private String getText(int start, int length) {
        try {
            return textComponent.getText(start, length);
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * For use auto checking the suggestions as a user types
     */
    private void checkForAndShowSuggestions() {

        int caret = textComponent.getCaretPosition() - 1;
        if (caret >= textComponent.getDocument().getLength()) {
            caret = textComponent.getDocument().getLength();
        }
        if(caret < 0)
            caret = 0;
        
        int start = 0;    
        try {
            start = Utilities.getWordStart(textComponent, caret);
        } catch (Exception e) {
            return;
        }
        
        int end = 0;
        try {
              end = Utilities.getWordEnd(textComponent, caret);
            } catch (Exception e) {
            return;
        }
        
        
        String word = getText(start, end - start).trim();
        
        if (word != null && word.length() >= lettersUntilShown) {
            startPosition = start;
            endPosition = end;
            typedWord = word;
            showSuggestions();
        }else{
            hideSuggestions();
        }

    }

}
