#include <iostream>
using namespace std;

int main()
{
    int vehicleCount, waitTime, typeWeight, emergency;
    int R1, R2, R3, R4, priority, greenTime;

    cout << "Enter vehicle count: ";
    cin >> vehicleCount;

    cout << "Enter waiting time: ";
    cin >> waitTime;

    cout << "Enter vehicle type weight: ";
    cin >> typeWeight;

    cout << "Emergency? (1=Yes, 0=No): ";
    cin >> emergency;

    
    R1 = vehicleCount;
    R2 = waitTime;
    R3 = typeWeight;

    // Calculate traffic density weight
    R4 = R1 * 2;

    // Calculate priority
    priority = R2 + R3 + R4;

    if (emergency == 1)
        priority = priority + 100;

    // Calculate green signal time
    greenTime = 30 + priority;

    if (emergency == 1)
        greenTime = greenTime + 15;

    cout << "\nPriority Score: " << priority;
    cout << "\nGreen Signal Time: " << greenTime << " seconds";

    if (emergency == 1)
        cout << "\nEmergency Priority Activated";
    else
        cout << "\nNormal Traffic";

    return 0;
}