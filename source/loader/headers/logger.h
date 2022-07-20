#pragma once

namespace Logger
{
	std::stringstream Debug;

	class _Log
	{
	public:
		std::function<const char* (void)> Type;
		std::function<const char* (void)> Prefix;
		std::string Message;
	};

	std::vector<std::unique_ptr<_Log>> Logs;
	std::mutex CoutLock;

	void Log(std::function<const char* (void)> Type, std::function<const char* (void)> Prefix, std::string Message)
	{
		CoutLock.lock();
		std::cout << Colors::Gray("[");
		std::cout << Type();
		std::cout << Colors::Gray("]");

		std::string PrefixBuffer = Prefix();

		if (PrefixBuffer == "")
			std::cout << ": ";
		else
		{
			std::cout << Colors::Gray(" [");
			std::cout << Prefix();
			std::cout << Colors::Gray("]: ");
		}

		std::cout << Message << std::endl;
		CoutLock.unlock();
	}
}