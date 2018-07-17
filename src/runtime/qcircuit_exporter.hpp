#ifndef _QCIRCUIT_EXPORTER_H
#define _QCIRCUIT_EXPORTER_H

#include "./compiler.hpp"
#include "./runtime.hpp"
#include "./svg.hpp"

#include <iostream>
#include <map>

using namespace graphics;

void exportQuantumCircuit (std::string name, program& prog){
    svg paint;

    struct timeline {
        ulong length;
        ulong height;
        ulong y;
        ulong lines;
        ulong x;
    };

    std::map<std::string, timeline> map;
    ulong nexty = 0;
    ulong cellHeight = 40;
    ulong cellWidth = 40;
    ulong cellBuffer = 25;

    for(std::vector<qasm::exec::executable*>::iterator it = prog.lines.begin(); it != prog.lines.end(); it++){
        bool is_definition = typeof(qasm::exec::declaration, *it);
        if(is_definition){
            qasm::exec::declaration* ptr = typeas(qasm::exec::declaration, *it);
            if(ptr->type != "qreg")
                continue;
            //Create new timeline
            timeline t;
            t.length = cellHeight;
            t.lines = ptr->size;
            t.height = (ptr->size * (cellHeight)) + 25;
            t.y = nexty;
            t.x = 1;
            nexty += t.height + cellBuffer;
            map[ptr->name] = t;

            paint.addShape((shape*)new text(12, t.y + 12, ptr->name));
            for(ulong i = 0; i < t.lines; i++){
                ulong midline =  t.y + i*cellHeight + cellHeight/2;
                paint.addShape((shape*)new line(0, midline, cellWidth, midline));
            }

            continue;
        }
        bool is_gate = typeof(qasm::exec::apply_gate, *it);
        if(is_gate){
            qasm::exec::apply_gate* ptr = typeas(qasm::exec::apply_gate, *it);
            if(ptr->param_names.size() < 1)
                continue;
            //Create gate
            timeline& t1 = map[ptr->param_names[0]];
            //Underlying lines
            for(ulong i = 0; i < t1.lines; i++){
                ulong midline =  t1.y + i*cellHeight + cellHeight/2;
                paint.addShape((shape*)new line(t1.x * cellWidth, midline, t1.x * cellWidth + cellWidth, midline));
            }
            //Gates and connectors
            for(size_t t = 0; t < ptr->param_indecies.size(); t++){
                if(t != ptr->param_indecies.size() - 1){
                    //Connector
                    ulong midline =  t1.y + t*cellHeight + cellHeight/2;
                    paint.addShape((shape*) new rect(t1.x * cellWidth + cellWidth/2 - 8, midline - 8, 16, 16));
                    //paint.addShape((shape*)new line(t1.x * cellWidth + cellWidth / 2, ,t1.x * cellWidth + cellWidth / 2, ));
                } else{
                    //Gate
                    ulong top = t1.y + t * cellHeight;
                    rect* r = new rect(
                        t1.x * cellWidth, 
                        top,  
                        cellWidth,
                        cellHeight
                    );
                    r->setFill({255,255,255});
                    paint.addShape((shape*)r);
                    paint.addShape((shape*)new text(t1.x * cellWidth + cellWidth/4, top + cellHeight/2, ptr->name));
                }
            }
            t1.x++;
            //Connectors out of gate
            for(ulong i = 0; i < t1.lines; i++){
                ulong midline =  t1.y + i*cellHeight + cellHeight/2;
                paint.addShape((shape*)new line(t1.x * cellWidth, midline, t1.x * cellWidth + cellWidth, midline));
            }
            t1.x++;
            continue;
        }
        bool is_measure = typeof(qasm::exec::measurement, *it);
        if(is_measure){
            qasm::exec::measurement* ptr = typeas(qasm::exec::measurement, *it);
            //Create gate
            timeline& t1 = map[ptr->qreg];
            //Underlying lines
            for(ulong i = 0; i < t1.lines; i++){
                ulong midline =  t1.y + i*cellHeight + cellHeight/2;
                if(ptr->measureWhole || i == ((ulong)ptr->qindex)){
                    polygon* poly = new polygon();
                    poly->setFill({255,255,255});
                    poly->then(t1.x * cellWidth, midline)
                    .then(t1.x * cellWidth + cellWidth, midline - cellHeight / 3)
                    .then(t1.x * cellWidth + cellWidth, midline + cellHeight / 3);
                    paint.addShape((shape*)poly);
                }else{
                    paint.addShape((shape*)new line(t1.x * cellWidth, midline, t1.x * cellWidth + cellWidth, midline));
                }
            }
            //Connectors out of measurement
            t1.x++;
            for(ulong i = 0; i < t1.lines; i++){
                ulong midline =  t1.y + i*cellHeight + cellHeight/2;
                paint.addShape((shape*)new line(t1.x * cellWidth, midline, t1.x * cellWidth + cellWidth, midline));
            }
            continue;
        }
    }

    //Save svg
    paint.saveFile(name);
}

#endif