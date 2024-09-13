#include "tick_data.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <iomanip> // for std::setprecision
#include <chrono>  // for timestamps
#include <random>  // for generating random intervals

using namespace std;

// List of stock symbols
const vector<string> stock_symbols = {"AAPL", "TSLA", "GOOG", "AMZN", "MSFT", "NFLX", "NVDA", "INTC", "AMD", "BOSE", "JS", "HRT", "META", "QLCM"};

// List of stock exchanges
const vector<string> stock_exchanges = {"NASDAQ", "NYSE", "AMEX", "BSE", "NSX", "PHLX"};

// Generate a random price in the range [100, 500]
int generateRandomPrice() {
    return 100 + static_cast<int>(static_cast<double>(rand()) / RAND_MAX * 400);
}

// Generate a random volume in the range [50, 500]
int generateRandomVolume() {
    return 50 + rand() % 451;  // Random volume between 50 and 500
}

// Generate a random stock symbol from the list
string generateRandomSymbol() {
    return stock_symbols[rand() % stock_symbols.size()];
}

// Generate a random stock exchange from the list
string generateRandomExchange() {
    return stock_exchanges[rand() % stock_exchanges.size()];
}

// Generate a random interval in milliseconds (between 1 ms and 2000 ms)
long long generateRandomInterval() {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(100, 2000);  // Random intervals between 100 ms and 2000 ms (2 seconds)
    return dist(gen);
}

// Generate a timestamp with realistic random increments
string generateTimestamp(chrono::system_clock::time_point& current_time) {
    // Increment the time by a random interval
    current_time += chrono::milliseconds(generateRandomInterval());

    time_t tick_time_t = chrono::system_clock::to_time_t(current_time);
    tm* time_info = localtime(&tick_time_t);

    // Format the timestamp: YYYY-MM-DD HH:MM:SS.mmm
    char buffer[30];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", time_info);

    // Append milliseconds
    auto ms = chrono::duration_cast<chrono::milliseconds>(current_time.time_since_epoch()).count() % 1000;
    stringstream timestamp;
    ms /= 100;
    timestamp << buffer << '.' << setfill('0') << setw(1) << ms;

    return timestamp.str();
}

// Function to generate random tick data and write it to a CSV file
void generateRandomTickData(const string& filename, int num_ticks) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file for writing!" << endl;
        return;
    }

    // Write header
    file << "Timestamp,Symbol,Price,Volume,Exchange\n";

    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Start time for the first tick
    chrono::system_clock::time_point current_time = chrono::system_clock::now();

    // Generate random tick data
    for (int i = 0; i < num_ticks; ++i) {
        string timestamp = generateTimestamp(current_time);
        string symbol = generateRandomSymbol();
        double price = generateRandomPrice();
        int volume = generateRandomVolume();
        string exchange = generateRandomExchange();

        // Write the generated tick data to the file
        file << timestamp << ","
             << symbol << ","
             << fixed << setprecision(0) << price << "," // Price with 2 decimal places
             << volume << ","
             << exchange << "\n";
    }

    file.close();
}

int main() {
    int n;
    std::cout << "Enter the number of tick data points to generate: ";
    std::cin >> n;
    string filename = "tick_data.csv";

    // Generate random tick data
    generateRandomTickData(filename, n);

    cout << "Random tick data generated in " << filename << endl;
    return 0;
}


