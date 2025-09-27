// Hospital Management System

#include <iostream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>
using namespace std;

// --------------------------
// File constants
// --------------------------
static const string PATIENTS_FILE = "patients.txt";
static const string DOCTORS_FILE = "doctors.txt";
static const string STAFF_FILE = "staff.txt";
static const string APPOINTMENTS_FILE = "appointments.txt";
static const string BILLS_FILE = "bills.txt";
static const string MEDICINES_FILE = "medicines.txt";
static const string USERS_FILE = "users.txt"; // username,role,password,linkedId



void setColor(int color) {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h, color);
}



// --------------------------
// Utility helpers
// --------------------------
// Prints a simple table row with left alignment
void printTableRow(const std::vector<std::string> &cols, int width = 20) {
    for (auto &c : cols) {
        cout << left << setw(width) << c;
    }
    cout << "\n";
}
//Animation typing style
void printSlow(string text, int speed=50, bool newline=true) {
    for (char c : text) {
        cout << c << flush;
        Sleep(speed);
    }
    if (newline) cout << endl;
}


inline string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

inline vector<string> splitCSV(const string &line) {
    vector<string> out;
    string cur;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '\"') {
            inQuotes = !inQuotes;
            continue;
        }
        if (c == ',' && !inQuotes) {
            out.push_back(trim(cur));
            cur.clear();
        } else cur.push_back(c);
    }
    out.push_back(trim(cur));
    return out;
}

inline string joinCSV(const vector<string> &parts) {
    string out;
    for (size_t i = 0; i < parts.size(); ++i) {
        string p = parts[i];
        bool needQuotes = p.find(',') != string::npos || p.find(' ') != string::npos;
        if (needQuotes) {
            out.push_back('\"');
            for (char c : p) {
                if (c == '\"') out += "\"\"";
                else out.push_back(c);
            }
            out.push_back('\"');
        } else out += p;
        if (i + 1 < parts.size()) out.push_back(',');
    }
    return out;
}


    static string nowString() {
    time_t t = time(0);   // get current time
    tm local_tm;
#ifdef _WIN32
    // If localtime_s is not available, fallback to localtime
    tm* tm_ptr = localtime(&t);
    if (tm_ptr) local_tm = *tm_ptr;
#else
    localtime_r(&t, &local_tm); // Thread-safe POSIX version
#endif
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &local_tm);
    return string(buf);



}

inline bool datetimeConflict(const string &a, const string &b) {
    if (a.empty() || b.empty()) return false;
    string A = a.substr(0, min<size_t>(a.size(), 16));
    string B = b.substr(0, min<size_t>(b.size(), 16));
    return A == B;
}

int toIntSafe(const string &s, int defaultVal = 0) {
    try { return stoi(s); } catch (...) { return defaultVal; }
}

double toDoubleSafe(const string &s, double defaultVal = 0.0) {
    try { return stod(s); } catch (...) { return defaultVal; }
}

// --------------------------
// Robust input helpers
// --------------------------
string readLineSafe() {
    string s;
    if (!getline(cin, s)) {
        // if EOF or error, clear and return empty string
        cin.clear();
        return string();
    }
    return s;
}

string promptString(const string &promptText, bool allowEmpty = false) {
    while (true) {
        cout << promptText << flush;
        string line = readLineSafe();
        if (!allowEmpty && trim(line).empty()) {
            cout << "Input cannot be empty. Please try again.\n";
            continue;
        }
        return trim(line);
    }
}

int promptInt(const string &promptText, int defaultVal = -1) {
    while (true) {
        cout << promptText << flush;
        string line = readLineSafe();
        if (trim(line).empty()) {
            if (defaultVal != -1) return defaultVal;
            cout << "Please enter a number.\n";
            continue;
        }
        if (line == "q" || line == "Q") return defaultVal;
        stringstream ss(line);
        int val;
        if (ss >> val) {
            char c;
            if (!(ss >> c)) return val;
        }
        cout << "Invalid integer. Try again.\n";
    }
}

double promptDouble(const string &promptText, double defaultVal = -1.0) {
    while (true) {
        cout << promptText << flush;
        string line = readLineSafe();
        if (trim(line).empty()) {
            if (defaultVal >= 0.0) return defaultVal;
            cout << "Please enter a number.\n";
            continue;
        }
        stringstream ss(line);
        double val;
        if (ss >> val) {
            char c;
            if (!(ss >> c)) return val;
        }
        cout << "Invalid number. Try again.\n";
    }
}

void pauseConsole() {
    cout << "Press Enter to continue..." << flush;
    readLineSafe();
}

// --------------------------
// Abstract service
// --------------------------
class HospitalService {
public:
    virtual ~HospitalService() = default;
    virtual void performService() = 0;
    virtual string name() const = 0;
};

// --------------------------
// Person classes
// --------------------------
class Person {
protected:
    int id;
    string name;
    int age;
    string gender;
    string contact;
public:
    Person() : id(0), age(0) {}
    Person(int id_, const string &name_, int age_, const string &gender_, const string &contact_)
        : id(id_), name(name_), age(age_), gender(gender_), contact(contact_) {}
    virtual ~Person() = default;
    int getId() const { return id; }
    void setId(int v) { id = v; }
    string getName() const { return name; }
    void setName(const string &v) { name = v; }
    int getAge() const { return age; }
    void setAge(int v) { age = v; }
    string getGender() const { return gender; }
    void setGender(const string &v) { gender = v; }
    string getContact() const { return contact; }
    void setContact(const string &v) { contact = v; }
    virtual string type() const = 0;
    virtual void displayInfo() const {
        cout << "ID: " << id << " | Name: " << name << " | Age: " << age << " | Gender: " << gender << " | Contact: " << contact << "\n";
    }
    virtual vector<string> toCSVRow() const = 0;
};

class Patient : public Person {
private:
    string medicalHistory;
    bool insured;
    string insuranceProvider;
    string nationalId;
public:
    Patient() : Person(), insured(false) {}
    Patient(int id_, const string &name_, int age_, const string &gender_, const string &contact_, bool insured_ = false, const string &prov = "", const string &nid = "")
        : Person(id_, name_, age_, gender_, contact_), medicalHistory(""), insured(insured_), insuranceProvider(prov), nationalId(nid) {}

    void addHistory(const string &entry) {
        if (!medicalHistory.empty()) medicalHistory += "\n";
        medicalHistory += entry;
    }
    string getHistory() const { return medicalHistory; }
    void setHistory(const string &h) { medicalHistory = h; }
    bool isInsured() const { return insured; }
    void setInsurance(bool v) { insured = v; }              // <-- added/ensured
    string getInsuranceProvider() const { return insuranceProvider; }
    void setInsuranceProvider(const string &p) { insuranceProvider = p; }
    string getNationalId() const { return nationalId; }
    void setNationalId(const string &v) { nationalId = v; }

    string type() const override { return "Patient"; }

    void displayInfo() const override {
        cout << "[Patient] ";
        Person::displayInfo();
        cout << "  Insured: " << (insured ? "Yes" : "No");
        if (insured) cout << " | Provider: " << insuranceProvider;
        if (!nationalId.empty()) cout << " | National ID: " << nationalId;
        cout << "\n";
        if (!medicalHistory.empty()) {
            cout << "  Medical History:\n";
            istringstream iss(medicalHistory);
            string line;
            while (getline(iss, line)) cout << "   - " << line << "\n";
        }
    }

    vector<string> toCSVRow() const override {
        return {to_string(id), name, to_string(age), gender, contact, (insured ? "1" : "0"), insuranceProvider, nationalId, medicalHistory};
    }

