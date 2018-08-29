#include "App.h"
#include<string>
#include <iostream>
#include <unordered_map>
#include<sstream>
using namespace std;
using namespace Three;

extern void WriteSuperHead(FILE* file, SuperHead&head);

namespace {
	enum class Command {
		Unhandled,
		Close,
		Help,
		Mount,
		Create,
		Format,
		Dir,
		ChangeDir,
		Delete,
		ShowFile,
		ShowFileMore,
		ShowFileAttr,
		Copy,
		CreateFSFile,
		CreateFSFolder,
		Rename,
	};

	static unordered_map<string, Command>commandMap = {
	{"close",Command::Close},
	{"help",Command::Help},
	{"mount",Command::Mount},
	{"create",Command::Create},
	{"fmt",Command::Format},
	{"dr",Command::Dir},
	{"cd",Command::ChangeDir },
	{"dl",Command::Delete},
	{"tp",Command::ShowFile},
	{"more",Command::ShowFileMore},
	{"att",Command::ShowFileAttr},
	{"cp",Command::Copy},
	{"createFile",Command::CreateFSFile},
	{"createFolder",Command::CreateFSFolder},
	{"rename",Command::Rename},
	};

}

App::App()
{
}


App::~App()
{
}

void App::Close() {

	miniFs.Reset();

}

void App::Help() {

	cout << "help" << endl;

}

void App::Run() {

	string cmd;
	string param1;
	string param2;
	miniFs.ShowPrefix();
	while (getline(cin, cmd)) {

		if (cmd.length() == 0) {
			miniFs.ShowPrefix();
			continue;
		}

		stringbuf s(cmd);
		istream in(&s);

		in >> param1;

		Command mappedCmd = commandMap[param1];

		switch (mappedCmd)
		{
		case Command::Close:
			Close();
			return;
		case Command::Help:
			Help();
			break;
		case Command::Mount:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
			}
			else {
				miniFs.Mount(param2);
			}

			break;
		case Command::Create:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
				break;
			}
			{
				long long fileSize = 1 << 30;
				in >> fileSize;

				int blockSize = 1 << 11;
				in >> blockSize;

				miniFs.CreateFS(param2, fileSize, blockSize);
			}
			break;
		case Command::Format:
			{
				long long fileSize = 1 << 30;
				in >> fileSize;

				int blockSize = 1 << 11;
				in >> blockSize;

				miniFs.Format(fileSize, blockSize);
			}
			break;
		case Command::Dir:
			in >> param2;
			if (in.fail()) {
				param2 = "";
			}
			miniFs.ShowDir(param2);
			break;
		case Command::ChangeDir:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
				break;
			}
			miniFs.ChangeDir(param2);
			break;
		case Command::Delete:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
				break;
			}
			miniFs.DelFSFile(param2);
			break;
		case Command::ShowFile:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
				break;
			}
			miniFs.ShowFSFile(param2,false);
			break;
		case Command::ShowFileMore:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
				break;
			}
			miniFs.ShowFSFile(param2,true);
			break;
		case Command::ShowFileAttr:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
				break;
			}
			miniFs.ShowFSFileAttr(param2);
			break;
		case Command::Copy:
			in >> param2;
			if (in.fail()) {
				cout << "please input the first filename\n";
			}
			else{
				string param3;
				in >> param3;

				if (in.fail()) {
					cout << "please input the second filename\n";
				}
				else {
					miniFs.CopyFSFile(param2, param3);
				}
			}
			break;
		case Command::CreateFSFolder:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
				break;
			}
			miniFs.CreateFSFolder(param2);
			break;
		case Command::CreateFSFile:
			in >> param2;
			if (in.fail()) {
				cout << "please input a name\n";
				break;
			}
			miniFs.CreateFSFile(param2);
			break;
		default:
			cout << "wrong command\n";
			break;
		}

		miniFs.ShowPrefix();
	}
}


