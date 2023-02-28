#include "Folder.h"
using namespace std;

void Message::save(Folder &f) {
	folders.insert(&f);
	f.addMsg(this);//向文件夹中加入本消息
}

void Message::remove(Folder& f) {
	folders.erase(&f);
	f.remMsg(this);//向文件夹中移除本消息
}

void Message::add_to_Folders(const Message& m) {
	for (auto f : m.folders) {
		f->addMsg(this);
	}
}

Message::Message(const Message& m):contents(m.contents),folders(m.folders) {
	add_to_Folders(m);
}

void Message::remove_from_Folders() {
	for (auto f : folders) {
		f->remMsg(this);
	}
}

Message::~Message() {
	remove_from_Folders();
}

Message& Message::operator=(const Message& rhs) {
	remove_from_Folders();
	contents = rhs.contents;
	folders = rhs.folders;
	add_to_Folders(rhs);
	return *this;
}

void swap(Message& lhs, Message& rhs) {
	cout << "my swap" << endl;
	for (auto f : lhs.folders) {
		f->remMsg(&lhs);
	}
	for (auto f : rhs.folders) {
		f->remMsg(&rhs);
	}
	swap(lhs.folders, rhs.folders);
	swap(lhs.contents, rhs.contents);
	for (auto f : lhs.folders) {
		f->addMsg(&lhs);
	}
	for (auto f : rhs.folders) {
		f->addMsg(&rhs);
	}
}