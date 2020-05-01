#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define MaxPoints 2
int nb_points = 0;

Mat MapCurveImage512;
Mat image, color_img;
Point    SrcPtInt[(int)MaxPoints];
unsigned char LUtable[256];

void help()
{
    cout <<
    "\nTU Dresden, CV2 Ex1, Holger Heidrich\n"
    "This program plays with colors simulating solarisation.\n"
    "Clicking two times into the \"GreyCurve\"-image gives two points.\n"
    "Make these points be the extremals of a polynomial of degree three in the range 0..255 x 0..255.\n"
    "The \"GreyCurve\"-image itself has size (512,512). Point (0,0) is upper left corner. \n"
    "Use the polynomial y(x) to map values x of the input image to values y of an output image.\n"
    "Saturate to 0 and 255 if the mapped grey / color values exceed these values. \n"
    "Call:\n"
    "./image Image [image-name Default: EasterEggs.jpg]\n" << endl;
}

void on_mouse( int event, int x, int y, int flags, void* param )
{
    switch( event )
    {
        case EVENT_LBUTTONDOWN:
        {
            // ad new point to polygon
            if (nb_points < MaxPoints)
            {
               SrcPtInt[nb_points].x=x;
               SrcPtInt[nb_points].y=y;
               if (nb_points)
               {   // second point in SrcPtInt
                   
                   MapCurveImage512 = 0;
                   
                  // read the two (extrema) points
                  int x1 = SrcPtInt[0].x;
                  int x2 = SrcPtInt[1].x;
                  int y1 = 511 - SrcPtInt[0].y; // "511 -" cause we have "0" in lower left corner
                  int y2 = 511 - SrcPtInt[1].y;

                 // determine polynomial coefficients of y(x):=a*(x-x0)³-b*x+c
                  // (want to cheat? see end of file)

                   // create the LUT for that polynom and
                  // draw the polynom in the MapCurveimage (pixelwise)
                      // your code for the polynomial and color transform (instead of the following line)'
                  double        dx = x1 - x2;
                  double        dy = y1 - y2;
                  float x0 = (float)(x1 + x2) / 2;
                  float a = (float)(-2.0*dy / pow(dx, 3.0));
                  float b = (float)(-3.0 / 2.0)*(dy / dx);
                  float c = (float)((y1 + y2) / 2.0 + b*x0);
                  
                   float value = 0 ;
                   int  i = 0;
                   for(i = 0; i<256;i++){
                       value = a * pow(i-x0, 3) - b * i + c;
                       if(value<0){
                           LUtable[i]=0;
                       }
                       else if (value > 255){
                          LUtable[i]=255;
                       }
                       else{
                           LUtable[i]=value;
                       }
                   }
                   
                   
                  line(MapCurveImage512, SrcPtInt[0], SrcPtInt[1], CV_RGB(255, 255, 255));
                  //draw map curve because the size of MapCurveImage is 512*512 so the y of polinomial is 0=<y<=511
                 //the MapCurveImage is grey image so the channal of it is one the type of each pixel is unchar
                   
                   for(i = 0; i<512; i++){
                       value = (int)(a * pow(i-x0, 3) - b * i + c);
                       if(value < 0){
                           value = 0;
                       }
                       else if(value > 511){
                           value = 511 ;
                       }
                       MapCurveImage512.at<uchar>(511 - value, i) = 255;
                       
                   }
                imshow("GreyCurve", MapCurveImage512);
                   
                // show the result
                Mat lut(1,256,CV_8U,LUtable);
                cv::LUT(image, lut, color_img);
                imshow( "result image", color_img);

                      // mapped_result_img = image.clone();
                      nb_points = 0;
               } else nb_points++;
               }
        }
        break;
    }
}  // void on_mouse( int event, int x, int y, int flags, void* param )


int main( int argc, char** argv )
{
    help();
     
    char* filename = argc == 3 ? argv[1] : (char*)"EasterEggs.jpg";
    image = imread(filename, 1);
    color_img = image.clone();

    namedWindow( "GreyCurve");
    namedWindow( "Fruits!");
    imshow( "Fruits!", color_img);
    
    MapCurveImage512.create(512, 512, CV_8U);// create(row, cols, type)
    MapCurveImage512 = 0;
    imshow("GreyCurve", MapCurveImage512);

    setMouseCallback( "GreyCurve", on_mouse, 0 );
    waitKey();
    
    return 0;
}





