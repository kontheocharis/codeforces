#include <iostream>
#include <vector>
#include <string>

bool ends_with(std::string& s, std::string ending)
{
    if (s.length() < ending.length()) return false;
    return (s.substr(s.length() - ending.length(), ending.length()).compare(ending) == 0);
}

int main(int argc, char *argv[])
{
    int test_count;
    std::cin >> test_count;

    std::vector<std::string> test_cases;

    for (int i = 0; i < test_count; ++i)
    {
        std::string test_case;
        std::cin >> test_case;
        test_cases.push_back(test_case);
    }

    for (auto& c : test_cases)
    {
        if (ends_with(c, "po"))
            std::cout << "FILIPINO\n";

        else if (ends_with(c, "desu") || ends_with(c, "masu"))
            std::cout << "JAPANESE\n";

        else if (ends_with(c, "mnida"))
            std::cout << "KOREAN\n";
    }

    return 0;
}
