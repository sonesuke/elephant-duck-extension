#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <vector>
#include "regex.h"



int regex_match(const std::string &data, const std::string &pattern, std::vector<std::string> &results)
{
    regex_t re;
    regmatch_t matches[100];
    int result_code;
    char buffer[1024 * 2];

    result_code = regcomp(&re, pattern.c_str(), REG_EXTENDED);
    if (result_code != 0) {
        regerror(result_code, &re, buffer, sizeof(buffer));
        std::cerr << "Compile error. " << buffer << std::endl;
        return 1;
    }

    result_code = regexec(&re, data.c_str(), 100, &(matches[0]), 0);
    if (result_code != 0) {
        regerror(result_code, &re, buffer, sizeof(buffer));
        std::cerr << "Execution error. " << buffer << std::endl;
        return 1;
    }

    for (int i = 0; i < 100; i++) {
        if (matches[i].rm_so == -1) {
            break;
        }
        results.push_back(data.substr(matches[i].rm_so, matches[i].rm_eo - matches[i].rm_so));
    }

    regfree(&re);
    return 0;
}



