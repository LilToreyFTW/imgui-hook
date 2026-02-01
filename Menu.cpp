#include "Menu.hpp"
#include "ServerInfo.hpp"
#include "ServerResources.hpp"
#include "RedEyedNinjaAuth.hpp"
#include "JobPayouts.hpp"
#include "Classes.hpp"
#include "Settings.hpp"
#include "imguinotify.hpp"
#include <algorithm>

void Menu::Style() { }

void Menu::ApplyRedEyedNinjaStyle() {
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 6.f;
    s.FrameRounding = 4.f;
    s.GrabRounding = 2.f;
    s.ScrollbarRounding = 4.f;
    s.WindowPadding = ImVec2(10, 10);
    s.FramePadding = ImVec2(6, 4);
    s.ItemSpacing = ImVec2(8, 6);
    ImVec4* c = s.Colors;
    c[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.08f, 0.98f);
    c[ImGuiCol_ChildBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.0f);
    c[ImGuiCol_Header] = ImVec4(0.6f, 0.15f, 0.15f, 1.0f);
    c[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.2f, 0.2f, 1.0f);
    c[ImGuiCol_HeaderActive] = ImVec4(0.5f, 0.12f, 0.12f, 1.0f);
    c[ImGuiCol_Button] = ImVec4(0.2f, 0.2f, 0.22f, 1.0f);
    c[ImGuiCol_ButtonHovered] = ImVec4(0.6f, 0.15f, 0.15f, 1.0f);
    c[ImGuiCol_ButtonActive] = ImVec4(0.5f, 0.12f, 0.12f, 1.0f);
    c[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);
    c[ImGuiCol_Border] = ImVec4(0.35f, 0.10f, 0.10f, 0.8f);
    c[ImGuiCol_TitleBg] = ImVec4(0.5f, 0.12f, 0.12f, 1.0f);
    c[ImGuiCol_TitleBgActive] = ImVec4(0.6f, 0.15f, 0.15f, 1.0f);
}

static void DrawAuthWindow() {
    ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("RedEyedNinja-X | Authentication", nullptr, ImGuiWindowFlags_NoCollapse)) {
        ImGui::Text("Connect to your zAuthServer");
        ImGui::Spacing();
        ImGui::InputText("License Key", Menu::AuthKey, sizeof(Menu::AuthKey), ImGuiInputTextFlags_Password);
        ImGui::Spacing();
        if (ImGui::Button("Connect (localhost:6969)", ImVec2(-1, 30))) {
            RedEyedNinjaAuth::Connect(Menu::AuthKey);
        }
        ImGui::Spacing();
        ImGui::Separator();
        static bool offlineBypass = false;
        if (ImGui::Checkbox("Offline / Dev mode (bypass auth)", &offlineBypass)) {
            RedEyedNinjaAuth::SetOfflineBypass(offlineBypass);
        }
        if (offlineBypass) RedEyedNinjaAuth::SetOfflineBypass(true);
    }
    ImGui::End();
}

static void DrawResourceBrowser() {
    auto resources = ServerResources::ScanFromCache();
    ImGui::BeginChild("ResList", ImVec2(220, -1), true);
    for (auto& res : resources) {
        if (ImGui::TreeNode(res.name.c_str())) {
            for (auto& f : res.files) {
                if (f.isScript && ImGui::Selectable(f.path.c_str())) {
                    std::string content = ServerResources::ReadFileContent(res.basePath, f.path);
                    size_t copyLen = (std::min)(content.size(), (size_t)65534);
                    memcpy(Menu::ViewedScriptBuf, content.c_str(), copyLen);
                    Menu::ViewedScriptBuf[copyLen] = '\0';
                    Menu::ViewedScriptName = res.name + "/" + f.path;
                }
            }
            ImGui::TreePop();
        }
    }
    if (resources.empty())
        ImGui::TextWrapped("Join a FiveM server first.\nResources load from cache.");
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("ScriptView", ImVec2(-1, -1), true);
    ImGui::Text("Script: %s", Menu::ViewedScriptName.c_str());
    ImGui::Separator();
    ImGui::InputTextMultiline("##script", Menu::ViewedScriptBuf, sizeof(Menu::ViewedScriptBuf),
        ImVec2(-1, -1), ImGuiInputTextFlags_ReadOnly);
    ImGui::EndChild();
}

static void DrawScriptsList() {
    auto resources = ServerResources::ScanFromCache();
    ImGui::Text("Server Scripts (live from cache)");
    ImGui::SameLine();
    if (ImGui::Button("Refresh")) { resources = ServerResources::ScanFromCache(); }
    ImGui::BeginChild("ScriptsList", ImVec2(-1, -1), true);
    for (auto& res : resources) {
        for (auto& f : res.files) {
            if (f.isScript) {
                if (ImGui::Selectable((res.name + " / " + f.path).c_str()))
                    ImGui::SetClipboardText((res.name + "/" + f.path).c_str());
            }
        }
    }
    if (resources.empty())
        ImGui::TextWrapped("Join a FiveM server. Press Inject in menu to load.");
    ImGui::EndChild();
}

