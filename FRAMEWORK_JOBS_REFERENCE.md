# FiveM Framework Jobs Reference

Researched from GitHub and official docs. Used by JobPayouts module.

## QB-Core Jobs (qb-core/shared/jobs.lua)

| Job ID | Label |
|--------|-------|
| unemployed | Civilian |
| bus | Bus |
| judge | Honorary |
| lawyer | Law Firm |
| reporter | Reporter |
| trucker | Trucker |
| tow | Towing |
| garbage | Garbage |
| vineyard | Vineyard |
| hotdog | Hotdog |
| police | Law Enforcement |
| ambulance | EMS |
| realestate | Real Estate |
| taxi | Taxi |
| cardealer | Vehicle Dealer |
| mechanic, mechanic2, mechanic3 | LS Customs |
| beeker | Beeker's Garage |
| bennys | Benny's Original Motor Works |

**Payout patterns:** `Player.Functions.AddMoney(type, amount)`, `grades.payment`, `payment = N`

---

## ESX / ESX-Legacy Jobs

### esx_jobs addon
- slaughterer, miner, fisherman, journalist, fueler, tailor

### Core jobs
- police, ambulance, mechanic, taxi, unemployed, realestate, cardealer

### Society accounts
- society_police, society_ambulance, society_mechanic, society_taxi, society_realestateagent, society_cardealer

**Payout patterns:** `xPlayer.addMoney(amount, reason)`, `xPlayer.addAccountMoney(account, amount, reason)`, `grade_salary`

---

## Payout Triggers Detected

| Pattern | Framework |
|---------|-----------|
| xPlayer.addMoney | ESX |
| xPlayer.addAccountMoney | ESX |
| Player.Functions.AddMoney | QB-Core |
| AddMoney, addMoney, GiveMoney | Both |
| payment =, salary, grade_salary | Both |
| TriggerServerEvent, TriggerEvent | Both |
| exports[ / exports. | Both |
| pay(, Pay(, reward, society_ | Both |

---

*Source: qb-core-framework/qb-core, esx-framework docs, ESX xPlayer API*
