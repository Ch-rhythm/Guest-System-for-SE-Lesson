#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <unordered_map>
#include <atomic>


using namespace std;


// 异常处理类
class InvalidPermissionException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid permission assigned.";
    }
};


class GuestNotFoundException : public exception {
public:
    const char* what() const noexcept override {
        return "Guest not found in the system.";
    }
};


class DataMismatchException : public exception {
public:
    const char* what() const noexcept override {
        return "Data mismatch between sources.";
    }
};


class InvalidInputException : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid input provided.";
    }
};



// 文件路径定义
const string GUESTS_DATA_FILE = "guests_data.txt";
const string EVENT_LOG_FILE = "event_log.txt";
const string ERROR_LOG_FILE = "error_log.txt";



// 证件类
class Certificate {
public:
    string certificateType;
    string certificateNumber;
    string issueDate;

    
    Certificate(const string& type, const string& number, const string& date)
        : certificateType(type), certificateNumber(number), issueDate(date) {}

    
    void displayInfo() const {
        cout << "Certificate Type: " << certificateType << endl;
        cout << "Certificate Number: " << certificateNumber << endl;
        cout << "Issue Date: " << issueDate << endl;
    }
};



// 交通工具类
class Transportation {
public:
    string vehicleID;
    string vehicleType;
    string driverName;

    
    Transportation(const string& id, const string& type, const string& driver)
        : vehicleID(id), vehicleType(type), driverName(driver) {}

    
    void displayInfo() const {
        cout << "Vehicle ID: " << vehicleID << endl;
        cout << "Vehicle Type: " << vehicleType << endl;
        cout << "Driver: " << driverName << endl;
    }
};



// 活动类
class Event {
public:
    string eventName;
    string eventDate;
    vector<string> participants;
    map<string, vector<string>> schedule; // 嘉宾 -> 活动时间段

    Event(const string& name, const string& date) : eventName(name), eventDate(date) {}

    void addParticipant(const string& guestName) {
        participants.push_back(guestName);
    }

    void removeParticipant(const string& guestName) {
        participants.erase(remove(participants.begin(), participants.end(), guestName), participants.end());
    }

    void updateEventDate(const string& newDate) {
        eventDate = newDate;
    }

    void scheduleEventForGuest(const string& guestName, const string& timeSlot) {
        schedule[guestName].push_back(timeSlot);
    }

    void displayInfo() const {
        cout << "Event Name: " << eventName << endl;
        cout << "Event Date: " << eventDate << endl;
        cout << "Participants: ";
        for (const auto& participant : participants) {
            cout << participant << ", ";
        }
        cout << endl;
    }

    void displaySchedule() const {
        for (const auto& entry : schedule) {
            cout << "Guest: " << entry.first << " has the following schedule: ";
            for (const auto& time : entry.second) {
                cout << time << ", ";
            }
            cout << endl;
        }
    }
};



// 嘉宾类
class Guest {
public:
    string name;
    int age;
    string gender;
    string contact;
    Event* event;
    Certificate* certificate;
    Transportation* transportation;
    string receptionPlan;  // 接待方案
    string accessLevel;    // 权限级别
    string role;           // 嘉宾角色：VIP, 工作人员等

