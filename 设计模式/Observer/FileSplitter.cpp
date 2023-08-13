/*************************************************************************
	> File Name: FileSplitter.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/8/13 11:39:02
 ************************************************************************/
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;


class IProgress{
public:
	virtual void DoProgress(float value)=0;
	virtual ~IProgress(){}
};

class FileSplitter{
	string m_filePath;
	int m_fileNumber;

	vector<IProgress*> m_iprogressList;//抽象通知机制，支持多个观察者
public:
	FileSplitter(const string& filePath,int fileNumber):m_filePath(filePath),m_fileNumber(fileNumber){}

	void split(){
		//do_somthing();
		for(int i=0;i<m_fileNumber;++i){
			float progressValue=m_fileNumber;
			progressValue=(i+1)/progressValue;
			onProgress(progressValue);
		}
	}
	//订阅
	void addIProgress(IProgress* iprogress){
		m_iprogressList.push_back(iprogress);
	}
	//取消订阅
	void removeIProgress(IProgress* iprogress){
		m_iprogressList.erase(find(m_iprogressList.begin(),m_iprogressList.end(),iprogress));
	}

protected:
	virtual void onProgress(float value){
		auto it=m_iprogressList.begin();

		while(it!=m_iprogressList.end()){
			(*it)->DoProgress(value);//更新进度
			++it;
		}
	}

};