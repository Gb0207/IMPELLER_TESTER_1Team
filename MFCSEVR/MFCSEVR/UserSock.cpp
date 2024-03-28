// UserSock.cpp: 구현 파일
//

#include "pch.h"
#include "MFCSEVR.h"
#include "UserSock.h"
#include "iostream"
#include <vector>
#include <string>
#include <afx.h>


// UserSock

UserSock::UserSock()
{
}

UserSock::~UserSock()
{
}


// UserSock 멤버 함수

/*함수이름: OnReceive(int nErrorCode)
기능: OnReceive 가상 함수 생성으로 수신받은 정수형 데이터 저장 및 콘솔 출력 함수
반환값: 없음*/
void UserSock::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);

	int data;
	Receive(&data, sizeof(int));
	std::cout << "From Python(result true:1 false:0) : " << data << std::endl;

	if (data == 1) {
		std::cout << "result true----------" << std::endl;
		ThreeCircles();
	}
	else {
		std::cout << "result false----------" << std::endl;
		BurrImpurity();
	}

}
/*함수이름: OnSend(int nErrorCode)
기능: OnSend 가상 함수 생성으로 송신할 파일 불러와 파일 사이즈와 파일 데이터 송신 후 콘솔 출력 함수
반환값: 없음*/
void UserSock::OnSend(int nErrorCode)
{
	CSocket::OnSend(nErrorCode);

	CFile pfile;
	pfile.Open(_T("C:/Users/lms/Documents/Cmfc_cnn_python/XX/cast_O (52).jpeg"),
		CFile::modeRead);

	int nFileData = pfile.GetLength(); //파일 사이즈
	Send(&nFileData, sizeof(nFileData)); //파일 사이즈 송신
	std::cout << "file size: " << nFileData << std::endl;

	char buf[28998];
	pfile.Read(buf, sizeof(buf));
	Send(&buf, sizeof(buf));
	std::cout << "file send" << std::endl;
	pfile.Close();
}

void UserSock::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);

	ShutDown(2);
	Close();
}

