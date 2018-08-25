/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package qlib.ide;

import java.awt.BorderLayout;
import java.awt.Desktop;
import java.awt.Dimension;
import java.awt.KeyEventDispatcher;
import java.awt.KeyboardFocusManager;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;
import javax.swing.border.EmptyBorder;
import resources.Loader;

/**
 * Class for the QlibIDE
 * @author halse
 */
public class QLibIDE {

    /**
     * Location of the config file
     */
    public static final String configLocation = "config.ini";
    
    /**
     * Location to the online resources for Qlib 
     */
    public static final String onlineResources = "https://github.com/qkmaxware/Qlib"; 
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> {
            try {
                //Load ini file from working directory
                IniIO.DEFAULT = IniIO.template(new String[]{
                    "Binary Path: ./qlib",
                    "Font Size: 14"
                });
                IniIO userConfig = IniIO.readAndUpdate(configLocation, IniIO.DEFAULT);

                QLibIDE ide = new QLibIDE(userConfig);
                ide.show();
            } catch (IOException ex) {
                Logger.getLogger(QLibIDE.class.getName()).log(Level.SEVERE, null, ex);
            }
        });
    }
    
    /**
     * Reference to the frame
     */
    private final JFrame frame;
    /**
     * Reference to the central panel
     */
    private final JPanel pane;
    /**
     * Reference to the output log
     */
    private final JTextArea log;
    /**
     * Reference to the scroll panel for the output log
     */
    private final JScrollPane log_scroll;
    /**
     * Reference to the tabbed pane for file managers
     */
    private final FileManager manager = new FileManager();
    /**
     * UI for configuring options
     */
    private OptionsMenu menu;
    /**
     * Date format
     */
    private final DateFormat dateFormat = new SimpleDateFormat("yyyy/MM/dd HH:mm:ss");
    
    /**
     * Reference to class for executing the QLib runtime
     */
    private final QLibRuntime runtime;
    
    /**
     * Create a new IDE instance
     * @param config
     * @throws IOException 
     */
    public QLibIDE(IniIO config) throws IOException{
        runtime = new QLibRuntime(new File(config.getString("Binary Path")));
        float fontSize = config.getInt("Font Size");
        if(fontSize < 1)
            fontSize = 1;
        manager.setFontSize(fontSize);
        
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
        log.setFont(log.getFont().deriveFont(fontSize));
        log.setLineWrap(true);
        log_scroll = new JScrollPane(log);
        log_scroll.setPreferredSize(new Dimension(120, 150));
        log_scroll.getVerticalScrollBar().setUnitIncrement(16);
        log.setEditable(false);
        pane.add(log_scroll, BorderLayout.SOUTH);
        
        menu = new OptionsMenu(config, configLocation);
        menu.setDefaultCloseOperation(JFrame.HIDE_ON_CLOSE);
        menu.setSize(340, 260);
        
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
                final JFileChooser fc;
                if(config.getString("last file") != null){
                    File file = new File(config.getString("last file"));
                    File parent = file.getParentFile();
                    fc = new JFileChooser(parent != null ? parent.getAbsolutePath() : "");
                }else{
                    fc = new JFileChooser();
                }
                //In response to a button click:
                int returnVal = fc.showOpenDialog(null);
                if (returnVal == JFileChooser.APPROVE_OPTION) {
                    File file = fc.getSelectedFile();
                    manager.open(file.getAbsolutePath());
                    
                    config.set("last file", file.getAbsolutePath());
                    IniIO.write(config, configLocation);
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
                    log(rpt.toString());
                }catch(Exception ex){
                    log(ex);
                }
            }
        });
        addShortcutKey(KeyEvent.VK_ENTER, run.getMouseListeners());
        
        JLabel clear = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-551-erase.png")));
        header.add(clear);
        clear.setToolTipText("Clear Terminal");
        clear.setBorder(new EmptyBorder(4, 4, 4, 4));
        clear.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                log.setText("");
            }
        });
        
                
        header.add(Box.createHorizontalGlue());
        
        JLabel help = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-196-info-sign.png")));
        header.add(help);
        help.setToolTipText("Documentation");
        help.setBorder(new EmptyBorder(4, 4, 4, 4));
        help.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (Desktop.isDesktopSupported()) {
                    try {
                        Desktop.getDesktop().browse(new URI(onlineResources));
                    } 
                    catch (Exception ex) {
                        JOptionPane.showMessageDialog(null, "Qlib - IDE can't open the default web browser to the online help resources. Please visit: " + onlineResources, "Can't open default web browser", JOptionPane.ERROR_MESSAGE);
                    } 
                }
            }
        });
        
        JLabel settings = new JLabel(new ImageIcon(Loader.resources.load("glyphicons-440-wrench.png")));
        header.add(settings);
        settings.setToolTipText("Preferences");
        settings.setBorder(new EmptyBorder(4, 4, 4, 4));
        settings.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                menu.setVisible(true);
            }
        });
        
        //Shortcut to open last opened file
        addShortcutKey(KeyEvent.VK_T, new MouseListener[]{new MouseAdapter(){
            public void mouseClicked(MouseEvent e) {
                if(config.getString("last file") != null){
                    File file = new File(config.getString("last file"));
                    manager.open(file.getAbsolutePath());
                }
            }
        }});
        
        //--------
        
        frame.pack();
        frame.setSize(640, 480);
    }
    
    /**
     * Add a string to the log
     * @param message 
     */
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
    
    /**
     * Show the IDE
     */
    public void show(){
        frame.setVisible(true);
    }
    
    /**
     * Add a key press shortcut to the IDE
     * @param key
     * @param listeners 
     */
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