    Guest(const string& guestName, int guestAge, const string& guestGender, const string& guestContact, Event* eventObj, const string& access = "Normal", const string& guestRole = "Guest")
        : name(guestName), age(guestAge), gender(guestGender), contact(guestContact), event(eventObj), certificate(nullptr), transportation(nullptr), accessLevel(access), role(guestRole) {}

    
    void assignCertificate(Certificate* cert) {
        certificate = cert;
    }

    
    void assignTransportation(Transportation* trans) {
        transportation = trans;
    }

    
    void updateContact(const string& newContact) {
        contact = newContact;
    }

    
    void updateReceptionPlan(const string& newPlan) {
        receptionPlan = newPlan;
    }

    
    void updateAccessLevel(const string& newAccessLevel) {
        accessLevel = newAccessLevel;
    }

    
    void updateRole(const string& newRole) {
        role = newRole;
    }

    
    void displayInfo() const {
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;
        cout << "Gender: " << gender << endl;
        cout << "Contact: " << contact << endl;
        cout << "Reception Plan: " << receptionPlan << endl;
        cout << "Access Level: " << accessLevel << endl;
        cout << "Role: " << role << endl;
        event->displayInfo();
        if (certificate) {
            certificate->displayInfo();
        }
        else {
            cout << "No Certificate Assigned." << endl;
        }
        if (transportation) {
            transportation->displayInfo();
        }
        else {
            cout << "No Transportation Assigned." << endl;
        }
    }

    
    string getName() const {
        return name;
    }

    
    string getAccessLevel() const {
        return accessLevel;
    }

    
    string getRole() const {
        return role;
    }
};



// 嘉宾组类
class GuestGroup {
public:
    string groupName;
    vector<Guest*> guests;

    GuestGroup(const string& name) : groupName(name) {}

    void addGuest(Guest* guest) {
        guests.push_back(guest);
    }

    void removeGuest(const string& guestName) {
        auto it = find_if(guests.begin(), guests.end(), [&guestName](Guest* g) { return g->getName() == guestName; });
        if (it != guests.end()) {
            guests.erase(it);
        }
        else {
            throw GuestNotFoundException();
        }
    }

    void displayAllGuests() const {
        cout << "Group: " << groupName << endl;
        for (const auto& guest : guests) {
            guest->displayInfo();
            cout << "----------------------------" << endl;
        }
    }

    void filterGuestsByAccess(const string& accessLevel) const {
        cout << "Guests with access level " << accessLevel << ":" << endl;
        for (const auto& guest : guests) {
            if (guest->getAccessLevel() == accessLevel) {
                guest->displayInfo();
            }
        }
    }

    void filterGuestsByRole(const string& role) const {
        cout << "Guests with role " << role << ":" << endl;
        for (const auto& guest : guests) {
            if (guest->getRole() == role) {
                guest->displayInfo();
            }
        }
    }
};



// 数据同步与热更新管理
class DataSynchronizer {
public:
    static void synchronizeData(vector<Guest*>& sourceData, vector<Guest*>& targetData) {
        for (auto& guest : sourceData) {
            auto it = find_if(targetData.begin(), targetData.end(), [&guest](Guest* g) { return g->getName() == guest->getName(); });
            if (it == targetData.end()) {
                targetData.push_back(guest);
            }
        }
        cout << "Data synchronization complete." << endl;
    }

    static void hotUpdateConfig(const string& configFile) {
        ifstream file(configFile);
        if (file.is_open()) {
            cout << "Updating system configuration..." << endl;
            string line;
            while (getline(file, line)) {
                cout << "Config line: " << line << endl;
            }
            file.close();
        }
        else {
            throw runtime_error("Unable to open configuration file.");
        }
    }
};



// 权限管理类
class AccessControl {
public:
    map<string, string> accessControlMap;
    mutex accessMutex;

    void grantAccess(const string& guestName, const string& accessLevel = "Normal") {
        lock_guard<mutex> lock(accessMutex);
        accessControlMap[guestName] = accessLevel;
    }

    void revokeAccess(const string& guestName) {
        lock_guard<mutex> lock(accessMutex);
        accessControlMap.erase(guestName);
    }

    bool hasAccess(const string& guestName) const {
        return accessControlMap.find(guestName) != accessControlMap.end();
    }

    void displayAccessInfo() const {
        cout << "Access Control Information:" << endl;
        for (const auto& pair : accessControlMap) {
            cout << "Guest: " << pair.first << ", Access Level: " << pair.second << endl;
        }
    }
};



// 系统日志管理类
class SystemLog {
public:
    static void logAction(const string& action) {
        ofstream logFile(EVENT_LOG_FILE, ios::app);
        if (logFile.is_open()) {
            char dt[26];  // ctime_s requires a fixed-size buffer
            time_t now = time(0);
            ctime_s(dt, sizeof(dt), &now);
            logFile << dt << " ACTION: " << action << endl;
            logFile.close();
        }
    }

