#ifndef _ARGPARSER_H
#define _ARGPARSER_H

#include <map>
#include <iostream>
#include <vector>
#include <algorithm>

/// <Summary>
/// Class used to help parse command line parameters
/// </Summary>
class argz {
    public:
        /// <Summary>
        /// Command line arguments. Arguments are words without leading '-'
        /// </Summary>
        std::vector<std::string> arguments;
        /// <Summary>
        /// Command line flags. Flags are words with leading '-'
        /// </Summary>
        std::vector<std::string> flags;
        /// <Summary>
        /// Command line parameters. Parameters are values in the pattern -flag=value
        /// </Summary>
        std::map<std::string, std::string> parameters;

        /// <Summary>
        /// Parse given command line parameters
        /// </Summary>
        argz(int arg_count, char* arg_values[]) : arguments(), flags(), parameters(){
            //arg 0 is the program name so we ignore it
            for(int i = 1; i < arg_count; i++){
                std::string s = arg_values[i];
                if(s.size() > 0 && s[0] == '-'){
                    size_t splitter = s.find('=');
                    if(splitter == string::npos){
                        flags.push_back(s);
                    }else{
                        std::string pre = s.substr(0, splitter);
                        flags.push_back(pre);
                        parameters[pre] = s.substr(splitter + 1, s.length() - (splitter + 1));
                    }
                }else {
                    arguments.push_back(s);
                }
            }
        }
        /// <Summary>
        /// Test if a flag has been set
        /// </Summary>
        bool hasFlag(std::string flag){
            return std::find(flags.begin(), flags.end(), flag) != flags.end();
        }
        /// <Summary>
        /// Test if a given flag has an associated value
        /// </Summary>
        bool hasParameterFor(std::string flag){
            return parameters.count(flag) > 0;
        }
};

#endif