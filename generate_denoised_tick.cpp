#include "haar_wavelet.hpp"
#include "tick_data.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

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

// Export important tick data to a CSV file (only keeping the important ticks)
void exportImportantTickDataToCSV(const string& filename, const vector<TickData>& important_ticks) {
    ofstream file(filename);

    // Write the header
    file << "Timestamp,Symbol,Price,Volume,Exchange\n";

    // Write the important data points (those above threshold)
    for (const auto& tick : important_ticks) {
        file << tick.timestamp << "," << tick.symbol << "," << tick.price << "," << tick.volume << "," << tick.exchange << "\n";
    }

    file.close();
    cout << "Important tick data written to " << filename << endl;
}

// Main function to read tick data, denoise it, and remove less important samples
int main() {
    string input_filename = "tick_data.csv";     // Input CSV file with tick data
    string output_filename = "important_tick_data.csv";  // Output CSV file for important data
    double price_threshold = 150;    // Threshold for price (adjust as needed)
    double volume_threshold = 150;  // Threshold for volume (adjust as needed)
    double haar_level = 4;
    // Read the tick data from CSV
    vector<TickData> original_ticks = readTickDataFromCSV(input_filename);
    vector<TickData> important_ticks;  // To store important tick data
    vector<double> prices;
    vector<double> volumes;
    
    for (const auto& tick: original_ticks) {
        prices.push_back({static_cast<double>(tick.price)});
        volumes.push_back({static_cast<double>(tick.volume)});
    }
    // PRICES
    vector<double> transformed_prices = multi_level_haar_wavelet_transform(prices, haar_level);  // 3-level Haar transform
    vector<double> thresholded_prices = soft_threshold(transformed_prices, 0.5*price_threshold);        // Apply soft thresholding
    vector<double> denoised_prices = multi_level_inverse_haar_wavelet_transform(thresholded_prices, haar_level); // Reconstruct the denoised signal

    // VOLUMES
    vector<double> transformed_volumes = multi_level_haar_wavelet_transform(volumes, haar_level);  // 3-level Haar transform
    vector<double> thresholded_volumes = soft_threshold(transformed_volumes, 0.5*volume_threshold);        // Apply soft thresholding
    vector<double> denoised_volumes = multi_level_inverse_haar_wavelet_transform(thresholded_volumes, haar_level); // Reconstruct the denoised signal

    vector<double> new_prices;
    // Remove less important samples by setting them to zero (using the thresholded signal)
    for (size_t i = 0; i < denoised_prices.size(); ++i) {
        if (abs(denoised_prices[i]) < price_threshold) {
            denoised_prices[i] = 0;  // Remove less important samples (set to zero)
        }
        if (abs(denoised_volumes[i]) < volume_threshold) {
            denoised_volumes[i] = 0;  // Remove less important samples (set to zero)
        }
    }
    int i = 0;
    for (const auto& tick: original_ticks) {
        if (denoised_prices[i] != 0 && denoised_volumes[i] != 0) {
            TickData important_tick = tick;
            important_tick.price = static_cast<int>(denoised_prices[i]);
            important_tick.volume = static_cast<int>(denoised_volumes[i]);
            important_ticks.push_back(important_tick);
        }
        i++;
    }
    // Export only the important tick data to a CSV file
    exportImportantTickDataToCSV(output_filename, important_ticks);

    return 0;
}


