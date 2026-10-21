#include "menu.h"
#include "overlay.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

static int tab = 0;
static bool aimbot = false, silent = false, esp = false;

void MenuInit(HWND hwnd) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_device, g_context);

    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 8; s.FrameRounding = 6;
    s.Colors[ImGuiCol_WindowBg] = ImVec4(0.04f,0.04f,0.06f,1.0f);
    s.Colors[ImGuiCol_Header]   = ImVec4(0.0f,0.9f,1.0f,0.4f);
}

void MenuRender() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (g_menuOpen) {
        ImGui::SetNextWindowSize(ImVec2(760,480), ImGuiCond_FirstUseEver);
        ImGui::Begin("VISION", nullptr, ImGuiWindowFlags_NoCollapse);

        ImGui::BeginChild("rail", ImVec2(140,0), true);
        const char* tabs[] = {"Aimbot","Silent","Visuals","Misc"};
        for (int i=0;i<4;i++)
            if (ImGui::Selectable(tabs[i], tab==i)) tab = i;
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("panel", ImVec2(0,0), true);

        if (tab==0) {
            ImGui::Checkbox("Aimbot", &aimbot);
        } else if (tab==1) {
            ImGui::Checkbox("Silent Aim", &silent);
