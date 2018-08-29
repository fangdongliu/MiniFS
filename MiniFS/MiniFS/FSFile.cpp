#include "FSFile.h"
#include <windows.h>
#pragma comment(lib,"winmm.lib")

using namespace Three;
using namespace std;

FSFile::FSFile() {
	fileHead.type = FS_FILETYPE_FILE;
	fileHead.createTime = timeGetTime();
}

FSFile::FSFile(int blockId):FSFile() {

	fileHead.blockId = blockId;

}

void FSFile::ShowAttr() {

	cout << "filename:" << fileHead.filename<<endl;
	

}