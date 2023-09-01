#include "pch.h"
#include "Game.h"
#include "Application.h"
#include "Window.h"

Game::Game(const std::wstring& name, int width, int height, bool vSync):
	m_Name(name),
	m_Width(width),
	m_Height(height),
	m_VSync(vSync){

}

Game::~Game() {
	assert(!m_pWindow && "Use Game::Destroy before destruction.");
}

bool Game::Initialize() {
	if (!DirectX::XMVerifyCPUSupport()) {
		MessageBoxW(nullptr, L"Failed to verify DirectX Math library support.", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	m_pWindow = Application::Get().CreateRenderWindow(m_Name, m_Width, m_Height, m_VSync);
	m_pWindow->RegisterCallbacks(shared_from_this());
	m_pWindow->Show();
}

void Game::Destroy() {
	Application::Get().DestroyWindow(m_pWindow);
	m_pWindow.reset();
}

void Game::OnWindowDestroy(){
	UnloadContent();
}
