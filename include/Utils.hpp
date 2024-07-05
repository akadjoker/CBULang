
#pragma once

typedef unsigned char u8;
typedef signed char s8;
typedef char c8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long int u64;
typedef long int s64;
typedef float f32;
typedef double f64;

#define CONSOLE_COLOR_RESET "\033[0m"
#define CONSOLE_COLOR_GREEN "\033[1;32m"
#define CONSOLE_COLOR_RED "\033[1;31m"
#define CONSOLE_COLOR_PURPLE "\033[1;35m"
#define CONSOLE_COLOR_CYAN "\033[0;36m"
#define CONSOLE_COLOR_YELLOW "\033[1;33m"
#define CONSOLE_COLOR_BLUE "\033[0;34m"

void Log(int severity, const char *fmt, ...);

