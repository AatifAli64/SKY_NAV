# ✈️ SKY NAVIGATING AI

**SKY NAVIGATING AI** is an intelligent Flight Management System designed in C++, using Dijkstra’s Algorithm to find optimized flight routes based on price or time.  
It offers a fully functional Command-Line Interface (CLI) and a minimal Graphical User Interface (GUI) (work in progress), with the GUI currently able to display flights visually.

---

## 📜 Table of Contents
- [Features](#-features)
- [How It Works](#-how-it-works)
- [Project Structure](#-project-structure)
- [Technologies Used](#-technologies-used)
- [How to Run](#-how-to-run)
- [Flights File Format](#-flights-file-format)
- [Future Improvements](#-future-improvements)
- [Screenshots](#-screenshots)
- [License](#-license)

---

## 🚀 Features

### ✅ Command-Line Mode (Fully Functional)
- Display all available flights
- Search and book direct or connecting flights
- Search flights based on preferences (price/time)
- Multi-leg flight search using linked lists
- Generate and display subgraph for specific airlines
- Manage layovers
- Find the shortest path between cities using Dijkstra's algorithm

### 🖼️ Graphics Mode (Minimal GUI, Work in Progress)
- Currently displays all available flights visually
- SFML-based GUI under active development

---

## 🧠 How It Works
The system models the flight network as a graph:
- **Nodes** = Cities
- **Edges** = Flights (carrying data: cost, time, airline, date)

Users can:
- Run in **Console Mode (CLI)**
- Select **Graphics Mode** (basic flight visualization)

🔹 **Dijkstra’s Algorithm** finds optimized routes  
🔹 **Custom Linked Lists** manage multi-leg flights  
🔹 Flights are loaded from a `flights.txt` file

---

## 📂 Project Structure
SKY-NAVIGATING-AI/
│
├── include/ # Header files
├── src/ # Source files
├── flights.txt # Flight data input file
├── README.md # Project documentation
└── LICENSE # License file (optional)


---

## 🛠️ Technologies Used
| Technology       | Purpose                          |
|------------------|----------------------------------|
| C++              | Core logic                       |
| Dijkstra         | Shortest path algorithm          |
| SFML             | Graphics Mode (flight visualization) |
| Command-Line     | CLI-based interactive interface  |
| File I/O         | Handling flight data from text files |

---

## ⚙️ How to Run

### Prerequisites
- Visual Studio 2022 (or any C++17 compatible compiler)
- SFML 2.6.2 for Graphics Mode
- C++17 standard enabled in project settings

### Build & Run Steps
1. Clone the repository or download the files.
2. Open the project in Visual Studio 2022.
3. Configure project properties:
   - **C/C++ → General → Additional Include Directories**  
     ```
     C:\SFML-2.6.2-windows-vc17-64-bit\SFML-2.6.2\include
     ```
   - **Linker → General → Additional Library Directories**  
     ```
     C:\SFML-2.6.2-windows-vc17-64-bit\SFML-2.6.2\lib
     ```
   - **Linker → Input → Additional Dependencies**  
     ```
     sfml-graphics-d.lib
     sfml-window-d.lib
     sfml-system-d.lib
     ```
4. Set PATH to include SFML binaries (or copy DLLs to your executable folder):  

C:\SFML-2.6.2-windows-vc17-64-bit\SFML-2.6.2\bin


### Run Modes

#### Graphics Mode
- Currently displays available flights in a window using SFML.

#### Console Mode
- Fully functional flight management system:
  - Displays menus
  - Allows searches, bookings, and pathfinding
  - Uses the terminal interface

---

## 🗃️ Flights File Format (`flights.txt`)
Source,Destination,FlightNumber,Date,Price,Duration
### Example
Karachi,Lahore,PK300,12/03/2025,1000,2h
Lahore,Islamabad,PK301,13/03/2025,500,1h
Islamabad,Quetta,PK302,14/03/2025,1500,3h


---

## 🚧 Future Improvements
- 🔹 Complete the SFML-based GUI
- 🔹 Add Interactive Maps showing flight routes
- 🔹 Build Admin Panel to manage flights (add/remove/edit)
- 🔹 Support Passenger Accounts with booking history
- 🔹 Integrate Real-Time Flight APIs for live data
- 🔹 Cross-platform support for Linux & macOS

---

## 📝 License
This project is for educational use. Feel free to fork, modify, and distribute with credit.

---

## ✅ Summary
**SKY NAVIGATING AI** combines powerful pathfinding algorithms with intuitive CLI controls, and an emerging graphical interface to offer a streamlined flight management experience.  
Current focus: perfecting GUI functionality.
