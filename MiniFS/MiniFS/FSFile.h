#include"FileOperator.h"

#define FS_FILETYPE_NONE 0
#define FS_FILETYPE_FOLDER 1
#define FS_FILETYPE_FILE 2

namespace Three {

	class FSFolder;

	class FSFile {
		friend FSFolder;
	public:
		FSFile();
		FSFile(int blockId);
		FSFile(FSFileHead&head) { fileHead = head; }
		virtual ~FSFile(){}

		virtual void Show(){}
		void ShowAttr();
		void SetFileName(std::string&filename) { fileHead.filename = filename; }
		FSFolder* GetParent() { return parent; }
		void SetParent(FSFolder*parent) { this->parent = parent; }

		int ComputeSize() { return 14 + fileHead.filename.length(); }
		bool IsFolder() { return fileHead.type == FS_FILETYPE_FOLDER; }

	protected:
		FSFileHead fileHead;
		FSFolder* parent;
	};


}