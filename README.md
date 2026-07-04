Digital Image Processing
A Windows desktop application built with MFC (Microsoft Foundation Classes) and C++, implementing a wide range of digital image processing techniques — from basic point operations to advanced spatial-domain filters and geometric transforms.
---
✨ Features
Point Processing
Brightness & Contrast adjustment
Gamma correction 
Histogram equalization & histogram graph view
Posterization
Bit-plane slicing
Color balance (RGB channel adjustment)
HSI (Hue, Saturation, Intensity) adjustment
YCbCr color space adjustment
Contrast stretching
Solarization
Color invert
Neighborhood Processing
Average (box) blur
Gaussian blur
Low-pass / high-pass filtering
Median filter
Edge detection: Sobel, Kirsch, Prewitt
Spatial Domain Filtering
Canny edge detector
Scharr edge detection
Laplacian of Gaussian
Laplacian filter
Frei-Chen operator
Roberts cross operator
Difference of Gaussians
Marr-Hildreth
Kayyali operator
Robinson compass operator
Nevatia-Babu
Deriche filter
Unsharp masking
Emboss filter
Gradient magnitude
Bilateral filter
Wiener filter
Morphological operations: erosion, opening, closing, morphological gradient
Transform
Flip horizontal / vertical
Rotate 90° / 180° / 270°
Arbitrary-angle rotation (0°–360°) with a live, real-time canvas preview while dragging the slider
---
🛠️ Built With
C++ with MFC (Microsoft Foundation Classes)
GDI / GDI+ for image rendering and arbitrary-angle rotation
CImage for pixel-level image manipulation
Visual Studio 2026
---
🚀 Getting Started
Prerequisites
Windows 10/11
Visual Studio 2022 or later, with the MFC and C++ Desktop Development workload installed
Build & Run
Clone the repository:
```bash
   git clone https://github.com/praveen6150/digital-image-processing-mfc.git
   ```
Open `project1.sln` in Visual Studio.
Build the solution (`Ctrl+Shift+B`).
Run (`F5`) — or launch the prebuilt executable directly from `x64/Debug/project1.exe` if included in the repo.
Usage
File → Open an image (BMP, JPG, PNG, GIF supported via `CImage`).
Use the menu bar to apply Point Process, Neighborhood Processing, Spatial Domain Filtering, or Transform operations.
For rotation: Transform → Flip/Rotate, choose an option, and for arbitrary angles, drag the slider to preview the rotation live on the canvas before clicking OK.
---
📷 Screenshots
(Add a few screenshots here of the app in action — e.g., an original image alongside a filtered/rotated result)
---
📄 License
(Add your preferred license here, e.g., MIT, or leave this section out if undecided)
---
🙋 Author
Praveen — github.com/praveen6150