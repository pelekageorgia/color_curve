# color_curve
This program applies a basic color curve to a given image, using the threshold and gamma values the user provides

## Dependencies

-**CMake**

-**OpenMP**

-**OpenCV**

## Compiling
```
git clone https://github.com/pelekageorgia/color_curve.git
cd color_curve
mkdir build && cd build
cmake ..
make
```

- **Running the program with your own input**: to run the program you have to provide an image (8-bit monochrome or 8-bit color images are supported), your desired threshold and gamma values and the path for the output image. You can run the program as follows:

```
 ./color_curve <input_image> <t_value> <g_value> <output_image>
```

- **Running the program in testing mode**: you can run the program in testing mode using the provide test images and test values or you can run your own tests by adding the in src/testing.txt file. You can run the program in **tsting** mode as follows:

```
./color_curve --testing
```
