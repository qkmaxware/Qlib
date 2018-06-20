/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.ActionEvent;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.File;
import java.io.IOException;
import java.nio.file.Paths;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import javax.swing.border.EmptyBorder;
import javax.swing.text.DefaultCaret;
import resources.Loader;

/**
 *
 * @author halse
 */
public class QLibIDE {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                //Load ini file from working directory
                IniIO.DEFAULT = IniIO.template(new String[]{
                    "Binary Path: ./qlib",
                    ""
                });
                IniIO userConfig = IniIO.readAndUpdate("config.ini", IniIO.DEFAULT);

                QLibIDE ide = new QLibIDE(userConfig);
                ide.show();
            } catch (IOException ex) {
                Logger.getLogger(QLibIDE.class.getName()).log(Level.SEVERE, null, ex);
            }
        });
    }
    
    
    private final JFrame frame;
    private final JPanel pane;
    private final JTextArea log;
    private final JScrollPane log_scroll;
    private final FileManager manager = new FileManager();
    private final DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
    
    private final QLibRuntime runtime;
    
    public QLibIDE(IniIO config) throws IOException{
        runtime = new QLibRuntime(new File(config.getString("Binary Path")));
        
        frame = new JFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setTitle("QLib IDE");
        
        ImageIcon icon = new ImageIcon(Loader.resources.load("icon.png"));
        frame.setIconImage(icon.getImage());
        
        pane = new JPanel();
        pane.setLayout(new BorderLayout());
        frame.add(pane);
        
        Box header = Box.createHorizontalBox();
        JPanel header_container = new JPanel(new BorderLayout());
        header_container.add(header, BorderLayout.CENTER);
        pane.add(header_container, BorderLayout.NORTH);
        
        pane.add(manager.getPanel(), BorderLayout.CENTER);
        
        log = new JTextArea();
        log_scroll = new JScrollPane(log);
        log.setPreferredSize(new Dimension(120, 150));
        log_scroll.getVerticalScrollBar().setUnitIncrement(16);
        log.setEditable(false);
        pane.add(log_scroll, BorderLayout.SOUTH);
        
        //-Actions

        JLabel empty = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-703-file-plus.png")));
        header.add(empty);
        empty.setToolTipText("New Blank File (ctrl+n)");
        empty.setBorder(new EmptyBorder(4, 4, 4, 4));
        empty.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                manager.empty();
            }
        });
        addShortcutKey(KeyEvent.VK_N, empty.getMouseListeners());   
        
        JLabel load = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-145-folder-open.png")));
        header.add(load);
        load.setToolTipText("Load File (ctrl+o)");
        load.setBorder(new EmptyBorder(4, 4, 4, 4));
        load.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                //Create a file chooser
                final JFileChooser fc = new JFileChooser();
                //In response to a button click:
                int returnVal = fc.showOpenDialog(null);
                if (returnVal == JFileChooser.APPROVE_OPTION) {
                    File file = fc.getSelectedFile();
                    manager.open(file.getAbsolutePath());
                }
            }
        });
        addShortcutKey(KeyEvent.VK_O, load.getMouseListeners());
        
        JLabel save = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-447-floppy-save.png")));
        header.add(save);
        save.setToolTipText("Save Current File (ctrl+s)");
        save.setBorder(new EmptyBorder(4, 4, 4, 4));
        save.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                manager.save();
            }
        });
        addShortcutKey(KeyEvent.VK_S, save.getMouseListeners());
        
        JLabel close = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-193-remove-sign.png")));
        header.add(close);
        close.setDisplayedMnemonic(KeyEvent.VK_W);
        close.setToolTipText("Close Current File (ctrl+w)");
        close.setBorder(new EmptyBorder(4, 4, 4, 4));
        close.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                manager.close();
            }
        });
        addShortcutKey(KeyEvent.VK_W, close.getMouseListeners());
        
        header.add(Box.createHorizontalGlue());
        
        
        JLabel run = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-174-play.png")));
        header.add(run);
        run.setDisplayedMnemonic(KeyEvent.VK_ENTER);
        run.setToolTipText("Run Program (ctrl+enter)");
        run.setBorder(new EmptyBorder(4, 4, 4, 4));
        run.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                manager.save();
                File f = manager.getActive();
                if(f == null || !f.exists()){
                    return;
                }
                
                try{
                    RuntimeReport rpt = runtime.dispatch(f);
                    log(rpt.output);
                    if(!rpt.errors.trim().equals("")){
                        logSevere(rpt.errors);
                    }
                }catch(Exception ex){
                    log(ex);
                }
            }
        });
        addShortcutKey(KeyEvent.VK_ENTER, run.getMouseListeners());
                
        header.add(Box.createHorizontalGlue());
        
        JLabel help = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-196-info-sign.png")));
        header.add(help);
        help.setToolTipText("Documentation");
        help.setBorder(new EmptyBorder(4, 4, 4, 4));
        help.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {}
        });
        
        JLabel settings = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-440-wrench.png")));
        header.add(settings);
        settings.setToolTipText("Preferences");
        settings.setBorder(new EmptyBorder(4, 4, 4, 4));
        settings.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {}
        });
        
        //--------
        
        frame.pack();
        frame.setSize(640, 480);
    }
    
    public void logSevere(Object message){
        if(message == null)
            return;
        
        Date date = new Date();
        log.append("SEVERE: [");
        log.append(dateFormat.format(date));
        log.append("] - ");
        log.append(message.toString());
        log.append("\n");
        
        log_scroll.getVerticalScrollBar().setValue(log_scroll.getVerticalScrollBar().getMaximum());
    }
    
    public void log(Object message){
        if(message == null)
            return;
        
        Date date = new Date();
        log.append("[");
        log.append(dateFormat.format(date));
        log.append("] - ");
        log.append(message.toString());
        log.append("\n");
        
        log_scroll.getVerticalScrollBar().setValue(log_scroll.getVerticalScrollBar().getMaximum());
    }
    
    public void show(){
        frame.setVisible(true);
    }
    
    private void addShortcutKey(int key, MouseListener[] listeners){
        KeyboardFocusManager kmanager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        kmanager.addKeyEventDispatcher(new KeyEventDispatcher (){
            @Override
            public boolean dispatchKeyEvent(KeyEvent e) {
                if(e.getID() == KeyEvent.KEY_PRESSED && e.getKeyCode() == key && e.isControlDown()){
                    e.consume();
                    for(MouseListener listener : listeners){
                        listener.mouseClicked(null);
                    }
                }
                return false;
            }
        });
    }
    
    
}
