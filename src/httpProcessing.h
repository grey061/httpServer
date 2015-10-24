#ifndef HTTP_PROCESSING_H
#define HTTP_PROCESSING_H

#include <string>
#include <iostream>
#include <regex>

std::string parseGET(const std::string& req) {

    std::regex rgx("GET /(\\w.*) HTTP/1.1");
    std::smatch match;

    if (std::regex_search(req.begin(), req.end(), match, rgx)) return match[1];
    else return "";
}

#endif
