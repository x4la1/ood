#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include "InputStream.h"
#include "OutputStream.h"
#include "FileTransformer.h"

int main(int argc, char* argv[])
{
	std::vector<std::string> args;
	for (int i = 1; i < argc; ++i)
	{
		args.push_back(argv[i]);
	}

	try
	{
		FileTransformer ft(args);
		ft.Transform();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return 0;
	}

	return 0;
}


