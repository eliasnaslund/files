#include <iostream>
#include <fstream>
#include <thread>

#include <stdlib.h>
#include <unistd.h>

#include "subprocess.hpp"

using namespace subprocess::literals;

bool done{false};

std::string path;

bool command(const char * cmd)
{
	std::array<char, 128> buffer;
	std::string result;

	auto pipe = popen(cmd, "r");
	if (!pipe)
	{
		std::cout << "Failed to open pipe\n";
		return false;
	}

	while (fgets(buffer.data(), 128, pipe) != nullptr)
	{
		result += buffer.data();
	}

	auto rc = pclose(pipe);

	return rc == EXIT_SUCCESS;
}

void tracking()
{
	while (!done)
	{
		//std::string output;
		//auto start = std::chrono::high_resolution_clock::now();
		//("chrt 70 timeout 0.1 chronyc tracking"_cmd > output).run(std::nothrow);
		//auto stop = std::chrono::high_resolution_clock::now();
		//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
		//std::cout << "chrony " << output.size() << " in " << duration.count() << "ms" << "\n";
		auto cmd = "timeout 0.1 chronyc tracking";
		auto res = command(cmd);
		std::cout << "chrony " << res << "\n";

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void write_file()
{
	while (!done)
	{
		// Open file, do some writing, close file
		std::ofstream file;
		file.open(path);

		for (int i = 0; i < 100000; i++)
		{
			file << "lmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvbcdefghjklmnopqrstuvabcdefghjklmnopqrstuvlmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvabcdefghjklmnopqrstuvbcdefghjklmnopqrstuvabcdefghjklmnopqrstuv";
		}
		file.close();
	}
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << argv[0] << " <both/chrony/file> /path/to/file\n";
		return 1;
	}
	std::string cmd = argv[1];
	path = argv[2];

	if (cmd == "both")
	{
		std::thread t1(tracking);
		std::thread t2(write_file);
		std::this_thread::sleep_for(std::chrono::minutes(2));
		done = true;
		t1.join();
		t2.join();
	} 
	else if (cmd == "chrony")
	{
		std::thread t1(tracking);
		std::this_thread::sleep_for(std::chrono::minutes(2));
		t1.join();
		done = true;
	}
	else if (cmd == "file")
	{
		std::thread t2(write_file);
		std::this_thread::sleep_for(std::chrono::minutes(2));
		done = true;
		t2.join();
	}
	else
	{
		return 1;
	}

	return 0;
}
