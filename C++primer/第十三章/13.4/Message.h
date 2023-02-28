#include <iostream>
#include <algorithm>
#include <cstring>
#include <set>
class Folder;
class Message {
	friend class Folder;
	friend void swap(Message&, Message&);
public:
	explicit Message(const std::string& s = "") :contents(s) {}
	Message(const Message&);
	Message& operator=(const Message&);
	~Message();
	void save(Folder&);//在给定的文件夹中添加本Msg
	void remove(Folder&);//在给定的文件夹中删除本Msg
private:
	std::string contents;//Msg
	std::set<Folder*> folders;//包含这条Msg的文件夹
	void add_to_Folders(const Message&);//将这条Msg添加到参数的文件夹中
	void remove_from_Folders();//从文件夹中删除这条Msg

};