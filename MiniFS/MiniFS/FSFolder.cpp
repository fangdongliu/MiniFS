#include "FSFolder.h"

using namespace Three;
using namespace std;

FSFolder::FSFolder() {

	fileHead.type = FS_FILETYPE_FOLDER;

}

FSFolder::FSFolder(int blockId) :FSFile(blockId) {
	fileHead.type = FS_FILETYPE_FOLDER;
}

FSFolder::~FSFolder() {

	for (auto i : childs) {
		delete i.second;
	}

}

void FSFolder::UpdateDir(FileOperator& op) {

	int cur = 0;
	int remainSize = 0;
	BlockHead blockHead;
	blockHead.nextBlockId = fileHead.blockId;

	for (auto& i : childs) {

		if (i.second == nullptr)
			continue;

		writeFileHead:

		remainSize -= i.second->ComputeSize();

		if (remainSize >= 0) {
			op.Write(i.second->fileHead.type);
			op.Write(i.second->fileHead.blockId);
			op.Write(i.second->fileHead.createTime);
			op.Write(i.second->fileHead.filename);
			blockHead.size++;
		}
		else {
			if (!blockHead.nextBlockId)
				blockHead.nextBlockId = op.RequestEmptyBlock();

			if (cur) {
				op.SeekBlock(cur);
				op.Write(blockHead);
			}

			cur = blockHead.nextBlockId;
			op.SeekBlock(cur);
			op.Read(blockHead);
			op.Flush();

			blockHead.size = 0;
			remainSize = op.GetBlockSize() - sizeof(BlockHead);
			goto writeFileHead;
		}
	}

	if (blockHead.size > 0) {

		if (blockHead.nextBlockId)
			op.ReleaseBlock(blockHead.nextBlockId);

		blockHead.nextBlockId = 0;
		op.SeekBlock(cur);
		op.Write(blockHead);
	}

	op.Flush();

}

void FSFolder::AddChild(FSFile*file) {

	file->parent = this;
	childs[file->fileHead.filename] = file;

}

FSFile*& FSFolder::GetChild(std::string&fileName) {

	auto& i = childs[fileName];

	return i;

}

void FSFolder::Show() {

	for (auto i : childs) {

		if (i.second->IsFolder()) {
			cout << "/" << i.first<<endl;
		}
		else {
			cout << i.first<<endl;
		}

	}

}

void FSFolder::Clear() {

	for (auto i : childs) {
		delete i.second;
	}
	childs.clear();

}

void FSFolder::Load(FileOperator&op) {

	op.SeekBlock(fileHead.blockId);

	BlockHead blockHead;

readFiles:

	op.Read(blockHead);

	FSFileHead head;

	while (blockHead.size--) {

		op.Read(head.type);
		op.Read(head.blockId);
		op.Read(head.createTime);
		op.Read(head.filename);

		if (head.type == FS_FILETYPE_FOLDER) {
			FSFolder *folder = new FSFolder(head);
			AddChild(folder);
		}
		else {
			FSFile *file = new FSFile(head);
			AddChild(file);
		}

	}

	if (blockHead.nextBlockId) {

		op.SeekBlock(blockHead.nextBlockId);
		goto readFiles;

	}

	op.Flush();

}

void FSFolder::ReadAsRoot(FileOperator&op) {

	op.SeekBlock(1);

	BlockHead blockHead;

readFiles:

	op.Read(blockHead);

	FSFileHead head;

	while (blockHead.size--) {

		op.Read(head.type);
		op.Read(head.blockId);
		op.Read(head.createTime);
		op.Read(head.filename);

		if (head.type == FS_FILETYPE_FOLDER) {
			FSFolder *folder = new FSFolder(head);
			AddChild(folder);
		}
		else {
			FSFile *file = new FSFile(head);
			AddChild(file);
		}

	}

	if (blockHead.nextBlockId) {

		op.SeekBlock(blockHead.nextBlockId);
		goto readFiles;

	}

	fileHead.blockId = 1;

	op.Flush();

}

FSFolder* FSFolder::GetDestFolder(std::vector<std::string>&nameList, FileOperator& op) {

	FSFolder *dest = this;

	for (auto& i : nameList) {

		if (i == "..") {
			if (dest->GetParent() == nullptr) {
				throw exception("wrong path!");
			}
			dest = dest->GetParent();
		}
		else if (i != ".") {

			auto& file = dest->childs[i];

			if (file == nullptr) {

				std::string out;
				dest->GetAbsolutePath(out);

				out += i;

				cout << out << " doesn't exist,create one?[Y/N]:";

				char in;
				cin >> in;

				if (in == 'Y' || in == 'y') {

					FSFolder * folder = new FSFolder;

					folder->fileHead.filename = i;
					folder->fileHead.blockId = op.RequestEmptyBlock();

					folder->loaded = true;

					dest->AddChild(folder);
					dest->UpdateDir(op);

					dest = folder;


				}
				else {
					return nullptr;
				}
			}
			else {
				if (file->IsFolder()) {
					dest = dynamic_cast<FSFolder*>(file);
					if (!dest->IsLoaded()) {
						dest->Load(op);
					}
				}
				else {
					throw exception("wrong path!");
				}
			}

		}





	}

	return dest;

}

void FSFolder::GetAbsolutePath(std::string&out) {

	out = fileHead.filename+'/' + out;

	if (parent)
		parent->GetAbsolutePath(out);

}