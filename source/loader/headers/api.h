#pragma once

namespace API
{
	struct APIResponse
	{
		bool Online = false; std::string Error = "";
		bool Outdated = false;
		std::string MOTD = "";
		std::string DLLHash = "";
		std::string Version = "";
		std::string Commits = "";
	};

	void DownloadUpdate()
	{
		std::string FileName;
		TCHAR CurrentFileName[MAX_PATH];
		GetModuleFileName(NULL, CurrentFileName, sizeof(CurrentFileName));
		std::string Command = "RENAME ";
		std::string RandomFileName = Util::RandomFile(true);

		Command.append("\"").append(CurrentFileName).append("\" ").append(RandomFileName);

		system(Command.c_str());

		HTTP::SaveFileFromURL("https://penguins.asriel.dev/download", [&Command, &CurrentFileName, &RandomFileName](std::string) {
			std::string Arguments = "-d ";
			Arguments.append(RandomFileName);

			Killed = true;
			ShellExecute(NULL, "open", CurrentFileName, Arguments.c_str(), NULL, SW_SHOWDEFAULT);
			}, CurrentFileName);
			exit(0);
	}

	APIResponse FetchAPIStatus()
	{
		APIResponse Result;
		std::string Response;

		HTTP::DoCURLRequest([&Response](CURL* Handle)
		{
			curl_easy_setopt(Handle, CURLOPT_URL, "https://penguins.asriel.dev/loader/check?version=" LOADER_VERSION);
			curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, Util::CURL_WriteString);
			curl_easy_setopt(Handle, CURLOPT_WRITEDATA, &Response);
		});

		if (Response.find("<html") == std::string::npos)
		{
			nlohmann::json Object = nlohmann::json::parse(Response);
			Result.Online = Object.contains("motd");
			Result.Error = Object.contains("error") ? Object["error"] : "";
			Result.Outdated = Object.contains("outdated");
			Result.MOTD = Object.contains("motd") ? Object["motd"] : "";
			Result.Version = Object.contains("version") ? Object["version"] : "";
			Result.DLLHash = Object.contains("hash") ? Object["hash"] : "";
			Result.Commits = Object.contains("commits") ? Object["commits"] : "";
		}

		return Result;
	}
}