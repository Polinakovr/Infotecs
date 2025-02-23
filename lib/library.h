#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <algorithm>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

    EXPORT void sort_elem(std::string &str);
    EXPORT int sum(const std::string &str);
    EXPORT bool analyzer(const std::string &str);

#ifdef __cplusplus
}
#endif

#endif