#pragma once

char* file_dump(char const* name);

void file_change_listen(char const* fname, void* obj, void (*callback)(void*));