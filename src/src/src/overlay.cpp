#include "overlay.h"
#include "menu.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

HWND g_hwnd = nullptr;
bool g_running = true;
bool g_menuOpen = false;
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
IDXGISwapChain* g_swapChain = nullptr;
ID3D11RenderTargetView* g_rtv = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND,UINT,WPARAM,LPARAM);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w, l)) return true;
    if (msg == WM_DESTROY) { PostQuitMessage(0); return 0; }
    if (msg == WM_KEYDOWN && w == VK_RSHIFT) g_menuOpen = !g_menuOpen;
    return DefWindowProc(hwnd, msg, w, l);
}

bool CreateDeviceD3D(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC sd{};
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    D3D_FEATURE_LEVEL lv;
    const D3D_FEATURE_LEVEL lvs[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        lvs, 2, D3D11_SDK_VERSION, &sd, &g_swapChain, &g_device, &lv, &g_context))) return false;

    ID3D11Texture2D* bb = nullptr;
    g_swapChain->GetBuffer(0, IID_PPV_ARGS(&bb));
    g_device->CreateRenderTargetView(bb, nullptr, &g_rtv);
    if (bb) bb->Release();
    return true;
}

bool OverlayInit(HINSTANCE hInst) {
    WNDCLASSEX wc{ sizeof(wc) };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"VISION_OVERLAY";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassEx(&wc);

    int w = GetSystemMetrics(SM_CXSCREEN), h = GetSystemMetrics(SM_CYSCREEN);
    g_hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        L"VISION_OVERLAY", L"VISION", WS_POPUP, 0, 0, w, h,
        nullptr, nullptr, hInst, nullptr);
    if (!g_hwnd) return false;

    SetWindowDisplayAffinity(g_hwnd, WDA_EXCLUDEFROMCAPTURE);

    if (!CreateDeviceD3D(g_hwnd)) return false;

    MenuInit(g_hwnd);
    ShowWindow(g_hwnd, SW_SHOW);
    return true;
}

void OverlayLoop() {
    MSG msg;
    while (g_running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg); DispatchMessage(&msg);
            if (msg.message == WM_QUIT) g_running = false;
        }
        if (!g_running) break;

        LONG_PTR ex = GetWindowLongPtr(g_hwnd, GWL_EXSTYLE);
        if (g_menuOpen) ex &= ~WS_EX_TRANSPARENT; else ex |= WS_EX_TRANSPARENT;
        SetWindowLongPtr(g_hwnd, GWL_EXSTYLE, ex);

        const float c[4] = {0,0,0,0};
        g_context->OMSetRenderTargets(1, &g_rtv, nullptr);
        g_context->ClearRenderTargetView(g_rtv, c);

        MenuRender();
        g_swapChain->Present(1, 0);
    }
}

void OverlayShutdown() {
    MenuShutdown();
    if (g_rtv) g_rtv->Release();
    if (g_swapChain) g_swapChain->Release();
    if (g_context) g_context->Release();
    if (g_device) g_device->Release();
    if (g_hwnd) DestroyWindow(g_hwnd);
}