    static Patient fromCSV(const vector<string> &r) {
        Patient p;
        if (r.size() >= 1) p.id = toIntSafe(r[0], 0);
        if (r.size() >= 2) p.name = r[1];
        if (r.size() >= 3) p.age = toIntSafe(r[2], 0);
        if (r.size() >= 4) p.gender = r[3];
        if (r.size() >= 5) p.contact = r[4];
        if (r.size() >= 6) p.insured = (r[5] == "1");
        if (r.size() >= 7) p.insuranceProvider = r[6];
        if (r.size() >= 8) p.nationalId = r[7];
        if (r.size() >= 9) p.medicalHistory = r[8];
        return p;
    }
};

class Doctor : public Person {
private:
    string specialization;
    vector<string> bookedSlots;
    double consultationFee;
public:
    Doctor() : Person(), consultationFee(0.0) {}
    Doctor(int id_, const string &name_, int age_, const string &gender_, const string &contact_, const string &spec_, double fee = 0.0)
        : Person(id_, name_, age_, gender_, contact_), specialization(spec_), consultationFee(fee) {}
    string getSpecialization() const { return specialization; }
    void setSpecialization(const string &v) { specialization = v; }
    const vector<string> &getBookedSlots() const { return bookedSlots; }
    void addBookedSlot(const string &s) { bookedSlots.push_back(s); }
    void setBookedSlots(const vector<string> &v) { bookedSlots = v; }
    double getFee() const { return consultationFee; }
    void setFee(double v) { consultationFee = v; }

    string type() const override { return "Doctor"; }

    void displayInfo() const override {
        cout << "[Doctor] ";
        Person::displayInfo();
        cout << "  Specialization: " << specialization << " | Fee: " << consultationFee << "\n";
        if (!bookedSlots.empty()) {
            cout << "  Booked slots: ";
            for (size_t i = 0; i < bookedSlots.size(); ++i) {
                cout << bookedSlots[i];
                if (i + 1 < bookedSlots.size()) cout << ", ";
            }
            cout << "\n";
        }
    }

    vector<string> toCSVRow() const override {
        string slots;
        for (size_t i = 0; i < bookedSlots.size(); ++i) {
            slots += bookedSlots[i];
            if (i + 1 < bookedSlots.size()) slots += ';';
        }
        return {to_string(id), name, to_string(age), gender, contact, specialization, to_string(consultationFee), slots};
    }

    static Doctor fromCSV(const vector<string> &r) {
        Doctor d;
        if (r.size() >= 1) d.id = toIntSafe(r[0], 0);
        if (r.size() >= 2) d.name = r[1];
        if (r.size() >= 3) d.age = toIntSafe(r[2], 0);
        if (r.size() >= 4) d.gender = r[3];
        if (r.size() >= 5) d.contact = r[4];
        if (r.size() >= 6) d.specialization = r[5];
        if (r.size() >= 7) {
            // safe parse
            d.consultationFee = toDoubleSafe(r[6], 0.0);
        }
        if (r.size() >= 8) {
            string s = r[7];
            string tmp;
            for (char c : s) {
                if (c == ';') { if (!tmp.empty()) { d.bookedSlots.push_back(tmp); tmp.clear(); } }
                else tmp.push_back(c);
            }
            if (!tmp.empty()) d.bookedSlots.push_back(tmp);
        }
        return d;
    }
};

class Staff : public Person {
private:
    
    string username;
public:
	string role;
    Staff() {}
    Staff(int id_, const string &name_, int age_, const string &gender_, const string &contact_, const string &role_, const string &uname = "")
        : Person(id_, name_, age_, gender_, contact_), role(role_), username(uname) {}
    string getRole() const { return role; }
    void setRole(const string &v) { role = v; }
    string getUsername() const { return username; }
    void setUsername(const string &v) { username = v; }

    string type() const override { return "Staff"; }

    void displayInfo() const override {
        cout << "[Staff] ";
        Person::displayInfo();
        cout << "  Role: " << role;
        if (!username.empty()) cout << " | Username: " << username;
        cout << "\n";
    }

    vector<string> toCSVRow() const override {
        return {to_string(id), name, to_string(age), gender, contact, role, username};
    }

    static Staff fromCSV(const vector<string> &r) {
        Staff s;
        if (r.size() >= 1) s.id = toIntSafe(r[0], 0);
        if (r.size() >= 2) s.name = r[1];
        if (r.size() >= 3) s.age = toIntSafe(r[2], 0);
        if (r.size() >= 4) s.gender = r[3];
        if (r.size() >= 5) s.contact = r[4];
        if (r.size() >= 6) s.role = r[5];
        if (r.size() >= 7) s.username = r[6];
        return s;
    }
};

// --------------------------
// Pharmacy service
// --------------------------
class PharmacyService : public HospitalService {
private:
    map<string,int> stock;
    map<string,string> expiry;
public:
    PharmacyService() {}
    void loadFromFile(const string &fname) {
        stock.clear(); expiry.clear();
        ifstream in(fname);
        if (!in.is_open()) return;
        string line;
        while (getline(in, line)) {
            if (trim(line).empty()) continue;
            auto row = splitCSV(line);
            if (row.size() >= 1) {
                string name = row[0];
                int qty = (row.size() >= 2) ? toIntSafe(row[1], 0) : 0;
                string exp = (row.size() >= 3) ? row[2] : "";
                stock[name] = qty; expiry[name] = exp;
            }
        }
    }
    void saveToFile(const string &fname) {
        ofstream out(fname);
        for (auto &kv : stock) {
            string exp = "";
            auto it = expiry.find(kv.first);
            if (it != expiry.end()) exp = it->second;
            out << joinCSV({kv.first, to_string(kv.second), exp}) << "\n";
        }
    }
    void addMedicine(const string &name, int qty, const string &exp) {
        stock[name] += qty; expiry[name] = exp;
    }
    bool issueMedicine(const string &name, int qty) {
        auto it = stock.find(name);
        if (it == stock.end() || it->second < qty) return false;
        it->second -= qty; return true;
    }
    void listMedicines() const {
        cout << "--- Pharmacy Stock ---\n";
        for (auto &kv : stock) {
            string exp = "";
            auto it = expiry.find(kv.first);
            if (it != expiry.end()) exp = it->second;
            cout << setw(20) << left << kv.first << "Qty: " << setw(6) << kv.second << " Exp: " << exp << "\n";
        }
    }
    void checkLowStock(int threshold = 10) const {
        cout << "-- Low stock (threshold " << threshold << ") --\n";
        bool any = false;
        for (auto &kv : stock) {
            if (kv.second < threshold) {
                string exp = "";
                auto it = expiry.find(kv.first);
                if (it != expiry.end()) exp = it->second;
                cout << kv.first << " -> " << kv.second << " (exp: " << exp << ")\n";
                any = true;
            }
        }
        if (!any) cout << "No low stock medicines.\n";
    }
    void performService() override { cout << "Pharmacy service: dispense medicines.\n"; }
    string name() const override { return "Pharmacy"; }
};

// --------------------------
// Diagnostics service
// --------------------------
class DiagnosticsService : public HospitalService {
private:
    map<int, vector<string>> reports;
public:
    DiagnosticsService() {}
    void addReport(int pid, const string &rep) { reports[pid].push_back(nowString() + " | " + rep); }
    void showReports(int pid) const {
        auto it = reports.find(pid);
        if (it == reports.end() || it->second.empty()) { cout << "No reports for patient " << pid << "\n"; return; }
        cout << "Reports for patient " << pid << ":\n";
        for (auto &r : it->second) cout << " - " << r << "\n";
    }
    void performService() override { cout << "Diagnostics service: run tests.\n"; }
    string name() const override { return "Diagnostics"; }
};

// --------------------------
// Emergency & Surgery services (examples)
// --------------------------
class EmergencyService : public HospitalService {
public:
    EmergencyService() {}
    void performService() override { cout << "Emergency: quick admission.\n"; }
    string name() const override { return "Emergency"; }
};

