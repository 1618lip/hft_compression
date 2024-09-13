#include "haar_wavelet.hpp"

// Haar wavelet transform and denoising functions

// Function to perform 1D Haar Wavelet Transform (Single Level)
std::vector<double> haar_wavelet_transform(const std::vector<double>& signal) {
    int n = signal.size();
    std::vector<double> output(n);
    
    int half = n / 2;
    for (int i = 0; i < half; ++i) {
        output[i] = (signal[2 * i] + signal[2 * i + 1]) / std::sqrt(2.0);  // Approximation
        output[half + i] = (signal[2 * i] - signal[2 * i + 1]) / std::sqrt(2.0);  // Detail
    }
    
    return output;
}

// Function to perform inverse 1D Haar Wavelet Transform (Single Level)
std::vector<double> inverse_haar_wavelet_transform(const std::vector<double>& coeffs) {
    int n = coeffs.size();
    std::vector<double> output(n);
    
    int half = n / 2;
    for (int i = 0; i < half; ++i) {
        output[2 * i] = (coeffs[i] + coeffs[half + i]) / std::sqrt(2.0);  // Reconstruct original signal
        output[2 * i + 1] = (coeffs[i] - coeffs[half + i]) / std::sqrt(2.0);  // Reconstruct original signal
    }
    
    return output;
}

// Soft thresholding function
std::vector<double> soft_threshold(const std::vector<double>& coeffs, double threshold) {
    std::vector<double> thresholded_coeffs(coeffs.size());
    for (size_t i = 0; i < coeffs.size(); ++i) {
        if (std::abs(coeffs[i]) > threshold) {
            thresholded_coeffs[i] = std::copysign(std::abs(coeffs[i]) - threshold, coeffs[i]);
        } else {
            thresholded_coeffs[i] = 0;
        }
    }
    return thresholded_coeffs;
}

// Function to perform multi-level Haar wavelet decomposition
std::vector<double> multi_level_haar_wavelet_transform(std::vector<double> signal, int levels) {
    for (int i = 0; i < levels; ++i) {
        signal = haar_wavelet_transform(signal);
    }
    return signal;
}

// Function to perform multi-level inverse Haar wavelet reconstruction
std::vector<double> multi_level_inverse_haar_wavelet_transform(std::vector<double> coeffs, int levels) {
    for (int i = 0; i < levels; ++i) {
        coeffs = inverse_haar_wavelet_transform(coeffs);
    }
    return coeffs;
}

// int main() {
//     const int wavelet_depth = 4; // Number of decomposition levels
//     const double threshold = 0.5; // Threshold value for soft-thresholding

//     // Read the tick data from CSV
//     std::string filename = "tick_data.csv"; // Use the generated tick data
//     std::vector<TickData> tick_data = readTickDataFromCSV(filename);

//     // Extract price and volume as doubles for wavelet transformation
//     std::vector<double> price_values;
//     std::vector<double> volume_values;

//     for (const auto& tick : tick_data) {
//         price_values.push_back(stod(tick.price));  // Convert price to double
//         volume_values.push_back(stod(tick.volume));  // Convert volume to double
//     }

//     // Perform multi-level Haar wavelet decomposition on prices and volumes
//     std::vector<double> denoised_prices = multi_level_haar_wavelet_transform(price_values, wavelet_depth);
//     std::vector<double> denoised_volumes = multi_level_haar_wavelet_transform(volume_values, wavelet_depth);

//     // Apply soft thresholding
//     denoised_prices = soft_threshold(denoised_prices, threshold);
//     denoised_volumes = soft_threshold(denoised_volumes, threshold);

//     // Reconstruct the denoised signals
//     denoised_prices = multi_level_inverse_haar_wavelet_transform(denoised_prices, wavelet_depth);
//     denoised_volumes = multi_level_inverse_haar_wavelet_transform(denoised_volumes, wavelet_depth);

//     // Write the original and denoised tick data to a CSV file
//     std::ofstream output_file("denoised_tick_data.csv");
//     output_file << "time_index,original_price,original_volume,denoised_price,denoised_volume,type\n";
//     for (size_t i = 0; i < tick_data.size(); ++i) {
//         output_file << tick_data[i].time_index << "," 
//                     << tick_data[i].price << "," << tick_data[i].volume << ","
//                     << denoised_prices[i] << "," << denoised_volumes[i] << "," << tick_data[i].type << "\n";
//     }
//     output_file.close();

//     std::cout << "Denoised tick data written to denoised_tick_data.csv" << std::endl;

//     return 0;
// }




