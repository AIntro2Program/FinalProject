#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Struct to store flight information
struct Flight {
    int flightID;
    string destination;
    string date;
    double price;
    int availableSeats;
};

// Struct to store booking information
struct Booking {
    string name;
    string passportNumber;
    Flight flight;
};

// Hardcoded flights
Flight flights[8] = {
    {1, "Moscow", "2025-04-20", 250.0, 10},
    {2, "Cairo", "2025-04-21", 200.0, 10},
    {3, "Beijing", "2025-04-22", 320.0, 10},
    {4, "Istanbul", "2025-04-23", 400.0, 10},
    {5, "Seoul", "2025-04-24", 380.0, 10},
    {6, "Dubai", "2025-04-23", 450.0, 10},
    {7, "New York", "2025-04-24", 680.0, 10},
    {8, "Medina", "2025-04-25", 420.0, 10}
};

// Function to display flight information
void displayFlight(const Flight& flight) {
    cout << "----------------------------------------" << endl;
    cout << "ID: " << flight.flightID << endl;
    cout << "Destination: " << flight.destination << endl;
    cout << "Date: " << flight.date << endl;
    cout << "Price: $" << flight.price << endl;
    cout << "Available Seats: " << flight.availableSeats << endl;
}

// Function to search flights by destination
void searchDestination(Flight flights[], int size, string destination) {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (flights[i].destination == destination) {
            displayFlight(flights[i]);
            found = true;
        }
    }
    if (!found) {
        cout << "No flights found to " << destination << "." << endl;
    }
}

// Function to search flights by date
void searchDate(Flight flights[], int size, string date) {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (flights[i].date == date) {
            displayFlight(flights[i]);
            found = true;
        }
    }
    if (!found) {
        cout << "No flights found on " << date << "." << endl;
    }
}

// Function to search flights by price range
void searchPrice(Flight flights[], int size, float minPrice, float maxPrice) {
    bool found = false;
    for (int i = 0; i < size; i++) {
        if (flights[i].price >= minPrice && flights[i].price <= maxPrice) {
            displayFlight(flights[i]);
            found = true;
        }
    }
    if (!found) {
        cout << "No flights found in the price range $" << minPrice << " - $" << maxPrice << "." << endl;
    }
}

// Function to book a flight
void bookFlight() {
    int choice;
    cout << "\n===== Flight Search Menu =====" << endl;
    cout << "1. Search by Destination" << endl;
    cout << "2. Search by Date" << endl;
    cout << "3. Search by Price Range" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice (1-4): ";
    cin >> choice;

    if (choice == 1) {
        string dest;
        cout << "Enter destination: ";
        cin >> dest;
        searchDestination(flights, 8, dest);
    } else if (choice == 2) {
        string date;
        cout << "Enter date (YYYY-MM-DD): ";
        cin >> date;
        searchDate(flights, 8, date);
    } else if (choice == 3) {
        float minP, maxP;
        cout << "Enter min price: ";
        cin >> minP;
        cout << "Enter max price: ";
        cin >> maxP;
        searchPrice(flights, 8, minP, maxP);
    } else if (choice == 4) {
        cout << "Goodbye!" << endl;
        return;
    } else {
        cout << "Invalid choice. Try again." << endl;
        return;
    }

    // Allow user to book a flight from the search results
    cout << "\nWould you like to book a flight? (1 for Yes, 0 for No): ";
    int bookChoice;
    cin >> bookChoice;

    if (bookChoice == 1) {
        int flightID;
        cout << "Enter flight ID to book: ";
        cin >> flightID;

        bool valid = false;
        for (int i = 0; i < 8; ++i) {
            if (flights[i].flightID == flightID) {
                valid = true;
                if (flights[i].availableSeats > 0) {
                    Booking booking;
                    booking.flight = flights[i];

                    cout << "Enter your full name: ";
                    cin.ignore();  // To clear newline from input buffer
                    getline(cin, booking.name);

                    cout << "Enter passport number: ";
                    getline(cin, booking.passportNumber);

                    // Reduce seat count
                    flights[i].availableSeats--;

                    // Save to file
                    ofstream outFile("bookings_member3.txt", ios::app);
                    outFile << booking.name << "," << booking.passportNumber << ","
                            << booking.flight.destination << "," << booking.flight.date << ","
                            << booking.flight.price << "\n";
                    outFile.close();

                    cout << "\nBooking Confirmed for " << booking.name << "!\n";
                    cout << "Flight to " << booking.flight.destination
                         << " on " << booking.flight.date << ", $" << booking.flight.price << "\n";
                } else {
                    cout << "Sorry, no seats available on this flight.\n";
                }
                break;
            }
        }

        if (!valid) {
            cout << "Invalid flight ID.\n";
        }
    }
}