class SurgeryService : public HospitalService {
public:
    SurgeryService() {}
    void performService() override { cout << "Surgery: schedule & perform operation.\n"; }
    string name() const override { return "Surgery"; }
};

// --------------------------
// Billing & Appointment
// --------------------------

// Bill structure
struct Bill {
    int billId;
    int patientId;
    vector<pair<string,double>> items;
    string createdAt;
    bool insured;
    double coveragePercent;

    Bill() : billId(0), patientId(0), insured(false), coveragePercent(0.0) {}
    Bill(int id, int pid, bool ins, double cov) 
        : billId(id), patientId(pid), insured(ins), coveragePercent(cov) { 
        createdAt = nowString(); 
    }

    void addItem(const string &desc, double amt) { items.push_back({desc, amt}); }

    double base() const {
        double sum = 0;
        for (auto &it : items) sum += it.second;
        return sum;
    }

    double total() const {
        double b = base();
        return insured ? b * (1.0 - coveragePercent/100.0) : b;
    }

    // Formatted colorized print
    void print() const {
        setColor(11); // Cyan
        cout << "=====================================================\n";
        cout << " BILL ID: " << billId 
             << " | Patient ID: " << patientId 
             << " | Created: " << createdAt << "\n";
        cout << "=====================================================\n";

        setColor(14); // Yellow headers
        cout << setw(30) << left << "Item Description" 
             << setw(15) << right << "Amount\n";

        setColor(7); // White items
        cout << "-----------------------------------------------------\n";
        for (auto &it : items) {
            cout << setw(30) << left << it.first 
                 << setw(15) << right << fixed << setprecision(2) << it.second << "\n";
        }

        cout << "-----------------------------------------------------\n";
        setColor(10); // Green totals
        cout << setw(30) << left << "Base Amount:" 
             << setw(15) << right << fixed << setprecision(2) << base() << "\n";

        if (insured) {
            cout << setw(30) << left << "Insurance Coverage:" 
                 << setw(15) << right << coveragePercent << "%\n";
        }

        cout << setw(30) << left << "Total Payable:" 
             << setw(15) << right << fixed << setprecision(2) << total() << "\n";
        setColor(7); // Reset
        cout << "=====================================================\n";
    }

    // CSV export
    vector<string> toCSV() const {
        ostringstream oss;
        for (size_t i=0; i<items.size(); ++i) {
            oss << items[i].first << "#" << items[i].second;
            if (i+1<items.size()) oss << ";";
        }
        return {to_string(billId), to_string(patientId), (insured ? "1":"0"), 
                to_string(coveragePercent), createdAt, oss.str()};
    }

    // CSV import
    static Bill fromCSV(const vector<string> &r) {
        Bill b;
        if (r.size()>=1) b.billId = toIntSafe(r[0],0);
        if (r.size()>=2) b.patientId = toIntSafe(r[1],0);
        if (r.size()>=3) b.insured = (r[2]=="1");
        if (r.size()>=4) b.coveragePercent = toDoubleSafe(r[3], 0.0);
        if (r.size()>=5) b.createdAt = r[4];
        if (r.size()>=6) {
            string s = r[5];
            string cur;
            for (char c : s) {
                if (c == ';') {
                    if (!cur.empty()) {
                        auto pos = cur.find('#');
                        if (pos != string::npos) {
                            double amt = toDoubleSafe(cur.substr(pos+1), 0.0);
                            b.items.push_back({cur.substr(0,pos), amt});
                        }
                    }
                    cur.clear();
                } else cur.push_back(c);
            }
            if (!cur.empty()) {
                auto pos = cur.find('#');
                if (pos != string::npos) {
                    double amt = toDoubleSafe(cur.substr(pos+1), 0.0);
                    b.items.push_back({cur.substr(0,pos), amt});
                }
            }
        }
        return b;
    }
};



struct Appointment {
    int id;
    int patientId;
    int doctorId;
    string datetime;
    string type;
    string reason;
    Appointment() : id(0), patientId(0), doctorId(0) {}
    vector<string> toCSV() const { return {to_string(id), to_string(patientId), to_string(doctorId), datetime, type, reason}; }
    static Appointment fromCSV(const vector<string> &r) {
        Appointment a;
        if (r.size()>=1) a.id = toIntSafe(r[0],0);
        if (r.size()>=2) a.patientId = toIntSafe(r[1],0);
        if (r.size()>=3) a.doctorId = toIntSafe(r[2],0);
        if (r.size()>=4) a.datetime = r[3];
        if (r.size()>=5) a.type = r[4];
        if (r.size()>=6) a.reason = r[5];
        return a;
    }
};

// --------------------------
// User struct for login
// --------------------------
struct User {
    string username;
    string role;
    string password;
    int linkedId; // for patient or staff
    vector<string> toCSV() const { return {username, role, password, to_string(linkedId)}; }
    static User fromCSV(const vector<string> &r) {
        User u;
        if (r.size()>=1) u.username = r[0];
        if (r.size()>=2) u.role = r[1];
        if (r.size()>=3) u.password = r[2];
        if (r.size()>=4) u.linkedId = toIntSafe(r[3],0);
        return u;
    }
};

// --------------------------
// SHMS Database
// --------------------------
class SHMSDatabase {
private:
    int nextPersonId = 1;
    int nextAppointmentId = 1;
    int nextBillId = 1;

   
    map<int, Doctor> doctors;
    map<int, Staff> staffs;
    map<int, Appointment> appointments;
    map<int, Bill> bills;
    map<string, User> users;

    PharmacyService pharmacy;
    DiagnosticsService diagnostics;
    EmergencyService emergency;
    SurgeryService surgery;

public:
	 map<int, Patient> patients;
    SHMSDatabase() { loadAll(); seedIfEmpty(); }
    ~SHMSDatabase() { saveAll(); }

    // Persistence
    void loadAll() {
        loadUsers();
        loadPatients();
        loadDoctors();
        loadStaff();
        loadAppointments();
        loadBills();
        pharmacy.loadFromFile(MEDICINES_FILE);

        // set next ids
        nextPersonId = 1;
        for (auto &kv : patients) nextPersonId = max(nextPersonId, kv.first + 1);
        for (auto &kv : doctors) nextPersonId = max(nextPersonId, kv.first + 1);
        for (auto &kv : staffs) nextPersonId = max(nextPersonId, kv.first + 1);

        nextAppointmentId = 1;
        for (auto &kv : appointments) nextAppointmentId = max(nextAppointmentId, kv.first + 1);

        nextBillId = 1;
        for (auto &kv : bills) nextBillId = max(nextBillId, kv.first + 1);
    }

    void saveAll() {
        saveUsers();
        savePatients();
        saveDoctors();
        saveStaff();
        saveAppointments();
        saveBills();
        pharmacy.saveToFile(MEDICINES_FILE);
    }

    void loadUsers() {
        users.clear();
        ifstream in(USERS_FILE);
        if (!in.is_open()) {
            // create default admin and receptionist
            User admin{"admin","Admin","admin",0};
            User recept{"recept","Receptionist","recept",0};
            users[admin.username] = admin;
            users[recept.username] = recept;
            return;
        }
        string line;
        while (getline(in, line)) {
            if (trim(line).empty()) continue;
            auto r = splitCSV(line);
            User u = User::fromCSV(r);
            users[u.username] = u;
        }
    }
    void saveUsers() {
        ofstream out(USERS_FILE);
        for (auto &kv : users) out << joinCSV(kv.second.toCSV()) << "\n";
    }

