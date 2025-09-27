📝 Smart Hospital Management System — README.md
# 🏥 Smart Hospital Management System (C++)

![GitHub repo size](https://img.shields.io/github/repo-size/SaqlainMushtaq590/OOP-works?color=blue&label=Repo%20Size)
![C++](https://img.shields.io/badge/Language-C++-informational?style=flat&logo=c%2B%2B&logoColor=white&color=00599C)
![OOP](https://img.shields.io/badge/OOP-Principles-success?style=flat)

A **console-based C++ application** for managing hospital operations — appointments, billing, pharmacy, diagnostics, and role-based access — built entirely with **Object-Oriented Programming** principles.

---

## 🚀 Features

| Feature | Description |
|---------|-------------|
| **Role-based menus** | Admin, Receptionist, Doctor, Patient — each with tailored capabilities |
| **Appointments** | Schedule, view, and manage appointments with conflict checks |
| **Billing** | Create itemized bills with optional insurance coverage |
| **Pharmacy** | Add medicines with quantity and expiry, list and manage stock |
| **Diagnostics & Surgery** | Add diagnostic reports and schedule surgeries |
| **Persistence** | Data stored in simple file-based format (CSV-like) |

---

## 👥 Team Collaboration

This project was a **team effort**:

| Member | Contribution |
|--------|--------------|
| **Saqlain Mushtaq** | Lead developer — debugging, code changes, formatting, and feature integration |
| **Muneeba Arshad** | Initial codebase and core logic provider |
| **Afzaal Ahmed** | Testing, additional development contributions |

---

## 🖥️ OOP Concepts Used

| Concept | Where / How |
|---------|-------------|
| **Classes & Objects** | `Patient`, `Doctor`, `Staff`, `Appointment`, `Bill`, `Pharmacy`, `Diagnostics`, `Surgery`, `SHMSDatabase`, `User` |
| **Encapsulation** | Private members with getters and setters in person-derived classes |
| **Inheritance** | `Person → Patient/Doctor/Staff`; `HospitalService → Pharmacy/Diagnostics/Emergency/Surgery` |
| **Composition** | `SHMSDatabase` contains `Pharmacy`, `Diagnostics`, `Emergency`, `Surgery` (strong ownership) |
| **Aggregation** | `SHMSDatabase` maintains collections of `Patients`, `Doctors`, `Appointments` (loose association) |
| **Polymorphism** | `HospitalService` virtual interface used to call `performService()` for different services |

---

## 📸 UML Diagram

Here’s the UML diagram showing relationships between classes (composition, aggregation, inheritance):

![UML Diagram](Project/PROJECT WORK/A_UML_class_diagram_in_digital_vector_graphic_form.png.png)


---

## ⚙️ Installation & Usage

1. **Clone the repo:**
   ```bash
   git clone https://github.com/SaqlainMushtaq590/OOP-works.git
   cd OOP-works


Compile and run:

g++ main.cpp -o SmartHospital
./SmartHospital


Default logins:

Admin: admin / admin

Receptionist: recept / recept

Doctor/Patient accounts are created at runtime.

📂 Data Files Used

patients.txt

doctors.txt

staff.txt

appointments.txt

bills.txt

medicines.txt

users.txt

(Created and updated automatically by the program.)

🖥️ Example Console Screens

(Optional: add screenshots of your console menus to make the README visually rich.)

📝 Notes

Built in C++ using Dev-C++ (compatible with g++).

Role-based login ensures each user sees only relevant features.

Includes colorized console output and typing animations for better UX.

💡 Future Enhancements

Add a GUI (e.g. with SFML or Qt)

Online database connectivity

Extended reporting module

📜 License

MIT License © Saqlain Mushtaq 
              Muneeba Arshad
              Afzaal Ahmed 
              Smart Hospital Management System.
