#pragma once
#include "util.h"

namespace Inject
{
	void Inject(DWORD PID, std::string File)
	{
		const char* DLLPath = File.c_str();
		HANDLE Process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		LPVOID DLL = VirtualAllocEx(Process, 0, strlen(DLLPath) + 1, MEM_COMMIT, PAGE_READWRITE);
		WriteProcessMemory(Process, DLL, (LPVOID)DLLPath, strlen(DLLPath) + 1, 0);

		HANDLE LoadThread = CreateRemoteThread(Process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), DLL, 0, 0);
		WaitForSingleObject(LoadThread, 1);
		VirtualFreeEx(Process, DLL, strlen(DLLPath) + 1, MEM_RELEASE);

		int Seconds = 5;
		std::string String;

		while (Seconds > 0)
		{
			std::cout << Colors::Gray("") << "\r";
			String = "Successfully injected. Closing in ";
			String.append(std::to_string(Seconds)).append(" second").append(Seconds != 1 ? "s" : "").append(".");
			Util::CenteredText(String.c_str());

			Seconds--;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		exit(0);
	}

	void SetupInjection(std::string Process, std::string File)
	{
		DWORD PID = Util::GetPIDFromName(Process.c_str());

		if (!PID)
		{
			std::cout << Colors::Gray("") << "\r";
			std::string Line("Waiting for ");

			Line.append(Process).append(".");
			Util::CenteredText(Line);
		}

		std::thread([&PID, &Process, &File]()
		{
			while (!(PID = Util::GetPIDFromName(Process.c_str())))
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

			std::cout << Colors::GreenBright("") << "\r";
			Util::CenteredText("Press F2 to inject on main menu.");

			while (!(GetKeyState(VK_F2) & 0x8000))
				std::this_thread::sleep_for(std::chrono::milliseconds(100));

			Inject(PID, File);
		}).join();
	}
}