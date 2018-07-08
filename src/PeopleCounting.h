#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include "opencv2/shape.hpp"



class PeopleCounting{
private:
	// variables contain the background images
	cv::Mat bgc;
	cv::Mat bgd;

	// method show provided for debugging of images
	void show(cv::Mat img){
		cv::imwrite("img.png", img);
	    char quit = 'q';
	    cv::namedWindow("Internal Imgage",cv::WINDOW_NORMAL);
	    cv::resizeWindow("Internal Imgage", img.cols,img.rows);
	    cv::imshow("Internal Imgage",img);
	    while(quit != cv::waitKey(0));
	}

	// method that provides to remove the background
	void removeBG(cv::Mat src, cv::Mat& dst ){

		cv::Mat src_gray;
		cv::Mat bgd_gray;
		
		cvtColor(src, src_gray,CV_RGB2GRAY);
		cvtColor(bgd, bgd_gray,CV_RGB2GRAY);

		// increment the contrast before remove the background
		cv::equalizeHist(src_gray, src_gray);
		cv::equalizeHist(bgd_gray, bgd_gray);

		// intensity transform: negative of an image
		src_gray = (254) - src_gray;	
		bgd_gray = (254) - bgd_gray;

		// subtraction of background from the image
		src_gray = 254 - (src_gray - bgd_gray);
		
		dst = src_gray.clone();
	}

	// method that provides to remove the noise and set the imagi in blobls form
	void clean(cv::Mat img, cv::Mat& dst ){

		// threshold applied to increment the difference between the people and the background
		cv::threshold(img, img, 200, 255, CV_THRESH_TOZERO_INV);
		cv::threshold(img, img, 10, 255, CV_THRESH_TOZERO);

		// remove little noise
		cv::blur(img, img, cv::Size(9,9), cv::Point(-1,-1), cv::BORDER_DEFAULT );
		
		int type1 = cv::MORPH_ELLIPSE;
		int size1 = 4;

		int type2 = cv::MORPH_ELLIPSE;
		int size2 = 30;
			
		int type3 = cv::MORPH_ELLIPSE;
		int size3 = 30;

		int type4 = cv::MORPH_RECT;
		int size4 = 5;


		cv::Mat element1 = cv::getStructuringElement( type1,
		                   cv::Size( 2*size1 + 1, 2*size1+1 ),
		                   cv::Point( size1, size1 ) );

		cv::Mat element2 = cv::getStructuringElement( type2,
		                   cv::Size( 2*size2 + 1, 2*size2+1 ),
		                   cv::Point( size2, size2 ) );

		cv::Mat element3 = cv::getStructuringElement( type3,
		                   cv::Size( 2*size3 + 1, 2*size3+1 ),
		                   cv::Point( size3, size3 ) );

		cv::Mat element4 = cv::getStructuringElement( type4,
		                   cv::Size( 2*size4 + 1, 2*size4+1 ),
		                   cv::Point( size4, size4 ) );

		// dilate to make more strong the people near the wall of the room
		cv::dilate( img, img, element1 );

		// erode to divide the people each other
		cv::erode( img, img, element2 );
		
		// erode to delete the small component that link toghether the people still linked
		cv::erode( img, img, element4 );
		
		// dilate to create a circle shape blob of greater size
		cv::dilate( img, img, element3 );

		// equalize the histogram to have a greater contrast between the blob and the background
		cv::equalizeHist(img, img);
		dst = img.clone();
	}

	// method that use a blob detector to find the people in the post processed image
	void findPeople(cv::Mat img, cv::Mat& dst ,std::vector<cv::KeyPoint>& circles){
		// Setup SimpleBlobDetector parameters.
		cv::SimpleBlobDetector::Params params;

		// Change thresholds
		params.minThreshold = 0;
		params.maxThreshold = 255;

		// Filter by Area.
		params.filterByArea = true;
		params.minArea = 6000;
		params.maxArea = 30000;

		params.filterByCircularity = false;
		params.filterByConvexity = false;
		params.filterByInertia = false;
		params.filterByColor = true;
		params.blobColor = 255;
		std::vector<cv::KeyPoint> keypoints;

		// Set up detector with params
		cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);   

		// Detect blobs
		detector->detect( img, keypoints);
		if(keypoints.size() < 1){
			printf("No people find!!\n");
			return ;
		}

		//the total no of blobs detected are:
		size_t x = keypoints.size();
	
		//location of first blob
		cv::Point2f point1=keypoints.at(0).pt;
		float x1=point1.x;
		float y1=point1.y;

		// set the output vector circles with the keypoints
		for(size_t i = 0; i < keypoints.size(); i++){
			circles.push_back(keypoints[i]);
		}


		dst = img.clone();

		// draw the key point in the output image
	}

	// method that draw the circles passed as keypoints
	void drawCircle(cv::Mat src, cv::Mat& dst, std::vector<cv::KeyPoint> c){
		dst = src.clone();
		for (int i = 0; i < c.size(); i++)
		{
			cv::circle( dst, c[i].pt, c[i].size, cv::Scalar(0,255,0), 3, 8, 0 );
		}
	}

public:
	// method that set the background images
	void setBackground(cv::Mat color, cv::Mat depth)
	{
		bgc = color;
		bgd = depth;
	}

	// method that check the imput parameters and recall the other methods
	int detectPeople(cv::Mat imgc, cv::Mat imgd, cv::Mat& dst )
	{
		std::vector<cv::KeyPoint> circles;
		cv::Mat temp;

		// check on background images
		if(!bgc.data || !bgd.data){
			printf("Need the background images setted not empty\n");
			return -1;
		}

		// check on passed images
		if(!imgc.data || !imgd.data){
			printf("Need the color and depth images not empty\n");
			return -1;
		}

		removeBG(imgd, temp);
		clean(temp, temp);
		findPeople(temp, temp, circles);
		drawCircle(imgc, dst, circles);
		
		// return the number of people finded
		return circles.size();
	}
};
#endif