    void loadPatients() {
        patients.clear();
        ifstream in(PATIENTS_FILE);
        if (!in.is_open()) return;
        string line;
        while (getline(in, line)) {
            if (trim(line).empty()) continue;
            auto r = splitCSV(line);
            Patient p = Patient::fromCSV(r);
            patients[p.getId()] = p;
        }
    }
    void savePatients() {
        ofstream out(PATIENTS_FILE);
        for (auto &kv : patients) out << joinCSV(kv.second.toCSVRow()) << "\n";
    }

    void loadDoctors() {
        doctors.clear();
        ifstream in(DOCTORS_FILE);
        if (!in.is_open()) return;
        string line;
        while (getline(in, line)) {
            if (trim(line).empty()) continue;
            auto r = splitCSV(line);
            Doctor d = Doctor::fromCSV(r);
            doctors[d.getId()] = d;
        }
    }
    void saveDoctors() {
        ofstream out(DOCTORS_FILE);
        for (auto &kv : doctors) out << joinCSV(kv.second.toCSVRow()) << "\n";
    }

    void loadStaff() {
        staffs.clear();
        ifstream in(STAFF_FILE);
        if (!in.is_open()) return;
        string line;
        while (getline(in, line)) {
            if (trim(line).empty()) continue;
            auto r = splitCSV(line);
            Staff s = Staff::fromCSV(r);
            staffs[s.getId()] = s;
        }
    }
    void saveStaff() {
        ofstream out(STAFF_FILE);
        for (auto &kv : staffs) out << joinCSV(kv.second.toCSVRow()) << "\n";
    }

    void loadAppointments() {
        appointments.clear();
        ifstream in(APPOINTMENTS_FILE);
        if (!in.is_open()) return;
        string line;
        while (getline(in, line)) {
            if (trim(line).empty()) continue;
            auto r = splitCSV(line);
            Appointment a = Appointment::fromCSV(r);
            appointments[a.id] = a;
            // add booked slot to doctor if exists
            if (doctors.count(a.doctorId)) doctors[a.doctorId].addBookedSlot(a.datetime);
        }
    }
    void saveAppointments() {
        ofstream out(APPOINTMENTS_FILE);
        for (auto &kv : appointments) out << joinCSV(kv.second.toCSV()) << "\n";
    }

    void loadBills() {
        bills.clear();
        ifstream in(BILLS_FILE);
        if (!in.is_open()) return;
        string line;
        while (getline(in, line)) {
            if (trim(line).empty()) continue;
            auto r = splitCSV(line);
            Bill b = Bill::fromCSV(r);
            bills[b.billId] = b;
        }
    }
    void saveBills() {
        ofstream out(BILLS_FILE);
        for (auto &kv : bills) out << joinCSV(kv.second.toCSV()) << "\n";
    }

    // seed demo data if empty
    void seedIfEmpty() {
        if (patients.empty() && doctors.empty() && staffs.empty()) {
            Doctor d1;
		d1.setName("Dr. Ayesha Khan"); 
		d1.setAge(45); 
		d1.setGender("F"); 
		d1.setContact("+92-300-0000000"); 
		d1.setSpecialization("Cardiology"); 
		d1.setFee(60.0); 
		addDoctor(d1);
            Doctor d2; 
		d2.setName("Dr. Omar Ali"); 
		d2.setAge(38); 
		d2.setGender("M"); 
		d2.setContact("+92-300-1111111"); 
		d2.setSpecialization("General"); 
		d2.setFee(30.0); 
		addDoctor(d2);
            Patient p1; 
		p1.setName("Muneeba Arshad"); 
		p1.setAge(22); 
		p1.setGender("F"); 
		p1.setContact("+92-300-2222222"); 
		addPatient(p1);
            Patient p2; 
		p2.setName("Ali Hassan"); 
		p2.setAge(30); 
		p2.setGender("M"); 
		p2.setContact("+92-300-3333333"); 
		p2.setInsurance(true); 
		p2.setInsuranceProvider("DemoCare"); 
		addPatient(p2);
		
            pharmacy.addMedicine("Paracetamol", 100, "2026-12-31");
            pharmacy.addMedicine("Amoxicillin", 50, "2025-05-30");
            
            // ensure default users exist
            if (!users.count("admin")) users["admin"] = User{"admin","Admin","admin",0};
            if (!users.count("recept")) users["recept"] = User{"recept","Receptionist","recept",0};
            if (!users.count("muneeba")) users["muneeba"] = User{"muneeba","Patient","password", 1}; // link to pid 1
            saveAll();
        }
    }

    // CRUD operations
    int addPatient(const Patient &p) {
        int id = nextPersonId++;
        Patient cp = p; cp.setId(id);
        patients[id] = cp;
        return id;
    }
    int addDoctor(const Doctor &d) {
        int id = nextPersonId++;
        Doctor cp = d; cp.setId(id);
        doctors[id] = cp;
        return id;
    }
    int addStaff(const Staff &s) {
        int id = nextPersonId++;
        Staff cp = s; cp.setId(id);
        staffs[id] = cp;
        return id;
    }

    Patient* findPatient(int id) { if (patients.count(id)) return &patients[id]; return nullptr; }
    Doctor* findDoctor(int id) { if (doctors.count(id)) return &doctors[id]; return nullptr; }
    Staff* findStaff(int id) { if (staffs.count(id)) return &staffs[id]; return nullptr; }

    vector<Patient> searchPatientsByName(const string &name) {
        vector<Patient> out;
        for (auto &kv : patients) if (kv.second.getName().find(name) != string::npos) out.push_back(kv.second);
        return out;
    }
    vector<Doctor> searchDoctorsBySpec(const string &spec) {
        vector<Doctor> out;
        for (auto &kv : doctors) if (kv.second.getSpecialization().find(spec) != string::npos) out.push_back(kv.second);
        return out;
    }

    bool isDoctorAvailable(int doctorId, const string &datetime) {
        Doctor* d = findDoctor(doctorId);
        if (!d) return false;
        for (auto &slot : d->getBookedSlots()) if (datetimeConflict(slot, datetime)) return false;
        return true;
    }

    int scheduleAppointment(const Appointment &a) {
        if (!patients.count(a.patientId)) throw runtime_error("Patient not found");
        if (!doctors.count(a.doctorId)) throw runtime_error("Doctor not found");
        if (!isDoctorAvailable(a.doctorId, a.datetime)) throw runtime_error("Doctor not available at requested datetime (conflict)");
        int id = nextAppointmentId++;
        Appointment cp = a; cp.id = id;
        appointments[id] = cp;
        doctors[a.doctorId].addBookedSlot(a.datetime);
        return id;
    }

    vector<Appointment> getAppointmentsForPatient(int pid) {
        vector<Appointment> out;
        for (auto &kv : appointments) if (kv.second.patientId == pid) out.push_back(kv.second);
        return out;
    }
    vector<Appointment> getAppointmentsForDoctor(int did) {
        vector<Appointment> out;
        for (auto &kv : appointments) if (kv.second.doctorId == did) out.push_back(kv.second);
        return out;
    }

    bool cancelAppointment(int aid) {
        if (!appointments.count(aid)) return false;
        Appointment a = appointments[aid];
        if (doctors.count(a.doctorId)) {
            auto &slots = const_cast<vector<string>&>(doctors[a.doctorId].getBookedSlots());
            slots.erase(remove_if(slots.begin(), slots.end(), [&](const string &s){ return datetimeConflict(s, a.datetime); }), slots.end());
        }
        appointments.erase(aid);
        return true;
    }

    int createBill(int pid, bool insured, double coverage) {
        if (!patients.count(pid)) throw runtime_error("Patient not found");
        int id = nextBillId++;
        bills[id] = Bill(id, pid, insured, coverage);
        return id;
    }
    void addBillItem(int billId, const string &desc, double amt) {
        if (!bills.count(billId)) throw runtime_error("Bill not found");
        bills[billId].addItem(desc, amt);
    }
    Bill* getBill(int id) { if (bills.count(id)) return &bills[id]; return nullptr; }

