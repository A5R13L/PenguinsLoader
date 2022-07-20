#pragma once

namespace Colors
{
	HANDLE Console;

	void Init()
	{
		Console = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	const char* ModifyColor(const char* Output, WORD Params)
	{
		if (Console == nullptr) return "";

		SetConsoleTextAttribute(Console, Params);

		return Output;
	}

	const char* Terminator(const char* Input)
	{
		return ModifyColor(Input, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}

	const char* RedBright(const char* Input)
	{
		return ModifyColor(Input, FOREGROUND_RED | FOREGROUND_INTENSITY);
	}

	const char* GreenBright(const char* Input)
	{
		return ModifyColor(Input, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}

	const char* BlueBright(const char* Input)
	{
		return ModifyColor(Input, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}

	const char* YellowBright(const char* Input)
	{
		return ModifyColor(Input, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}

	const char* Gray(const char* Input)
	{
		return ModifyColor(Input, 7);
	}
}