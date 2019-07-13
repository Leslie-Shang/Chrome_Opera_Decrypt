#pragma once
#ifndef _CryptUnProtectData_
#define _Crypt_UnProtectData_
#include <windows.h>
#include <iostream>
using namespace std;

char* U2G(const char* utf8);
string Password_Handle(char* input);

#endif // !CryptUnProtectData.h
