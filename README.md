# PeopleCounting

## OBJECT
This is a simple computer vision's program that count how many people are in a room from the images of empty room, depth and color images, and the same images of the room within people.

## CONTENTS OF THE FOLDER
In the folder there is a dataset folder of samples, a src folder within a simple main and the PeopleCounting class and a documentation folder within the description of problem and how this solution works.

## EXECUTION
To compile and execute this file you must do in order from the main folder:
```
cmake CMakeLists.txt
```
```
make
```
```
./PeopleCounting
```
The result will be visible and saved on the principal folder.
When you press 'q' the program load another set of images from the dataset and execute for them overwriting the result saved in the folder.