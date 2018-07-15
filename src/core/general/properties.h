#ifndef _CPPPROPERTIES_H
#define _CPPPROPERTIES_H

#define GETTER(n) void get##n () { return n; }
#define SETTER(t, n) void set##n (t value) { n = value; }

#define GET(n) void get##n () 
#define SET(t, n) void set##n (t value) 

#endif