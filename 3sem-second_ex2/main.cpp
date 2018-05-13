#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>


/*
 * Size of english alphabet don't touch!
 */
const int alph_size = 26;

/*
 *
 * Find lowest lexigraphic string that has some suffixe function values as given
 */

class SmallestStringFinder {
public:
    /*
     * Initlize out class and start to find this string and put answer to found_string vector
     */
    explicit SmallestStringFinder(const std::vector<size_t>& suffixes);

    /*
     * returns string from found_string
     */
    std::string GetString() const;
    /*
     * find unused Lowest alphabet symbols
     */
    char FindUnused(size_t l, size_t r, std::vector<size_t> &left_indexes);
private:
    /*
     * answer to question
     */
    std::vector<char> found_string;
};

char SmallestStringFinder::FindUnused(size_t l, size_t r, std::vector<size_t> &left_indexes) {
    std::vector<bool> used(alph_size, false);
    size_t prev = r - l + 1;
    while (prev > 0) {
        used[found_string[prev] - 'a'] = true;
        if (left_indexes[prev] == 0) {
            break;
        }
        prev = prev - left_indexes[prev];
    }
    for (size_t j = 1; j < alph_size; j++) {
        if (!used[j]) {
            return static_cast<char>(j + 'a');
        }
    }
}

SmallestStringFinder::SmallestStringFinder(const std::vector<size_t>& suffixes) {

    std::vector<size_t> left_indexes(suffixes.size(), 0);
    found_string.resize(suffixes.size());
    found_string[0] = 'a';

    size_t l = 0;
    size_t r = 0;
    for (size_t i = 1; i < suffixes.size(); i++) {
        if (suffixes[i] == 0) {
            if (r >= i) {
                found_string[i] = found_string[i - l];
            } else {
                if (r + 1 == i && l != 0) {
                    found_string[i] = FindUnused(l, r, left_indexes);
                } else {
                    found_string[i] = 'b';
                }
            }
        } else {
            found_string[i] = found_string[0];
            if (suffixes[i] + i - 1 > r) {
                r = suffixes[i] + i - 1;
                l = i;
            }
            if (left_indexes[i + suffixes[i]] == 0) {
                left_indexes[i + suffixes[i]] = i;
            }
        }
    }
}

std::string SmallestStringFinder::GetString() const {
    return std::string(found_string.begin(), found_string.end());
}


int main() {

    freopen("input.txt", "r", stdin);

    std::vector<size_t> z; // Values of z-function

    size_t new_num; // temp value to store values of z-function
    while (std::cin >> new_num) {
        z.push_back(new_num); // reading z-function value and put ot to the vector
    }

    SmallestStringFinder smallestStringFinder = SmallestStringFinder(z); // Find the lowest string that can have such z-function values
    std::cout << smallestStringFinder.GetString(); // show this string
}
