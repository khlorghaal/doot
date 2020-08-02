#include "file.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "hash_map.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#elif UNIX

#endif

namespace doot{

//LOTSA TODO

bool file_lock(str& fname){
	#ifdef _WIN32

	#elif UNIX

	#endif
	return false;
}
bool file_unlock(str& fname){
	#ifdef _WIN32

	#elif UNIX

	#endif
	return false;
}
bool file_dump(vector<byte>& ret, str& name){
	FILE* file= 0;
	errno_t ferr= fopen_s(&file, name.cstr(), "r");
	char* errstr= strerror(ferr);
	if(!!ferr)
		return true;

	ret.realloc_greed(0x1000);

	//SEEK_END doesnt always work, therefore
	//preallocation is unpossible but unnecessary
	char buf;
	while(fread(&buf,1,1,file))
		ret.make(buf);
	ret.make(0);//null terminator
	
	if(ferror(file)){
		fclose(file);
		return true;
	}
	fclose(file);
	return false;
}
bool file_dump(str& ret, str& name){
	vector<char> res;
	if(file_dump(res,name))
		return true;
	ret= res;
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
	fchgmap.make<fchgcall&&>(fname.cstr(), {callback,callbackarg});
	return false;
}

}