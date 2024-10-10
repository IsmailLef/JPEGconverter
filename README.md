# JPEG to PPM Encoding Project

## Project Overview

This project focuses on encoding images from PPM  (Portable Pixmap) format to JPEG format. The process involves several modules, each responsible for a specific part of the encoding pipeline. The goal is to efficiently convert and process image data, leveraging various techniques to ensure high performance and accuracy.


## Major Steps
The project was developed in two major steps:

### 1. Gray Level Conversion:
- The first step involved creating a binary named ppm2jpg_gray that converts PPM images to JPEG format in gray levels.

### 2. Color Conversion:
- The second and final step was to develop a binary named ppm2jpg_color that performs the conversion with full color support.

## Functional Modules

### 1. Command Line Parameter Management (Module traitement)

- **Input:** Command line arguments
- **Output:** File path, new image name, and sampling factors
- **Description:** This module handles the command line parameters, extracting necessary information such as the file path, new image name, and sampling factors for further processing.

### 2. PPM File Handling (Module PPM_FILE)

- **Input:** Image name
- **Output:** Pixel matrix and image dimensions
- **Description:** Reads data from a PPM file and organizes it into a pixel matrix, also capturing the dimensions of the image.

### 3. Data Segmentation (Module decoupage)

- **Input:** Pixel matrix from PPM_FILE
- **Output:** Matrix of Minimum Coded Units (MCUs)
- **Description:** Segments the pixel matrix into MCUs, which are smaller blocks of data used in the encoding process.

### 4. RGB to YCbCr Conversion (Module RGB)

- **Input:** MCUs with RGB components
- **Output:** MCUs with YCbCr components
- **Description:** Converts the RGB components of the pixels into YCbCr components, which are more suitable for compression.

### 5. Discrete Cosine Transform (DCT) (Module DCT)

- **Input:** YCbCr matrix
- **Output:** Frequency domain matrix
- **Description:** Applies the Discrete Cosine Transform to convert the spatial domain data into the frequency domain, which is essential for compression.

### 6. Zigzag Transformation (Module zigzag)

- **Input:** Matrix
- **Output:** Zigzag vector
- **Description:** Transforms the matrix into a vector using a zigzag pattern, optimizing the data for further compression steps.

### 7. Quantization (Module Quantification)

- **Input:** Matrix
- **Output:** Quantized Y or Y and C components
- **Description:** Performs quantization on the matrix, reducing the precision of the data to achieve compression.

### 8. AC and DC Coding (Module Codage AC_RLE et DC)

- **Input:** MCUs
- **Output:** Encoded data stream (RLE and DC)
- **Description:** Handles the encoding of data using Run-Length Encoding (RLE) for AC coefficients and differential coding for DC coefficients.

### 9. Downsampling (Module downsampling)

- **Input:** MCU
- **Output:** Downsampled MCU
- **Description:** Reduces the resolution of the chrominance components to further compress the data.

### 10. Compression (Module compression)

- **Input:** PPM image
- **Output:** JPEG image
- **Description:** This module integrates all other modules to perform the complete transformation of a PPM image into a JPEG image.