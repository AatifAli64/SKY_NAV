#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
using namespace std;

int ReturnTimeinMins(string& time)
{
    int depHours, depMinutes, arrHours, arrMinutes;
    sscanf_s(time.c_str(), "%d:%d", &depHours, &depMinutes);

    // Convert both times to minutes
    int minutes = depHours * 60 + depMinutes;
    return minutes;
}

double calculateTimeDifference(const string& departure_time, const string& arrival) {
    int depHours, depMinutes, arrHours, arrMinutes;

    // Parse the departure_time and arrival times
    sscanf_s(departure_time.c_str(), "%d:%d", &depHours, &depMinutes);
    sscanf_s(arrival.c_str(), "%d:%d", &arrHours, &arrMinutes);

    // Convert both times to minutes
    int depTimeInMinutes = depHours * 60 + depMinutes;
    int arrTimeInMinutes = arrHours * 60 + arrMinutes;

    // Handle overnight flights
    if (arrTimeInMinutes < depTimeInMinutes) {
        arrTimeInMinutes += 24 * 60;
    }

    // Calculate the difference in minutes
    return (arrTimeInMinutes - depTimeInMinutes) / 60.0;
}

bool isDateBeforeOrEqual(string date1, string date2) {
    int day1, month1, year1;
    int day2, month2, year2;

    // Parse date1 (format: DD/MM/YYYY)
    sscanf_s(date1.c_str(), "%d/%d/%d", &day1, &month1, &year1);

    // Parse date2 (format: DD/MM/YYYY)
    sscanf_s(date2.c_str(), "%d/%d/%d", &day2, &month2, &year2);

    // Compare years
    if (year1 < year2) return true;
    if (year1 > year2) return false;

    // Compare months
    if (month1 < month2) return true;
    if (month1 > month2) return false;

    // Compare days
    return day1 <= day2; // Allow equal days
}

class LLNode {
public:
    string origin;
    string destination;
    string departure_time;
    string arrival_time;
    int flight_price;
    string airline;
    LLNode* next;

    LLNode(const string& ori, const string& dest, const string& dep, const string& arr, double price, const string& air)
        : origin(ori), destination(dest), departure_time(dep), arrival_time(arr), flight_price(price), airline(air), next(NULL) {
    }
};

class LinkedList {
public:
    LLNode* head;
    LLNode* tail;

    LinkedList() : head(NULL), tail(NULL) {}

    // Add a segment to the linked list
    void AddLink(const string& ori, const string& dest, const string& dep, const string& arr, double price, const string& air) {
        LLNode* newNode = new LLNode(ori, dest, dep, arr, price, air);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Display the entire journey
    void DisplayPath() {
        if (!head) {
            cout << "No route available.\n";
            return;
        }
        LLNode* temp = head;
        cout << "Journey Details:\n";
        while (temp) {
            cout << temp->origin << " -> " << temp->destination
                << " | Departure: " << temp->departure_time
                << " | Arrival: " << temp->arrival_time
                << " | Price: " << temp->flight_price
                << " | Airline: " << temp->airline << "\n";
            temp = temp->next;
        }
    }

    // Clear the linked list
    void Clear() {
        LLNode* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = NULL;
    }
};

class PriorityQueue {
public:
    struct Node {
        int cityIndex;
        double priority; // This could represent either price or time
        Node* next;

        Node(int city, double priority) : cityIndex(city), priority(priority), next(nullptr) {}

    };

private:
    Node* front;

public:
    PriorityQueue() : front(nullptr) {}

    bool isEmpty() {
        return front == nullptr;
    }

    void enqueue(int cityIndex, double priority) {
        Node* newNode = new Node(cityIndex, priority);

        if (!front || front->priority > priority) {
            // Insert at the front if queue is empty or the priority is smaller than front's priority
            newNode->next = front;
            front = newNode;
        }
        else {
            Node* temp = front;
            while (temp->next && temp->next->priority <= priority) {
                temp = temp->next;
            }
            newNode->next = temp->next;
            temp->next = newNode;
        }
    }

    int dequeue() {
        if (isEmpty()) {
            return -1;
        }

        Node* temp = front;
        int cityIndex = temp->cityIndex;
        front = front->next;
        delete temp;
        return cityIndex;
    }
};

class Flight {
public:
    string source;
    string destination;
    string departure_time;
    string arrival_time;
    double price;
    string airline;
    double total_time;
    string date;

    Flight() : source(""), destination(""), departure_time(""), arrival_time(""), price(0.0), airline(""), total_time(0.0), date("") {}

    Flight(string source, string destination, string date, string departure_time, string arrival_time, double price, string airline) {
        this->source = source;
        this->destination = destination;
        this->date = date;
        this->departure_time = departure_time;
        this->arrival_time = arrival_time;
        this->price = price;
        this->airline = airline;
        this->total_time = calculateTimeDifference(departure_time, arrival_time);
    }
};

class Node {
public:
    Flight* flight;
    Node* next;

    Node() : flight(nullptr), next(nullptr) {}
    Node(string source, string destination, string date, string departure_time, string arrival_time, double price, string airline) {
        flight = new Flight(source, destination, date, departure_time, arrival_time, price, airline);
        next = nullptr;
    }
};

class Queue {
public:
    Node* front;
    Node* rear;

    Queue() : front(nullptr), rear(nullptr) {}

    // Destructor to clean up memory
    ~Queue() {
        while (!isEmpty()) {
            Dequeue();
        }
    }

    // Check if the queue is empty
    bool isEmpty() {
        return front == nullptr;
    }

    // Add a flight to the queue
    void Enqueue(Node* node) {
        if (rear) {
            rear->next = node; // Link the current rear to the new node
        }
        else {
            front = node; // If the queue is empty, set the front to the new node
        }
        rear = node; // Update rear to the new node
    }

