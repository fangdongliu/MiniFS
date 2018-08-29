#pragma once

#include<iostream>
#include"structDef.h"

namespace Three {

	class FileOperator
	{
	public:
		FileOperator();
		~FileOperator();

		static void CreateFS(std::string&name, long fileSize, int blockSize);


		void Format(long fileSize, int blockSize);

		int GetBlockSize() { return superHead.blockSize; }

		void ReleaseBlock(int blockId){}
		int RequestEmptyBlock();

		void ReadSuperHead();

		bool Ready() { return file != nullptr; }

		void Open(std::string&name, const char* type = "rb+");
		void Close();

		void SeekBlock(int blockId);
		void Flush();

		template<typename T>
		void Write(T t) {
			int len=fwrite(&t, 1, sizeof(T), file);
			
			if (len != sizeof(T)) 
				throw std::exception("write file failed!");
			
		}

		template<typename T>
		void Read(T& t) {
			int len = fread(&t, 1, sizeof(T), file);

			if (len != sizeof(T))
				throw std::exception("read file failed!");

		}

		template<>
		void Write<std::string>(std::string t) {
			short len = t.length();
			Write(len);

			int len2 = fwrite(t.c_str(), 1, len, file);

			if (len != len2)
				throw std::exception("write file failed!");

		}

		template<>
		void Read<std::string>(std::string& t) {
			short len;
			Read(len);
			
			char *buf = new char[len + 1];

			int len2 = fread(buf, 1, len, file);

			buf[len] = 0;

			if (len2 != len)
				throw std::exception("read file failed!");

			t = buf;
			delete[]buf;
		}



	private:
		FILE * file;
		SuperHead superHead;


	};

}