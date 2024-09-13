#ifndef TICK_DATA_HPP
#define TICK_DATA_HPP

#include <string>
#include <vector>


struct TickData {
    std::string timestamp;  // Timestamp in the format YYYY-MM-DD HH:MM:SS.mmm
    std::string symbol;     // Stock symbol (e.g., AAPL, TSLA)
    double price;           // Stock price
    int volume;             // Trade volume
    std::string exchange;   // Stock exchange (e.g., NASDAQ, NYSE)
};

// Function to read tick data from a CSV file
std::vector<TickData> readTickDataFromCSV(const std::string& filename);

// Function to export original and denoised tick data to a CSV file
void exportDenoisedDataToCSV(const std::string& filename, const std::vector<TickData>& original_ticks,
                             const std::vector<double>& denoised_prices, const std::vector<double>& denoised_volumes);

// Function to encode TickData into a string format for compression (used in other files)
std::string encodeTickData(const TickData& tick);

// Optional functions for compression or denoising that may be used by other cpp files
std::string convertTagToBinary(double tag, int precision);
long double binaryStringToTag(const std::string& binaryString);

// Benchmarks the compression ratio by compressing and encoding the tick data
double benchmarkCompression(const std::vector<TickData>& ticks);
#endif  // TICK_DATA_HPP




