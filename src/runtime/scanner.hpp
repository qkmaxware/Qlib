#ifndef _SCANNER_H
#define _SCANNER_H

#include <iostream>
#include <fstream>

/// <Summary>
/// Class to read through a file line by line
/// </Summary>
class scanner{
    private:
        /// <Summary>
        /// File object
        /// </Summary>
        std::ifstream file;
        /// <Summary>
        /// File line count
        /// </Summary>
        ulong file_line;
    public:
        /// <Summary>
        /// Create scanner for file
        /// </Summary>
        scanner(std::string file_name) :file(file_name), file_line(1){
        }
        /// <Summary>
        /// Get the next line
        /// </Summary>
        bool next(std::string& line){
            if(std::getline(file, line)){
                file_line ++;
                return true;
            }
            return false;
        }
        /// <Summary>
        /// Get the line number
        /// </Summary>
        ulong getLineNumber(){
            return file_line;
        }
};

#endif