    // Remove a flight from the queue
    Node* Dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty. Cannot dequeue." << endl;
            return nullptr;
        }

        Node* temp = front;
        front = front->next; // Move the front pointer to the next node
        if (front == nullptr) {
            rear = nullptr; // If the queue is empty now, set rear to nullptr
        }
        temp->next = nullptr; // Disconnect the node from the queue
        return temp;
    }

    // Peek at the front of the queue
    Node* peek() {
        if (!isEmpty()) {
            return front;
        }
        return nullptr;
    }
};

class Adjlist {
public:
    Node* head;

    Adjlist() : head(nullptr) {}
    Node* getHead() {
        return head;
    }
    void add_flight(Flight* flight) {
        Node* new_node = new Node();
        new_node->flight = flight;
        new_node->next = head;
        head = new_node;
    }
    void Insert(string src, string des, string da, string d_t, string a_t, double price, string line)
    {
        Node* newNode = new Node(src, des, da, d_t, a_t, price, line);
        newNode->next = head;
        head = newNode;
    }
};

class FlightGraph {
    Adjlist* adjlist;
    string* cities;
    int city_count;
    sf::RenderWindow* window;
    sf::VideoMode videomode;
    sf::Event event;
    sf::Font font;
    sf::Text text;
    float scrollOffset;
    sf::Text menuText[7]; // For menu options
    bool isSearchingfl = false;
    const float scrollSpeed = 20.0f;

public:
    FlightGraph() : adjlist(NULL), cities(NULL), city_count(0), window(NULL), scrollOffset(0) {}

    FlightGraph(int max_cities) {
        videomode.height = 600;
        videomode.width = 800;
        window = new sf::RenderWindow(videomode, "FLIGHT NAVIGATOR AI", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);

        if (!font.loadFromFile("ARIAL.ttf")) {
            cerr << "Error loading font\n";
        }
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);

        for (int i = 0; i < 7; i++) {
            menuText[i].setFont(font);
            menuText[i].setCharacterSize(24);
            menuText[i].setFillColor(sf::Color::White);
        }

