#include <iostream>
#include <stdio.h>

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;


int main()
{
    // Read Images
    //Mat BlueApple = imread("Images/BlueApple.bmp");
    //Mat BlueCar = imread("Images/BlueCar.bmp");
    //Mat GreenApple = imread("Images/GreenApple.bmp");
    //Mat GreenCar = imread("Images/GreenCar.bmp");
    Mat RedApple = imread("Images/RedApple.bmp");
    Mat RedCar = imread("Images/RedCar.bmp");
    Mat frame = RedApple;

    Mat imgWithRect ;
    frame.copyTo(imgWithRect);

    Mat HSVPic;
    cvtColor(frame,HSVPic,COLOR_BGR2HSV);

    //Erode & Dilate
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(11, 16));
    morphologyEx(HSVPic, HSVPic, MORPH_OPEN, kernel);

    int colours_hue[6] = {0,20,35,78,120,180};
    String colour_names[5] = {"Red","Yellow","Green","Blue","Pink"};
    int colour_min,colour_max,min_val, max_val;
    int colour_index;
    int Maximum = -999;
    Mat HSVPic2 = HSVPic.clone();
    Mat OutputBinaryImg;
    for (int i = 0; i < 5; ++i) {
      colour_min = colours_hue[i];
      colour_max = colours_hue[i+1]-1;
      inRange(HSVPic2, Scalar(colour_min,50,40), Scalar(colour_max, 255, 255), HSVPic);
      if (countNonZero(HSVPic)>Maximum){
            Maximum = countNonZero(HSVPic);
            colour_index=i;
            min_val=colour_min;
            max_val=colour_max;
        }
        cout << "Colour: "<<colour_names[i] << endl;
        cout << "Current Pixels: "<< countNonZero(HSVPic) << endl;
        cout << "Maximum Pixels: "<< Maximum <<" ("<< colour_names[colour_index]<<")" << endl<<endl;
    }
    inRange(HSVPic2, Scalar(min_val,50,40), Scalar(max_val, 255, 255), OutputBinaryImg);
    cout << "Color: "<<colour_names[colour_index] << endl;

    vector < vector < Point> >contours;
    findContours(OutputBinaryImg,contours,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);

    size_t max_contour=0;
    for(size_t i=0;i<contours.size();++i){
        if(contours.size()>max_contour){
            max_contour=i;
        }
    }
    Rect boundRect = boundingRect(contours[max_contour]);
    rectangle(imgWithRect,boundRect.tl(),boundRect.br(),(0,0,0),3);

    namedWindow("Contour");
    imshow("Contour",imgWithRect);
    waitKey();
    destroyWindow("Contour");
    return 0;
}
