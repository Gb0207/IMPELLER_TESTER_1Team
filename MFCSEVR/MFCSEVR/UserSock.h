#pragma once

// OpenCv를 위한 헤더 파일
#include "opencv2/opencv.hpp"

// C++를 위한 헤더 파일
#include <iostream>

// UserSock 명령 대상

class UserSock : public CSocket
{
public:
	UserSock();
	virtual ~UserSock();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	void ThreeCircles();
	void BurrImpurity();
	cv::Mat Cam; // Mat 클래스 Cam
	CImage Camimg; // CImage 클래스 Camimg
};


