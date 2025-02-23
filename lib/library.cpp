#include "library.h"
void sort_elem(std::string &str)
{
    std::string res;
    auto iter1 = str.begin();
    auto iter2 = str.end();
    std::sort(iter1, iter2, [](char a, char b)
              { return a > b; });
    for (char &a : str)
    {
        if (a % 2 == 0)
        {
            res += "КВ";
        }
        else
        {
            res += a;
        }
    }
    str = res;
}
int sum(const std::string &str)
{
    int sum = 0;
    for (auto &a : str)
    {
        if (isdigit(a))
        {
            sum += (a - '0');
        }
    }
    return sum;
}
bool analyzer(const std::string &str)
{
    if(str.length() > 2 and stoi(str)%32 == 0)
    {
        return 1;
    }
    else{
        return 0;
    }
}