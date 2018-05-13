#include <iostream>
#include <string.h>
#include <vector>


/*
 * Class finds the indexes where certain string is
 * entering to some given string and puts answer
 * to indexes_of_string_ vector
 *
 */

class IndexesOfStringEnterFinder {
public:
    /*
     * Takes to string big_string - string we we must search a small_string
     * Searches this indexes and puts them to the privat vector indexes_of_string
     */
    IndexesOfStringEnterFinder(const std::string &big_string, const std::string &small_string);

    /*
     * Gives Indexes of String entering to a big one in vector<size_t> format
     */
    std::vector<size_t>& GetIndexesOfString();
private:
    /*
     * Indexes where smal_string enters to big one
     */
    std::vector<size_t> indexes_of_string_;
};

IndexesOfStringEnterFinder::IndexesOfStringEnterFinder(const std::string &big_string, const std::string &small_string) {
    size_t length_of_small_string = small_string.length();
    std::vector<size_t> prefix_counter(small_string.length(), 0);
    indexes_of_string_.resize(0);

    for (size_t i = 1; i < length_of_small_string; i++) {
        prefix_counter[i] = prefix_counter[i - 1];

        while (small_string[i] != small_string[prefix_counter[i]]
               && prefix_counter[i] > 0) {
            prefix_counter[i] = prefix_counter[prefix_counter[i] - 1];
        }

        if (small_string[i] == small_string[prefix_counter[i]]) {
            prefix_counter[i]++;
        }
    }

    size_t prev_prefix = 0;
    for (size_t i = 0; i < big_string.length(); i++) {

// We can be out of bound!!!
        if (prev_prefix == length_of_small_string) {
            prev_prefix = prefix_counter[prev_prefix - 1];
        }

        while (small_string[prev_prefix] != big_string[i]
               && prev_prefix != 0) {
            prev_prefix = prefix_counter[prev_prefix - 1];
        }

        if (small_string[prev_prefix] == big_string[i]) {
            prev_prefix++;
        }

        if (prev_prefix == length_of_small_string) {
            indexes_of_string_.push_back(i + 1 -  length_of_small_string);
        }
    }
}

std::vector<size_t>& IndexesOfStringEnterFinder::GetIndexesOfString() {
    return indexes_of_string_;
}


int main() {
    std::string a, b;
    std::cin >> a >> b;
    IndexesOfStringEnterFinder finder = IndexesOfStringEnterFinder(b, a);
    const std::vector<size_t>& answer = finder.GetIndexesOfString();

    for (size_t i = 0; i < answer.size(); i++) {
        std::cout << answer[i] << " ";
    }
}
