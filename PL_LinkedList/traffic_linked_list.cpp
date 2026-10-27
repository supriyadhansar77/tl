#include <iostream>
#include <string>
using namespace std;

struct Vehicle {
    int id;
    string type;
    bool emergency;
    int waitTime;
    Vehicle* next;

    Vehicle(int i, string t, bool e, int w)
        : id(i), type(t), emergency(e), waitTime(w), next(nullptr) {}
};

class TrafficLane {
private:
    Vehicle* head;

public:
    TrafficLane() : head(nullptr) {}

    void insertVehicle(int id, string type, bool emergency, int waitTime) {
        Vehicle* newNode = new Vehicle(id, type, emergency, waitTime);

        if (head == nullptr) {
            head = newNode;
        } else {
            Vehicle* temp = head;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = newNode;
        }

        cout << "Vehicle inserted successfully.\n";
    }

    void displayVehicles() {
        if (head == nullptr) {
            cout << "Lane is empty.\n";
            return;
        }

        Vehicle* temp = head;
        cout << "\nID\tType\t\tEmergency\tWait Time\n";
        cout << "---------------------------------------------\n";

        while (temp != nullptr) {
            cout << temp->id << "\t"
                 << temp->type << "\t\t"
                 << (temp->emergency ? "YES" : "NO") << "\t\t"
                 << temp->waitTime << " min\n";
            temp = temp->next;
        }
    }

    void searchVehicle(int id) {
        Vehicle* temp = head;

        while (temp != nullptr) {
            if (temp->id == id) {
                cout << "Vehicle Found: "
                     << temp->id << " | "
                     << temp->type << " | Wait = "
                     << temp->waitTime << " min\n";
                return;
            }
            temp = temp->next;
        }

        cout << "Vehicle not found.\n";
    }

    void updateVehicle(int id, int newWaitTime) {
        Vehicle* temp = head;

        while (temp != nullptr) {
            if (temp->id == id) {
                temp->waitTime = newWaitTime;
                cout << "Vehicle updated successfully.\n";
                return;
            }
            temp = temp->next;
        }

        cout << "Vehicle not found.\n";
    }

    void deleteVehicle(int id) {
        if (head == nullptr) {
            cout << "Lane is empty.\n";
            return;
        }

        if (head->id == id) {
            Vehicle* del = head;
            head = head->next;
            delete del;
            cout << "Vehicle deleted successfully.\n";
            return;
        }

        Vehicle* temp = head;

        while (temp->next != nullptr) {
            if (temp->next->id == id) {
                Vehicle* del = temp->next;
                temp->next = temp->next->next;
                delete del;
                cout << "Vehicle deleted successfully.\n";
                return;
            }
            temp = temp->next;
        }

        cout << "Vehicle not found.\n";
    }

    void findEmergencyVehicle() {
        Vehicle* temp = head;

        while (temp != nullptr) {
            if (temp->emergency) {
                cout << "\nEmergency Vehicle Detected!\n";
                cout << "ID: " << temp->id << "\n";
                cout << "Type: " << temp->type << "\n";
                cout << "Wait Time: " << temp->waitTime << " min\n";
                return;
            }
            temp = temp->next;
        }

        cout << "No emergency vehicle found.\n";
    }

    ~TrafficLane() {
        while (head != nullptr) {
            Vehicle* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    TrafficLane lane;

    lane.insertVehicle(101, "Car", false, 10);
    lane.insertVehicle(102, "Bike", false, 15);
    lane.insertVehicle(103, "Ambulance", true, 20);

    lane.displayVehicles();

    cout << "\nSearching Vehicle 103:\n";
    lane.searchVehicle(103);

    cout << "\nUpdating Vehicle 102:\n";
    lane.updateVehicle(102, 5);

    cout << "\nEmergency Check:\n";
    lane.findEmergencyVehicle();

    cout << "\nDeleting Vehicle 101:\n";
    lane.deleteVehicle(101);

    lane.displayVehicles();

    return 0;
}