        city_count = 0;
        cities = new string[max_cities];
        adjlist = new Adjlist[max_cities];
        scrollOffset = 0;
    }

    const bool isWindowOpen() const {
        return window->isOpen();
    }

    ~FlightGraph() {
        delete[] adjlist;
        delete[] cities;
    }

    int findOrAddCity(const string& city) {
        for (int i = 0; i < city_count; ++i) {
            if (cities[i] == city) return i;
        }
        cities[city_count] = city;
        return city_count++;
    }

    void addFlight(const string& source, const string& destination, const string& date, const string& departure_time, const string& arrival_time, double price, const string& airline) {
        int source_index = findOrAddCity(source);
        Flight* flight = new Flight(source, destination, date, departure_time, arrival_time, price, airline);
        adjlist[source_index].add_flight(flight);
    }

    void pollEvents(bool& isSearching) {
        while (window->pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Escape) {
                    isSearching = true; // Enable search mode when Escape is pressed
                }
                if (event.key.code == sf::Keyboard::Up) {
                    scrollOffset += scrollSpeed;
                }
                if (event.key.code == sf::Keyboard::Down) {
                    scrollOffset -= scrollSpeed;
                }
                break;
            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.delta > 0) {
                    scrollOffset += scrollSpeed;
                }
                else {
                    scrollOffset -= scrollSpeed;
                }
                break;
            }
        }

        // Limit scrolling
        if (scrollOffset > 0) {
            scrollOffset = 0;
        }
        float maxOffset = -text.getGlobalBounds().height + window->getSize().y;
        if (scrollOffset < maxOffset) {
            scrollOffset = maxOffset;
        }
    }


    void displayFlights() {
        ostringstream oss;
        oss << "Flights Information:\n";
        oss << "Total cities: " << city_count << "\n";
        for (int i = 0; i < city_count; ++i) {
            oss << "\nCity: " << cities[i] << "\n";
            Node* temp = adjlist[i].head;
            while (temp) {
                oss << "  To: " << temp->flight->destination
                    << " Date: " << temp->flight->date
                    << " Airline: " << temp->flight->airline
                    << " Price: $" << temp->flight->price
                    << " Duration: " << temp->flight->total_time << " hrs\n";
                temp = temp->next;
            }
        }
        text.setString(oss.str());
        text.setPosition(10, scrollOffset); // Position text based on scrollOffset
        while (true) {
            if (window->pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    break;
                }
                if (event.type == sf::Event::Closed) {
                    window->close();
                    break;
                }
            }
            render();
        }
        // No infinite loop, handle events in the main loop
        window->clear(sf::Color::White); // Optional, depending on whether you want a white background

        // Draw text and other elements
        window->draw(text);
        window->display();  // Display updated content
    }

    void searchFlights(const string& source, const string& destination) {
        int sourceIndex = findOrAddCity(source);
        Node* temp = adjlist[sourceIndex].head;

        ostringstream oss;
        oss << "Search Results:\n";

        while (temp) {
            if (temp->flight->destination == destination) {
                oss << "Flight from " << source << " to " << destination << " on " << temp->flight->date << ":\n";
                oss << "  Airline: " << temp->flight->airline << "\n";
                oss << "  Departure: " << temp->flight->departure_time << "\n";
                oss << "  Arrival: " << temp->flight->arrival_time << "\n";
                oss << "  Price: $" << temp->flight->price << "\n";
                oss << "  Duration: " << temp->flight->total_time << " hrs\n\n";
            }
            temp = temp->next;
        }

        text.setString(oss.str());
        text.setPosition(10, scrollOffset); // Update position based on scrollOffset

        while (true) {
            if (window->pollEvent(event)) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    break;
                }
                if (event.type == sf::Event::Closed) {
                    window->close();
                    break;
                }
            }
            render();
        }

        isSearchingfl = false;
    }

    bool isSearchingfll() {
        return isSearchingfl;
    }

    void searchFlighthelper(string& source1, string& destination1) {
        sf::String source, destination;

        sf::Text promptText;
        promptText.setFont(font);
        promptText.setCharacterSize(20);
        promptText.setFillColor(sf::Color::Black);

        sf::Text inputText;
        inputText.setFont(font);
        inputText.setCharacterSize(20);
        inputText.setFillColor(sf::Color::Red);

        bool isEnteringSource = true; // Flag to track input for source or destination

        // Main input loop
        while (window->isOpen()) {
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window->close();
                    return;
                }

                // Handle text entered event
                if (event.type == sf::Event::TextEntered) {
                    // Handle backspace
                    if (event.text.unicode == 8) {
                        if (isEnteringSource && source.getSize() > 0) {
                            source.erase(source.getSize() - 1);
                        }
                        else if (!isEnteringSource && destination.getSize() > 0) {
                            destination.erase(destination.getSize() - 1);
                        }
                    }
                    // Handle regular character input
                    else if (event.text.unicode < 128) {
                        if (isEnteringSource) {
                            source += static_cast<char>(event.text.unicode);
                        }
                        else {
                            destination += static_cast<char>(event.text.unicode);
                        }
                    }
                }

                // Handle key press events
                if (event.type == sf::Event::KeyPressed) {
                    // If Enter is pressed, switch from source to destination input
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (isEnteringSource) {
                            isEnteringSource = false;  // Switch to destination input
                        }
                        else {
                            // After both inputs are done, finalize and store the input values
                            source1 = source.toAnsiString();
                            destination1 = destination.toAnsiString();

                            // Trim and normalize strings
                            source1.erase(source1.find_last_not_of(" \t\n\r") + 1);
                            source1.erase(0, source1.find_first_not_of(" \t\n\r"));
                            destination1.erase(destination1.find_last_not_of(" \t\n\r") + 1);
                            destination1.erase(0, destination1.find_first_not_of(" \t\n\r"));

                            isSearchingfl = true;  // Set the flag to start the search
                            return;  // Exit the function after processing input
                        }
                    }
                    // Handle Escape key to exit the input mode
                    else if (event.key.code == sf::Keyboard::Escape) {
                        return;  // Exit the search helper
                    }
                }
            }

            // Update the prompt and input text based on the current state (source or destination)
            if (isEnteringSource) {
                promptText.setString("Enter source city:");
                inputText.setString(source);
            }
            else {
                promptText.setString("Enter destination city:");
                inputText.setString(destination);
            }

            // Position the text elements
            promptText.setPosition(10, 10);  // Set position for the prompt
            inputText.setPosition(10, 40);   // Set position for input field (below the prompt)

            // Clear the window and redraw everything
            window->clear(sf::Color::White);

            // Draw prompt and input text
            window->draw(promptText);
            window->draw(inputText);

            // Display updated window
            window->display();
        }
    }


    string getFirstFlightDate(int sourceIndex) {
        if (adjlist[sourceIndex].head == nullptr) {
            return ""; // No flights from this city
        }

        string earliestDate = adjlist[sourceIndex].head->flight->date; // Assume the first date
        Node* temp = adjlist[sourceIndex].head;

        while (temp) {
            if (isDateBeforeOrEqual(temp->flight->date, earliestDate)) {
                earliestDate = temp->flight->date; // Update to the earliest date
            }
            temp = temp->next;
        }

        return earliestDate;
    }

    void renderMenu(int& one, int& two, int& three, int& four, int& five, int& six) {
        string menuOptions[] = {
            "1. Display all flights",
            "2. Search and book a direct or connecting flight",
            "3. Search flights with specific preferences",
            "4. Search multi-leg flights using a linked list",
            "5. Generate and display subgraph for an airline",
            "6. Layover Manager",
            "7. Exit"
        };

        // Don't clear the window here; it's done in the main loop

        for (int i = 0; i < 7; i++) {
            menuText[i].setString(menuOptions[i]);
            menuText[i].setPosition(50, 50 + i * 50);
            window->draw(menuText[i]);
        }

        window->display();

        bool waitingForInput = true;

        while (waitingForInput) {
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window->close();
                    return;
                }

                if (event.type == sf::Event::MouseButtonPressed) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
                    handleMenuClick(mousePos, one, two, three, four, five, six);
                    waitingForInput = false;
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        waitingForInput = false;
                    }
                }
            }
        }
    }

    void handleMenuClick(sf::Vector2i mousePos, int& one, int& two, int& three, int& four, int& five, int& six) {
        for (int i = 0; i < 7; i++) {
            sf::FloatRect bounds = menuText[i].getGlobalBounds();
            if (bounds.contains(mousePos.x, mousePos.y)) {
                executeMenuOption(i + 1, one, two, three, four, five, six);
                break;
            }
        }
    }

    void executeMenuOption(int option, int& one, int& two, int& three, int& four, int& five, int& six) {
        switch (option) {
        case 1:
            one = 1;
            break;
        case 2:
            two = 1;
            break;
        case 3:
            cout << "Search flights with specific preferences selected.\n";
            three = 1;
            break;
        case 4:
            cout << "Search multi-leg flights using a linked list selected.\n";
            four = 1;
            break;
        case 5:
            cout << "Generate and display subgraph for an airline selected.\n";
            five = 1;
            break;
        case 6:
            cout << "Layover Manager selected.\n";
            six = 1;
            break;
        case 7:
            window->close();
            break;
        default:
            cout << "Invalid option selected.\n";
        }
    }


    void dijkstraWithGraphics(int sourceIndex, int destIndex, bool usePrice, const std::string& currentDate) {
        double* dist = new double[city_count];
        bool* visited = new bool[city_count];
        int* prev = new int[11];
        std::string* flightDate = new std::string[city_count];
        std::string* airlineUsed = new std::string[city_count];
        PriorityQueue pq;

        sf::RenderWindow window(sf::VideoMode(1200, 800), "Flight Path Visualization");
        sf::Font font;
        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Error: Could not load font file.\n";
            return;
        }

        sf::Vector2f cityPositions[11] = {
            {100, 100}, {1100, 100}, {600, 150}, {200, 500}, {1000, 500},
            {150, 700}, {1050, 700}, {400, 400}, {800, 400}, {600, 600}, {600, 100}
        };

        for (int i = 0; i < city_count; i++) {
            dist[i] = (i == sourceIndex) ? 0.0 : (usePrice ? INT_MAX : DBL_MAX);
            visited[i] = false;
            prev[i] = -1;
            flightDate[i] = "";
            airlineUsed[i] = "";
        }

        pq.enqueue(sourceIndex, 0.0);

        sf::RectangleShape background(sf::Vector2f(1200, 800));
        background.setFillColor(sf::Color(40, 44, 52));

        while (!pq.isEmpty() && window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    return;
                }
            }

            int currentCity = pq.dequeue();
            visited[currentCity] = true;

            window.clear();
            window.draw(background);

            // Draw all connections in green
            for (int i = 0; i < city_count; i++) {
                Node* temp = adjlist[i].head;
                while (temp) {
                    int neighbor = findOrAddCity(temp->flight->destination);
                    sf::Vertex line[] = {
                        sf::Vertex(cityPositions[i], sf::Color::Green),
                        sf::Vertex(cityPositions[neighbor], sf::Color::Green)
                    };
                    window.draw(line, 2, sf::Lines);
                    temp = temp->next;
                }
            }

            // Highlight the shortest path in red
            int trace = destIndex;
            while (trace != -1 && prev[trace] != -1) {
                sf::Vertex line[] = {
                    sf::Vertex(cityPositions[trace], sf::Color::Red),
                    sf::Vertex(cityPositions[prev[trace]], sf::Color::Red)
                };
                window.draw(line, 2, sf::Lines);
                trace = prev[trace];
            }

            Node* temp = adjlist[currentCity].head;
            while (temp) {
                int neighbor = findOrAddCity(temp->flight->destination);

                if (!isDateBeforeOrEqual(currentDate, temp->flight->date)) {
                    temp = temp->next;
                    continue;
                }

                if (prev[currentCity] != -1 && !isDateBeforeOrEqual(flightDate[currentCity], temp->flight->date)) {
                    temp = temp->next;
                    continue;
                }

                double pitstop_time = ReturnTimeinMins(temp->flight->departure_time) - ReturnTimeinMins(flightDate[currentCity]);
                if (pitstop_time >= 0) {
                    if (!visited[neighbor]) {
                        double weight = usePrice ? temp->flight->price : temp->flight->total_time;
                        double newDist = dist[currentCity] + weight;
                        if (newDist < dist[neighbor]) {
                            dist[neighbor] = newDist;
                            prev[neighbor] = currentCity;
                            flightDate[neighbor] = temp->flight->date;
                            airlineUsed[neighbor] = temp->flight->airline;
                            pq.enqueue(neighbor, newDist);
                        }
                    }
                }
                temp = temp->next;
            }

            // Draw city squares
            for (int i = 0; i < city_count; i++) {
                sf::RectangleShape citySquare(sf::Vector2f(30, 30));
                citySquare.setPosition(cityPositions[i].x - 15, cityPositions[i].y - 15);

                if (i == currentCity) {
                    citySquare.setFillColor(sf::Color::Yellow);
                }
                else if (visited[i]) {
                    citySquare.setFillColor(sf::Color::Green);
                }
                else {
                    citySquare.setFillColor(sf::Color::Blue);
                }

                citySquare.setOutlineThickness(2);
                citySquare.setOutlineColor(sf::Color::White);
                window.draw(citySquare);

                sf::Text cityName;
                cityName.setFont(font);
                cityName.setString(cities[i]);
                cityName.setCharacterSize(18);
                cityName.setFillColor(sf::Color::White);
                cityName.setPosition(cityPositions[i].x + 20, cityPositions[i].y - 10);
                window.draw(cityName);
            }

            window.display();

            // Wait for user input to continue or exit
            bool proceed = false;
            while (!proceed) {
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Enter) {
                            proceed = true;
                        }
                        else if (event.key.code == sf::Keyboard::Escape) {
                            delete[] dist;
                            delete[] visited;
                            delete[] prev;
                            delete[] flightDate;
                            delete[] airlineUsed;
                            return;
                        }
                    }
                }
            }
        }

        delete[] dist;
        delete[] visited;
        delete[] prev;
        delete[] flightDate;
        delete[] airlineUsed;
    }




    void dijkstra(int sourceIndex, int destIndex, bool usePrice, const string currentDate) {
        double* dist = new double[city_count];
        bool* visited = new bool[city_count];
        int* prev = new int[city_count];
        string* flightDate = new string[city_count]; // Track flight dates
        string* airlineUsed = new string[city_count]; // Track airlines
        PriorityQueue pq;

        // Initialize distances, visited cities, previous cities, flight dates, and airlines
        for (int i = 0; i < city_count; i++) {
            dist[i] = (i == sourceIndex) ? 0.0 : (usePrice ? INT_MAX : DBL_MAX);
            visited[i] = false;
            prev[i] = -1;
            flightDate[i] = ""; // Initialize flight dates to empty
            airlineUsed[i] = ""; // Initialize airlines to empty
        }

        pq.enqueue(sourceIndex, 0.0); // Start with the source city
        while (!pq.isEmpty()) {
            int currentCity = pq.dequeue(); // Get the current city
            visited[currentCity] = true;
            cout << "Visiting " << cities[currentCity] << endl;

            Node* temp = adjlist[currentCity].head;
            while (temp) {
                int neighbor = findOrAddCity(temp->flight->destination);

                // Check if the flight date is valid (must be after or equal to the current date)
                if (!isDateBeforeOrEqual(currentDate, temp->flight->date)) {
                    temp = temp->next;
                    continue; // Skip flights before the start date
                }

                // Ensure the flight date is greater than or equal to the previous city's flight date
                if (prev[currentCity] != -1 && !isDateBeforeOrEqual(flightDate[currentCity], temp->flight->date)) {
                    temp = temp->next;
                    continue; // Skip flights if the date is earlier than the previous city's flight date
                }

                // Pitstop time check: ensure enough time between flights (layover)
                double pitstop_time = ReturnTimeinMins(temp->flight->departure_time) - ReturnTimeinMins(flightDate[currentCity]);
                if (pitstop_time >= 0) {

                    // If the flight satisfies the conditions
                    if (!visited[neighbor]) {
                        double weight = usePrice ? temp->flight->price : temp->flight->total_time;
                        double newDist = dist[currentCity] + weight;
                        if (newDist < dist[neighbor]) {
                            dist[neighbor] = newDist;
                            prev[neighbor] = currentCity;
                            flightDate[neighbor] = temp->flight->date; // Save the date of this flight
                            airlineUsed[neighbor] = temp->flight->airline; // Save the airline of this flight
                            pq.enqueue(neighbor, newDist);
                            cout << "Flight to " << cities[neighbor] << " on " << temp->flight->date
                                << " with " << temp->flight->airline << endl;
                        }
                    }
                }
                temp = temp->next;
            }
        }

        // Print the shortest path
        if (dist[destIndex] == (usePrice ? INT_MAX : DBL_MAX)) {
            cout << "No path found!" << endl;
        }
        else {
            cout << "Shortest path from " << cities[sourceIndex] << " to " << cities[destIndex] << ": " << endl;
            int current = destIndex;
            while (current != -1) {
                if (prev[current] != -1) {
                    cout << cities[current] << " (Flight date: " << flightDate[current]
                        << ", Airline: " << airlineUsed[current] << ") <- ";
                }
                else {
                    cout << cities[current] << " <- ";
                }
                current = prev[current];
            }
            cout << "End" << endl;
            cout << "Total " << (usePrice ? "Price" : "Time") << ": " << dist[destIndex] << endl;
        }

        delete[] dist;
        delete[] visited;
        delete[] prev;
        delete[] flightDate;
        delete[] airlineUsed;
    }


    void SearchAndBookFlights(const string& source, const string& destination, const string& travel_date)
    {
        int source_index = findOrAddCity(source);
        int destination_index = findOrAddCity(destination);

        if (source_index == -1 || destination_index == -1) {
            cout << "Invalid City ---> " << source << " OR " << destination << endl;
            return;
        }

        // Check for direct flights
        Node* temp = adjlist[source_index].head;
        bool direct_found = false;
        while (temp)
        {
            if (temp->flight->destination == destination && temp->flight->date == travel_date)
            {
                cout << "Direct Flight Found:\n";
                cout << "Flight from " << source << " to " << destination << " on " << temp->flight->date << ":\n";
                cout << "  Airline: " << temp->flight->airline << "\n";
                cout << "  Date: " << temp->flight->date << "\n";
                cout << "  Departure: " << temp->flight->departure_time << "\n";
                cout << "  Arrival: " << temp->flight->arrival_time << "\n";
                cout << "  Price: " << temp->flight->price << "\n";
                cout << "  Duration: " << temp->flight->total_time << " hrs\n";
                direct_found = true;
            }
            temp = temp->next;
            cout << endl;
        }

        if (direct_found)
        {
            cout << "\nFlight is booked.\n";
            return; // Stop if a direct flight is found
        }

        // Check for connecting flights
        cout << "No direct flight found. Searching for connecting flights...\n\n";
        temp = adjlist[source_index].head;
        bool connectionFound = false;

        while (temp)
        {
            int PitStopIndex = findOrAddCity(temp->flight->destination);
            if (PitStopIndex != -1)
            {
                Node* leg2_flight = adjlist[PitStopIndex].head;
                while (leg2_flight)
                {
                    // Ensure the connecting flight is on the same date and after arrival time
                    double pitstop_time = ReturnTimeinMins(leg2_flight->flight->departure_time) - ReturnTimeinMins(temp->flight->arrival_time);
                    if (leg2_flight->flight->destination == destination && leg2_flight->flight->date == travel_date && temp->flight->date == travel_date && pitstop_time >= 0)
                    {
                        cout << "Connecting Flight Found:\n";
                        cout << "First Leg: " << source << " -> " << temp->flight->destination << "\n";
                        cout << "  Date: " << temp->flight->date << "\n";
                        cout << "  Departure: " << temp->flight->departure_time
                            << ", Arrival: " << temp->flight->arrival_time
                            << ", Price: " << temp->flight->price
                            << ", Airline: " << temp->flight->airline << "\n";

                        cout << "Second Leg: " << temp->flight->destination << " -> " << destination << "\n";
                        cout << "  Date: " << leg2_flight->flight->date << "\n";
                        cout << "  Departure: " << leg2_flight->flight->departure_time
                            << ", Arrival: " << leg2_flight->flight->arrival_time
                            << ", Price: " << leg2_flight->flight->price
                            << ", Airline: " << leg2_flight->flight->airline << "\n";
                        connectionFound = true;
                        cout << endl;
                    }
                    leg2_flight = leg2_flight->next;
                }
            }
            temp = temp->next;
        }

        if (!connectionFound)
        {
            cout << "No connecting flights found.\n";
            return;
        }
        cout << "\nFlight is booked.\n";
    }

    void SearchFlightsWithPreferences(const string& source, const string& destination, const string& travel_date, const string& preferred_airline = "", const string& transit_city = "")
    {
        int source_index = findOrAddCity(source);
        int destination_index = findOrAddCity(destination);

        if (source_index == -1 || destination_index == -1) {
            cout << "Invalid City ---> " << source << " OR " << destination << endl;
            return;
        }

        bool flightFound = false;

        // Check for direct flights with airline preference
        Node* temp = adjlist[source_index].head;
        while (temp)
        {
            if (temp->flight->destination == destination && temp->flight->date == travel_date && (preferred_airline.empty() || temp->flight->airline == preferred_airline))
            {
                cout << "Direct Flight Found:\n";
                cout << "Flight from " << source << " to " << destination << " on " << temp->flight->date << ":\n";
                cout << "  Airline: " << temp->flight->airline << "\n";
                cout << "  Departure: " << temp->flight->departure_time << "\n";
                cout << "  Arrival: " << temp->flight->arrival_time << "\n";
                cout << "  Price: " << temp->flight->price << "\n";
                cout << "  Duration: " << temp->flight->total_time << " hrs\n";
                flightFound = true;
            }
            temp = temp->next;
        }

        if (flightFound) return; // Stop if a direct flight is found.

        // Check for connecting flights with preferences
        cout << "No direct flight found. Searching for connecting flights...\n\n";
        temp = adjlist[source_index].head;
        while (temp) {
            if (!transit_city.empty() && temp->flight->destination != transit_city)
            {
                temp = temp->next;
                continue; // Skip flights not matching the transit city preference
            }

            int pitstop_index = findOrAddCity(temp->flight->destination);
            if (pitstop_index != -1)
            {
                Node* leg2_flight = adjlist[pitstop_index].head;
                while (leg2_flight) {
                    if (leg2_flight->flight->destination == destination && leg2_flight->flight->date == travel_date && (preferred_airline.empty() || leg2_flight->flight->airline == preferred_airline) && ReturnTimeinMins(leg2_flight->flight->departure_time) > ReturnTimeinMins(temp->flight->arrival_time))
                    {
                        cout << "Connecting Flight Found:\n";
                        cout << "First Leg: " << source << " -> " << temp->flight->destination << "\n";
                        cout << "  Departure: " << temp->flight->departure_time
                            << ", Arrival: " << temp->flight->arrival_time
                            << ", Price: " << temp->flight->price
                            << ", Airline: " << temp->flight->airline << "\n";

                        cout << "Second Leg: " << temp->flight->destination << " -> " << destination << "\n";
                        cout << "  Departure: " << leg2_flight->flight->departure_time
                            << ", Arrival: " << leg2_flight->flight->arrival_time
                            << ", Price: " << leg2_flight->flight->price
                            << ", Airline: " << leg2_flight->flight->airline << "\n";
                        flightFound = true;
                    }
                    leg2_flight = leg2_flight->next;
                }
            }
            temp = temp->next;
        }

        if (!flightFound)
        {
            cout << "No flights matching preferences found.\n";
        }
    }

    void SearchFlightsWithLayovers(const string& source, const string& destination, const string& travel_date)
    {
        int source_index = findOrAddCity(source);
        int destination_index = findOrAddCity(destination);

        if (source_index == -1 || destination_index == -1) {
            cout << "Invalid City ---> " << source << " OR " << destination << endl;
            return;
        }

        Queue layoverQueue; // Queue for layover management
        bool found = false;

        // Enqueue all flights from the source city that are not direct to the destination
        Node* temp = adjlist[source_index].head;
        while (temp) {
            if (temp->flight->date == travel_date && temp->flight->destination != destination) {
                Node* layoverNode = new Node(temp->flight->source, temp->flight->destination, temp->flight->date, temp->flight->departure_time, temp->flight->arrival_time, temp->flight->price, temp->flight->airline);
                layoverQueue.Enqueue(layoverNode);
            }
            temp = temp->next;
        }

        // Process flights in the queue
        while (!layoverQueue.isEmpty()) {
            Node* layoverFlight = layoverQueue.Dequeue(); // Get a flight from the queue
            int layoverCityIndex = findOrAddCity(layoverFlight->flight->destination);


            Node* connection = adjlist[layoverCityIndex].head; // Flights from layover city to the final destination
            while (connection) {
                if (connection->flight->destination == destination && connection->flight->date == travel_date) {

                    int prevArrival = ReturnTimeinMins(layoverFlight->flight->arrival_time);
                    int nextDeparture = ReturnTimeinMins(connection->flight->departure_time);

                    if (nextDeparture > prevArrival) {

                        int layoverTime = (nextDeparture - prevArrival) / 60; // Layover time in hours
                        double totalCost = layoverFlight->flight->price + connection->flight->price;

                        // Print flight details
                        cout << "Connecting Flight Found:\n";
                        cout << "  First Leg: " << source << " -> " << layoverFlight->flight->destination << endl;
                        cout << "    Departure: " << layoverFlight->flight->departure_time << ", Arrival: " << layoverFlight->flight->arrival_time
                            << ", Price: " << layoverFlight->flight->price << ", Airline: " << layoverFlight->flight->airline << endl;
                        cout << "  Second Leg: " << layoverFlight->flight->destination << " -> " << destination << endl;
                        cout << "    Departure: " << connection->flight->departure_time << ", Arrival: " << connection->flight->arrival_time
                            << ", Price: " << connection->flight->price << ", Airline: " << connection->flight->airline << endl;
                        cout << "  Total Layover: " << layoverTime << " hours\n";
                        cout << "  Total Cost: " << totalCost << "\n\n";

                        found = true;
                    }
                }
                connection = connection->next;
            }

            // Clean up memory for the processed layover flight
            delete layoverFlight;
        }

        if (!found) {
            cout << "No connecting flights found." << endl;
        }
    }

    void SearchFlightsWithLinkedList(const string& origin, const string& travel_date) {
        int origin_index = findOrAddCity(origin);
        if (origin_index == -1) {
            cout << "City not found: " << origin << endl;
            return;
        }

        LinkedList route; // Linked list to store the journey

        string currentCity = origin;
        bool journeyActive = true;

        while (journeyActive) {
            int currentCityIndex = findOrAddCity(currentCity);
            if (currentCityIndex == -1) {
                cout << "City not found: " << currentCity << endl;
                break;
            }

            cout << "\nAvailable flights from " << currentCity << " on " << travel_date << ":\n";
            Node* temp = adjlist[currentCityIndex].head;
            bool flightsAvailable = false;

            // Display all flights from the current city
            while (temp) {
                if (temp->flight->date == travel_date) {
                    cout << "Destination: " << temp->flight->destination
                        << " | Departure: " << temp->flight->departure_time
                        << " | Arrival: " << temp->flight->arrival_time
                        << " | Price: " << temp->flight->price
                        << " | Airline: " << temp->flight->airline << "\n";
                    flightsAvailable = true;
                }
                temp = temp->next;
            }

            if (!flightsAvailable) {
                cout << "No flights available from " << currentCity << " on " << travel_date << ".\n";
                break;
            }

            // Ask user to select the next destination or finish the journey
            string nextDestination;
            cout << "\nEnter your next destination (or type 'end' to finish the journey): ";
            cin >> nextDestination;

            if (nextDestination == "end") {
                journeyActive = false;
            }
            else {
                // Find the selected flight and add it to the route
                temp = adjlist[currentCityIndex].head;
                bool flightFound = false;

                while (temp) {
                    if (temp->flight->destination == nextDestination && temp->flight->date == travel_date) {
                        route.AddLink(currentCity, temp->flight->destination, temp->flight->departure_time, temp->flight->arrival_time, temp->flight->price, temp->flight->airline);
                        currentCity = nextDestination; // Update current city to the selected destination
                        flightFound = true;
                        break;
                    }
                    temp = temp->next;
                }

                if (!flightFound) {
                    cout << "No valid flight found to " << nextDestination << ".\n";
                    journeyActive = false;
                }
            }
        }

        // Display the completed journey
        cout << "\nYour completed journey:\n";
        route.DisplayPath();
    }

    // Getters for cities and adjlist
    string* getCities() const {
        return cities;
    }

    Adjlist* getAdjList() const {
        return adjlist;
    }

    int getCityCount() const {
        return city_count;
    }

    void AddEdge(string src, string dest, string da, string d_t, string a_t, double price, string line)
    {
        int src_index = findOrAddCity(src);

        if (src_index == -1) // Ensure source city exists
        {
            cout << "\nCity does not exist/Spelling invalid: " << src << "\n";
            return;
        }

        // Insert the flight details into the adjacency list for the source city
        adjlist[src_index].Insert(src, dest, da, d_t, a_t, price, line);
    }

    void render() {
        window->clear(sf::Color::White);
        window->draw(text);
        window->display();
    }
};

