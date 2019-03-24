#pragma once
#include "vector.hpp"

bool file_lock(string fname);
bool file_unlock(string fname);
bool file_dump(vector<byte>&, string name);//null terminated
bool file_change_listen(string fname, void (*callback)(void*), void* callbackarg);