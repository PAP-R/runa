#pragma once

#include <any>
#include <map>
#include <string>

class ArgMap : public std::map<std::string, std::any> {
  public:
    template <class T>
    T get(std::string const &key) {
        if (!contains(key)) {
            return {};
        }

        return std::any_cast<T>(this->at(key));
    }
};