#include "tick_data.hpp"
#include "ae.hpp"
#include "haar_wavelet.hpp"

#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>
#include <numeric>
#include <limits>
#include <cmath>

using namespace std;
using namespace std::chrono;

// Function to convert a tag to binary string representation
string convertTagToBinary(double tag, int precision) {
    double low = 0.0;
    double high = 1.0;
    string binaryString = "";

    for (int i = 0; i < precision; ++i) {
        double mid = (low + high) / 2;

        if (tag < mid) {
            binaryString += '0';
            high = mid;  // Update high boundary
        } else {
            binaryString += '1';
            low = mid;  // Update low boundary
        }

        if (high - low < 1e-64) {  // Precision threshold
            break;
        }
        string s0 = "000000000";
        if (binaryString.find(s0) != std::string::npos) {
            binaryString = binaryString.substr(0, binaryString.length() - s0.length());
            break;
        }
        string s1 = "111111111";
        if (binaryString.find(s1) != std::string::npos) {
            binaryString = binaryString.substr(0, binaryString.length() - s1.length() - 1);
            break;
        }
    }
    return binaryString;
}

// Converts a binary string back to a tag
long double binaryStringToTag(const string& binaryString) {
    long double tag = 0.0;
    double fraction = 0.5;

    for (char bit : binaryString) {
        if (bit == '1') {
            tag += fraction;
        }
        fraction /= 2.0;
    }

    return tag;
}

// Function to encode TickData into a string format for compression
string encodeTickData(const TickData& tick) {
    stringstream ss;
    ss << tick.timestamp << tick.symbol << tick.price 
       << tick.volume << tick.exchange;
    return ss.str();
}

string tickToInts(TickData tick) {
    string toReturn = "";
    string t = encodeTickData(tick);
    for (int i = 0; i < t.length(); i++) {
        if (t[i] == ':') {
            continue;
        }
        toReturn += to_string(int(t[i]));
    }
    return toReturn;
}
// Function to read tick data from a CSV file
vector<TickData> readTickDataFromCSV(const string& filename) {
    vector<TickData> ticks;
    ifstream file(filename);
    string line;

    getline(file, line); // Skip header

    while (getline(file, line)) {
        if (line.empty()) {
            break;
        }
        stringstream ss(line);
        string timestamp, symbol, price, volume, exchange;
        
        getline(ss, timestamp, ',');
        getline(ss, symbol, ',');
        getline(ss, price, ',');
        getline(ss, volume, ',');
        getline(ss, exchange, ',');

        ticks.push_back({timestamp, symbol, stod(price), stoi(volume), exchange});
    }

    return ticks;
}

// Function to calculate the compression ratio without denoising (one tick at a time)
double benchmarkCompression(const vector<TickData>& ticks) {
    double uncompressed = 0;
    double compressed = 0;

    for (const auto& tick : ticks) {
        string coded_tick = tickToInts(tick);
        string first_half = coded_tick.substr(0, coded_tick.size()/2);
        string second_half = coded_tick.substr(coded_tick.size()/2, coded_tick.size());
        //cout << "Original Tick: " << first_half + " " + second_half << endl;
        map<char, double> frequency_table;

        for (char c : first_half) {
            frequency_table[c]++;
        }

        uncompressed += 4*coded_tick.size();
        // Create ArithmeticEncoding object for first half
        ArithmeticEncoding AE1(frequency_table, false);
        auto probability_table1 = AE1.get_probability_table(frequency_table);

        // Encode 
        auto [encoded_tag1, encoder1, interval_min_value1, interval_max_value1] = AE1.encode(first_half, probability_table1);
        string binaryString1 = convertTagToBinary(encoded_tag1, 64);

        compressed += binaryString1.size();

        // Decode the first half
        //double decoded_tag1 = binaryStringToTag(binaryString1);
        auto [decoded_msg_vector1, decoder1] = AE1.decode(encoded_tag1, first_half.size(), probability_table1);
        string decoded_msg1 = decoded_msg_vector1;
        //cout << "Decoded tick: " << decoded_msg1 << endl;
        // Verify decoding of the first half
        if (first_half != decoded_msg1) {
            cout << "Warning: Decoded 1st half tick doesn't match original!" << endl;
        } else {
            cout << "1st half Tick decoded successfully!" << endl;
        }
        frequency_table.clear();
        for (char c : second_half) {
            frequency_table[c]++;
        }

        // Create ArithmeticEncoding object for first half
        ArithmeticEncoding AE2(frequency_table, false);
        auto probability_table2 = AE2.get_probability_table(frequency_table);

        // Encode 
        auto [encoded_tag2, encoder2, interval_min_value2, interval_max_value2] = AE2.encode(second_half, probability_table2);
        string binaryString2 = convertTagToBinary(encoded_tag2, 64);

        compressed += binaryString2.size();

        // Decode the first half
        //double decoded_tag2 = binaryStringToTag(binaryString2);
        auto [decoded_msg_vector2, decoder2] = AE2.decode(encoded_tag2, second_half.size(), probability_table2);
        string decoded_msg2 = decoded_msg_vector2;
        //cout << "Decoded tick: " << decoded_msg2 << endl;
        // Verify decoding of the first half
        if (second_half != decoded_msg2) {
            cout << "Warning: Decoded 2nd half doesn't match original!" << endl;
        } else {
            cout << "2nd half decoded successfully!" << endl;
        }
    }
    return compressed/uncompressed;
}






