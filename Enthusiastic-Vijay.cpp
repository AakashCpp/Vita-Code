#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>
#include <limits>

bool isSmaller(const std::string& s1, const std::string& s2) {
    if (s1.length() < s2.length()) return true;
    if (s1.length() > s2.length()) return false;
    return s1 < s2;
}

std::string subtract(std::string num1, std::string num2) {
    std::string diff = "";
    int n1 = num1.length(), n2 = num2.length();
    
    std::reverse(num1.begin(), num1.end());
    std::reverse(num2.begin(), num2.end());

    int borrow = 0;
    for (int i = 0; i < n2; i++) {
        int sub = (num1[i] - '0') - (num2[i] - '0') - borrow;
        if (sub < 0) {
            sub = sub + 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        diff += std::to_string(sub);
    }

    for (int i = n2; i < n1; i++) {
        int sub = (num1[i] - '0') - borrow;
        if (sub < 0) {
            sub = sub + 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        diff += std::to_string(sub);
    }

    std::reverse(diff.begin(), diff.end());

    size_t first_digit = diff.find_first_not_of('0');
    if (std::string::npos == first_digit) {
        return "0";
    }
    return diff.substr(first_digit);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::map<std::string, char> digit_map = {
        {" _ | ||_ ", '0'}, {"     |  |", '1'}, {" _  _||_ ", '2'},
        {" _  _| _|", '3'}, {"   |_|  |", '4'}, {" _ |_  _|", '5'},
        {" _ |  |_|", '6'}, {" _   |  |", '7'}, {" _ |_||_|", '8'},
        {" _ |_| _|", '9'}
    };

    std::string line1, line2, line3;
    int k;

    std::getline(std::cin, line1);
    std::getline(std::cin, line2);
    std::getline(std::cin, line3);
    std::cin >> k;
    
    std::string original_number_str = "";
    int num_digits = (line1.length() + 1) / 4;

    for (int i = 0; i < num_digits; ++i) {
        int start = i * 4;
        std::string key = line1.substr(start, 3) + 
                          line2.substr(start, 3) + 
                          line3.substr(start, 3);
        
        if (digit_map.count(key)) {
            original_number_str += digit_map[key];
        } else {
            original_number_str += "?";
        }
    }

    std::vector<std::pair<char, char>> toggle_rules = {
        {'4', '0'}, {'7', '1'}, {'8', '7'}
    };

    std::string modified_number_str = original_number_str;
    std::set<int> toggled_indices;
    int toggles_done = 0;

    while (toggles_done < k) {
        int best_index = -1;
        char best_new_digit = ' ';
        bool found_toggle_this_step = false;

        for (const auto& rule : toggle_rules) {
            char old_digit = rule.first;
            char new_digit = rule.second;

            for (int i = 0; i < num_digits; ++i) {
                if (toggled_indices.find(i) == toggled_indices.end() && 
                    modified_number_str[i] == old_digit) {
                    
                    best_index = i;
                    best_new_digit = new_digit;
                    found_toggle_this_step = true;
                    break;
                }
            }
            if (found_toggle_this_step) {
                break;
            }
        }
        
        if (best_index != -1) {
            modified_number_str[best_index] = best_new_digit;
            toggled_indices.insert(best_index);
            toggles_done++;
        } else {
            break;
        }
    }

    std::string anagram_str = modified_number_str;
    std::next_permutation(anagram_str.begin(), anagram_str.end());

    std::string difference_str;
    if (isSmaller(original_number_str, anagram_str)) {
        difference_str = subtract(anagram_str, original_number_str);
    } else {
        difference_str = subtract(original_number_str, anagram_str);
    }
    
    std::cout << difference_str << "\n";

    return 0;
}