void UserSock::ThreeCircles()
{
	// 허프 함수 3회
	//cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	cv::Mat gr_img = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);

	std::vector<cv::Vec3f> small_circles; // 작은 원 정보 담는 벡터
	cv::HoughCircles(gr_img, small_circles, cv::HOUGH_GRADIENT, 1, 30, 150, 30, 0, 30); 
	// 허프 원 변환 알고리즘을 통한 원 검출 | 허프 원함수
	// (입력받은 영상<그레이스케일만 받음>, 원 정보 담을 벡터, 방법<고정>, 입력/축적의 비율, 인접 원 중심 최소 거리, Canny 검출기 높은 임곗값, 
	// 중심 임곗값(원 검출 위한 정보로 낮으면 더 많은 원, 높으면 더 적은<정확한> 원), 검출할 원 최소 반지름, 검출할 원 최대 반지름)

	std::vector<cv::Vec3f> mid_circles;
	cv::HoughCircles(gr_img, mid_circles, cv::HOUGH_GRADIENT, 1, 30, 150, 60, 50, 70);

	std::vector<cv::Vec3f> big_circles;
	cv::HoughCircles(gr_img, big_circles, cv::HOUGH_GRADIENT, 1, 30, 150, 47, 90, 123);

	cv::Mat color_img; // 컬러 이미지 받을 Mat
	cvtColor(gr_img, color_img, cv::COLOR_GRAY2BGR);
	
	for (const auto& cs : small_circles) { // 원 그리기
		cv::Point centers(cvRound(cs[0]), cvRound(cs[1])); // 중심점 좌표 포인트
		int radiuss = cvRound(cs[2]); // 반지름 포인트
		//cv::circle(color_img, centers, 2, cv::Scalar(255, 255, 255), 1, cv::LINE_AA); // 중심점 찍기
		cv::circle(color_img, centers, radiuss, cv::Scalar(0, 0, 255), 3, cv::LINE_AA); 
		// 원 그리기 함수 | 원함수
		// (원그림을 받을 이미지, 원의 중심 좌표, 원의 반지름, 선 색상(BGR 순) 입력, 선 굵기, 선 타입)
		cv::putText(color_img, std::to_string((int)cs[2]), cv::Point((int)cs[0] - 10, (int)cs[1] + 50), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 255), 1);
		// 영상에 문자열 출력하는 함수 | 텍스트 출력 함수
		// (텍스트가 쓰일 이미지, 입력할 텍스트, 텍스트 출력할 위치<x 좌표, y 좌표>, 폰트 종류, 폰트 크기(확대 비율), 폰트 색상, 문자 두께)
	}

	for (const auto& cm : mid_circles) {
		cv::Point centerm(cvRound(cm[0]), cvRound(cm[1]));
		int radiusm = cvRound(cm[2]);
		cv::circle(color_img, centerm, radiusm, cv::Scalar(0, 255, 255), 3, cv::LINE_AA);
		cv::putText(color_img, std::to_string((int)cm[2]), cv::Point((int)cm[0] - 15, (int)cm[1] + 85), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 255), 1);
	}

	for (const auto& cb : big_circles) {
		cv::Point centerb(cvRound(cb[0]), cvRound(cb[1]));
		int radiusb = cvRound(cb[2]);
		cv::circle(color_img, centerb, radiusb, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
		cv::putText(color_img, std::to_string((int)cb[2]), cv::Point((int)cb[0] - 15, (int)cb[1] + 121), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
	}

	cv::imshow("result", color_img); // 이미지 출력 함수 | ("창 이름", 불러올 이미지)
	// 허프 함수 3회
}

void UserSock::BurrImpurity()
{
	// 사출불량(Burr, 이바리, 가장자리 성형 불량) / 이물질(Impurity) 검출 | 탬플릿 매칭으로 시도 | 성공
	//cv::Mat gr_img; // 그레이스케일 이미지 받을 Mat
	cv::Mat gr_img = cv::imread("./cast.bmp", cv::IMREAD_GRAYSCALE);

	cv::Mat burr_img = cv::imread("./def/burr.bmp", cv::IMREAD_GRAYSCALE); // Burr 이미지 회색 조로 읽음

	cv::Mat impurity_img = cv::imread("./def/impurity.bmp", cv::IMREAD_GRAYSCALE);

	cv::Mat tmpl_burr_img; // 템플릿 매칭 받을 Mat
	cv::matchTemplate(gr_img, burr_img, tmpl_burr_img, cv::TM_CCOEFF_NORMED); // 템플릿 매칭 함수 | 유사도 계산
	// matchTemplate(입력 Mat, 템플릿 Mat, 유사도 결과 Mat, 유사도 계산 방법, 마스크 영상<생략 시 noArray()>)
	// gr_img와 burr_img의 유사도를 TM_CCOEFF_NORMED 방법으로 계산하여 res_img에 저장

	cv::Mat tmpl_impurity_img;
	cv::matchTemplate(gr_img, impurity_img, tmpl_impurity_img, cv::TM_CCOEFF_NORMED);

	double min_bVal, max_bVal; // 최솟값, 최댓값을 받을 doule형 변수 선언
	//cv::Point minLoc, maxLoc; // 최솟값, 최댓값 좌표를 받을 Point형 변수 선언
	cv::minMaxLoc(tmpl_burr_img, 0, &max_bVal, 0, 0); // 행렬의 최솟값/최댓값을 찾는 함수
	// minMaxLoc(입력 Mat, <주소의 경우 필요 없으면 0 지정>최솟값을 받을 double형 변수 주소, 최댓값을 받을 double형 변수 주소, 
	// 최솟값 위치 좌표 받을 Point형 변수 주소, 최댓값 위치 좌표 받을 Point형 변수 주소, 마스크 영상<생략 가능>)
	// tmpl_img의 최솟값/최댓값을 각 변수 주소에, 최솟값/최댓값 위치 좌표를 각 Point형 주소에 마스크 영상의 픽셀값이 0이 아닌 위치에서 연산 수행
	// Val를 이용하면 유사도를 알 수 있다.

	double max_iVal;
	cv::minMaxLoc(tmpl_impurity_img, 0, &max_iVal, 0, 0);

	cv::Mat color_img; // 컬러 이미지 담을 Mat
	cv::cvtColor(gr_img, color_img, cv::COLOR_GRAY2BGR); // 회색 조 이미지 컬러로 만듦

	if (max_bVal * 100 > 50) {
		std::cout << "Burr 불량" << std::endl;
	}

	else if (max_iVal * 100 > 70) {
		std::cout << "impurity 불량" << std::endl;
	}

	else {
		std::cout << "void 불량" << std::endl;
	}

	cv::imshow("result", color_img); // 이미지 출력 함수 | ("창 이름", 불러올 이미지)
	// 사출불량(Burr, 이바리, 가장자리 성형 불량) / 이물질(Impurity) 검출 | 탬플릿 매칭으로 시도 | 성공
}