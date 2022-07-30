#pragma once
#include "vector.hpp"
#include "string.hpp"

namespace doot{

bool file_lock(str& fname);
bool file_unlock(str& fname);
bool file_dump(vector<ui8>&, str& name);//null terminated
bool file_dump(str& ret, str& name);//overwrites ret
bool file_change_listen(str fname, void (*callback)(void*), void* callbackarg);

}