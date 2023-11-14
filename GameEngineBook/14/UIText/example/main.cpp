/*************************************************************************
	> File Name: main.cpp
	> Author: godxin999
	> Mail: A996570107@163.com
	> Created Time: 2023/9/25 22:30:06
 ************************************************************************/

#include "utils/Application.h"
#include "component/GameObject.h"

int main() {
	Application::set_assets_path("../../../assets/");
	//system("pause");
	Application::Init();

	GameObject* go = new GameObject("LoginSceneGo");
	//go->AddComponent("Transform");
	go->AddComponent("LoginScene");

	Application::Run();
	return 0;
}