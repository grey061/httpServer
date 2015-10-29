#ifndef HTTP_PROCESSING_H
#define HTTP_PROCESSING_H

#include <string>
#include <iostream>
#include <fstream>
#include <regex>

std::string parseGET(const std::string& req) {

    std::regex rgx("GET /(\\w.*) HTTP/1.1");
    std::smatch match;

    if (std::regex_search(req.begin(), req.end(), match, rgx)) return match[1];
    else return "";
}

std::string LoadFile(std::ifstream &stream)
{
    std::string s;
    stream.seekg(0, std::ios::end);
    s.resize(stream.tellg());
    stream.seekg(0, std::ios::beg);
    stream.read(&s[0], s.size());

    return s;
}

std::string loadSite(const std::string& path) {

    std::ifstream site;
    site.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        site.open(path);
        return LoadFile(site);
    }
    catch (std::ifstream::failure e) {
        throw e;
    }
}

#endif
