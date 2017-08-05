Project: 
Object Detection & Background Subtraction

Overview:
Program will read in a folder of 21 images into an Image object, pixel by pixel, and use a frame-differencing method to detect a certain object in each image, and remove said object from the image. 

Image Class: 
Each Image object has variables to contain parameters for the .ppm header format, and each object contains an array of RGB structs to hold each pixel’s RGB values.
The read_image() function opens a .ppm image file, and reads the contents in to the object, pixel by pixel.
The write_image() function writes the current object’s RGB values to a new or existing file, essentially creating a new image.  Any pixel that has not been marked as part of the object in question will be colored all white. 
The overloaded (-) operator returns a new Image (result) object is used for our method of subtraction. When subtracting one image object from another, the RGB values of each object are compared, pixel by pixel, to find the absolute difference of each pixel. If the absolute difference between pixels is greater than the threshold (a constant int set to 20), then the pixels of the resulting image =  |RGB1 – RGB2|, else: keep the original RGB values from the image – this will keep the object’s original pixels.

Main Function: 
In our main function, we initialize all 21 images in an array of Image objects. We use our subtraction operator to perform frame differencing on the images at a rate of D(N) = |I(T) – I(T+N)|, which is the same as Frame 1 – Frame 2. The result, the detected object is written to the file “NEW_IMAGE.ppm” which will consist of the object and the subtracted background. 