    // new: get bills for a patient (safe, efficient)
    vector<Bill> getBillsForPatient(int pid) const {
        vector<Bill> out;
        for (auto &kv : bills) if (kv.second.patientId == pid) out.push_back(kv.second);
        return out;
    }

    // pharmacy, diagnostics wrappers
    PharmacyService& getPharmacy() { return pharmacy; }
    DiagnosticsService& getDiagnostics() { return diagnostics; }
    EmergencyService& getEmergency() { return emergency; }
    SurgeryService& getSurgery() { return surgery; }

    // users
    bool addUser(const User &u) { if (users.count(u.username)) return false; users[u.username] = u; return true; }
    bool authenticate(const string &uname, const string &pwd, User &out) const {
        auto it = users.find(uname); if (it == users.end()) return false;
        if (it->second.password != pwd) return false;
        out = it->second; return true;
    }

    void listPatients() const {
     cout << "--- Patients ---\n";
      for (auto &kv : patients) kv.second.displayInfo();
	 }
	 
	 void printPatientsTable() const {
    setColor(11); // Cyan heading
    cout << "\n======================================== Patients List ==============================================\n";
    setColor(14); // Yellow for headers
    cout << setw(5) << left << "ID"
         << setw(25) << left << "Name"
         << setw(5) << left << "Age"
         << setw(8) << left << "Gender"
         << setw(18) << left << "Contact"
         << setw(15) << left << "Insured"
         << setw(18) << left << "Insurance Provider\n";
    setColor(7);
    cout << "---------------------------------------------------------------------------------------------------------\n";

    for (auto &kv : patients) {
        auto &p = kv.second;
        cout << setw(5) << left << p.getId()
             << setw(25) << left << p.getName()
             << setw(5) << left << p.getAge()
             << setw(8) << left << p.getGender()
             << setw(18) << left << p.getContact()
             << setw(15) << left << (p.isInsured() ? "Yes" : "No")
             << setw(18) << left << (p.isInsured() ? p.getInsuranceProvider() : "-") << "\n";
    }

    setColor(11);
    cout << "===========================================================================================================\n";
    setColor(7);
}

void printSinglePatientAsTable(int pid) {
    Patient* p = findPatient(pid);
    if (!p) {
        setColor(12); 
        cout << "Patient not found.\n"; 
        setColor(7);
        return;
    }

    setColor(11);
    cout << "\n==================== My Information ====================\n";
    setColor(14);
    cout << setw(5) << left << "ID"
         << setw(20) << left << "Name"
         << setw(5) << left << "Age"
         << setw(8) << left << "Gender"
         << setw(15) << left << "Contact"
         << setw(10) << left << "Insured"
         << setw(20) << left << "Insurance Provider\n";
    setColor(7);
    cout << "-------------------------------------------------------------\n";

    cout << setw(5) << left << p->getId()
         << setw(20) << left << p->getName()
         << setw(5) << left << p->getAge()
         << setw(8) << left << p->getGender()
         << setw(15) << left << p->getContact()
         << setw(10) << left << (p->isInsured() ? "Yes" : "No")
         << setw(20) << left << (p->isInsured() ? p->getInsuranceProvider() : "-") 
         << "\n";

    setColor(11);
    cout << "===========================================================\n";
    setColor(7);
}


	 
    void listDoctors() const { 
    cout << "--- Doctors ---\n"; 
    for (auto &kv : doctors) kv.second.displayInfo();
     }
     
     void printDoctorsTable() const {
        setColor(11); // Cyan heading
        cout << "\n========================================== Doctors List ==============================================\n";
        setColor(14); // Yellow for headers
        cout << setw(5) << left << "ID"
             << setw(25) << left << "Name"
             << setw(5) << left << "Age"
             << setw(8) << left << "Gender"
             << setw(18) << left << "Contact"
             << setw(18) << left << "Specialization"
             << setw(8) << right << "Fee\n";
        setColor(7); // White rows
        cout << "---------------------------------------------------------------------------------------------------------\n";

        for (auto &kv : doctors) {
            auto &d = kv.second;
            cout << setw(5) << left << d.getId()
                 << setw(25) << left << d.getName()
                 << setw(5) << left << d.getAge()
                 << setw(8) << left << d.getGender()
                 << setw(18) << left << d.getContact()
                 << setw(18) << left << d.getSpecialization()
                 << setw(8) << right << fixed << setprecision(2) << d.getFee() << "\n";
        }

        setColor(11); // Cyan footer
        cout << "===========================================================================================================\n";
        setColor(7); // Reset to white
    }
     
    void listStaff() const { 
    cout << "--- Staff ---\n";
     for (auto &kv : staffs) kv.second.displayInfo(); 
     }
     
     void printStaffTable() const {
    setColor(11);
    cout << "\n======================== Staff List =======================\n";
    setColor(14);
    cout << setw(5) << left << "ID"
         << setw(25) << left << "Name"
         << setw(5) << left << "Age"
         << setw(8) << left << "Gender"
         << setw(18) << left << "Contact"
         << setw(15) << left << "Role\n";
    setColor(7);
    cout << "-------------------------------------------------------------\n";

    for (auto &kv : staffs) {
        auto &s = kv.second;
        cout << setw(5) << left << s.getId()
             << setw(25) << left << s.getName()
             << setw(5) << left << s.getAge()
             << setw(8) << left << s.getGender()
             << setw(18) << left << s.getContact()
             << setw(15) << left << s.getRole() << "\n";
    }

    setColor(11);
    cout << "===============================================================\n";
    setColor(7);
}

     
    void listAppointments() const { 
    cout << "--- Appointments ---\n";
     for (auto &kv : appointments) cout << "Appointment ID: " << kv.second.id << "\nPatient ID: " << kv.second.patientId << "\nDoctor ID: " << kv.second.doctorId << "\nDate/time : " << kv.second.datetime << "\nInsured : " << kv.second.type << "\nReason: " << kv.second.reason << "\n"; }
    
    void printAppointmentsTable() const {
    setColor(11);
    cout << "\n====================== Appointments ======================\n";
    setColor(14);
    cout << setw(5) << left << "AID"
         << setw(8) << left << "PID"
         << setw(8) << left << "DID"
         << setw(20) << left << "Date/Time"
         << setw(12) << left << "Type"
         << setw(25) << left << "Reason\n";
    setColor(7);
    cout << "-------------------------------------------------------------\n";

    for (auto &kv : appointments) {
        auto &a = kv.second;
        cout << setw(5) << left << a.id
             << setw(8) << left << a.patientId
             << setw(8) << left << a.doctorId
             << setw(20) << left << a.datetime
             << setw(12) << left << a.type
             << setw(25) << left << a.reason << "\n";
    }

    setColor(11);
    cout << "=============================================================\n";
    setColor(7);
}

    
    void listBills() const {
     cout << "--- Bills ---\n";
      for (auto &kv : bills) {
	kv.second.print(); cout << "\n"; } 
	}
	
	void printBillsTable() const {
    setColor(11);
    cout << "\n=================== Bills List ===================\n";
    setColor(14);
    cout << setw(5) << left << "BID"
         << setw(12) << left << "Insured"
         << setw(10) << left << "Total\n";
    setColor(7);
    cout << "-----------------------------------------------------\n";

    for (auto &kv : bills) {
        auto &b = kv.second;
      cout 
     << setw(8) << left << b.patientId
     << setw(12) << left << (b.insured ? "Yes" : "No") 
     << setw(10) << left << fixed << setprecision(2) << b.total() << "\n";
}

    setColor(11);
    cout << "======================================================\n";
    setColor(7);
}


