#include <iostream>
#include <string>
#include <fstream>
#include <cv.h>
#include <highgui.h>

using namespace std;
using namespace cv;

// read file -> process -> output result
// width = 5
// height = 8
// number of boards = how many sample

int main()
{
	int numBoards = 0;
	int numCornersHor = 5;
	int numCornersVer = 8;
	vector<string> filenames;

	int numSquares = numCornersHor * numCornersVer;
	Size board_sz = Size(numCornersHor, numCornersVer);

	////
	string line;
	string inputFile;
	cout << "input input file name: " << endl;
	cin >> inputFile;
	ifstream myfile(inputFile);
	if(!myfile.is_open())
	{
		cout << "cannot find file " << inputFile << endl;
		return -1;
	}

	if(myfile.is_open())
	{
		while(getline(myfile, line))
		{
			filenames.push_back(line);
			numBoards++;
		}
		myfile.close();
	}
	////

	vector<vector<Point3f>> object_points;
	vector<vector<Point2f>> image_points;

	vector<Point2f> corners;
	int successes = 0;

	Mat image;
	Mat gray_image;

	vector<Point3f> obj;
	for(int j = 0; j < numSquares; j++)
		obj.push_back(Point3f(j/numCornersHor, j%numCornersHor, 0.0f));

	while(successes < numBoards)
	{
		////
		image = imread(filenames.at(successes));
        cout << "read image " << filenames.at(successes) << endl;
		////
		cvtColor(image, gray_image, CV_BGR2GRAY);

		bool found = findChessboardCorners(image, board_sz, corners, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

		if(found)
		{
			cornerSubPix(gray_image, corners, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
			drawChessboardCorners(gray_image, board_sz, corners, found);

			cout << "legal image: " << filenames.at(successes) << endl;
		}
		else if(!found)
        {
            cout << "illegal image: " << filenames.at(successes) << endl;
            return -1;
        }

		imshow("win1", image);
		imshow("win2", gray_image);
		waitKey(0);

		successes++;
		if(successes >= numBoards)
			break;

		////

		image_points.push_back(corners);
		object_points.push_back(obj);

		cout << "Snap stored!" << endl;
/*
		successes++;

		if(successes >= numBoards)
			break;
		*/
	}

	cout << "image read complete" << endl;

	Mat intrinsic = Mat(3, 3, CV_32FC1);
	Mat distCoeffs;
	vector<Mat> rvecs;
	vector<Mat> tvecs;

	intrinsic.ptr<float>(0)[0] = 1;
	intrinsic.ptr<float>(1)[1] = 1;

	calibrateCamera(object_points, image_points, image.size(), intrinsic, distCoeffs, rvecs, tvecs);

	cout << "done calibration" << endl;

	// write intrinsic & distort coefficient into file
	FileStorage fs("newdistort.xml", FileStorage::WRITE);
	fs << "intrinsic" << intrinsic;
	fs << "distort" << distCoeffs;
	fs.release();
	cout << "outputted distort coefficient";

	Mat imageUndistorted;
	string str = "undistorted_";
	int count = 0;

	while(1)
	{
		////
		image = imread(filenames.at(count));
		cout << "read image " << filenames.at(count) << endl;
		undistort(image, imageUndistorted, intrinsic, distCoeffs);

		imshow("win1", image);
		imshow("win2", imageUndistorted);
		waitKey(0);

		string outputname = filenames.at(count);
		outputname.insert(0, str);

		cout << outputname << endl;

        try {
            imwrite(outputname, imageUndistorted);
        }
        catch(runtime_error& ex) {
            fprintf(stderr, "Exception writing image: %s\n", ex.what());
            return 1;
        }

        cout << "write " << outputname << " successfully" << endl;

		count++;
		if(count >= numBoards)
		{
			cout << "finish" << endl;
			break;
		}
		////
	}

	return 0;
}
