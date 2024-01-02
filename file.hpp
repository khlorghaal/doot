#pragma once
#include "vector.hpp"
#include "string.hpp"

namespace doot{

bool file_lock(str& fname);
void file_unlock(str& fname);
bool file_dump(vector<u8>&, str& name);//null terminated
bool file_dump(str& ret, str& name);//overwrites ret
bool file_change_listen(str fname, void (*callback)(void*), void* callbackarg);

}