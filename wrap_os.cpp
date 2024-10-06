//OS specific stuff

#ifndef DOOT_NO_MAIN
	extern int dootmain(int, char**);
#endif
// DOOT_NO_MAIN for main't-ed linked libs

#ifdef __linux__
#include <time.h>
#include <sys/termios.h>
#include <pthread.h>
#include <stdio.h>
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

/*
#ifdef _WIN32
#endif
#ifdef __linux__
#endif
*/

#ifndef DOOT_NO_MAIN
	#import "thread.hpp"
	#ifdef _WIN32
		int main(int argc, char* argv[], char* envp[]){
	#endif
	#ifdef __linux__
		int main(int argc, char** argv){
	#endif

			doot::warp::init();

			#ifdef DEBUG
				#ifdef _WIN32
					//this is for a VS setup
					//assume windows users wont launch from term even when debug build
					doot::create_console();
					freopen("stdout.txt", "w", stdout);
					freopen("stderr.txt", "w", stderr);
				#endif

				#ifdef __linux__
					//gdb doesnt like stdio being rerouted, this is easier
					freopen("stdout", "w", stdout);
					freopen("stderr", "w", stderr);
				#endif

				::doot::run_tests();

			#endif

			return dootmain(argc,argv);
		}
#endif

#import "time.hpp"

namespace doot{

void create_console(){
	#ifdef _WIN32
		//FreeConsole();
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	#endif
	#ifdef __linux__
		//todo what even
	#endif
}
nsec current_time(){
	#ifdef _WIN32
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		return t.QuadPart/(f.QuadPart/1000000000ll);
	#endif
	#ifdef __linux__
		timespec t;
		clock_gettime(CLOCK_MONOTONIC,&t);
		return t.tv_nsec;
	#endif
}

void namethread(cstr s){
	#ifdef _WIN32
		//fixme link Kernel32.lib
		#error
		SetThreadDescription(GetCurrentThread(), s);
	#endif
	#ifdef __linux__
	    pthread_setname_np(pthread_self(), s);
	#endif
}

}


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "file.hpp"
#include "hmap.hpp"


namespace doot{

bool file_lock(str& name){
	FILE* file= fopen(name, "rw");
	#ifdef _WIN32
		#error
	#endif
	#ifdef __linux__
		bool r= !!ftrylockfile(file);
	#endif
	fclose(file);
	retr;
}
void file_unlock(str& name){
	FILE* file= fopen(name, "rw");
	#ifdef _WIN32
		#error
	#endif
	#ifdef __linux__
		funlockfile(file);
	#endif
	fclose(file);
}

struct fchgcall{
	#ifdef _WIN32
	#endif
	#ifdef __linux__
	#endif
	void (*call)(void*);
	void* obj;
	void invoke(){
		call(obj);
	}
};
hmap<str, fchgcall> fchgmap;
void fchg_(str fnam){
	may_if(call, fchgmap[fnam])
		call.invoke();
	else
		err("fook fnam findn't");//future self will HATE this //future self: lmoa
}
bool file_change_listen(str fname, void (*callback)(void*), void* callbackarg){
	#ifdef _WIN32
	#endif
	#ifdef __linux__
	#endif
	fchgmap.add<fchgcall>(fname, {callback,callbackarg});
	return false;
}

}