#include "doot.hpp"
using namespace doot;

#ifdef _WIN32
	int dootmain(int argc, char** argv);

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
		//FreeConsole();
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}


	uint64 unendian(uint64 i){ return _byteswap_uint64(i); };
	uint32 unendian(uint32 i){ return _byteswap_ulong(i);  };
	uint16 unendian(uint16 i){ return _byteswap_ushort(i); };

	}

#endif
