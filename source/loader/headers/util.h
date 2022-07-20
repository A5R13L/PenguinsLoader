#pragma once

namespace Util
{
	void CenteredText(std::string Text)
	{
		HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
		PCONSOLE_SCREEN_BUFFER_INFO BufferInfo = new CONSOLE_SCREEN_BUFFER_INFO();
		GetConsoleScreenBufferInfo(Console, BufferInfo);

		COORD NewSBSize = BufferInfo->dwSize;
		if (NewSBSize.X > Text.size())
		{
			size_t Position = ((NewSBSize.X - Text.size()) / 2);
			for (size_t Index = 0; Index < Position; Index++) std::cout << " ";
		}

		std::cout << Text;
	}

	void SplitByCharacter(std::vector<std::string>& Keys, std::string Key, char Splitter)
	{
		std::stringstream String(Key);
		std::string Buffer;

		while (std::getline(String, Buffer, Splitter))
		{
			Keys.push_back(Buffer);
		}
	}

	void ReplaceAll(std::string& Input, const std::string From, const std::string To)
	{
		size_t Position = 0;

		while ((Position = Input.find(From, Position)) != std::string::npos)
		{
			Input.replace(Position, From.length(), To);
			Position += To.length();
		}
	}

	const char AlphaNumbers[] = "0123456789""ABCDEFGHIJKLMNOPQRSTUVWXYZ""abcdefghijklmnopqrstuvwxyz";
	int StringLength = sizeof(AlphaNumbers) - 1;

	std::string RandomFile(bool IsExe = false)
	{
		srand((int)time(0));

		std::string Buffer;
		for (size_t Index = 0; Index < 32; Index++)
			Buffer += AlphaNumbers[rand() % StringLength];

		Buffer.append(IsExe ? ".exe" : ".dll");
		return Buffer;
	}

	size_t CURL_WriteString(char* Contents, size_t Size, size_t BufferSize, void* Buffer)
	{
		((std::string*)Buffer)->append(Contents, Size * BufferSize);
		return Size * BufferSize;
	}

	int BarCurrent;
	double LastProgress, ProgressBarDiff;

	int CURL_ProgressBar(void* Bar, double Total, double Downloaded)
	{
		if (Total == 0 || Downloaded == 0) return 0;

		if (LastProgress != round(Downloaded / Total * 100))
		{
			BarCurrent = 25;
			ProgressBarDiff = round(Downloaded / Total * BarCurrent);

			std::string Buffer;

			std::cout << "\r";

			Buffer.append("[");

			if (round(Downloaded / Total * 100) < 10)
				Buffer.append("0").append(std::to_string((int)round(Downloaded / Total * 100)));
			else
				Buffer.append(std::to_string((int)round(Downloaded / Total * 100)));

			Buffer.append("%] [");

			for (int i = 0; i <= ProgressBarDiff; i++)
				Buffer.append("#");

			for (int i = 0; i < BarCurrent - ProgressBarDiff; i++)
				Buffer.append(".");

			Buffer.append("]");
			std::cout << Colors::Gray("");
			CenteredText(Buffer);
			LastProgress = round(Downloaded / Total * 100);
		}

		return 0;
	}

	DWORD GetPIDFromName(const char* Name) {
		PROCESSENTRY32 Process;
		Process.dwSize = sizeof(PROCESSENTRY32);

		HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (Process32First(Snapshot, &Process) == TRUE)
		{
			while (Process32Next(Snapshot, &Process) == TRUE)
			{
				if (_stricmp(Process.szExeFile, Name) == 0)
				{
					CloseHandle(Snapshot);
					return Process.th32ProcessID;
				}
			}
		}

		CloseHandle(Snapshot);
		return 0;
	}
}