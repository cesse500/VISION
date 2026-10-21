#pragma once
#include <windows.h>
#include <d3d11.h>

extern HWND g_hwnd;
extern bool g_running;
extern bool g_menuOpen;
extern ID3D11Device* g_device;
extern ID3D11DeviceContext* g_context;
extern IDXGISwapChain* g_swapChain;
extern ID3D11RenderTargetView* g_rtv;

bool OverlayInit(HINSTANCE hInst);
void OverlayLoop();
void OverlayShutdown();
