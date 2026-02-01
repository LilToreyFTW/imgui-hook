#include "JobPayouts.hpp"
#include "ServerResources.hpp"
#include <cctype>
#include <cstring>
#include <set>
#include <algorithm>

namespace JobPayouts {

static std::vector<PayoutTrigger> g_triggers;
static std::vector<JobInfo> g_jobs;

// QB-Core default jobs (from qb-core/shared/jobs.lua)
static const char* QB_JOBS[] = {
    "unemployed", "bus", "judge", "lawyer", "reporter", "trucker", "tow", "garbage",
    "vineyard", "hotdog", "police", "ambulance", "realestate", "taxi", "cardealer",
    "mechanic", "mechanic2", "mechanic3", "beeker", "bennys", nullptr
};

// ESX / ESX-Legacy default jobs (esx_jobs, esx_ambulancejob, etc)
static const char* ESX_JOBS[] = {
    "slaughterer", "miner", "fisherman", "journalist", "fueler", "tailor",
    "police", "ambulance", "mechanic", "taxi", "unemployed", "realestate",
    "cardealer", "society_police", "society_ambulance", "society_mechanic",
    "society_taxi", "society_realestateagent", "society_cardealer", nullptr
};

static int ExtractNumber(const std::string& s, size_t start) {
    while (start < s.size() && !std::isdigit(s[start])) start++;
    if (start >= s.size()) return 0;
    int v = 0;
    while (start < s.size() && std::isdigit(s[start]))
        v = v * 10 + (s[start++] - '0');
    return v;
}


static void AddTrigger(std::vector<PayoutTrigger>& out, const std::string& label, int amt,
    const std::string& resource, const std::string& file, const std::string& framework,
    const std::string& pattern, std::set<std::string>& seen) {
    if (amt <= 0 || amt >= 10000000) return;
    std::string key = resource + "|" + file + "|" + pattern + "|" + std::to_string(amt);
    if (seen.count(key)) return;
    seen.insert(key);
    PayoutTrigger t;
    t.jobOrEvent = label + " @" + std::to_string(amt);
    t.resource = resource;
    t.sourceFile = file;
    t.framework = framework;
    t.defaultAmount = amt;
    t.editedAmount = amt;
    t.pattern = pattern;
    out.push_back(t);
}

static void ParseScriptForPayouts(const std::string& content, const std::string& resource,
    const std::string& file, std::vector<PayoutTrigger>& out, std::set<std::string>& seen) {
    std::string framework = "Unknown";
    if (resource.find("qb") != std::string::npos || resource.find("QBCore") != std::string::npos ||
        resource.find("qb-") != std::string::npos || content.find("QBCore") != std::string::npos ||
        content.find("qb-core") != std::string::npos)
        framework = "QB-Core";
    else if (resource.find("esx") != std::string::npos || resource.find("ESX") != std::string::npos ||
        content.find("ESX") != std::string::npos || content.find("xPlayer") != std::string::npos ||
        content.find("es_extended") != std::string::npos)
        framework = "ESX/ESX-Legacy";

    struct { const char* pat; const char* lbl; size_t skip; } patterns[] = {
        { "xPlayer.addMoney(", "xPlayer.addMoney", 18 },
        { "xPlayer.addAccountMoney(", "addAccountMoney", 24 },
        { "Player.Functions.AddMoney(", "AddMoney(QB)", 26 },
        { "AddMoney(", "AddMoney", 9 },
        { "addMoney(", "addMoney", 9 },
        { "GiveMoney(", "GiveMoney", 10 },
        { "payment =", "payment", 10 },
        { "payment", "payment", 7 },
        { "salary", "salary", 6 },
        { "grade_salary", "grade_salary", 13 },
        { "TriggerServerEvent", "TriggerServerEvent", 18 },
        { "TriggerEvent", "TriggerEvent", 13 },
        { "exports[", "exports", 8 },
        { "pay(", "pay", 4 },
        { "Pay(", "Pay", 4 },
        { "reward", "reward", 6 },
        { "society_", "society", 8 },
    };

    for (const auto& p : patterns) {
        size_t pos = 0;
        while ((pos = content.find(p.pat, pos)) != std::string::npos) {
            int amt = ExtractNumber(content, pos + p.skip);
            if (amt > 0) {
                AddTrigger(out, p.lbl, amt, resource, file, framework, p.pat, seen);
            }
            pos++;
        }
    }

    if (content.find("grades") != std::string::npos) {
        size_t pos = 0;
        while ((pos = content.find("payment", pos)) != std::string::npos) {
            size_t eq = content.find("=", pos);
            if (eq != std::string::npos && eq - pos < 20) {
                int amt = ExtractNumber(content, eq + 1);
                if (amt > 0)
                    AddTrigger(out, "grades.payment", amt, resource, file, framework, "grades", seen);
            }
            pos++;
        }
    }
}

static void ParseScriptForJobs(const std::string& content, const std::string& resource,
    const std::string& file, std::vector<JobInfo>& out, std::set<std::string>& seen) {
    std::string framework = "Unknown";
    if (resource.find("qb") != std::string::npos || content.find("QBCore") != std::string::npos ||
        content.find("QBShared") != std::string::npos)
        framework = "QB-Core";
    else if (resource.find("esx") != std::string::npos || content.find("ESX") != std::string::npos ||
        content.find("xPlayer") != std::string::npos)
        framework = "ESX/ESX-Legacy";

    for (const char** jobs = QB_JOBS; *jobs; jobs++) {
        if (content.find(*jobs) != std::string::npos) {
            std::string key = resource + "|" + file + "|" + *jobs;
            if (seen.count(key)) continue;
            seen.insert(key);
            JobInfo j;
            j.name = *jobs;
            j.framework = "QB-Core";
            j.resource = resource;
            j.sourceFile = file;
            out.push_back(j);
        }
    }
    for (const char** jobs = ESX_JOBS; *jobs; jobs++) {
        if (content.find(*jobs) != std::string::npos) {
            std::string key = resource + "|" + file + "|" + *jobs;
            if (seen.count(key)) continue;
            seen.insert(key);
            JobInfo j;
            j.name = *jobs;
            j.framework = "ESX/ESX-Legacy";
            j.resource = resource;
            j.sourceFile = file;
            out.push_back(j);
        }
    }

    if (content.find("esx_job") != std::string::npos || content.find("esx_jobs") != std::string::npos) {
        std::string key = resource + "|" + file + "|esx_jobs";
        if (!seen.count(key)) {
            seen.insert(key);
            JobInfo j;
            j.name = "esx_jobs";
            j.framework = "ESX/ESX-Legacy";
            j.resource = resource;
            j.sourceFile = file;
            out.push_back(j);
        }
    }
}

std::vector<JobInfo> ScanJobs() {
    g_jobs.clear();
    std::set<std::string> seen;
    auto resources = ServerResources::ScanFromCache();
    for (const auto& res : resources) {
        for (const auto& f : res.files) {
            if (f.isScript) {
                std::string content = ServerResources::ReadFileContent(res.basePath, f.path);
                if (!content.empty())
                    ParseScriptForJobs(content, res.name, f.path, g_jobs, seen);
            }
        }
    }
    return g_jobs;
}

std::vector<PayoutTrigger> ScanPayoutTriggers() {
    g_triggers.clear();
    std::set<std::string> seen;
    auto resources = ServerResources::ScanFromCache();
    for (const auto& res : resources) {
        for (const auto& f : res.files) {
            if (f.isScript) {
                std::string content = ServerResources::ReadFileContent(res.basePath, f.path);
                if (!content.empty())
                    ParseScriptForPayouts(content, res.name, f.path, g_triggers, seen);
            }
        }
    }
    return g_triggers;
}

void SetEditedAmount(size_t index, int amount) {
    if (index < g_triggers.size() && amount >= 0)
        g_triggers[index].editedAmount = amount;
}

PayoutTrigger* GetTrigger(size_t index) {
    return index < g_triggers.size() ? &g_triggers[index] : nullptr;
}

}
