#ifndef AE_HPP
#define AE_HPP

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <tuple>

using namespace std;

class ArithmeticEncoding {
    public:
        ArithmeticEncoding(map<char, double> frequency_table, bool save_stages = false);

        map<char, double> get_probability_table(const map<char, double>& frequency_table);

        tuple<long double, long double, long double> get_encoded_value(const map<char, vector<long double>>& last_stage_probs);

        map<char, vector<long double>> process_stage(const map<char, double>& probability_table, long double stage_min, long double stage_max);

        tuple<long double, vector<map<char, vector<long double>>>, long double, long double> encode(const string& msg, const map<char, double>& probability_table);

        void rescale(long double &stage_min, long double &stage_max);

        tuple<string, vector<map<char, vector<long double>>>> decode(long double encoded_msg, int msg_length, const map<char, double>& probability_table);

    private:
        map<char, double> probability_table;
        bool save_stages;
    };

#endif // AE_HPP

