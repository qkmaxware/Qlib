#ifndef _CPPPROPERTIES_H
#define _CPPPROPERTIES_H

#define GETTER(t, n) t get##n () { return n; }
#define SETTER(t, n) void set##n (t value) { n = value; }

#define GET(t, n) t get##n () 
#define SET(t, n) void set##n (t value) 

#define typeof(t, obj) (dynamic_cast<t*> (obj) != NULL)
#define typeas(t, obj) dynamic_cast<t*> (obj)

#endif