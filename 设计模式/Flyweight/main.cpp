/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/3 9:59:31
 ************************************************************************/


#include "Editor.hpp"
#include <iostream>


std::unordered_set<std::shared_ptr<Font>> FontFactory::fontSet;

int main(){
	Editor editor;
	editor.insert(editor.end(),'H',"Arial",12,false,false);
	editor.insert(editor.end(),'e',"Arial",12,false,false);
	editor.insert(editor.end(),'l',"Arial",12,false,false);
	editor.insert(editor.end(),'l',"Arial",12,false,false);
	editor.insert(editor.end(),'o',"Arial",12,false,false);
	editor.insert(editor.end(),'!',"Arial",12,false,false);
	std::cout<<editor.getText()<<std::endl;
	editor.erase(5);
	std::cout<<editor.getText()<<std::endl;
}