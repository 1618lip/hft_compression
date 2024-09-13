# Arithmetic Coding Compression for HFT Tick Data with Denoising using Wavelet Transforms

This project demonstrates **Arithmetic Coding Compression** for **High-Frequency Trading (HFT)** tick data with **denoising** using **Haar Wavelet Transforms**. The tick data is compressed using arithmetic coding and the wavelet transforms help reduce noise in the data.

## Table of Contents
1. [Introduction](#introduction)
2. [Mathematical Background](#mathematical-background)
    - [Arithmetic Coding](#arithmetic-coding)
    - [Haar Wavelet Transform](#haar-wavelet-transform)
3. [Project Structure](#project-structure)
4. [How to Run](#how-to-run)
5. [Requirements](#requirements)
6. [Step-by-Step Instructions](#step-by-step-instructions)
7. [Sample Usage](#sample-usage)


## Introduction
High-Frequency Trading (HFT) generates massive amounts of tick data in real-time. Compressing this data efficiently while maintaining its integrity is crucial. **Arithmetic Coding** is a variable-length entropy encoding method that compresses data efficiently by encoding an entire message as a range of real numbers. 

To reduce noise in the tick data, **Haar Wavelet Transforms** are applied. Haar wavelets help in denoising the signal, preserving relevant data while removing noise.

This project combines arithmetic coding with wavelet-based denoising for optimal tick data compression.



## Mathematical Background

### Arithmetic Coding

**Arithmetic Coding** works by encoding a message into a single number between 0 and 1. The core idea is to represent the message as an interval $[low, high)$ that shrinks as more symbols are encoded.

Let:
- $P(x)$ be the probability of symbol $x$,
- $C(x)$ be the cumulative probability of symbols less than $x$.

For a message $m = x_1, x_2, \dots, x_n$, the encoding interval is progressively updated as follows:

#### Encoding Steps:
1. Initialize the interval as $[0, 1)$.
2. For each symbol $x_i$:
   - The new interval is given by:
     $$
     low = low + (high - low) \cdot C(x_i)
     $$
     $$
     high = low + (high - low) \cdot P(x_i)
     $$
3. After encoding all symbols, any number within the final interval $[low, high)$ can be used as the encoded message.

#### Decoding Steps:
Decoding is the reverse process. Given the encoded value, you retrieve each symbol by determining which interval the encoded number falls into.



### Haar Wavelet Transform

**Haar Wavelet Transform** (HWT) is used to decompose a signal into its approximation and detail components, which can then be used for denoising. 

Given a discrete signal $f[n]$, the Haar Wavelet Transform computes two sets of coefficients:
1. **Approximation Coefficients** $a[n]$: Represents the average.
2. **Detail Coefficients** $d[n]$: Represents the difference.

#### Haar Transform Equations:
For a signal $f = [f_0, f_1, f_2, f_3, \dots]$, the transform at level 1 is computed as:

$$
a[i] = \frac{f[2i] + f[2i+1]}{\sqrt{2}}
$$
$$
d[i] = \frac{f[2i] - f[2i+1]}{\sqrt{2}}
$$

The inverse transform reconstructs the original signal using:

$$
f[2i] = \frac{a[i] + d[i]}{\sqrt{2}}
$$
$$f[2i+1] = \frac{a[i] - d[i]}{\sqrt{2}}
$$

The **multi-level wavelet transform** repeatedly applies this operation to the approximation coefficients to further decompose the signal.


## Project Structure

```bash
.
├── README.md               # Project README
├── tick_data.hpp           # Tick data Header file
├── tick_data.cpp           # Tick data implementation
├── ae.hpp                  # Arithmetic Encoding & Decoding Header
├── ae.cpp                  # Arithmetic Encoding & Decoding Implementation
├── haar_wavelet.hpp        # Haar Wavelet Transform Header
├── haar_wavelet.cpp        # Haar Wavelet Transform implementation
├── benchmark.cpp           # Main execution file
├── generate.cpp            # Generate random tick data
├── tick_data.csv           # CSV file for tick data (generated)
├── important_tick_data.csv # CSV file filtered/denoised tick data
└──  Makefile                # Make file 
```


## How to Run

### Requirements
- C++ compiler supporting C++11 or higher
- Python 3.x (for visualization and plotting)
- `g++` for compiling C++ code
- Python packages: `pandas`, `matplotlib` (specified in `requirements.txt`)

### Step-by-Step Instructions

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/1618lip/hft_compression.git
   cd hft_compression
   ```

2. **Compile C++ Code**:
   Compile the project files (`tick_data.cpp`, `benchmark.cpp`, `ae.cpp`, `haar_wavelet.cpp`) using the `Makefile`:

   ```bash
   make
   ```
   If you see warnings, just ignore it. 

3. **Generate Tick Data**:
   To generate random tick data, run the `generate` program:

   ```bash
   g++ -o generate generate.cpp
   ./generate
   Enter the number of tick data points to generate: <enter an integer>
   Random tick data generated in tick_data.csv
   ```

   This will create a file called `tick_data.csv` with randomly generated tick data.

4. **Generate Denoised & Filtered Tick Data**:
    Once you have `tick_data.csv`, generate the denoised and filtered tick data: 

    ```bash
    g++ -o denoise generate_denoised_tick.cpp haar_wavelet.cpp
    ./denoise 
    ```

5. **Run Compression and Denoising**:
   After generating the data, use the `benchmark` program to compress and denoise the tick data:

   ```bash
   ./benchmark
   ```

   This will read the generated tick data, apply Haar Wavelet Transform for denoising, compress the data using arithmetic coding, and print the compression ratios.

6. **See Filtering Effect:** (Optional):
   To visualize the original vs filtered tick data using Python:
   ```bash
   python plot.py
   ```



## Sample Usage

```bash
# Step 0: Compile all Files
make

# Step 1: Generate 100 random tick data
./generate 100

# Step 2: Denoise & Filter generated tick data
./denoise

# Step 2: Compress the tick data with and without denoising
./benchmark

# Step 3: Visualize the tick data (optional, using Python)
python plot.py
```

This project demonstrates efficient compression for HFT tick data and integrates denoising using Haar Wavelet Transforms to preserve important data while removing noise.



## License
This project is licensed under the MIT License.
