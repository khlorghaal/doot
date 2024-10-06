#pragma once
#include "list.hpp"
#include "string.hpp"

namespace doot{

bool file_lock(str& fname);
void file_unlock(str& fname);
bool file_dump(list<u8>&, str& name);
bool file_dump(str& ret,  str& name);//appends
bool file_change_listen(str fname, void (*callback)(void*), void* callbackarg);

}