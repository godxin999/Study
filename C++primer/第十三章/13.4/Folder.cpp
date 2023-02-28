#include "Folder.h"
using namespace std;

void Folder::addMsg(Message *m) {
	messages.insert(m);
}

void Folder::remMsg(Message* m) {
	messages.erase(m);
}

void Folder::createFolder(const Folder& f) {
	for (auto m : f.messages) {
		m->save(*this);
	}

}
void Folder::deleteFolder() {
	for (auto m : messages) {
		m->folders.erase(this);
	}
}

Folder::Folder(const Folder& f) :messages(set<Message*>()) {
	createFolder(f);
}


Folder& Folder::operator=(const Folder& f) {
	deleteFolder();
	createFolder(f);
	return *this;
}

Folder::~Folder() {
	deleteFolder();
}