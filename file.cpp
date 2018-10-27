#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include "vector.hpp"
#include "hash_map.hpp"

#ifdef _WIN32
namespace windows{
#include <Windows.h>
}
#elif UNIX

#endif

//LOTSA TODO

bool file_lock(char const* fname){
	#ifdef _WIN32
	//LockFile();
	#elif UNIX

	#endif
	return false;
}
bool file_unlock(char const* fname){
	#ifdef _WIN32
	//LockFile();
	#elif UNIX

	#endif
	return false;
}

char* file_dump(char const* name){
	FILE* file= fopen(name,"r");
	if(!file)
		return 0;
	
	vector<char> vec(0x800);
	//SEEK_END doesnt always work
	//preallocation is unpossible,
	//but unnecessary
	char buf;
	while(fread(&buf,1,1,file))
		vec<<buf;
	vec<<0;//null terminator
	
	if(ferror(file)){
		fclose(file);
		return 0;
	}

	fclose(file);
	void* ret= vec.base;
	vec.base= 0;//dont free
	return (char*)ret;
}

struct fchgcall{
	void* obj;
	void (*call)(void*);
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
void file_change_listen(char const* fname, void* obj, void (*callback)(void*)){
	#ifdef _WIN32

	#elif UNIX

	#endif
	*fchgmap.put(fname)= {obj,callback};
}