    static void logError(const string& error) {
        ofstream logFile(ERROR_LOG_FILE, ios::app);
        if (logFile.is_open()) {
            char dt[26];  // ctime_s requires a fixed-size buffer
            time_t now = time(0);
            ctime_s(dt, sizeof(dt), &now);
            logFile << dt << " ERROR: " << error << endl;
            logFile.close();
        }
    }
};



// 数据存储类
class DataStorage {
public:
    static void saveGuestData(const Guest& guest) {
        ofstream outFile(GUESTS_DATA_FILE, ios::app);
        if (outFile.is_open()) {
            outFile << guest.getName() << endl;
            outFile.close();
        }
    }

    static void loadGuestData() {
        ifstream inFile(GUESTS_DATA_FILE);
        string guestName;
        if (inFile.is_open()) {
            while (getline(inFile, guestName)) {
                cout << "Loaded guest: " << guestName << endl;
            }
            inFile.close();
        }
    }
};



// 用户输入处理类
class UserInputHandler {
public:
    static string getInput(const string& prompt) {
        string input;
        cout << prompt;
        getline(cin, input);
        return input;
    }

    static int getIntInput(const string& prompt) {
        string input;
        int value;
        while (true) {
            cout << prompt;
            getline(cin, input);
            stringstream(input) >> value;
            if (value != 0 || input == "0") {
                return value;
            }
            cout << "Invalid input. Please enter a valid integer." << endl;
        }
    }
};



// 系统主函数
int main() {

    // 创建多个事件
    Event event1("Opening Ceremony", "2024-01-01");
    Event event2("Closing Ceremony", "2024-01-02");


    // 创建多个嘉宾
    Guest guest1("John Doe", 45, "Male", "john.doe@email.com", &event1, "VIP");
    Guest guest2("Jane Smith", 30, "Female", "jane.smith@email.com", &event2);


    // 创建多个证件
    Certificate cert1("VIP", "123456", "2023-12-15");
    Certificate cert2("Regular", "654321", "2023-12-16");


    // 创建多个交通工具
    Transportation trans1("A123", "Luxury", "James Brown");
    Transportation trans2("B234", "Standard", "Anna White");


    // 分配证件和交通工具
    guest1.assignCertificate(&cert1);
    guest1.assignTransportation(&trans1);
    guest2.assignCertificate(&cert2);
    guest2.assignTransportation(&trans2);


    // 更新接待方案
    guest1.updateReceptionPlan("VIP reception at the main hall");
    guest2.updateReceptionPlan("General reception at the entrance");


    // 创建嘉宾组
    GuestGroup vipGroup("VIP Guests");
    vipGroup.addGuest(&guest1);
    vipGroup.addGuest(&guest2);


    // 数据同步
    vector<Guest*> sourceData = { &guest1 };
    vector<Guest*> targetData = { &guest2 };

    DataSynchronizer::synchronizeData(sourceData, targetData);


    // 权限管理
    AccessControl accessControl;
    accessControl.grantAccess("John Doe", "VIP");
    accessControl.grantAccess("Jane Smith", "Normal");


    // 检查入场权限
    if (accessControl.hasAccess("John Doe")) {
        cout << "John Doe is allowed to enter the venue." << endl;
    }


    // 日志记录
    SystemLog systemLog;
    systemLog.logAction("John Doe's certificate assigned.");
    systemLog.logAction("Jane Smith's reception plan updated.");
    systemLog.logError("Failed to load guest data.");


    // 数据持久化
    DataStorage::saveGuestData(guest1);
    DataStorage::saveGuestData(guest2);
    DataStorage::loadGuestData();


    // 用户交互
    string guestName = UserInputHandler::getInput("Enter guest name to search: ");
    cout << "Searching for guest: " << guestName << endl;

    return 0;
}
