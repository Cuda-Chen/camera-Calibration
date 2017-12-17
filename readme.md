compile information of caliTest.cpp and cameraCalibration.cpp:
* language: C++
* IDE: code::block 12.11
* compiler: gcc ver. 4.7.1 (tdm-1) with MinGW
* openCV ver.: 2.4.13
* problem you may encounter: cannot search file using absolute path(both programs)

caliTest.cpp

	description:
	This code is used for camera calibration via a bunch of existing images
	
	how to build this program:
	* mkdir build
	* cd build
	* cmake ..
	* make
	
	input:
	A list of filenames of distorted images (I use inputPic.txt)
	
	output:
	Calibrated images of those distorted images being input
	
cameraCalibration.cpp

	NOTE:
	This code is not being used 

	discription:
	Camera calibration test by using built-in camera of my laptop
	
	input:
	Images captured by built-in camera of my laptop
	
	output:
	Calibrated images of those captured images
	