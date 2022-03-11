#pragma once
#include <vector>
#include <string>

class StringUtil {
public:
	std::vector<std::string> delimit(std::string str, char delimiter) {
        std::vector<std::string> delimited;
        std::string curr = "";
        for (char c : str) {

            if (c == delimiter) {
                delimited.push_back(curr);
                curr = "";
            }
            else {
                curr = curr + c;
            }

        }

        if (curr.size() > 0) {
            //Add the last element
            delimited.push_back(curr);
        }

        return delimited;
	}
};