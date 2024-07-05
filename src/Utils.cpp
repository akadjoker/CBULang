#include "Utils.hpp"
#include <stdarg.h>
#include <time.h>
#include <string.h>

void Log(int severity, const char* fmt, ...)
{

		const char* type;
		const char* color;
		switch (severity) 
        {
			case  0:
				type = "info";
				color = CONSOLE_COLOR_GREEN;
				break;
			case 1:
				type = "warning";
				color = CONSOLE_COLOR_PURPLE;
				break;
            case 2:
				type = "error";
				color = CONSOLE_COLOR_RED;
				break;
            case 3:
                type = "PRINT";
                color = CONSOLE_COLOR_YELLOW;
                break;
			default:
				break;
		}


		time_t rawTime;
		struct tm* timeInfo;
		char timeBuffer[80];

		time(&rawTime);
		timeInfo = localtime(&rawTime);

		strftime(timeBuffer, sizeof(timeBuffer), "[%H:%M:%S]", timeInfo);

		char consoleFormat[1024];
		snprintf(consoleFormat, 1024, "%s%s %s%s%s: %s\n", CONSOLE_COLOR_CYAN,
				timeBuffer, color, type, CONSOLE_COLOR_RESET, fmt);


		char fileFormat[1024];
		snprintf(fileFormat, 1024, "%s %s: %s\n", timeBuffer, type, fmt);

		va_list argptr;

		va_start(argptr, fmt);
		vprintf(consoleFormat, argptr);
		va_end(argptr);

		char buf[4096];

		va_start(argptr, fmt);
		vsprintf(buf, fmt, argptr);
		va_end(argptr);
}
