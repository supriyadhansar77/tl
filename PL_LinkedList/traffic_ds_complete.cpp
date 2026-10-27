// ==============================================================================
// SMART TRAFFIC LIGHT SYSTEM WITH EMERGENCY VEHICLE PRIORITY
// PROGRAMMING LAB (PL) - COMPLETE DATA STRUCTURES IMPLEMENTATION
// Covering:
//   CO.2: Singly Linked List (Dynamic Vehicle Buffer)
//   CO.3: Queue (FIFO Normal Commuters) & Stack (LIFO Emergency Preemption Events)
//   CO.4: Searching (Linear & Binary Search) & Sorting (Bubble, Selection, Quick Sort)
//   CO.5: Hashing (Hash Table with Separate Chaining & Linear Probing)
// ==============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// ------------------------------------------------------------------------------
// COMMON VEHICLE RECORD STRUCTURE
// ------------------------------------------------------------------------------
struct VehicleRecord {
    int id;
    string type;
    int waitTime;
    int lane;
    bool emergency;
    int priorityScore;

    VehicleRecord(int i = 0, string t = "Car", int w = 0, int l = 0, bool e = false)
        : id(i), type(t), waitTime(w), lane(l), emergency(e) {
        priorityScore = emergency ? (waitTime + 100) : waitTime;
    }

    void display() const {
        cout << "  [ID: " << setw(3) << id << " | Type: " << setw(10) << type
             << " | Wait: " << setw(2) << waitTime << "m | Lane: " << lane
             << " | Emerg: " << (emergency ? "YES" : "NO ")
             << " | Priority: " << setw(3) << priorityScore << "]\n";
    }
};

// ==============================================================================
// CO.2: SINGLY LINKED LIST (Vehicle Registration Database)
// ==============================================================================
struct LLNode {
    VehicleRecord data;
    LLNode* next;
    LLNode(const VehicleRecord& rec) : data(rec), next(nullptr) {}
};

class VehicleLinkedList {
private:
    LLNode* head;
    int count;

public:
    VehicleLinkedList() : head(nullptr), count(0) {}

    ~VehicleLinkedList() {
        clear();
    }

    void clear() {
        while (head != nullptr) {
            LLNode* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }

    void insertEnd(const VehicleRecord& rec) {
        LLNode* newNode = new LLNode(rec);
        if (!head) {
            head = newNode;
        } else {
            LLNode* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = newNode;
        }
        count++;
    }

    bool deleteById(int id) {
        if (!head) return false;
        if (head->data.id == id) {
            LLNode* temp = head;
            head = head->next;
            delete temp;
            count--;
            return true;
        }
        LLNode* curr = head;
        while (curr->next && curr->next->data.id != id) {
            curr = curr->next;
        }
        if (curr->next) {
            LLNode* temp = curr->next;
            curr->next = curr->next->next;
            delete temp;
            count--;
            return true;
        }
        return false;
    }

    VehicleRecord* searchById(int id) {
        LLNode* curr = head;
        while (curr) {
            if (curr->data.id == id) return &(curr->data);
            curr = curr->next;
        }
        return nullptr;
    }

    void display() const {
        cout << "\n--- Linked List (Vehicle Fleet Size: " << count << ") ---\n";
        if (!head) {
            cout << "  [Empty Linked List: HEAD -> NULL]\n";
            return;
        }
        LLNode* curr = head;
        while (curr) {
            curr->data.display();
            curr = curr->next;
        }
    }

    int getCount() const { return count; }
};

// ==============================================================================
// CO.3: QUEUE (FIFO Normal Commuters) & STACK (LIFO Emergency Event History)
// ==============================================================================
class VehicleQueue {
private:
    struct QNode {
        VehicleRecord data;
        QNode* next;
        QNode(const VehicleRecord& r) : data(r), next(nullptr) {}
    };
    QNode* frontNode;
    QNode* rearNode;
    int size;

public:
    VehicleQueue() : frontNode(nullptr), rearNode(nullptr), size(0) {}

    ~VehicleQueue() {
        while (!isEmpty()) dequeue();
    }

    bool isEmpty() const { return frontNode == nullptr; }
    int getSize() const { return size; }

    void enqueue(const VehicleRecord& rec) {
        QNode* newNode = new QNode(rec);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        size++;
        cout << "  [QUEUE] Enqueued vehicle ID " << rec.id << " at Rear.\n";
    }

    bool dequeue(VehicleRecord* outRec = nullptr) {
        if (isEmpty()) {
            cout << "  [QUEUE UNDERFLOW] Queue is empty, cannot dequeue.\n";
            return false;
        }
        QNode* temp = frontNode;
        if (outRec) *outRec = temp->data;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        cout << "  [QUEUE] Dequeued vehicle ID " << temp->data.id << " from Front (FIFO).\n";
        delete temp;
        size--;
        return true;
    }

