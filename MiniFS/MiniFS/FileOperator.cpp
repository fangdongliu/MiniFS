#include "FileOperator.h"

using namespace Three;
using namespace std;

FileOperator::FileOperator()
{
	file = nullptr;
}


FileOperator::~FileOperator()
{
	Close();
}

int FileOperator::RequestEmptyBlock() {

	if (superHead.emptyBlockCount > 0) {
		superHead.emptyBlockCount--;

		int ans = superHead.firstEmptyBlockId;
	
		SeekBlock(superHead.firstEmptyBlockId);

		BlockHead blockHead;
		Read(blockHead);

		superHead.firstEmptyBlockId = blockHead.nextBlockId;

		SeekBlock(ans);

		blockHead.nextBlockId = 0;
		blockHead.refCount = 1;
		blockHead.size = 0;

		Write(blockHead);
		Flush();
		return ans;
	}
	else {
		throw exception("space not enough!");
	}

}

void FileOperator::CreateFS(std::string&name, long fileSize, int blockSize) {

	FileOperator op;
	op.Open(name, "wb");
	op.Format(fileSize, blockSize);
}

void FileOperator::Format(long fileSize, int blockSize) {

	SeekBlock(0);
	
	superHead.blockSize = blockSize;
	superHead.fileSize = fileSize;
	superHead.firstEmptyBlockId = 2;
	superHead.lastEmptyBlockId = fileSize / blockSize - 1;
	superHead.emptyBlockCount = superHead.lastEmptyBlockId - superHead.firstEmptyBlockId + 1;
	superHead.recycleBinCount = 0;
	superHead.recycleBinNextBlockId = 0;

	Write(superHead);

	SeekBlock(1);

	BlockHead blockHead;
	blockHead.nextBlockId = 0;
	blockHead.size = 0;
	blockHead.refCount = 0;

	Write(blockHead);

	int emptyCount = superHead.emptyBlockCount;

	int cur = 2;
	SeekBlock(cur);

	do {
		blockHead.nextBlockId = ++cur;
		Write(blockHead);
		SeekBlock(cur);
	} while (--emptyCount);
	blockHead.nextBlockId = 0;
	Write(blockHead);

	Flush();
}


void FileOperator::ReadSuperHead() {

	SeekBlock(0);
	Read(superHead);

	Flush();

}

void FileOperator::Open(std::string&name, const char* type /* = "rb+" */) {

	if (fopen_s(&file, name.c_str(), type)) {
		throw exception("open file failed!");
	}
}

void FileOperator::Close() {

	if (file) {
		SeekBlock(0);
		Write(superHead);

		Flush();
		fclose(file);
		file = 0;
	}

}

void FileOperator::SeekBlock(int blockId) {

	long dest = blockId;
	dest *= superHead.blockSize;

	fseek(file, dest, SEEK_SET);
}

void FileOperator::Flush() {

	fseek(file, 0, SEEK_CUR);

}
