#ifndef _ARGPARSER_H
#define _ARGPARSER_H

#include <map>
#include <iostream>
#include <vector>
#include <algorithm>

class argz {
    public:
        std::vector<std::string> arguments;
        std::vector<std::string> flags;
        std::map<std::string, std::string> parameters;

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

        bool hasFlag(std::string flag){
            return std::find(flags.begin(), flags.end(), flag) != flags.end();
        }
};

#endif