// Function to display all bookings
void displayBookings() {
    ifstream inFile("bookings_member3.txt");
    if (!inFile) {
        cout << "No bookings found." << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}

// Function to confirm a booking by name or ID
void confirmBooking() {
    string searchTerm;
    cout << "Enter booking name or flight ID to confirm: ";
    cin.ignore();  // To clear newline from input buffer
    getline(cin, searchTerm);

    ifstream inFile("bookings_member3.txt");
    ofstream tempFile("temp.txt");
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.find(searchTerm) != string::npos) {
            cout << "Booking Confirmed: " << line << endl;
            found = true;
        } else {
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();

    if (found) {
        remove("bookings_member3.txt");
        rename("temp.txt", "bookings_member3.txt");
    } else {
        cout << "No booking found with that name or ID." << endl;
    }
}

// Function to cancel a booking by name or ID
void cancelBooking() {
    string searchTerm;
    cout << "Enter booking name or flight ID to cancel: ";
    cin.ignore();  // To clear newline from input buffer
    getline(cin, searchTerm);

    ifstream inFile("bookings_member3.txt");
    ofstream tempFile("temp.txt");
    string line;
    bool found = false;

    // Read through the file and copy lines to the temporary file, except the ones to be canceled
    while (getline(inFile, line)) {
        // Check if the line exactly contains the search term (name or ID)
        // We assume each line in the booking file is structured as:
        // name, passportNumber, destination, date, price
        if (line.find(searchTerm) != string::npos) {
            // This line matches the search term, so we don't write it to the temp file
            cout << "Booking Cancelled: " << line << endl;
            found = true;
        } else {
            // This line does not match, so we keep it
            tempFile << line << endl;
        }
    }

    inFile.close();
    tempFile.close();

    if (found) {
        // Replace the original file with the updated file (without the canceled booking)
        remove("bookings_member3.txt");
        rename("temp.txt", "bookings_member3.txt");
    } else {
        cout << "No booking found with that name or ID." << endl;
    }
}
// Main function
int main() {
    while (true) {
        int choice;
        cout << "\n===== Flight Search Menu =====" << endl;
        cout << "1. Search for Flights" << endl;
        cout << "2. Booking Management" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice (1-3): ";
        cin >> choice;

        if (choice == 1) {
            bookFlight();
        } else if (choice == 2) {
            int bookingChoice;
            while (true) {
                cout << "\n===== Booking Management Menu =====" << endl;
                cout << "1. View All Bookings" << endl;
                cout << "2. Confirm a Booking" << endl;
                cout << "3. Cancel a Booking" << endl;
                cout << "4. Exit to Main Menu" << endl;
                cout << "Enter your choice (1-4): ";
                cin >> bookingChoice;

                if (bookingChoice == 1) {
                    displayBookings();
                } else if (bookingChoice == 2) {
                    confirmBooking();
                } else if (bookingChoice == 3) {
                    cancelBooking();
                } else if (bookingChoice == 4) {
                    break;
                } else {
                    cout << "Invalid choice. Try again." << endl;
                }
            }
        } else if (choice == 3) {
            cout << "Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid choice. Try again." << endl;
        }
    }
    return 0;
}
