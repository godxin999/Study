#include "Message.h"

class Folder {
public:
	Folder() :messages(std::set<Message*>()) {}
	Folder(const Folder&);
	Folder& operator=(const Folder&);
	~Folder();
	void addMsg(Message*);//加入消息
	void remMsg(Message*);//移除消息
private:
	std::set<Message*> messages;//消息目录
	void createFolder(const Folder&);//根据参数创建新文件夹
	void deleteFolder();//删除文件夹
};