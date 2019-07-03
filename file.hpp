#pragma once
#include "vector.hpp"
#include "string.hpp"

namespace doot{

bool file_lock(string fname);
bool file_unlock(string fname);
bool file_dump(vector<byte>&, string name);//null terminated
bool file_dump(string& ret, string name);//overwrites ret
bool file_change_listen(string fname, void (*callback)(void*), void* callbackarg);

}