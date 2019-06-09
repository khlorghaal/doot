#ifdef _WIN32
int dootmain(int argc, char** argv);

using namespace doot;

#ifndef DOOT_NO_MAIN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

int main(int argc, char* argv[], char* envp[]){
	#ifdef DEBUG
	doot::create_console();
	#elif
	#error
	freopen("log.txt", "w", stdout);
	freopen("log.txt", "w", stderr);
	#endif

	return dootmain(argc, argv);
}
#endif


namespace doot{
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
