#pragma once

#include <string>
#include <vector>

struct JobInfo {
    std::string name;
    std::string framework;
    std::string resource;
    std::string sourceFile;
};

struct PayoutTrigger {
    std::string jobOrEvent;
    std::string resource;
    std::string sourceFile;
    std::string framework;
    int defaultAmount;
    int editedAmount;
    std::string pattern;
};

namespace JobPayouts {
    std::vector<JobInfo> ScanJobs();
    std::vector<PayoutTrigger> ScanPayoutTriggers();
    void SetEditedAmount(size_t index, int amount);
    PayoutTrigger* GetTrigger(size_t index);
}
