#include "MiniFS.h"
#include<exception>
#include<iostream>
#include<Windows.h>
using namespace std;
using namespace Three;

#define FILE_TYPE_UNUSED 0
#define FILE_TYPE_FOLDER 1
#define FILE_TYPE_FILE 2

namespace {
	void CutPathToList(string&pathName, vector<string>&out) {

		out.clear();
		int last = 0;
		int cur = 0;

		for (auto i : pathName) {
			if (i == '/' || i == '\\') {
				if (cur > last) {
					out.push_back(pathName.substr(last, cur - last));
				}
				last = cur + 1;
			}
			cur++;
		}

		if (cur > last) {
			out.push_back(pathName.substr(last, cur - last));
		}
	}

}


MiniFS::MiniFS()
{
	current = &root;
	temp = nullptr;
}


MiniFS::~MiniFS()
{
	Reset();
	op.Close();
}

void MiniFS::Reset() {

	current = &root;
	temp = nullptr;
	root.Clear();
	recyclerBin.Clear();

	

}

void MiniFS::CreateFS(std::string&name, long long fileSize, int blockSize) {

	FileOperator::CreateFS(name, fileSize, blockSize);

}

void MiniFS::Mount(std::string&name) {

	Reset();

	op.Close();
	op.Open(name);

	op.ReadSuperHead();

	root.ReadAsRoot(op);

	recyclerBin.ReadAsRecycleBin(op);
}

void MiniFS::Format(long long fileSize, int blockSize) {

	Reset();
	op.Format(fileSize, blockSize);

}

void MiniFS::ShowDir(std::string& folderName) {

	vector<string>pathList;

	CutPathToList(folderName, pathList);

	FSFolder* dest = current->GetDestFolder(pathList, op);

	if (dest) {
		dest->Show();
	}
	else {
		cout << "目标路径不存在" << endl;
	}

}

void MiniFS::ChangeDir(std::string&pathName) {

	vector<string>pathList;
	CutPathToList(pathName, pathList);
	auto dest = current->GetDestFolder(pathList, op);

	if (dest) {
		current = dest;
	}
	else {
		cout << "目标路径不存在" << endl;
	}

}

void MiniFS::ShowFSFileAttr(std::string&fileName) {

	vector<string>pathList;
	CutPathToList(fileName, pathList);
	string name = pathList.back();

	pathList.pop_back();

	auto dest = current->GetDestFolder(pathList, op);

	if (dest) {
		auto child = dest->GetChild(name);

		if (child) {
			child->ShowAttr();
		}
		else {
			cout << "目标文件不存在" << endl;
		}

	}
	else {
		cout << "目标路径不存在" << endl;
	}


}

void MiniFS::CreateFSFile(std::string&fileName) {

	vector<string>pathList;
	CutPathToList(fileName, pathList);
	string name = pathList.back();

	pathList.pop_back();

	auto dest = current->GetDestFolder(pathList, op);

	if (dest) {

		if (auto&child=dest->GetChild(name)) {
			cout << "file already exist\n";
		}
		else {

			FSFileHead head;

			int emptyBlockId = op.RequestEmptyBlock();

			child = new FSFile(emptyBlockId);

			child->SetFileName(name);

			child->SetParent(dest);

			dest->UpdateDir(op);
		}
	}
}

void MiniFS::CreateFSFolder(std::string&folderName) {

	vector<string>pathList;
	CutPathToList(folderName, pathList);
	string name = pathList.back();

	pathList.pop_back();

	auto dest = current->GetDestFolder(pathList, op);
	
	if (dest) {


		if (auto&child=dest->GetChild(name)) {
			cout << "file already exist\n";
		}
		else {
			FSFileHead head;

			int emptyBlockId = op.RequestEmptyBlock();

			child = new FSFolder(emptyBlockId);

			child->SetFileName(name);

			child->SetParent(dest);

			dest->UpdateDir(op);
		}
	}
}

void MiniFS::ShowPrefix() {

	if (op.Ready()) {

		string buf;
		current->GetAbsolutePath(buf);

		cout <<"root:/"<< buf << " : ";

	}
	else {
		cout << "Mini-FS : ";
	}

}