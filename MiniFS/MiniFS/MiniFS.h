#pragma once
#include <vector>
#include"FSFolder.h"
namespace Three {

	template<typename T>
	void FSWrite(FILE* file, T data) {
		fwrite(&data, 1, sizeof(T), file);
	}

	class MiniFS
	{
	public:
		MiniFS();
		~MiniFS();
	private:
		FileOperator op;

		FSFolder *current;
		FSFolder root;
		FSFolder recyclerBin;

		FSFile * temp;
	public:
		void Reset();

		void CreateFS(std::string&name, long long fileSize, int blockSize);
		void Mount(std::string&name);
		void Format(long long fileSize, int blockSize);

		void ShowDir(std::string& folderName);
		void ChangeDir(std::string&pathName);

		void ShowFSFileAttr(std::string&fileName);
		void ShowFSFile(std::string& fileName,bool more){}
		void CopyFSFile(std::string&from,std::string&to){}
		void DelFSFile(std::string&fileName){}

		
		

		void CreateFSFile(std::string&fileName);
		void CreateFSFolder(std::string&folderName);

		void CutFSFile();
		void PasteFSFile();

		void ShowPrefix();
	};

}