    void printStatistics() {
        cout << "\n--- Hospital Statistics ---\n";
        cout << "Total patients: " << patients.size() << "\n";
        cout << "Total doctors: " << doctors.size() << "\n";
        cout << "Total staff: " << staffs.size() << "\n";
        cout << "Total appointments: " << appointments.size() << "\n";
        double revenue = 0; for (auto &kv : bills) revenue += kv.second.total();
        cout << "Total revenue: " << fixed << setprecision(2) << revenue << "\n";
        map<int,int> cnt;
        for (auto &kv : appointments) cnt[kv.second.doctorId]++;
        int best = -1, bestCnt = 0;
        for (auto &kv : cnt) if (kv.second > bestCnt) { best = kv.first; bestCnt = kv.second; }
        if (best == -1) cout << "No bookings yet\n"; else cout << "Most booked doctor: " << (doctors.count(best) ? doctors[best].getName() : "Unknown") << " (" << bestCnt << " bookings)\n";
        cout << "---------------------------\n";
    }
};
// -----------------------------------------------------------------------------------------------------------------------------
//                                                     Menus
// -----------------------------------------------------------------------------------------------------------------------------
//                           *****************Admin menu*****************************
void adminMenu(SHMSDatabase &db, const User &me) {


    while (true) {
    	system("cls");
        setColor(11); // Cyan heading
        printSlow("\n=== Admin Menu ===\n",3);
        setColor(7);   // Reset to white

        // Menu options with colors
        setColor(10); cout << "1) "; setColor(7); cout << "Add Doctor\n";
        setColor(10); cout << "2) "; setColor(7); cout << "Add Staff\n";
        setColor(10); cout << "3) "; setColor(7); cout << "List Doctors\n";
        setColor(10); cout << "4) "; setColor(7); cout << "List Staff\n";
        setColor(10); cout << "5) "; setColor(7); cout << "List Patients\n";
        setColor(10); cout << "6) "; setColor(7); cout << "List Appointments\n";
        setColor(10); cout << "7) "; setColor(7); cout << "List Bills\n";
        setColor(10); cout << "8) "; setColor(7); cout << "View Statistics\n";
        setColor(10); cout << "9) "; setColor(7); cout << "Surgery service\n";
        setColor(10); cout <<"10) "; setColor(7); cout << "Save & Return\n";
        setColor(12); cout << "0) "; setColor(7); cout << "Exit Program\n";

        int choice = promptInt("Enter choice: ");

        if (choice == 1) {
        	system("cls");
            Doctor d;
            d.setName(promptString("Doctor Name: "));
            d.setAge(promptInt("Age: "));
            d.setGender(promptString("Gender: "));
            d.setContact(promptString("Contact: "));
            d.setSpecialization(promptString("Specialization: "));
            d.setFee(promptDouble("Consultation Fee: ", 0.0));
            int id = db.addDoctor(d);

            setColor(10); cout << "Doctor added with ID " << id << "\n"; setColor(7);

            string create = promptString("Create login for this doctor? (y/n): ", true);
            if (!create.empty() && (create[0]=='y'||create[0]=='Y')) {
                string uname = promptString("Username: ");
                string pwd = promptString("Password: ");
                User u{uname, "Doctor", pwd, id};
                if (db.addUser(u)) {
                    setColor(10); cout << "User created successfully.\n"; setColor(7);
                } else {
                    setColor(12); cout << "Username already exists!\n"; setColor(7);
                }
            }
        } 
        else if (choice == 2) {
        	system("cls");
            Staff s;
            s.setName(promptString("Staff Name: "));
            s.setAge(promptInt("Age: "));
            s.setGender(promptString("Gender: "));
            s.setContact(promptString("Contact: "));
            s.setRole(promptString("Role: "));
            int id = db.addStaff(s);

            setColor(10); cout << "Staff added with ID " << id << "\n"; setColor(7);

            string uname = promptString("Create username for staff (leave blank to skip): ", true);
            if (!uname.empty()) {
                string pwd = promptString("Password: ");
                User u{uname, s.role, pwd, id};
                if (db.addUser(u)) {
                    setColor(10); cout << "User created successfully.\n"; setColor(7);
                } else {
                    setColor(12); cout << "Username already exists!\n"; setColor(7);
                }
            }
        } 
        else if (choice == 3) { db.printDoctorsTable();
        pauseConsole();
	  }
        else if (choice == 4) { db.printStaffTable();
        pauseConsole();
        }
        else if (choice == 5) { db.printPatientsTable();
        pauseConsole();
        }
        else if (choice == 6) { db.printAppointmentsTable();
        pauseConsole();
	  }
	  else if (choice == 7) { db.printBillsTable();
	  pauseConsole();
	  }
        else if (choice == 8) { db.printStatistics();
        pauseConsole();
	  }
	 else if (choice == 9) {
    int pid = promptInt("Enter Patient ID for surgery: ");
    if (db.findPatient(pid)) {
        db.getSurgery().performService(); // or pass pid if your function expects it
        setColor(10); cout << "Surgery scheduled for patient " << pid << "\n"; setColor(7);
    } else {
        setColor(12); cout << "Patient not found.\n"; setColor(7);
    }
    pauseConsole();
}

	  else if (choice == 10) { 
            db.saveAll();
            setColor(10); cout << "All data saved successfully.\n"; setColor(7);
            return;
        }
        else if(choice==0) {
        	db.saveAll();
        	setColor(10); cout << "All data saved. Exiting program.\n"; setColor(7);
            exit(0);
	  }
        
        else  {
            setColor(12); cout << "Invalid choice! Try again.\n"; setColor(7);
        }
    }
}
     //Receptionist Menu----------------------------------------------------------------------------
	void receptionistMenu(SHMSDatabase &db, const User &me) {
 	  while (true) {
    	system("cls");
        setColor(11); // Cyan heading
        printSlow("\n=== RECEPTIONIST MENU ===", 15);
        setColor(7);

        // menu lines
        setColor(10); cout << "1) "; setColor(7); cout << "Register Patient\n";
        setColor(10); cout << "2) "; setColor(7); cout << "List Patients (table)\n";
        setColor(10); cout << "3) "; setColor(7); cout << "Schedule Appointment\n";
        setColor(10); cout << "4) "; setColor(7); cout << "View Appointments (table)\n";
        setColor(10); cout << "5) "; setColor(7); cout << "Create Bill for Patient\n";
        setColor(10); cout << "6) "; setColor(7); cout << "Add Medicine to Pharmacy\n";
        setColor(10); cout << "7) "; setColor(7); cout << "Register Emergency Admission\n";
        setColor(10); cout << "8) "; setColor(7); cout << "Save & Return\n";
        setColor(12); cout << "0) "; setColor(7); cout << "Exit program\n";

        int choice = promptInt("Enter choice: ");

        // 1) Register patient
        if (choice == 1) {
        	system("cls");
            Patient p;
            p.setName(promptString("Name: "));
            p.setAge(promptInt("Age: "));
            p.setGender(promptString("Gender: "));
            p.setContact(promptString("Contact: "));
            string ins = promptString("Insured? (1=yes,0=no): ");
            if (!ins.empty() && ins[0]=='1') {
                p.setInsurance(true);
                p.setInsuranceProvider(promptString("Insurance provider: "));
            }
            p.setNationalId(promptString("National ID (optional): ", true));
            int pid = db.addPatient(p);

            setColor(10);
            cout << "Patient registered with ID " << pid << "\n";
            setColor(7);

            string create = promptString("Create login for patient? (y/n): ", true);
            if (!create.empty() && (create[0]=='y'||create[0]=='Y')) {
                string uname = promptString("Username: ");
                string pwd = promptString("Password: ");
                User u{uname,"Patient",pwd,pid};
                if (db.addUser(u)) { setColor(10); cout << "User created.\n"; setColor(7); }
                else { setColor(12); cout << "Username exists.\n"; setColor(7); }
            }
        }

        // 2) List Patients (table)
        else if (choice == 2) {
        	system("cls");
            db.printPatientsTable();
            pauseConsole();
        }

        // 3) Schedule Appointment
        else if (choice == 3) {
        	system("cls");
            int pid = promptInt("Patient ID: ");
            int did = promptInt("Doctor ID: ");
            string date = promptString("Date (YYYY-MM-DD): ");
            string time = promptString("Time (HH:MM): ");
            string dt = date + " " + time;
            string type = promptString("Type (online/walk-in): ");
            string reason = promptString("Reason: ");
            Appointment a; a.patientId = pid; a.doctorId = did; a.datetime = dt; a.type = type; a.reason = reason;
            try {
                int aid = db.scheduleAppointment(a);
                setColor(10);
                cout << "Appointment scheduled with ID " << aid << "\n";
                setColor(7);
            } catch (exception &ex) {
                setColor(12); cout << "Error: " << ex.what() << "\n"; setColor(7);
            }
            pauseConsole();
        }

        // 4) View Appointments (table)
        else if (choice == 4) {
        	system("cls");
            db.printAppointmentsTable();
            pauseConsole();
        }

        // 5) Create Bill for Patient
        else if (choice == 5) {
        	system("cls");
            int pid = promptInt("Patient ID: ");
            Patient* pp = db.findPatient(pid);
            if (!pp) { setColor(12); cout << "Patient not found\n"; setColor(7); continue; }
            bool insured = pp->isInsured();
            double cov = insured ? promptDouble("Insurance coverage percent: ", 0.0) : 0.0;
            int bid = db.createBill(pid, insured, cov);

            setColor(10);
            cout << "Bill created with ID " << bid << ". Enter items (type 'done' for description to finish):\n";
            setColor(7);

            while (true) {
                string desc = promptString("Item description: ", true);
                if (trim(desc) == "done" || desc.empty()) break;
                double amt = promptDouble("Amount: ");
                db.addBillItem(bid, desc, amt);
            }
            Bill* b = db.getBill(bid);
            if (b) b->print();
            pauseConsole();
        }

        // 6) Add Medicine to Pharmacy
        else if (choice == 6) {
        	system("cls");
        	printSlow("================Medicine to Pharmacy=============",3);

            string name = promptString("Medicine name: ");
            int qty = promptInt("Qty: ");
            string exp = promptString("Expiry (YYYY-MM-DD): ");
            db.getPharmacy().addMedicine(name, qty, exp);
            setColor(10);
            cout << "Medicine added.\n";
            setColor(7);
            pauseConsole();
        }

        // 7) Register Emergency Admission
        else if (choice == 7) {
        	system("cls");
        	printSlow("================Energency Admission=============",3);
            int pid = promptInt("Patient ID for emergency admission: ");
            if (db.findPatient(pid)) {
                db.getEmergency().performService();
                setColor(10); cout << "Emergency admission registered.\n"; setColor(7);
            } else {
                setColor(12); cout << "Patient not found.\n"; setColor(7);
            }
            pauseConsole();
        }

        // 8) Save & Return
        else if (choice == 8) {
            db.saveAll();
            setColor(10); cout << "Saved.\n"; setColor(7);
            return;
        }

        // 0) Exit program
        else if (choice == 0) {
            db.saveAll();
            setColor(10); cout << "Saved. Exiting.\n"; setColor(7);
            exit(0);
        }

        else {
            setColor(12); cout << "Invalid choice.\n"; setColor(7);
        }
    }
}

