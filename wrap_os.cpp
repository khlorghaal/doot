//OS specific stuff
#import "time.hpp"

#ifndef DOOT_NO_MAIN
	extern int dootmain(int, char**);
#endif
// DOOT_NO_MAIN used for linked libs
// whom havent main

#ifdef __linux__
#include <time.h>
#include <sys/termios.h>

#ifndef DOOT_NO_MAIN
	int main(int argc, char** argv){
		return dootmain(argc,argv);
	}
#endif
namespace doot{

void create_console(){
	//TODO
}
nsec current_time(){
	timespec t;
	clock_gettime(CLOCK_MONOTONIC,&t);
	return t.tv_nsec;
}

}
#endif

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifndef DOOT_NO_MAIN
	int main(int argc, char* argv[], char* envp[]){
		#ifdef DEBUG
		//this is for a VS setup
		//assume windows users wont launch from term even when debug build
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

nsec current_time(){
	LARGE_INTEGER f;
	QueryPerformanceFrequency(&f);
	LARGE_INTEGER t;
	QueryPerformanceCounter(&t);
	return t.QuadPart/(f.QuadPart/1000000000ll);
}

void create_console(){
	//FreeConsole();
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
}

}

#endif


#include "file.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include "hash_map.hpp"


namespace doot{

bool file_lock(str& name){
	FILE* file= fopen(name, "rw");
	return !!ftrylockfile(file);
}
void file_unlock(str& name){
	FILE* file= fopen(name, "rw");
	funlockfile(file);
}
bool file_dump(vector<u8>& ret, str& name){
	FILE* file= fopen(name, "r");
	//char* errstr= strerror(ferr);
	if(!!errno)
		return true;

	ret.realloc_greed(0x1000);
	//todo use filesize metadata

	//SEEK_END doesnt always work, therefore
	//preallocation is unpossible but unnecessary
	char buf;
	while(fread(&buf,1,1,file))
		ret.add(buf);
	ret.add(0);//null terminator
	
	if(ferror(file)){
		fclose(file);
		return true;
	}
	fclose(file);
	return false;
}
bool file_dump(str& ret, str& name){
	vector<u8> res;
	if(file_dump(res,name))
		return true;
	EACH(r,res)
		ret+= r;
	return false;
}

struct fchgcall{
	void (*call)(void*);
	void* obj;
	void invoke(){
		call(obj);
	}
};
hash_map<char const*, fchgcall> fchgmap;
void fchg_(char const* fnam){
	if(fchgcall* call= fchgmap[fnam])
		call->invoke();
	else
		throw;
}
bool file_change_listen(str fname, void (*callback)(void*), void* callbackarg){
	#ifdef _WIN32

	#elif UNIX

	#endif
	fchgmap.add<fchgcall>(fname, {callback,callbackarg});
	return false;
}

}