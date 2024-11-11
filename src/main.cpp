#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif 

int main(int argc, char* argv[]) {
    cv::VideoCapture cap(0); // VideoCapture 클래스의 객체 cap 생성(1번 캡처장치 열기)
	cv::Mat frame; // Mat 클래스의 객체 frame 생성

	if(cap.isOpened()) { // 캡처장치가 영상 캡처를 성공하면
		while(1) {
			cap >> frame; // 영상 정보를 frame로 전달
			if(frame.empty()) break; // frame에 영상이 없는 경우 루프 종료
            cv::imshow("new", frame);
			if(cv::waitKey(1) == 27) break; // ESC 키를 눌러 영상출력 창 닫기
		}
	} else {
		std::cout << "Error: Failed to open the Camera." << std::endl;
	}

	cv::namedWindow("new", cv::WINDOW_AUTOSIZE);

    cap.release(); // 카메라관련 리소스 해제(캡처장치 닫기)
	cv::destroyAllWindows(); 

	Py_Initialize();
	//PyRun_SimpleString("");
    // 파이썬 코드로 딥러닝 모델 코드 작성
	Py_Finalize();
	return 0;
}