// ==================== Patient Menu (Table Format) ====================
void patientMenu(SHMSDatabase &db, const User &me) {
    if (me.role != "Patient") {
        setColor(12); cout << "Not a patient account.\n"; setColor(7);
        return;
    }

    int pid = me.linkedId;
    if (!db.findPatient(pid)) {
        setColor(12); cout << "Linked patient record missing.\n"; setColor(7);
        return;
    }

    while (true) {
        system("cls");
        setColor(11);
        printSlow("=== PATIENT MENU ===", 15);
        setColor(7);

        setColor(10); cout << "1) "; setColor(7); cout << "View My Info\n";
        setColor(10); cout << "2) "; setColor(7); cout << "View My Appointments\n";
        setColor(10); cout << "3) "; setColor(7); cout << "View My Bills\n";
        setColor(10); cout << "4) "; setColor(7); cout << "View My Diagnostic Reports\n";
        setColor(10); cout << "5) "; setColor(7); cout << "Save & Return\n";
        setColor(12); cout << "0) "; setColor(7); cout << "Exit Program\n";

        int choice = promptInt("Enter choice: ");

        if (choice == 1) {
   	 db.printSinglePatientAsTable(pid);
  	  pauseConsole();
		}


        else if (choice == 2) {
    auto ap = db.getAppointmentsForPatient(pid);
    setColor(11);
    printSlow("\n=================== My Appointments ===================\n",3);
    setColor(14);
    cout << setw(5) << left << "AID"
         << setw(8) << left << "DID"
         << setw(20) << left << "Date/Time"
         << setw(15) << left << "Type"
         << setw(25) << left << "Reason\n";
    setColor(7);
    cout << "-------------------------------------------------------------\n";

    if (ap.empty()) {
        cout << "No appointments found.\n";
    } else {
        for (auto &a : ap) {
            cout << setw(5) << left << a.id
                 << setw(8) << left << a.doctorId
                 << setw(20) << left << a.datetime
                 << setw(15) << left << a.type
                 << setw(25) << left << a.reason << "\n";
        }
    }

    setColor(11);
    cout << "=============================================================\n";
    setColor(7);
    pauseConsole();
}

        else if (choice == 3) {
            auto bills = db.getBillsForPatient(pid);
            setColor(11); cout << "\n=== My Bills ===\n"; setColor(7);
            if (bills.empty()) cout << "No bills found.\n";
            else for (auto &b : bills) b.print();
            pauseConsole();
        }
        else if (choice == 4) {
            db.getDiagnostics().showReports(pid);
            pauseConsole();
        }
        else if (choice == 5) {
            db.saveAll();
            setColor(10); cout << "Saved. Returning.\n"; setColor(7);
            return;
        }
        else if (choice == 0) {
            db.saveAll();
            setColor(10); cout << "Saved. Exiting.\n"; setColor(7);
            exit(0);
        }
        else {
            setColor(12); cout << "Invalid choice.\n"; setColor(7);
            pauseConsole();
        }
    }
}

