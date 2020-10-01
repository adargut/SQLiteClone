//
// Created by Adar on 30/09/2020.
//

#ifndef SQLITECLONE_UTILS_H
#define SQLITECLONE_UTILS_H

#include <string>
#include <iostream>
#include <vector>
#include "row.h"

using string = std::string;

static inline std::vector<string> split_input(const string &input) {
    std::vector<string> res;
    int l = 0;
    int r = 0;
    for (int i = 0; i < input.size(); i++) {
        if (input.at(i) != ' ') {
            r++;
        }
        else {
            res.push_back(input.substr(l, (r - l)));
            l = i + 1;
            r = l;
        }
    }
    res.push_back(input.substr(l));
    return res;
}

#endif //SQLITECLONE_UTILS_H
