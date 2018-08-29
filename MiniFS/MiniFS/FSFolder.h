#include "FSFile.h"
#include <unordered_map>
namespace Three {

	class FSFolder :public FSFile {
	public:
		FSFolder();
		FSFolder(int blockId);
		FSFolder(FSFileHead&head) { fileHead = head; }
		~FSFolder();

		void Show()override;

		void Load(FileOperator&);

		void ReadAsRoot(FileOperator&);
		void ReadAsRecycleBin(FileOperator&){}

		void GetAbsolutePath(std::string&out);

		void AddChild(FSFile*);
		void UpdateDir(FileOperator&);
		void Clear();
		bool IsLoaded() { return loaded; }
		FSFile*& GetChild(std::string&fileName);

		FSFolder* GetDestFolder(std::vector<std::string>&nameList, FileOperator&);

	protected:
		bool loaded;
		std::unordered_map<std::string, FSFile*>childs;

	};

}