//
//  main.cpp
//  OpenCVStart
//
//  Created by 陶冶 on 24.04.20.
//  Copyright © 2020 陶冶. All rights reserved.
//

#include <iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp" //reading and writing
#include "opencv2/highgui.hpp" //show the image
#include "math.h"
using namespace std;
using namespace cv;



 



double ds(int v_x, int v_y, int n_x, int n_y){
    int base1 = v_x-n_x;
    int base2 = v_y-n_y;
    return sqrt(pow(base1, 2) + pow(base2, 2));
}
double ws_metric(double x, int sigma){
    return (1/(2*M_PI*sigma*sigma)) * exp(0-pow(x, 2)/(2*pow(sigma, 2)));
}
double wr_metric(double x, int sigma){
    return 1/(1+x*x/(sigma*sigma));
}
void bilateral(Mat& src, Mat& dst, int dia, int sigma_color, int sigma_space)
{
    int row = src.rows, column = src.cols;
    
    int sum = 0 ;
    unsigned char  g1, g2;
    int neighbor_x =0 , neighbor_y = 0, corner_row = 0, corner_column = 0;
    double  distance = 0, ws = 0, wr = 0 , v_unter = 0 , v_upper = 0, gray = 0;
    for (int i = 0; i < row; i++){
        for (int j = 100 ; j < column; j++){
            //cout <<"src: "<< int(src.at<uchar>(i,j))<<endl;
            neighbor_x = i > dia ? i-dia : 0;
            neighbor_y = j > dia ? j-dia : 0;
            //cout << j <<endl;
            corner_row = (i + dia)<row ? i+dia : row;
            corner_column = (j + dia)<column ? j+dia : column;
            int k = 0 ;
            int l = 0 ;
            for(l = neighbor_x; l <= corner_row; l++){
                
                for (k = neighbor_y; k <= corner_column; k++) {
                    //cout<< k <<endl;
                    if (l == i and k == j )
                    {
                        
                    }
                    else{
                        distance = ds(i, j, l, k);
                        // return the Eclidean distance between the neighbor
                        ws = ws_metric(distance, sigma_space);
                        g1 = src.at<uchar>(i,j) ;
                        g2 = src.at<uchar>(l, k);
                        wr = ws_metric(double(abs(g1-g2)), sigma_color);
                        //wr = wr_metric(double(abs(g1-g2)), sigma_color);
                        v_unter += ws * wr;
                        v_upper += ws * wr * g2;
                    }
                     sum += 1;
                }
               
            }
            gray = v_upper / v_unter;
            if (gray < 0 ){
                gray = 0 ;
            }
            if (gray > 255) {
                gray = 255;
            }
            dst.at<uchar>(i,j) = gray;
            v_upper = 0 ;
            v_unter = 0;
        }
    }
};

void bilateral_1_norm(Mat& src, Mat& dst, int dia, int sigma_color, int sigma_space)
{
    int row = src.rows, column = src.cols;
    // v is v(f)(v)
    // gray is the intensity of the image
    int sum = 0 ;
    unsigned char  g1, g2, gray = 0;
    int neighbor_x =0 , neighbor_y = 0, corner_row = 0, corner_column = 0;
    double  distance = 0, ws = 0, wr = 0 , v_unter = 0 , v_upper = 0 ;
    for (int i = 0; i < row; i++){
        for (int j = 0 ; j < column; j++){
            
            //cout <<"src: "<< int(src.at<uchar>(i,j))<<endl;
            neighbor_x = i > dia ? i-dia : 0;
            neighbor_y = j > dia ? j-dia : 0;
            
            corner_row = (i + dia)<row ? i+dia:row;
            corner_column = (j + dia)<column ? j+dia:column;
            int k = 0 ;
            int l = 0 ;
            for(l = neighbor_x; l <= corner_row; l++){
                
                for (k = neighbor_y; k <= corner_column; k++) {
                    
                    if (l == i and k == j )
                    {
                        
                    }
                    else{
                        distance = abs(i-l) + abs(j-k);
                        // return the Eclidean distance between the neighbor
                        ws = ws_metric(distance, sigma_space);
                        g1 = src.at<uchar>(i,j) ;
                        g2 = src.at<uchar>(l, k);
                        wr = ws_metric(double(abs(g1-g2)), sigma_color);
                        //wr = wr_metric(double(abs(g1-g2)), sigma_color);
                        v_unter += ws * wr;
                        v_upper += ws * wr * g2;
                        
                    }
                    sum += 1;
                }
                
                
            }
            gray = v_upper / v_unter;
            if (gray < 0 ){
                gray = 0 ;
            }
            if (gray > 255) {
                gray = 255;
            }
            //cout << int(gray)<< endl;
            dst.at<uchar>(i,j) = gray;
            v_upper = 0 ;
            v_unter = 0;
        }
    }
}
int main( int argc, char ** argv )
{
    Mat src;
   
    const char* filename = argc >=3 ? argv[1] : "Lena.png";
    src = imread(filename, IMREAD_COLOR );
    const char* filename1 = argc >=3 ? argv[1] : "EasterEggs.jpg";
    //imshow("original image", src);
   
    Mat src1 = imread(filename1);
    
    cvtColor(src1,src1, COLOR_RGB2GRAY);
    imshow("original image", src1);
    Mat dst = src1.clone();
    /*
    bilateral(src1, dst, 5, 50, 50);
    Mat dst2 = src1.clone();
    bilateral_1_norm(src1, dst2, 5, 50, 50);
    imshow("gaussian-2-norm", dst);
    imshow("gaussian-1-norm", dst2);
    */
    for(int i = 0; i < 10 ; i++){
        bilateral(src1, dst, 5, 10, 10);
        imshow("gaussian-2-norm", dst);
        src1 = dst.clone();
        
    }

      
     
       waitKey();
       return 0;

}