// ==================== Doctor Menu (Table Format) ====================
void doctorMenu(SHMSDatabase &db, const User &me) {
    if (me.role != "Doctor") {
        setColor(12); cout << "Not a doctor account.\n"; setColor(7);
        return;
    }

    int did = me.linkedId;
    if (!db.findDoctor(did)) {
        setColor(12); cout << "Doctor record missing.\n"; setColor(7);
        return;
    }

    while (true) {
        system("cls");
        setColor(11);
        printSlow("=== DOCTOR MENU ===", 15);
        setColor(7);

        setColor(10); cout << "1) "; setColor(7); cout << "View My Appointments\n";
        setColor(10); cout << "2) "; setColor(7); cout << "Add Diagnostic Report for Patient\n";
        setColor(10); cout << "3) "; setColor(7); cout << "Schedule Surgery for Patient\n";
        setColor(10); cout << "4) "; setColor(7); cout << "Save & Return\n";
        setColor(12); cout << "0) "; setColor(7); cout << "Exit Program\n";

        int choice = promptInt("Enter choice: ");

        if (choice == 1) {
            // Appointments Table for this doctor
            auto ap = db.getAppointmentsForDoctor(did);
            setColor(11);
            cout << "\n=== My Appointments ===\n"; setColor(7);
            if (ap.empty()) cout << "No appointments found.\n";
            else {
                cout << setw(5) << "AID" << setw(8) << "PID" << setw(20) << "Date/Time" << setw(15) << "Type" << setw(25) << "Reason\n";
                for (auto &a : ap)
                    cout << setw(5) << a.id << setw(8) << a.patientId << setw(20) << a.datetime << setw(15) << a.type << setw(25) << a.reason << "\n";
            }
            pauseConsole();
        }
        else if (choice == 2) {
            int pid = promptInt("Patient ID: ");
            string rep = promptString("Report text: ");
            db.getDiagnostics().addReport(pid, rep);
            setColor(10); cout << "Report saved.\n"; setColor(7);
            pauseConsole();
        }
        else if (choice == 3) {
            int pid = promptInt("Patient ID: ");
            db.getSurgery().performService();
            setColor(10); cout << "Surgery scheduled for patient " << pid << "\n"; setColor(7);
            pauseConsole();
        }
        else if (choice == 4) {
            db.saveAll();
            setColor(10); cout << "Saved. Returning.\n"; setColor(7);
            return;
        }
        else if (choice == 0) {
            db.saveAll();
            setColor(10); cout << "Saved. Exiting.\n"; setColor(7);
            exit(0);
        }
        else {
            setColor(12); cout << "Invalid choice.\n"; setColor(7);
            pauseConsole();
        }
    }
}



// ==================== Guest View Menu (Table Format) ====================
void guestViewMenu(SHMSDatabase &db) {
    while (true) {
        system("cls");
        setColor(11); // Cyan heading
        printSlow("=== GUEST VIEW MENU ===", 15);
        setColor(7);

        printSlow("Browse hospital information without login", 10);

        // Menu lines
        setColor(10); cout << "1) "; setColor(7); cout << "View Doctors List\n";
        setColor(10); cout << "2) "; setColor(7); cout << "View Medicines in Pharmacy\n";
        setColor(10); cout << "3) "; setColor(7); cout << "View Patients (public info)\n";
        setColor(10); cout << "4) "; setColor(7); cout << "View Appointments (public info)\n";
        setColor(10); cout << "5) "; setColor(7); cout << "View Hospital Statistics\n";
        setColor(10); cout << "6) "; setColor(7); cout << "Return to Main Menu\n";
        setColor(12); cout << "0) "; setColor(7); cout << "Exit Program\n";

        int choice = promptInt("Enter choice: ");

        if (choice == 1) {
            // Doctors Table
            db.printDoctorsTable();
            pauseConsole();
        }
        else if (choice == 2) {
            // Pharmacy Medicines
            db.getPharmacy().listMedicines();
            pauseConsole();
        }
        else if (choice == 3) {
            // Patients Table
            db.printPatientsTable();
            pauseConsole();
        }
        else if (choice == 4) {
            // Appointments Table
            db.printAppointmentsTable();
            pauseConsole();
        }
        else if (choice == 5) {
            // Statistics
            db.printStatistics();
            pauseConsole();
        }
        else if (choice == 6) {
            // Back to main menu
            db.saveAll();
            setColor(10); cout << "Returning to Main Menu.\n"; setColor(7);
            break;
        }
        else if (choice == 0) {
            db.saveAll();
            setColor(10); cout << "Saved. Exiting.\n"; setColor(7);
            exit(0);
        }
        else {
            setColor(12); cout << "Invalid choice.\n"; setColor(7);
            pauseConsole();
        }
    }
}



// ======================================================((     Main loop   ))===========================================================

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    SHMSDatabase db;

    // ========= Banner =========
    setColor(14); // Yellow
    printSlow("\t\t===========================================", 2);

    setColor(11); // Cyan
    printSlow("\t\t    Smart Hospital Management System", 30);

    setColor(14); 
    printSlow("\t\t===========================================", 2);

    setColor(7); 

    // ========= Menu Loop =========
while (true) {
    setColor(14); printSlow("\nMain Menu:", 30);
    setColor(10); printSlow("1) Login", 30);
    setColor(9);  printSlow("2) Continue as Guest", 30);
    setColor(13); printSlow("3) Register (patient)", 30);
    setColor(12); printSlow("0) Exit", 30);
    setColor(7);

    int choice = promptInt("Enter choice: ");

    if (choice == 1) {  // LOGIN
      // ========= Login Menu =========
	while (true) {
    system("cls");
    setColor(11); // Cyan heading
    cout << "\nLogin As:\n";
    setColor(7);  

    setColor(10); printSlow( "1)  Admin\n",4);
    setColor(10); printSlow( "2) Receptionist\n",4);
    setColor(10); printSlow( "3) Doctor\n",4);
    setColor(10); printSlow( "4) Patient\n",4);
    setColor(12); printSlow( "0) Cancel\n",4);

    int roleChoice = promptInt("Enter choice: ");
    string expectedRole;

    if (roleChoice == 1) expectedRole = "Admin";
    else if (roleChoice == 2) expectedRole = "Receptionist";
    else if (roleChoice == 3) expectedRole = "Doctor";
    else if (roleChoice == 4) expectedRole = "Patient";
    else if (roleChoice == 0) break;
    else {
        setColor(12); cout << "Invalid choice.\n"; setColor(7);
        continue;
    }

    setColor(11); cout << "\nEnter credentials for " << expectedRole << ":\n"; setColor(7);
    string uname = promptString("Username: ");
    string pwd = promptString("Password: ");

    User u;
    if (db.authenticate(uname, pwd, u) && u.role == expectedRole) {
        setColor(10); printSlow("Login successful as " + expectedRole, 30); setColor(7);

        if (expectedRole == "Admin") adminMenu(db, u);
        else if (expectedRole == "Receptionist") receptionistMenu(db, u);
        else if (expectedRole == "Doctor") doctorMenu(db, u);
        else if (expectedRole == "Patient") patientMenu(db, u);
        break; // exit after menu returns
    } else {
        setColor(12); printSlow("Invalid credentials or role mismatch.", 30); setColor(7);
    }
}

    }

    else if (choice == 2) {
        guestViewMenu(db);
    }

    else if (choice == 3) {
         setColor(13);
            printSlow("Register a new patient user:", 30);
            setColor(7);

            string name = promptString("Full name: ");
            int age = promptInt("Age: ");
            string gender = promptString("Gender: ");
            string contact = promptString("Contact: ");
            string ins = promptString("Insured? (1=yes,0=no): ");

            Patient p; 
            p.setName(name); 
            p.setAge(age); 
            p.setGender(gender); 
            p.setContact(contact);

            if (!ins.empty() && ins[0]=='1') { 
                p.setInsurance(true); 
                p.setInsuranceProvider(promptString("Insurance provider: ")); 
            }

            int pid = db.addPatient(p);
            setColor(10);
            printSlow("Patient registered with ID " + to_string(pid) + ". Create login now.", 30);
            setColor(7);

            string uname = promptString("Username: ");
            string pwd = promptString("Password: ");
            User u{uname,"Patient",pwd,pid};
            if (db.addUser(u)) {
                setColor(10);
                printSlow("User created. You can login now.", 30);
            } else {
                setColor(12);
                printSlow("Username exists.", 30);
            }
            setColor(7);
        } 
        
    else if (choice == 0) {
        db.saveAll();
        setColor(10);
        printSlow("Saved. Exiting.", 30);
        setColor(7);
        break;
    }

    else {
        setColor(12);
        printSlow("Invalid choice.", 30);
        setColor(7);
    }
} 
 return 0;
 
 }


// The end of our Porject 
// It wsa to fantastic and awesome Project and we worked together on it
// we will try to make new projects, better then this one 


//==============================
//     thank you
//==============================











