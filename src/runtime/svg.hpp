#ifndef _SVG_H
#define _SVG_H

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory> 

#include "colour.hpp"

namespace graphics {

/// <Summary>
/// Describes a rectangular region of space. Usually for the purposes of bounding a certain area.
/// </Summary>
class bounds {
    private:
        size_t x;
        size_t y;
        size_t w;
        size_t h;
    public:
        bounds() : x(0), y(0), w(0), h(0){}
        bounds(size_t x, size_t y, size_t w, size_t h) : x(x), y(y), w(w), h(h) {}

        size_t getWidth() {
            return w;
        }

        size_t getHeight() {
            return h;
        }

        size_t getMinX(){
            return x;
        }

        size_t getMaxX(){
            return x + w;
        }

        size_t getMinY(){
            return y;
        }

        size_t getMaxY(){
            return y + h;
        }
};

/// <Summary>
/// Describes an abstact shape
/// </Summary>
class shape {
    private:
        colour stroke;
        colour fill;
        size_t stroke_width;
    public:
        shape() : stroke(), fill(), stroke_width(1) {} 
        virtual ~shape(){}
        virtual bounds& getExtents()=0; 
        virtual std::string encode() { return ""; };

        void setStrokeWidth(size_t t){
            stroke_width = t;
        }

        size_t getStrokeWidth(){
            return stroke_width;
        }

        void setStroke(colour s){
            this->stroke = s;
        }

        void setFill(colour f){
            this->fill = f;
        }

        colour& getStroke(){
            return stroke;
        }

