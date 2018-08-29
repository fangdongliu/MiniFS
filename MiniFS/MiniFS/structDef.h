#include <string>

namespace Three {

	struct SuperHead {
		long fileSize;
		int blockSize;
		int firstEmptyBlockId;
		int lastEmptyBlockId;
		int emptyBlockCount;
		int recycleBinCount;
		int recycleBinNextBlockId;
	};

	struct BlockHead {
		int refCount;
		int size;
		int nextBlockId;
	};

	struct FSFileHead {
		int type;
		int blockId;
		int createTime;
		std::string filename;
	};


}