FlightGraph SubGraphGeneration(FlightGraph& og_graph, const string& limiting_airline)
{
    FlightGraph subGraph; // Ensure a default constructor is available for FlightGraph

    for (int i = 0; i < og_graph.getCityCount(); ++i) // Use a method to get the number of cities
    {
        Node* current = og_graph.getAdjList()[i].getHead(); // Access the adjacency list for the city

        subGraph.findOrAddCity(og_graph.getCities()[i]); // Use the city name from the source graph

        while (current)
        {
            if (current->flight->airline == limiting_airline)
            {
                subGraph.findOrAddCity(current->flight->destination);
                subGraph.AddEdge(
                    og_graph.getCities()[i],
                    current->flight->destination,
                    current->flight->date,
                    current->flight->departure_time,
                    current->flight->arrival_time,
                    current->flight->price,
                    current->flight->airline
                );
            }
            current = current->next;
        }
    }

    return subGraph;
}


int no_of_lines_in_file() {
    ifstream flights_file("Flights.txt");
    if (!flights_file) {
        cout << "Error: File not found\n";
        return -1;
    }
    int count = 0;
    string line;
    while (getline(flights_file, line)) {
        count++;
    }

    flights_file.close();
    return count;
}

void read_from_flights_file(FlightGraph* flight_graph) {
    ifstream flights_file("Flights.txt");
    if (!flights_file) {
        cout << "Error: File not found\n";
        return;
    }

    string source, destination, date, departure_time, arrival_time, airline;
    double price;

    while (flights_file >> source >> destination >> date >> departure_time >> arrival_time >> price >> airline) {
        flight_graph->addFlight(source, destination, date, departure_time, arrival_time, price, airline);
    }

    flights_file.close();
}

