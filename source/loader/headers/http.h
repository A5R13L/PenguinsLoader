#pragma once

namespace HTTP
{
	void DoCURLRequest(std::function<void(CURL*)> Callback)
	{
		CURL* Handle = curl_easy_init();
		Callback(Handle);

		curl_easy_perform(Handle);
	}

	void SaveFileFromURL(const char* URL, std::function<void(std::string)> Callback, std::string FileName = "")
	{
		FILE* File;

		if (FileName.empty())
		{
			std::filesystem::path TempFile = getenv("TMP");
			TempFile.append(Util::RandomFile());

			FileName = TempFile.generic_string();
		}

		File = fopen(FileName.c_str(), "wb");

		DoCURLRequest([&URL, &File](CURL* Handle)
			{
				curl_easy_setopt(Handle, CURLOPT_URL, URL);
				curl_easy_setopt(Handle, CURLOPT_WRITEFUNCTION, NULL);
				curl_easy_setopt(Handle, CURLOPT_WRITEDATA, File);
				curl_easy_setopt(Handle, CURLOPT_NOPROGRESS, FALSE);
				curl_easy_setopt(Handle, CURLOPT_PROGRESSFUNCTION, Util::CURL_ProgressBar);
			});

		fclose(File);
		Callback(FileName);
	}

}