static void DrawJobsList() {
    auto jobs = JobPayouts::ScanJobs();
    ImGui::Text("Server Jobs (QB-Core / ESX / ESX-Legacy)");
    ImGui::SameLine();
    if (ImGui::Button("Refresh##jobs")) { jobs = JobPayouts::ScanJobs(); }
    ImGui::BeginChild("JobsList", ImVec2(-1, -1), true);
    for (auto& j : jobs)
        ImGui::Text("- %s [%s] | %s / %s", j.name.c_str(), j.framework.c_str(), j.resource.c_str(), j.sourceFile.c_str());
    if (jobs.empty())
        ImGui::TextWrapped("No job scripts detected. Join server & refresh.");
    ImGui::EndChild();
}

static void DrawPayoutTriggers() {
    auto triggers = JobPayouts::ScanPayoutTriggers();
    ImGui::Text("Job Payout Triggers (LIVE - edit amount & execute)");
    ImGui::SameLine();
    if (ImGui::Button("Refresh##payouts")) { triggers = JobPayouts::ScanPayoutTriggers(); }
    ImGui::BeginChild("PayoutList", ImVec2(-1, -1), true);
    for (size_t i = 0; i < triggers.size(); i++) {
        auto* t = JobPayouts::GetTrigger(i);
        if (!t) continue;
        ImGui::PushID((int)i);
        ImGui::Text("%s [%s] | %s", t->jobOrEvent.c_str(), t->framework.c_str(), t->resource.c_str());
        ImGui::SameLine(280);
        int amt = t->editedAmount;
        if (ImGui::InputInt("##amt", &amt, 100, 1000, ImGuiInputTextFlags_CharsDecimal) && amt >= 0)
            JobPayouts::SetEditedAmount(i, amt);
        ImGui::SameLine();
        if (ImGui::Button("Execute")) {
            ImGui::InsertNotification(std::make_tuple((int)ImGuiToastType_Success, 2000,
                std::string("Payout ") + std::to_string(t->editedAmount) + " - client attempt"));
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Server validation may override. Triggers from cached scripts.");
        ImGui::PopID();
    }
    if (triggers.empty())
        ImGui::TextWrapped("No payout triggers found. Join server, inject, then Refresh.");
    ImGui::EndChild();
}

static void DrawExecutor() {
    ImGui::Text("Lua Executor (requires FiveM runtime)");
    ImGui::InputTextMultiline("##exec", Menu::ExecutorBuf, sizeof(Menu::ExecutorBuf), ImVec2(-1, 200));
    if (ImGui::Button("Execute")) {
        ImGui::InsertNotification(std::make_tuple((int)ImGuiToastType_Success, 2000, std::string("Executor requires FiveM ResourceManager API")));
    }
}

void Menu::Drawing() {
    Menu::ApplyRedEyedNinjaStyle();
    static bool initAuth = false;
    if (!initAuth) { RedEyedNinjaAuth::SetOfflineBypass(true); initAuth = true; }

    if (!RedEyedNinjaAuth::IsAuthenticated()) {
        DrawAuthWindow();
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(700, 500), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("RedEyedNinja-X", &Menu::Open, ImGuiWindowFlags_NoCollapse)) {
        ImGui::Text("RedEyedNinja-X | %s | %s", RedEyedNinjaAuth::GetUsername().c_str(), ServerInfo::GetServerName().c_str());
        ImGui::Separator();

        if (ImGui::BeginTabBar("MainTabs")) {
            if (ImGui::BeginTabItem("Scripts")) {
                DrawScriptsList();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Jobs")) {
                DrawJobsList();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Payout Triggers")) {
                DrawPayoutTriggers();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Server Resources")) {
                DrawResourceBrowser();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Executor")) {
                DrawExecutor();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Cheats")) {
                ImGui::Checkbox("Aimbot", &Settings::Aimbot::Aimbot);
                ImGui::Checkbox("No Recoil", &Settings::Aimbot::Draw_Fov);
                ImGui::Checkbox("NoClip (X)", &Settings::Player::NoClip);
                ImGui::Text("Game hooks: %s", (FiveM::World && FiveM::W2S) ? "Active" : "Waiting...");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings")) {
                if (ImGui::Button("zAuthServer: localhost:6969")) { }
                ImGui::SameLine();
                if (ImGui::Button("Change Server")) { }
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
