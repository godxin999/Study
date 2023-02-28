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

Message::Message(Message&& m) :contents(move(m.contents)) {
	move_Folders(&m);
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

Message& Message::operator=(Message&& rhs) {
	if (this != &rhs) {//检查自赋值情况
		remove_from_Folders();//销毁左侧运算对象的旧状态
		contents = move(rhs.contents);
		move_Folders(&rhs);//重置Folders指向本Msg
	}
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

void Message::move_Folders(Message* m) {
	folders = move(m->folders);//使用set的移动赋值运算符
	for (auto f : folders) {
		f->remMsg(m);//删除旧Msg
		f->addMsg(this);//将本条Msg加入Folder中
	}
	m->folders.clear();//清空，确保销毁m是无害的，方便移动构造函数调用
}
