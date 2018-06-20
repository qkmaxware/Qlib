/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package resources;

import java.awt.Image;
import java.io.IOException;
import java.io.InputStream;
import javax.imageio.ImageIO;

/**
 *
 * @author halse
 */
public class Loader {
    
    public static Loader resources = new Loader();
    
    public Image load(String resource) throws IOException {
        InputStream input = getClass().getResourceAsStream(resource);
        Image image = ImageIO.read(input);
        return image;
    }
    
}
