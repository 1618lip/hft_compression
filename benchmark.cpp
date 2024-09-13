#include "tick_data.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    string filename1 = "tick_data.csv"; // Replace with your actual file path
    vector<TickData> ticks1 = readTickDataFromCSV(filename1);
    
    string filename2 = "important_tick_data.csv"; // Replace with your actual file path
    vector<TickData> ticks2 = readTickDataFromCSV(filename2);

    // Compress the first and second halves for both the files
    cout << "Processing without denoising..." << endl;
    double no_dn = benchmarkCompression(ticks1);

    cout << "\nProcessing with Haar Wavelet denoising..." << endl;
    double dn = benchmarkCompression(ticks2);
    
    cout << "No Denoise: " <<no_dn << " vs Denoise: " << dn << endl;
    return 0;
}