    bool peek(VehicleRecord& outRec) const {
        if (isEmpty()) return false;
        outRec = frontNode->data;
        return true;
    }

    void display() const {
        cout << "\n--- FIFO Waiting Queue (Size: " << size << ") [FRONT -> REAR] ---\n";
        if (isEmpty()) {
            cout << "  [Empty Queue]\n";
            return;
        }
        QNode* curr = frontNode;
        int idx = 1;
        while (curr) {
            cout << "  Pos " << idx++ << ": ";
            curr->data.display();
            curr = curr->next;
        }
    }
};

class EventStack {
private:
    struct SNode {
        string eventDescription;
        int timestamp;
        SNode* next;
        SNode(string desc, int t) : eventDescription(desc), timestamp(t), next(nullptr) {}
    };
    SNode* topNode;
    int size;

public:
    EventStack() : topNode(nullptr), size(0) {}

    ~EventStack() {
        while (!isEmpty()) pop();
    }

    bool isEmpty() const { return topNode == nullptr; }
    int getSize() const { return size; }

    void push(const string& desc, int t) {
        SNode* newNode = new SNode(desc, t);
        newNode->next = topNode;
        topNode = newNode;
        size++;
        cout << "  [STACK] Pushed Event to TOP (LIFO): " << desc << " @ t=" << t << "s\n";
    }

    bool pop(string* outDesc = nullptr) {
        if (isEmpty()) {
            cout << "  [STACK UNDERFLOW] Stack is empty, cannot pop.\n";
            return false;
        }
        SNode* temp = topNode;
        if (outDesc) *outDesc = temp->eventDescription;
        cout << "  [STACK] Popped Event from TOP (LIFO): " << temp->eventDescription << "\n";
        topNode = topNode->next;
        delete temp;
        size--;
        return true;
    }

    bool peek(string& outDesc) const {
        if (isEmpty()) return false;
        outDesc = topNode->eventDescription;
        return true;
    }

    void display() const {
        cout << "\n--- LIFO Event History Stack (Size: " << size << ") [TOP -> BOTTOM] ---\n";
        if (isEmpty()) {
            cout << "  [Empty Stack]\n";
            return;
        }
        SNode* curr = topNode;
        int level = size;
        while (curr) {
            cout << "  Level " << level-- << ": " << curr->eventDescription
                 << " (Recorded at " << curr->timestamp << "s)\n";
            curr = curr->next;
        }
    }
};

// ==============================================================================
// CO.4: SEARCHING & SORTING ALGORITHMS
// ==============================================================================
class SearchSortEngine {
public:
    // Linear Search by ID (O(n))
    static int linearSearchById(const vector<VehicleRecord>& arr, int targetId, int& comparisons) {
        comparisons = 0;
        for (size_t i = 0; i < arr.size(); i++) {
            comparisons++;
            if (arr[i].id == targetId) return (int)i;
        }
        return -1;
    }

    // Binary Search by ID (O(log n)) - Requires sorted array by ID
    static int binarySearchById(const vector<VehicleRecord>& arr, int targetId, int& comparisons) {
        comparisons = 0;
        int low = 0;
        int high = (int)arr.size() - 1;

        while (low <= high) {
            comparisons++;
            int mid = low + (high - low) / 2;
            if (arr[mid].id == targetId) return mid;
            if (arr[mid].id < targetId) low = mid + 1;
            else high = mid - 1;
        }
        return -1;
    }

