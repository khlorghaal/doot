#ifdef _WIN32

namespace windows{

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdio>
#include <string.h>
#include "vector.hpp"

#ifndef DOOT_NO_MAIN
#ifndef __GNUC__
#ifndef CLANG
int main(int argc, char** argv);
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_ HINSTANCE hPrevInstance, 
	_In_ LPSTR pCmdLine, 
	_In_ int nCmdShow){
	vector<char*> args;

	char* mark= strtok(pCmdLine," ");
	while(!!mark){
		mark= strtok(NULL," ");
		args.push(mark);
	}

	int argc= args.size();
	char** argv= args.base;

	#ifdef DEBUG
	doot::create_console();
	#elif
	#error
	freopen("log.txt", "w", stdout);
	freopen("log.txt", "w", stderr);
	#endif

	return main(argc, argv);
}
LRESULT CALLBACK WndProc(  
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam){
	return 0;
}
#endif
#endif
#endif
}

namespace doot{
using namespace windows;
typedef time_t int64_t;//ms
time_t current_time(){
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t.QuadPart/(f.QuadPart/1000ll);
}
int64 current_time_us(){
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t.QuadPart/(f.QuadPart/1000000ll);
}
void create_console(){
	FreeConsole();
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}
}

#endif
