/*************************************************************************
	> File Name: MainForm.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/13 11:38:20
 ************************************************************************/

#include "FileSplitter.cpp"
using namespace std;
class ConsoleNotifier:public IProgress{
public:
	virtual void DoProgress(float value){
		cout<<".";
	}
};
class TextBox{};
class ProgressBar{};
class Form{};
class MainForm:public Form,public IProgress{
	TextBox* txtFilePath;
	TextBox* txtFileNumber;
	ProgressBar* progressBar;
public:
	void button1_click(){
		string filePath = txtFilePath->getText();
		int number = atoi(txtFileNumber->getText().c_str());

		ConsoleNotifier cn;

		FileSplitter splitter(filePath, number);

		splitter.addIProgress(this); //订阅通知
		splitter.addIProgress(&cn); //订阅通知

		splitter.split();

		splitter.removeIProgress(this);

	}
	virtual void DoProgress(float value){
		progressBar->setValue(value);
	}

};
