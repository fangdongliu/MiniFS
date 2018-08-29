#pragma once
#include<string>
#include "MiniFS.h"
namespace Three {
	class App
	{
	public:
		App();
		~App();

	private:
		MiniFS miniFs;

	public:
		void Close();
		void Help();

		void Run();
	};

}