// Helper function to safely take integer input
int getIntInput() {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid number: ";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// Helper function to safely take boolean input (1/0)
bool getBoolInput() {
    int value;
    while (true) {
        cin >> value;
        if (cin.fail() || (value != 0 && value != 1)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1 (Yes) or 0 (No): ";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return static_cast<bool>(value);
        }
    }
}

int main() {
    int max_cities = no_of_lines_in_file();
    FlightGraph flight_graph(max_cities);
    read_from_flights_file(&flight_graph);

    int choice;
    cout << "Enter 1 for graphics mode, anything else for console mode: ";
    choice = getIntInput();

    if (choice == 1) {

        int one = 0;
        int two = 0;
        int three = 0;
        int four = 0;
        int five = 0;
        int six = 0;
        bool isSearching = false; // Tracks whether we're in search mode
        string source, destination;

        while (flight_graph.isWindowOpen()) {
            flight_graph.pollEvents(isSearching);

            string source1, destination1;
            cout << "Enter source city: ";
            cin >> source1;
            cout << "Enter destination city: ";
            cin >> destination1;

            int sourceIndex = flight_graph.findOrAddCity(source1);
            int destIndex = flight_graph.findOrAddCity(destination1);

            cout << "Use price (1) or time (0) for Dijkstra's algorithm? ";
            bool usePrice = getBoolInput();

            string earliestDate = flight_graph.getFirstFlightDate(sourceIndex);
            flight_graph.dijkstraWithGraphics(sourceIndex, destIndex, usePrice, earliestDate);

            if (two == 1) {
                flight_graph.searchFlighthelper(source, destination);
                if (flight_graph.isSearchingfll()) {
                    flight_graph.searchFlights(source, destination);
                }
                two = 0;
            }

            /*
            if (isSearching) {

                string source1, destination1;
                cout << "Enter source city: ";
                cin >> source1;
                cout << "Enter destination city: ";
                cin >> destination1;

                int sourceIndex = flight_graph.findOrAddCity(source1);
                int destIndex = flight_graph.findOrAddCity(destination1);

                bool usePrice;
                // Ask user whether to use price or time for Dijkstra's algorithm
                cout << "Use price (1) or time (0) for Dijkstra's algorithm? ";
                cin >> usePrice;

                string earliestDate = flight_graph.getFirstFlightDate(sourceIndex);

                flight_graph.dijkstra(sourceIndex, destIndex, usePrice, earliestDate);

                flight_graph.searchFlighthelper(source, destination);
                if (flight_graph.isSearchingfll()) {
                    flight_graph.searchFlights(source, destination);
                }
                isSearching = false; // Exit search mode after searching
            }
            */

            if (one == 1) {
                flight_graph.displayFlights(); // Show all flights
                one = 0;
            }

            flight_graph.renderMenu(one, two, three, four, five, six);
            flight_graph.render();
        }

    }
    else {

        bool programActive = true;

        while (programActive) {
            cout << "\nFlight Management System\n";
            cout << "=========================\n";
            cout << "1. Display all flights\n";
            cout << "2. Search and book a direct or connecting flight\n";
            cout << "3. Search flights with specific preferences\n";
            cout << "4. Search multi-leg flights using a linked list\n";
            cout << "5. Generate and display subgraph for an airline\n";
            cout << "6. Layover Manager\n";
            cout << "7. Shortest Path Finder.\n";
            cout << "8. Exit\n";
            cout << "Enter your choice: ";

            int choice = getIntInput();

            switch (choice) {
            case 1: {
                cout << "\nDisplaying all flights:\n";
                flight_graph.displayFlights();
                break;
            }
            case 2: {
                string source, destination, date;
                cout << "\nEnter the city you want to go from: ";
                cin >> source;
                cout << "\nEnter the city you want to go to: ";
                cin >> destination;
                cout << "\nEnter the date you want to travel (dd/mm/yyyy): ";
                cin >> date;
                flight_graph.SearchAndBookFlights(source, destination, date);
                break;
            }
            case 3: {
                string source, destination, date, airline, transitCity;
                cout << "\nEnter the city you want to go from: ";
                cin >> source;
                cout << "\nEnter the city you want to go to: ";
                cin >> destination;
                cout << "\nEnter the date you want to travel (dd/mm/yyyy): ";
                cin >> date;
                cout << "\nDo you have a preferred airline? (leave empty if none): ";
                cin.ignore();
                getline(cin, airline);
                cout << "\nDo you have a preferred transit city? (leave empty if none): ";
                getline(cin, transitCity);
                flight_graph.SearchFlightsWithPreferences(source, destination, date, airline, transitCity);
                break;
            }
            case 4: {
                string origin, date;
                cout << "\nEnter the city you want to start from: ";
                cin >> origin;
                cout << "\nEnter the date you want to travel (dd/mm/yyyy): ";
                cin >> date;
                flight_graph.SearchFlightsWithLinkedList(origin, date);
                break;
            }
            case 5: {
                string airline;
                cout << "\nEnter the airline to generate a subgraph: ";
                cin >> airline;
                FlightGraph subGraph = SubGraphGeneration(flight_graph, airline);
                cout << "\nDisplaying the subgraph for airline: " << airline << endl;
                subGraph.displayFlights();
                break;
            }
            case 6: {
                string source, destination, date;
                cout << "\nEnter the city you want to go from: ";
                cin >> source;
                cout << "\nEnter the city you want to go to: ";
                cin >> destination;
                cout << "\nEnter the date you want to travel (dd/mm/yyyy): ";
                cin >> date;
                flight_graph.SearchFlightsWithLayovers(source, destination, date);
                break;
            }
            case 7: {
                string source, destination, date;
                char metric;
                cout << "\nEnter the city you want to start from: ";
                cin >> source;
                cout << "\nEnter the city you want to go to: ";
                cin >> destination;
                cout << "\nEnter the date you want to travel (dd/mm/yyyy): ";
                cin >> date;
                cout << "\nMinimize by (p)rice or (t)ime? ";
                cin >> metric;

                int sourceIndex = flight_graph.findOrAddCity(source);
                int destIndex = flight_graph.findOrAddCity(destination);

                if (sourceIndex == -1 || destIndex == -1) {
                    cout << "Invalid city name(s).\n";
                    break;
                }

                bool minimizeByPrice = (metric == 'p' || metric == 'P');
                flight_graph.dijkstra(sourceIndex, destIndex, minimizeByPrice, date);
                break;
            }
            case 8: {
                cout << "\nExiting the system. Goodbye!\n";
                programActive = false;
                break;
            }
            default:
                cout << "\nInvalid choice. Please try again.\n";
            }
        }
    }

    return 0;
}
