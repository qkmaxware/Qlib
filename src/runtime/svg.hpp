#ifndef _SVG_H
#define _SVG_H

#include <vector>
#include <iostream>

namespace graphics {

class bounds {
    private:
        size_t x;
        size_t y;
        size_t w;
        size_t h;
    public:
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

class shape {
    private:
    public:
        virtual ~shape(){}
        virtual bounds& getExtents(); 
        //virtual std::string encode();
};

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
};

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
            size_t mixX;
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
};

class rect : public shape {
    private:
        bounds b;
    public: 
        rect(size_t x, size_t y, size_t width, size_t height) : b(x, y, width, height) {}
        bounds& getExtents(){
            return b;
        }
};

class svg {
    private:
        size_t width;
        size_t height;
        std::vector<shape*> shapes;

    public:
        svg(size_t width, size_t height) : width(width), height(height) : shapes() {};
        ~svg(){};

        void addShape(Shape shape){
            shapes.push_back(&shape);
        }
        void addShape(Shape* shape){
            shapes.push_back(shape);
        }

        void saveFile(std::string filename){

        }
};

}

#endif