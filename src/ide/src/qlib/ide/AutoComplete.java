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
 *
 * @author chalseth
 */
public class AutoComplete {

    private final JTextComponent textComponent;
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
    private final ArrayList<String> suggestions = new ArrayList<String>();
    private int lettersUntilShown = 1;
    private String typedWord;
    private int startPosition;
    private int endPosition;
    private final JWindow window = new JWindow();
    private JList list = new JList();
    private DefaultListModel model = new DefaultListModel();

    private int width = 430;
    private int maxHeight = 160;
    
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

    public boolean isVisible() {
        return window.isVisible();
    }

    public void addWord(String s) {
        suggestions.add(s);
    }

    public void removeWord(String s) {
        suggestions.remove(s);
    }

    private void insertSuggestion(Object o) {
        String k = String.valueOf(o);
        String s = textComponent.getText();
        String n = s.substring(0, startPosition) + k + s.substring(endPosition);
        textComponent.setText(n);
        int cpos = Math.min(startPosition + k.length(), n.length());
        textComponent.setCaretPosition(cpos);
        hideSuggestions();
    }

    private void showSuggestions() {
        if (typedWord == null) {
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
            } catch (Exception ex) {}
        }
    }

    public void setCharactersUntilSuggestions(int characters) {
        this.lettersUntilShown = characters;
    }

    private void hideSuggestions() {
        window.setVisible(false);
    }

    private String getText(int start, int length) {
        try {
            return textComponent.getText(start, length);
        } catch (Exception e) {
            return null;
        }
    }

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
        
        
        String word = getText(start, end - start);
        
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
