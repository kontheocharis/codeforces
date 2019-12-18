#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>

long count_beautiful_years(long endpoint)
{
    long answer = 0;
    long no_of_digits = std::to_string(endpoint).length();
    answer += 9 * (no_of_digits - 1);

    for (int i = 9; i >= 1; --i)
    {
        double curr = 0;

        for (long a = 0; a < no_of_digits; ++a) 
        {
            curr += i * pow(10, a);
        }


        if (curr <= endpoint)
        {
            answer += i;
            break;
        }
    }

    return answer;
}

int main(int argc, char *argv[])
{
    long test_cases;
    std::cin >> test_cases;

    std::vector<long> points;

    for (int i = 0; i < test_cases; ++i)
    {
        long point;
        std::cin >> point;
        points.push_back(point);
    }

    for (auto& p : points)
    {
        std::cout << count_beautiful_years(p) << '\n';
    }

    return 0;
}
