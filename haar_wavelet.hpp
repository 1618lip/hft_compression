#ifndef HAAR_WAVELET_HPP
#define HAAR_WAVELET_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

// Haar wavelet transform and denoising functions

// Function to perform 1D Haar Wavelet Transform (Single Level)
std::vector<double> haar_wavelet_transform(const std::vector<double>& signal);

// Function to perform inverse 1D Haar Wavelet Transform (Single Level)
std::vector<double> inverse_haar_wavelet_transform(const std::vector<double>& coeffs);

// Soft thresholding function
std::vector<double> soft_threshold(const std::vector<double>& coeffs, double threshold);

// Function to perform multi-level Haar wavelet decomposition
std::vector<double> multi_level_haar_wavelet_transform(std::vector<double> signal, int levels);

// Function to perform multi-level inverse Haar wavelet reconstruction
std::vector<double> multi_level_inverse_haar_wavelet_transform(std::vector<double> coeffs, int levels);

#endif // HAAR_WAVELET_HPP

