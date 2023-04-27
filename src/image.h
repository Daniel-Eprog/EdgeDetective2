#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class Image
{

    public:

        //struct for storing pixel color data
        struct Pixel
        {
            //pixel color variables
            unsigned char blue;
            unsigned char green;
            unsigned char red;
            //pixel constructor
            Pixel(unsigned char bval, unsigned char gval, unsigned char rval)
            : blue{bval}, green{gval}, red{rval} { }

        };

        //struct for storing image header data
        struct IMGheader
        {

            char idLength;
            char colorMapType;
            char dataTypeCode;
            short colorMapOrigin;
            short colorMapLength;
            char colorMapDepth;
            short xOrigin;
            short yOrigin;
            short width;
            short height;
            char bitsPerPixel;
            char imageDescriptor;
            
        };

        //general functions for loading and exporting images

        void loadIMG(string file);//loads image from TGA file
        void exportIMG(string file) const;//exports an image as TGA file
        void getHeader() const; //prints header information

        //Preruquiste functions
        void convertGrayScale(); //converts the desired image into grayscale
        void gaussianBlur(); //applies blur to reduce noise

        //Initial Edgedetection kernels
        void prewittEdgeDetection(); //convolve using prewitt kernel
        void sobelEdgeDetection(); //convolve using sobel kernel
        void cannyEdgeDetection(); //convolve using sobel kernel and us methods for canny algorithm steps

        //canny algorithm steps
        void nonMaxSuppression();//narrows pixel fields based on the angle of intesity
        void doubleThreshold();//narrows pixel values to two possible values
        void hysteresis();//determines pixel intesity based on neighboring pixels

        //general convolution
        vector<double> convolve(vector<vector<float>> convolutionKernel);

        private:
            IMGheader imageHeader; //stores header information in custom struct 
            vector<vector<Pixel>> image; //stores pixel information in map
            vector<double> edgeAngles; //stores the intensity edges after initial edge detection
            bool imageLoaded = false; //determines if a image is loaded

};

#endif