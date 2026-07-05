# Digital Image Processing App (MFC)

A feature-rich Windows desktop application built with C++ and Microsoft Foundation Classes (MFC). It implements a wide range of digital image processing techniques — from basic point operations to advanced spatial-domain filters and geometric transforms.

---

## ✨ Features

### 🟥 Point Processing
*   Brightness & Contrast adjustment
*   Gamma correction
*   Histogram equalization & histogram graph view
*   Posterization
*   Bit-plane slicing
*   Color balance (RGB channel adjustment)
*   HSI (Hue, Saturation, Intensity) adjustment
*   YCbCr color space adjustment
*   Contrast stretching
*   Solarization
*   Color invert

### 🟩 Neighborhood Processing & Spatial Domain Filtering
*   Average (box) blur
*   Gaussian blur
*   Low-pass / high-pass filtering
*   Median filter
*   Edge detection: Sobel, Kirsch, Prewitt, Scharr, Roberts cross, Laplacian, Laplacian of Gaussian
*   Canny edge detector
*   Frei-Chen operator
*   Difference of Gaussians
*   Marr-Hildreth
*   Kayyali operator
*   Robinson compass operator
*   Nevatia-Babu
*   Deriche filter
*   Unsharp masking
*   Emboss filter
*   Gradient magnitude
*   Bilateral filter
*   Wiener filter
*   Morphological operations: erosion, opening, closing, morphological gradient

### 🟦 Transform Operations
*   Flip horizontal / vertical
*   Rotate 90° / 180° / 270°
*   **Arbitrary-angle rotation (0°–360°):** Features a live, real-time canvas preview while dragging the slider.

---

## 🛠️ Built With
*   **Language:** C++
*   **Framework:** MFC (Microsoft Foundation Classes)
*   **Graphics Engine:** GDI / GDI+ (for image rendering and arbitrary-angle rotation)
*   **Image Handling:** `CImage` for pixel-level image manipulation
*   **IDE:** Visual Studio 2022 / 2026

---

## 🚀 Getting Started

### Prerequisites
*   Windows 10/11
*   Visual Studio 2022 or later (with the **Desktop development with C++** and **MFC** workloads installed)

### Build & Run
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/praveen6150/digital-image-processing-mfc.git](https://github.com/praveen6150/digital-image-processing-mfc.git)
