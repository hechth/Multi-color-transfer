#include "Transfer.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <windows.h>

#pragma warning(disable: 4244)

using namespace cv;

#define VIDEO

#ifdef VIDEO
	// SET YOUR VIDEO
	#define SOURCE_VIDEO "../Time-lapse.flv"
	//#define SAVE_VIDEO 
	#ifdef SAVE_VIDEO
		#define OUTPUT_FILE	"colored.avi"
	#endif
#else
	#define SOURCE_PIC "source/1.jpg"
#endif
std::vector<std::string> images = {
									"images/1.jpg",
									"images/6.png",
									"images/7.jpg",
									};

int main()
{
	ColorMachine cmachine("1");
	for(auto image: images)
		cmachine.AddLayer(image);
	transfer_method method = METHOD_REINHARD;
	cmachine.SetMethod(method);
#ifdef VIDEO
	VideoCapture video;
	video.open(SOURCE_VIDEO);
	if(!video.isOpened())
		return 1;
	Mat frame;
	VideoWriter writer;
	while(video.read(frame))
	{
		if((frame.cols >=1024) || (frame.rows >= 800))
		{
			resize(frame, frame, Size(0, 0), 0.5f, 0.5f); // video was too big
		}
		cmachine.SetSource(frame);
		cmachine.Prepare(method); // first time for all, then only for source
		cmachine.ShowWindows(true); // will work only first time
		Mat colored = cmachine.TransferColor();
#ifdef SAVE_VIDEO
		if (!writer.isOpened())
			writer.open(OUTPUT_FILE, VideoWriter::fourcc('X','V','I','D'),
			30, colored.size());
		writer << colored;
#endif
		int key = waitKey(5);
		if(key == 49)
		{
			cmachine.SetMethod(METHOD_REINHARD);
			method = METHOD_REINHARD;
			continue;
		}
		if(key == 50)
		{
			cmachine.SetMethod(METHOD_XIAO);
			method = METHOD_XIAO;
			continue;
		}
		if(key == 27)
		{
			break; // ESCAPE
		}
	}
	video.release();
#else
	cmachine.SetSource(SOURCE_PIC);
	while(1)
	{
		cmachine.ShowWindows(true);
		cmachine.Prepare(method);
		cmachine.TransferColor();
		int key = waitKey(0); //
		if(key == 49)
		{
			cmachine.SetMethod(METHOD_REINHARD);
			method = METHOD_REINHARD;
			continue;
		}
		if(key == 50)
		{
			cmachine.SetMethod(METHOD_XIAO);
			method = METHOD_XIAO;
			continue;
		}
		if(key == 27)
		{
			break; // ESCAPE
		}
	}
	#endif
	return 0;
}