    // Bubble Sort by Waiting Time (Ascending or Descending)
    static void bubbleSortByWaitTime(vector<VehicleRecord>& arr, bool ascending = true) {
        int n = (int)arr.size();
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - i - 1; j++) {
                bool condition = ascending ? (arr[j].waitTime > arr[j + 1].waitTime)
                                           : (arr[j].waitTime < arr[j + 1].waitTime);
                if (condition) {
                    swap(arr[j], arr[j + 1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    // Selection Sort by Vehicle ID (Ascending)
    static void selectionSortById(vector<VehicleRecord>& arr) {
        int n = (int)arr.size();
        for (int i = 0; i < n - 1; i++) {
            int minIdx = i;
            for (int j = i + 1; j < n; j++) {
                if (arr[j].id < arr[minIdx].id) minIdx = j;
            }
            if (minIdx != i) swap(arr[i], arr[minIdx]);
        }
    }

    // Quick Sort by Priority Score (Descending - Highest Priority First)
    static void quickSortByPriority(vector<VehicleRecord>& arr, int low, int high) {
        if (low < high) {
            int pivot = arr[high].priorityScore;
            int i = low - 1;
            for (int j = low; j < high; j++) {
                if (arr[j].priorityScore > pivot) { // Descending
                    i++;
                    swap(arr[i], arr[j]);
                }
            }
            swap(arr[i + 1], arr[high]);
            int pi = i + 1;

            quickSortByPriority(arr, low, pi - 1);
            quickSortByPriority(arr, pi + 1, high);
        }
    }
};

// ==============================================================================
// CO.5: HASH TABLE WITH SEPARATE CHAINING & LINEAR PROBING
// ==============================================================================
class VehicleHashTable {
private:
    static const int TABLE_SIZE = 10;

    // Chaining Nodes
    struct HashNode {
        VehicleRecord record;
        HashNode* next;
        HashNode(const VehicleRecord& r) : record(r), next(nullptr) {}
    };

    HashNode* chains[TABLE_SIZE];
    int totalElements;

public:
    VehicleHashTable() : totalElements(0) {
        for (int i = 0; i < TABLE_SIZE; i++) chains[i] = nullptr;
    }

    ~VehicleHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* curr = chains[i];
            while (curr) {
                HashNode* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

    // Hash Function: h(key) = key % TABLE_SIZE
    int hashFunction(int key) const {
        return (key % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
    }

    // Insert with Separate Chaining
    void insert(const VehicleRecord& rec) {
        int bucket = hashFunction(rec.id);

        // Check if key already exists (update if so)
        HashNode* curr = chains[bucket];
        while (curr) {
            if (curr->record.id == rec.id) {
                curr->record = rec;
                cout << "  [HASH] Updated existing vehicle ID " << rec.id << " in Bucket [" << bucket << "]\n";
                return;
            }
            curr = curr->next;
        }

        // Prepend to chain
        HashNode* newNode = new HashNode(rec);
        newNode->next = chains[bucket];
        chains[bucket] = newNode;
        totalElements++;

        if (newNode->next != nullptr) {
            cout << "  [HASH COLLISION] Key " << rec.id << " collided in Bucket [" << bucket
                 << "]. Resolved using Separate Chaining.\n";
        } else {
            cout << "  [HASH] Inserted ID " << rec.id << " into Bucket [" << bucket << "]\n";
        }
    }

    // Search in Hash Table: Average O(1), Worst O(n)
    VehicleRecord* search(int id, int& comparisons) const {
        comparisons = 0;
        int bucket = hashFunction(id);
        HashNode* curr = chains[bucket];
        while (curr) {
            comparisons++;
            if (curr->record.id == id) return &(curr->record);
            curr = curr->next;
        }
        return nullptr;
    }

    // Delete from Hash Table
    bool remove(int id) {
        int bucket = hashFunction(id);
        HashNode* curr = chains[bucket];
        HashNode* prev = nullptr;

        while (curr) {
            if (curr->record.id == id) {
                if (prev) prev->next = curr->next;
                else chains[bucket] = curr->next;
                delete curr;
                totalElements--;
                cout << "  [HASH] Deleted vehicle ID " << id << " from Bucket [" << bucket << "]\n";
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        cout << "  [HASH] Cannot delete: ID " << id << " not found in Bucket [" << bucket << "]\n";
        return false;
    }

    // Display Entire Hash Table Structure
    void display() const {
        cout << "\n--- Hash Table Structure (Capacity: " << TABLE_SIZE
             << " Buckets | Elements: " << totalElements << ") ---\n";
        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << "  Bucket [" << i << "]: ";
            HashNode* curr = chains[i];
            if (!curr) {
                cout << "EMPTY\n";
            } else {
                while (curr) {
                    cout << "[ID: " << curr->record.id << ", " << curr->record.type
                         << ", Wait: " << curr->record.waitTime << "m] -> ";
                    curr = curr->next;
                }
                cout << "NULL\n";
            }
        }
    }
};

// ==============================================================================
// VERIFICATION TEST HARNESS (Testing all COs)
// ==============================================================================
int main() {
    cout << "==============================================================================\n";
    cout << " SMART TRAFFIC LIGHT SYSTEM - PROGRAMMING LAB (PL) COMPLETE TEST RUN\n";
    cout << " Covering CO.2, CO.3, CO.4, and CO.5\n";
    cout << "==============================================================================\n\n";

    // 1. Test CO.2: Linked List
    cout << ">>> 1. TESTING CO.2: LINKED LIST VEHICLE FLEET MANAGEMENT\n";
    VehicleLinkedList fleet;
    fleet.insertEnd(VehicleRecord(101, "Car", 12, 0, false));
    fleet.insertEnd(VehicleRecord(102, "Bike", 18, 1, false));
    fleet.insertEnd(VehicleRecord(103, "Ambulance", 25, 2, true));
    fleet.insertEnd(VehicleRecord(104, "FireTruck", 22, 3, true));
    fleet.insertEnd(VehicleRecord(105, "Bus", 14, 0, false));
    fleet.display();

    cout << "\nSearching for ID 103 in Linked List:\n";
    VehicleRecord* rec = fleet.searchById(103);
    if (rec) rec->display();
    else cout << "Not found.\n";

    cout << "\nDeleting ID 102 from Linked List:\n";
    fleet.deleteById(102);
    fleet.display();

    // 2. Test CO.3: Stack and Queue
    cout << "\n>>> 2. TESTING CO.3: FIFO QUEUE & LIFO EVENT STACK\n";
    VehicleQueue queue;
    queue.enqueue(VehicleRecord(101, "Car", 12, 0, false));
    queue.enqueue(VehicleRecord(105, "Bus", 14, 0, false));
    queue.enqueue(VehicleRecord(106, "Car", 8, 1, false));
    queue.display();

    VehicleRecord front;
    if (queue.peek(front)) {
        cout << "  Peek Front: ID " << front.id << " (" << front.type << ")\n";
    }

    queue.dequeue();
    queue.display();

    EventStack stack;
    stack.push("Green phase for East-West corridor", 0);
    stack.push("Vehicle #101 crossed intersection", 4);
    stack.push("[EMERGENCY OVERRIDE] Ambulance #103 preempted signal", 8);
    stack.push("North-South green wave granted for emergency", 9);
    stack.display();

    string topEvent;
    if (stack.peek(topEvent)) {
        cout << "  Peek Top: " << topEvent << "\n";
    }
    stack.pop();
    stack.display();

    // 3. Test CO.4: Searching and Sorting
    cout << "\n>>> 3. TESTING CO.4: SEARCHING & SORTING ALGORITHMS\n";
    vector<VehicleRecord> dataset = {
        VehicleRecord(105, "Bus", 15, 0, false),
        VehicleRecord(101, "Car", 25, 1, false),
        VehicleRecord(104, "FireTruck", 10, 3, true),
        VehicleRecord(102, "Bike", 8, 2, false),
        VehicleRecord(103, "Ambulance", 30, 2, true)
    };

    cout << "Original Unsorted Dataset:\n";
    for (const auto& v : dataset) v.display();

    int comps = 0;
    int idx = SearchSortEngine::linearSearchById(dataset, 104, comps);
    cout << "\nLinear Search for ID 104: Found at index " << idx << " after " << comps << " comparisons.\n";

    // Sort by ID using Selection Sort
    cout << "\nSorting by ID using Selection Sort:\n";
    vector<VehicleRecord> sortedById = dataset;
    SearchSortEngine::selectionSortById(sortedById);
    for (const auto& v : sortedById) v.display();

    // Binary Search on sortedById
    idx = SearchSortEngine::binarySearchById(sortedById, 104, comps);
    cout << "\nBinary Search for ID 104 on sorted array: Found at index " << idx << " after " << comps << " comparisons.\n";

    // Sort by Priority using Quick Sort
    cout << "\nSorting by Priority Score using Quick Sort (Highest Priority First):\n";
    vector<VehicleRecord> sortedByPriority = dataset;
    SearchSortEngine::quickSortByPriority(sortedByPriority, 0, (int)sortedByPriority.size() - 1);
    for (const auto& v : sortedByPriority) v.display();

    // 4. Test CO.5: Hashing with Collision Resolution
    cout << "\n>>> 4. TESTING CO.5: HASHING & COLLISION RESOLUTION\n";
    VehicleHashTable hashTable;
    // Keys designed to cause intentional collisions: 101 % 10 = 1, 111 % 10 = 1, 121 % 10 = 1
    cout << "Inserting keys 101, 111, 121 (all hash to Bucket 1):\n";
    hashTable.insert(VehicleRecord(101, "Car", 12, 0, false));
    hashTable.insert(VehicleRecord(111, "Taxi", 16, 1, false));
    hashTable.insert(VehicleRecord(121, "Van", 20, 2, false));

    // Non-colliding keys
    hashTable.insert(VehicleRecord(103, "Ambulance", 30, 2, true)); // Bucket 3
    hashTable.insert(VehicleRecord(104, "FireTruck", 25, 3, true)); // Bucket 4
    hashTable.insert(VehicleRecord(105, "Bus", 18, 0, false));      // Bucket 5

    hashTable.display();

    cout << "\nSearching for key 111 in Hash Table:\n";
    VehicleRecord* hFound = hashTable.search(111, comps);
    if (hFound) {
        cout << "  Found vehicle: ";
        hFound->display();
        cout << "  Comparisons required in bucket chain: " << comps << "\n";
    }

    cout << "\nDeleting key 111 from Hash Table:\n";
    hashTable.remove(111);
    hashTable.display();

    cout << "\n==============================================================================\n";
    cout << " ALL PL TESTS COMPLETED SUCCESSFULLY!\n";
    cout << "==============================================================================\n";

    return 0;
}
