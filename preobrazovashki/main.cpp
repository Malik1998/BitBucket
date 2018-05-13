#include <iostream>
#include <vector>


/*
 * The size of the English alphabet, don't touch
 *
 */
const size_t alph_size = 26;


/*
 * Returns the lowest lexigrpahic string from z-function
 *
 */
std::string FromZToText(const std::vector<size_t> &suffixes) {

    std::vector<size_t> left_indexes(suffixes.size(), 0);
    std::string found_string;
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
                            found_string[i] = static_cast<char> (j + 'a');
                            break;
                        }
                    }
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
    return found_string;
}

/*
 * Returns the lowest lexigrpahic string from prefix-function
 *
 */

std::string FromPrefixToText(const std::vector<size_t> &prefix) {

    std::string found_string;
    found_string.resize(prefix.size());
    found_string[0] = 'a';

    for (size_t i = 1; i < prefix.size(); i++) {
        if (prefix[i] == 0) {
            std::vector<bool> used(alph_size, false);
            size_t prev = prefix[i - 1];
            while (prev > 0 ) {
                used[found_string[prev] - 'a'] = true;
                prev = prefix[prev - 1];
            }
            for (size_t j = 1; j < alph_size; j++) {
                if (!used[j]) {
                    found_string[i] = static_cast<char> (j + 'a');
                    break;
                }
            }
        } else {
            found_string[i] = found_string[prefix[i] - 1];
        }
    }
    return found_string;
}

/*
 * Returns the prefix-function valuse in vector from z-function
 *
 */

std::vector<size_t> FromZToPrefix(const std::vector<size_t> &suffixes) {
    std::vector<size_t> prefix(suffixes.size(), 0);

    for (size_t i = 1; i < suffixes.size(); i++) {
        if (suffixes[i] > 0 && prefix[i + suffixes[i] - 1] == 0) {
            prefix[i + suffixes[i] - 1] = suffixes[i];
        }
    }

    size_t cur_suf = 0;
    for (size_t i = suffixes.size() - 1; i > 0; i--) {
        if (cur_suf > 0 && prefix[i] == 0) {
            prefix[i] = cur_suf;
            cur_suf--;
        } else if (prefix[i] > 0) {
            cur_suf = static_cast<size_t >(std::max(static_cast<int>(prefix[i]) - 1, static_cast<int>(cur_suf) - 1));
        }
    }

    return prefix;
}

/*
 * Returns the prefix-function from text
 *
 */

std::vector<size_t> FromTextToPrefix(const std::string &a) {
    std::vector<size_t> prefix(a.length(), 0);

    for (size_t i = 1; i < a.length(); i++) {
        prefix[i] = prefix[i - 1];
        while (prefix[i] > 0 && a[prefix[i]] != a[i]) {
            prefix[i] = prefix[prefix[i] - 1];
        }
        if (a[prefix[i]] == a[i]) {
            prefix[i]++;
        }
    }

    return prefix;
}

/*
 * Returns the z-fucntion string from text
 */

std::vector<size_t> FromTextToZ(const std::string &a) {
    std::vector<size_t> Z(a.length(), 0);
    Z[0] = a.length();
    size_t l = 0, r = 0;

    for (size_t i = 1; i < a.length(); i++) {
        if (i <= r) {
            Z[i] = std::min(r - i + 1, Z[i - l]);
        }
        while (i + Z[i] < Z[0] && a[Z[i]] == a[i]) {
            Z[i]++;
        }
        if (i + Z[i] > r + 1) {
            l = i;
            r = i + Z[i] - 1;
        }
    }

    return Z;
}

/*
 * Returns the z-function string from prefix-function
 *
 */

std::vector<size_t> FromPrefixToZ(const std::vector<size_t> &suffixes) {
    return FromTextToZ(FromPrefixToText(suffixes));
}

int main() {

}
