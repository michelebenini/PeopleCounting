#include "PeopleCounting.h"
#include "iostream"

void show(cv::Mat img, char *name, int i );

int main( int argc, char** argv)
{
    // load dataset of name of color image
    std::vector<cv::String> dataset_color;
    cv::glob("dataset/color/*",dataset_color,true);

    // load dataset of name of depht image
    std::vector<cv::String> dataset_depth;
    cv::glob("dataset/depth/*",dataset_depth,true);
    
    PeopleCounting pc;

    // read the background image
    cv::Mat bgc = cv::imread(dataset_color[0]);
    cv::Mat bgd = cv::imread(dataset_depth[0]);

    // check background image
    if(!bgc.data || !bgd.data){
        printf("Error: Background Image not found!\n");
    }
    
    // set the background image in PeopleCounting class
    pc.setBackground(bgc, bgd);

    // for every image in the dataset exept for background
    for(int n = 1; n < dataset_color.size() && n < dataset_depth.size(); n++){
        int npeople = 0;

        // get the name of n-th image color and depth
        cv::String choosen_c = dataset_color[n];
        cv::String choosen_d = dataset_depth[n];

        // read the image
        printf("[ %d ] IMAGE \n",n );
        std::cout << "Load color image : < " << choosen_c << " >" << std::endl;
        cv::Mat img_c = cv::imread(choosen_c);
        std::cout << "Load depth image : < " << choosen_d << " >" << std::endl;
        cv::Mat img_d = cv::imread(choosen_d);

        // check loaded image
        if(!img_c.data || !img_d.data){
            printf("Error color or depth image not found!\n");
        }
        
        // inizialize the destination image
        cv::Mat dst = img_c.clone();
    
        // call the detectPeople method of PeopleCounting class
        npeople = pc.detectPeople(img_c, img_d, dst);
        
        // show and print the result
        printf("People founded: %d\n\n",npeople );
        show(img_c, (char*)"color" , 1);
        show(img_d, (char*)"depth", 1 );
        show(dst, (char*)"result", 0 );
    }
    return 0;
}

// method used to quickly show and write an image
void show(cv::Mat img, char* name, int i){
    char quit = 'q';
    char filename[10];
    sprintf(filename, "%s.png",name);
    cv::imwrite(filename, img);
    cv::namedWindow(name,cv::WINDOW_NORMAL);
    cv::resizeWindow(name, img.cols,img.rows);
    cv::imshow(name,img);
    if(i == 0)while(quit != cv::waitKey(0));
}
