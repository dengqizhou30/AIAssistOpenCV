// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <memory>

using namespace std;

#define LOG_FILENAME "logfile.log"
#define BUF_SIZE 512
#define __L_INFO(format, ...) { FILE* fp = NULL; fopen_s(&fp, LOG_FILENAME, "a+"); if(fp) { char buf[BUF_SIZE]; _snprintf_s(buf, BUF_SIZE, _TRUNCATE, format, __VA_ARGS__); fprintf(fp, buf); fprintf(fp, "\n"); fclose(fp); }}
#define __L_ERROR(format, ...) { FILE* fp = NULL; fopen_s(&fp, LOG_FILENAME, "a+"); if(fp) { char buf[BUF_SIZE]; _snprintf_s(buf, BUF_SIZE, _TRUNCATE, format, __VA_ARGS__); fprintf(fp, buf); fprintf(fp, "\n"); fclose(fp); }}
#define __L_FUNC __L(__FUNCTION__)
#define __L_MTHD __L(__FUNCTION__" [%p]", (void*) this)