﻿#include "Util.h"
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

using namespace std;

string FormatTime(time_t tm) {
	char buffer[9] = { 0 };
	strftime(buffer, 9, "%H:%M:%S", localtime(&tm));
	return buffer;
}

void PrintTime(std::function<void()> fn, const std::string& tag) {
	float start = GetTime();
	cerr << tag << " started at " << FormatTime(time(0)) << endl;

	fn();

	float end = GetTime();
	cerr << tag << " finished at " << FormatTime(time(0)) << ",in " << (end - start) / 1000.0f << "s" << endl;
}


#ifdef _WIN32

static LONGLONG InitTimeVal() {
	static bool flag = true;
	static LARGE_INTEGER time;
	if (flag) {
		flag = false;
		QueryPerformanceCounter(&time);
	}
	return time.QuadPart;
}

float GetTime() {
	LARGE_INTEGER time;
	QueryPerformanceFrequency(&time);
	LONGLONG freq = time.QuadPart;
	QueryPerformanceCounter(&time);
	return (time.QuadPart - InitTimeVal()) * 1000.0f / freq;
}


#else

static time_t InitTimeVal() {
	static bool flag = true;
	static timeval time;
	if (flag) {
		flag = false;
		gettimeofday(&time, NULL);
	}
	return time.tv_sec;
}

float GetTime() {
	timeval time;
	gettimeofday(&time, NULL);
	return (time.tv_sec - InitTimeVal()) * 1000.0f + time.tv_usec / 1000.0f;
}

#endif