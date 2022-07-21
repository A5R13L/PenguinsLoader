#include "loader/headers/loader.h"

namespace Loader
{
	void Init()
	{
		curl_global_init(CURL_GLOBAL_ALL);
		Colors::Init();
		
		API::APIResponse Result = API::FetchAPIStatus();
		std::cout << Colors::BlueBright("") << std::endl;

		if (!Result.Online)
			Util::CenteredText(std::string("API is offline. [Error: ").append(!Result.Error.empty() ? Result.Error : "N/A").append("]"));
		else
		{
			std::cout << Colors::BlueBright("");
			std::vector<std::string> Lines;

			if (Result.Outdated)
			{
				Util::SplitByCharacter(Lines, OutOfDateHeader, '\n');
							
				for (auto Line : Lines)
					Util::CenteredText(Line.append("\n"));

				std::cout << Colors::Terminator("");
				API::DownloadUpdate();

				return;
			}

			Util::ReplaceAll(Header, "{MOTD}", Result.MOTD);
			Util::ReplaceAll(Header, "{VERSION}", Result.Version);
			Util::ReplaceAll(Header, "{HASH}", Result.DLLHash);
			Util::ReplaceAll(Header, "{COMMITS}", Result.Commits);
			Util::SplitByCharacter(Lines, Header, '\n');

			for (auto Line : Lines)
				Util::CenteredText(Line.append("\n"));

			std::cout << Colors::Terminator("") << std::endl;

			HTTP::SaveFileFromURL("https://penguins.asriel.dev/loader/download", [&Lines](std::string File)
			{
				system("cls");
				std::cout << Colors::BlueBright("") << std::endl;

				for (auto Line : Lines)
					Util::CenteredText(Line.append("\n"));

				std::cout << Colors::Terminator("") << std::endl;

				Inject::SetupInjection("gmod.exe", File);
			});
		}
	}
}

#undef max

int main(int ArgumentCount, char* Arguments[])
{
	if (ArgumentCount == 3)
	{
		if (strstr(Arguments[1], "-d"))
		{
			std::string Command = "DEL ";
			Command.append(Arguments[2]);

			system(Command.c_str());
		}
	}

	std::thread([]() {
		bool Pressed = false;

		while (true) {
			bool Key = GetKeyState(VK_F4) & 0x8000;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

			if (!Key && Pressed)
				Pressed = false;
			else if (Key && !Pressed)
			{
				Pressed = true;
				system("rundll32 url.dll,FileProtocolHandler \"https://penguins.asriel.dev/linker\"");
			}
		}
	}).detach();

	Loader::Init();
	std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::hours(std::numeric_limits<int>::max()));

	return 0;
}
