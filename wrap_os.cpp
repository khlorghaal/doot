//OS specific stuff

int dootmain(int argc, char** argv);

#ifdef __linux__
#include <sys/time.h>
#include <sys/termios.h>

#ifndef DOOT_NO_MAIN
	int main(int argc, char** argv){
		dootmain(argc,argv);
	}
#endif
namespace doot{
void create_console(){

}
}
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef DOOT_NO_MAIN
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

time_t current_time(){
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t.QuadPart/(f.QuadPart/1000ll);
}
i64 current_time_us(){
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t.QuadPart/(f.QuadPart/1000000ll);
}

void create_console(){
	//FreeConsole();
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

}

#endif