        colour& getFill() {
            return fill;
        }
};

/// <Summary>
/// Describes a line between 2 points
/// </Summary>
class line : public shape {
    private:
        size_t sx;
        size_t sy;
        size_t ex;
        size_t ey;
        bounds b;
    public:
        line(size_t startX, size_t startY, size_t endX, size_t endY) : sx(startX), sy(startY), ex(endX), ey(endY) {
            size_t minX = startX < endX ? startX : endX;
            size_t maxX = startX > endX ? startX : endX;

            size_t minY = startY < endY ? startY : endY;
            size_t maxY = startY > endY ? startY : endY;

            b = bounds(minX, minY, maxX - minX, maxY - minY);
        }
        bounds& getExtents(){
            return b;
        }
        std::string encode() {
            stringstream sb;
            sb << "<line x1=\"" << sx;
            sb << "\" y1=\"" << sy;
            sb << "\" x2=\"" << ex;
            sb << "\" y2=\"" << ey;
            sb << "\" stroke=\"" << (this->getStroke()).toHex() << "\"";
            sb << " stroke-width=\"" << (this->getStrokeWidth()) << "\"";
            sb << " stroke-opacity=\"" << (this->getStroke()).getOpacity() << "\"";
            sb << "/>";
            return sb.str();
        }
};

/// <Summary>
/// Describes a collection of lines between 'N' points
/// </Summary>
class polyline : public shape {
    private:
        std::vector<size_t> xs;
        std::vector<size_t> ys;
        bounds b;
    public:
        polyline() : xs(), ys(), b(0,0,0,0) {}
        polyline& then(size_t x, size_t y){
            xs.push_back(x);
            ys.push_back(y);
            this->recalculateBounds();
            return *this;
        }
        void recalculateBounds(){
            size_t minX;
            size_t minY;
            size_t maxX;
            size_t maxY;
            for(size_t i = 0; i < xs.size(); i++){
                size_t x = xs[i];
                size_t y = ys[i];
                if(x < minX || i == 0)
                    minX = x;
                if(x > maxX || i == 0)
                    maxX = x;
                if(y < minY || i == 0)
                    minY = y;
                if(y > maxX || i == 0)
                    maxY = y;
            }
            b = bounds(minX, minY, maxX - minX, maxY - minY);
        }
        bounds& getExtents(){
            return b;
        }
        virtual std::string encode() {
            stringstream sb;
            sb << "<polyline points=\"";
            for(size_t i = 0; i < xs.size(); i++){
                if(i > 0){
                    sb << ", ";
                }
                sb << xs[i] << " " << ys[i];
            }
            sb << "\" stroke=\"" << (this->getStroke()).toHex() << "\"";
            sb << " stroke-width=\"" << (this->getStrokeWidth()) << "\"";
            sb << " stroke-opacity=\"" << (this->getStroke()).getOpacity() << "\"";
            sb << "/>";
            return sb.str();
        }
        size_t countPoints(){
            return xs.size();
        }
        size_t getX(size_t point){
            return xs[point];
        }
        size_t getY(size_t point){
            return ys[point];
        }
};

/// <Summary>
/// Describes a polygon of 'N' points
/// </Summary>
class polygon : public polyline {
    public:
        std::string encode() {
            stringstream sb;
            sb << "<polygon points=\"";
            for(size_t i = 0; i < countPoints(); i++){
                if(i > 0){
                    sb << ", ";
                }
                sb << getX(i) << " " << getY(i);
            }
            sb << "\" stroke=\"" << (this->getStroke()).toHex() << "\""; 
            sb << " stroke-width=\"" << (this->getStrokeWidth()) << "\"";
            sb << " stroke-opacity=\"" << (this->getStroke()).getOpacity() << "\" ";
            sb << " fill=\"" << (this->getFill()).toHex() << "\"";
            sb << " fill-opacity=\"" << (this->getFill()).getOpacity() << "\" ";
            sb << "/>";
            return sb.str();
        }
};

/// <Summary>
/// Describes a rectangular shape
/// </Summary>
class rect : public shape {
    private:
        bounds b;
    public: 
        rect(size_t x, size_t y, size_t width, size_t height) : b(x, y, width, height) {}
        bounds& getExtents(){
            return b;
        }
        std::string encode() {
            stringstream sb;
            sb << "<rect x=\"" << b.getMinX() << "\" ";
            sb << "y=\"" << b.getMinY() << "\" ";
            sb << "width=\"" << b.getWidth() << "\" ";
            sb << "height=\"" << b.getHeight() << "\" ";
            sb << "stroke=\"" << (this->getStroke()).toHex() << "\" ";
            sb << "stroke-opacity=\"" << (this->getStroke()).getOpacity() << "\" ";
            sb << "stroke-width=\"" << (this->getStrokeWidth()) << "\" ";
            sb << "fill=\"" << (this->getFill()).toHex() << "\" ";
            sb << "fill-opacity=\"" << (this->getFill()).getOpacity() << "\" ";
            sb << "/>";
            return sb.str();
        }
};

/// <Summary>
/// Describes a a string of text shown on an svg
/// </Summary>
class text : public shape {
    private:
        bounds b;
        size_t x;
        size_t y;
        std::string value;
        float rotation;
    public: 
        text(size_t x, size_t y, std::string value) : b(x, y, 1, 1), x(x), y(y), value(value), rotation(0) {}
        bounds& getExtents(){
            return b;
        }
        std::string encode() {
            //TODO add css properties for font details
            stringstream sb;
            sb << "<text x=\"" << b.getMinX() << "\" ";
            sb << "y=\"" << b.getMinY() << "\" ";
            sb << "stroke=\"" << (this->getStroke()).toHex() << "\" ";
            sb << "stroke-opacity=\"" << (this->getStroke()).getOpacity() << "\" ";
            sb << "stroke-width=\"" << (this->getStrokeWidth()) << "\" ";
            sb << "fill=\"" << (this->getFill()).toHex() << "\" ";
            sb << "fill-opacity=\"" << (this->getFill()).getOpacity() << "\" ";
            sb << "rotation=\"" << (this->rotation) << "\" ";
            sb << ">";
            sb << value;
            sb << "</text>";
            return sb.str();
        }
};

/*
    Example usage
    graphics::svg mysvg;
    graphics::rect r = graphics::rect(4,4, 10, 10);
    r.setFill({255,0,0,0});
    r.setStroke({0,0,255,0});
    mysvg.addShape(&r);
    mysvg.saveFile("Myfile.svg");
*/

/// <Summary>
/// Class used as a container to describe the contents of an SVG file
/// </Summary>
class svg {
    private:
        size_t width;
        size_t height;
        bool autosize;
        std::vector<std::shared_ptr<shape>> shapes;

    public:
        svg() : width(0), height(0), autosize(true), shapes() {};
        svg(size_t width, size_t height) : width(width), height(height), autosize(false), shapes() {};
        ~svg(){};

        void addShape(shape* ptr){
            addShape(std::shared_ptr<shape>(ptr));
        }

        void addShape(std::shared_ptr<shape> shape){
            shapes.push_back(shape);
            if(autosize){
                if(shape->getExtents().getMaxX() > width){
                    width = shape->getExtents().getMaxX();
                }
                if(shape->getExtents().getMaxY() > height){
                    height = shape->getExtents().getMaxY();
                }
            }
        }

        void saveFile(std::string filename){
            std::ofstream file(filename);
            file << "<svg width=\"" << width << "\" height=\"" << height << "\">\n";
            for(std::vector<std::shared_ptr<shape>>::iterator it = shapes.begin(); it != shapes.end(); it++){
                file << "\t";
                file << (*it)->encode();
                file << "\n";
            }
            file << "</svg>";
        }
};

}

#endif