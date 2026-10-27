// ==============================================================================
// SMART TRAFFIC LIGHT SYSTEM WITH EMERGENCY VEHICLE PRIORITY
// Complete 3D Four-Way Traffic Junction using C++ and OpenGL with FreeGLUT
// Computer Graphics (CG) & Object-Oriented Programming (PSOOP) Implementation
//
// CONTROLS:
//   E / e        : Dispatch Emergency Ambulance (Blue beacon, preemption, green wave)
//   F / f        : Dispatch Emergency Fire Truck
//   SPACE        : Pause / Resume simulation
//   S / s        : Manually switch active traffic signal phase
//   R / r        : Reset simulation & clear emergency status
//   Arrow Keys   : Orbit camera around junction (Left / Right / Up / Down)
//   + / -        : Zoom in / Zoom out
//   0 / Home     : Reset camera to default elevated perspective
//   V / v        : Toggle View Mode (3D Perspective, Overhead 2D, Driver View)
//   ESC          : Exit application
// ==============================================================================

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

// ------------------------------------------------------------------------------
// CONSTANTS & CONFIGURATION
// ------------------------------------------------------------------------------
const float PI = 3.14159265358979323846f;

// Road & Junction Dimensions
const float ROAD_WIDTH       = 12.0f; // Total road width (2 lanes each direction)
const float HALF_ROAD        = ROAD_WIDTH / 2.0f; // 6.0f
const float LANE_WIDTH       = 2.7f;
const float STOP_LINE_DIST   = 6.8f;
const float ROAD_LENGTH      = 50.0f;

// Window Dimensions
int windowWidth  = 1280;
int windowHeight = 800;

// Camera State (Elevated Perspective by default - Complete Junction View)
float defaultCameraYaw      = 45.0f;
float defaultCameraPitch    = 38.0f;
// Increased distance to ensure full junction is visible on startup
float defaultCameraDistance = 65.0f;

float cameraYaw      = defaultCameraYaw;
float cameraPitch    = defaultCameraPitch;
float cameraDistance = defaultCameraDistance;

int viewMode = 0; // 0: Elevated 3D Perspective, 1: 2D Overhead Top-Down, 2: Driver Cam

// Simulation State
bool simulationPaused = false;
float simulationTime = 0.0f;

// Traffic Signal Phases
enum SignalPhase {
    PHASE_EW_GREEN,
    PHASE_EW_YELLOW,
    PHASE_ALL_RED_1,
    PHASE_NS_GREEN,
    PHASE_NS_YELLOW,
    PHASE_ALL_RED_2,
    PHASE_EMERGENCY_CLEARANCE
};

SignalPhase currentPhase = PHASE_EW_GREEN;
float phaseTimer = 10.0f;
const float DURATION_GREEN   = 10.0f;
const float DURATION_YELLOW  = 2.5f;
const float DURATION_ALL_RED = 1.5f;

// Emergency Priority State
bool emergencyActive        = false;
bool emergencyPassed        = false;
int activeEmergencyId       = 0;
std::string emergencyUnitName = "NONE";
SignalPhase preEmergencyPhase = PHASE_EW_GREEN;
float beaconFlashTimer      = 0.0f;
bool beaconState            = false;

// Statistics & Priority Scoring
int totalVehiclesProcessed = 0;
int activePriorityScore    = 0;
int avgWaitTimeSec         = 8;

// LIFO Event Stack & FIFO Queue
std::vector<std::string> systemEventStack;
std::queue<int> normalWaitingQueue;

// ------------------------------------------------------------------------------
// CO.5: CLIPPING ALGORITHM (COHEN-SUTHERLAND & OPENGL HARDWARE CLIPPING)
// ------------------------------------------------------------------------------
bool clippingDemoActive       = false; // Cohen-Sutherland Line Clipping Demo (Key 'C')
bool hardwareClipPlaneActive  = false; // OpenGL Hardware Clip Plane Demo (Key 'P')
const float CLIP_XMIN = -16.0f;
const float CLIP_XMAX =  16.0f;
const float CLIP_ZMIN = -16.0f;
const float CLIP_ZMAX =  16.0f;

// 4-bit Outcodes: [TOP, BOTTOM, RIGHT, LEFT]
const int CS_INSIDE = 0; // 0000
const int CS_LEFT   = 1; // 0001
const int CS_RIGHT  = 2; // 0010
const int CS_BOTTOM = 4; // 0100
const int CS_TOP    = 8; // 1000

int computeCSOutCode(float x, float z) {
    int code = CS_INSIDE;
    if (x < CLIP_XMIN)      code |= CS_LEFT;
    else if (x > CLIP_XMAX) code |= CS_RIGHT;
    if (z < CLIP_ZMIN)      code |= CS_BOTTOM;
    else if (z > CLIP_ZMAX) code |= CS_TOP;
    return code;
}

// ------------------------------------------------------------------------------
// HELPER DRAWING PRIMITIVES
// ------------------------------------------------------------------------------
void setMaterialColor(float r, float g, float b, float shininess = 16.0f, float emission = 0.0f) {
    glColor3f(r, g, b);
    GLfloat ambient[]  = { r * 0.35f, g * 0.35f, b * 0.35f, 1.0f };
    GLfloat diffuse[]  = { r * 0.85f, g * 0.85f, b * 0.85f, 1.0f };
    GLfloat specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat emissive[] = { r * emission, g * emission, b * emission, 1.0f };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emissive);
}

void drawSolidBox(float cx, float cy, float cz, float sx, float sy, float sz,
                  float r, float g, float b, float shininess = 16.0f, float emission = 0.0f) {
    glPushMatrix();
    glTranslatef(cx, cy, cz);
    glScalef(sx, sy, sz);
    setMaterialColor(r, g, b, shininess, emission);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawSolidCylinder(float cx, float cy, float cz, float radius, float height,
                       float r, float g, float b) {
    glPushMatrix();
    glTranslatef(cx, cy, cz);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    setMaterialColor(r, g, b);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, radius, radius, height, 16, 4);
    gluDisk(q, 0.0, radius, 16, 1);
    glTranslatef(0.0f, 0.0f, height);
    gluDisk(q, 0.0, radius, 16, 1);
    gluDeleteQuadric(q);
    glPopMatrix();
}

void drawSolidSphere(float cx, float cy, float cz, float radius,
                     float r, float g, float b, float emission = 0.0f) {
    glPushMatrix();
    glTranslatef(cx, cy, cz);
    setMaterialColor(r, g, b, 32.0f, emission);
    glutSolidSphere(radius, 16, 16);
    glPopMatrix();
}

void drawSolidCone(float cx, float cy, float cz, float baseRadius, float height,
                   float r, float g, float b) {
    glPushMatrix();
    glTranslatef(cx, cy, cz);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    setMaterialColor(r, g, b);
    glutSolidCone(baseRadius, height, 16, 8);
    glPopMatrix();
}

// Flat 2D circular disk helper (used for flat 2D traffic light lamps and halos)
void drawFlatCircle2D(float cx, float cy, float cz, float radius,
                      float r, float g, float b, float emission = 0.0f) {
    glPushMatrix();
    glTranslatef(cx, cy, cz);
    setMaterialColor(r, g, b, 16.0f, emission);
    GLUquadric* q = gluNewQuadric();
    gluDisk(q, 0.0, radius, 20, 1);
    gluDeleteQuadric(q);
    glPopMatrix();
}

// ------------------------------------------------------------------------------
// OBJECT-ORIENTED VEHICLE CLASS HIERARCHY
// Demonstrating Classes, Inheritance, Encapsulation & Polymorphism
// ------------------------------------------------------------------------------
class Vehicle {
protected:
    int id;
    std::string type;
    int waitTime;
    float length, width, height;
    float maxSpeed;

public:
    float x, y, z;
    float vx, vz;
    float rotationAngle; // Y-axis rotation in degrees
    int lane;            // 0: Eastbound (+X), 1: Westbound (-X), 2: Southbound (+Z), 3: Northbound (-Z)
    float speed;
    float r, g, b;
    bool emergency;
    bool hasCrossed;
    bool stopped;

    Vehicle(int vId, std::string vType, int vWait, int vLane,
            float vLength, float vWidth, float vHeight,
            float vr, float vg, float vb, bool isEmerg = false)
        : id(vId), type(vType), waitTime(vWait), length(vLength), width(vWidth), height(vHeight),
          lane(vLane), r(vr), g(vg), b(vb), emergency(isEmerg),
          speed(0.0f), hasCrossed(false), stopped(false) {

        y = 0.0f;
        maxSpeed = emergency ? 0.16f : 0.085f;

        // Position by Lane
        if (lane == 0) { // Eastbound: Starts at -X, moves +X along z = -LANE_WIDTH/2
            x = -ROAD_LENGTH;
            z = -LANE_WIDTH * 0.65f;
            vx = 1.0f; vz = 0.0f;
            rotationAngle = 90.0f;
        } else if (lane == 1) { // Westbound: Starts at +X, moves -X along z = +LANE_WIDTH/2
            x = ROAD_LENGTH;
            z = LANE_WIDTH * 0.65f;
            vx = -1.0f; vz = 0.0f;
            rotationAngle = 270.0f;
        } else if (lane == 2) { // Southbound: Starts at -Z, moves +Z along x = +LANE_WIDTH/2
            x = LANE_WIDTH * 0.65f;
            z = -ROAD_LENGTH;
            vx = 0.0f; vz = 1.0f;
            rotationAngle = 0.0f;
        } else { // Northbound: Starts at +Z, moves -Z along x = -LANE_WIDTH/2
            x = -LANE_WIDTH * 0.65f;
            z = ROAD_LENGTH;
            vx = 0.0f; vz = -1.0f;
            rotationAngle = 180.0f;
        }
    }

    virtual ~Vehicle() {}

    // Encapsulation: Accessors & Mutators
    int getId() const { return id; }
    std::string getType() const { return type; }
    int getWaitTime() const { return waitTime; }
    void incrementWait() { waitTime++; }
    bool isEmergency() const { return emergency; }
    float getLength() const { return length; }

    // Polymorphism: Virtual Priority Calculation (Overridden by EmergencyVehicle)
    virtual int getPriority() const {
        return waitTime; // Normal vehicle priority is waiting time
    }

    // Virtual Draw Method (Specialized 3D models for Bus, Bike, Car)
    virtual void draw() const {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

        if (type == "Bus") {
            // Main Coach Chassis
            drawSolidBox(0.0f, height * 0.45f, 0.0f, width, height * 0.70f, length, r, g, b, 24.0f);
            // Aerodynamic Roof Shell
            drawSolidBox(0.0f, height * 0.88f, 0.0f, width * 0.94f, height * 0.20f, length * 0.96f, r * 0.85f, g * 0.85f, b * 0.85f);
            // Front Windshield
            drawSolidBox(0.0f, height * 0.65f, length * 0.48f, width * 0.88f, height * 0.38f, 0.08f, 0.15f, 0.25f, 0.35f, 32.0f);
            // Route Destination Display Board
            drawSolidBox(0.0f, height * 0.92f, length * 0.49f, width * 0.70f, 0.16f, 0.06f, 0.05f, 0.05f, 0.05f, 16.0f, 0.8f);
            // Side Passenger Windows (Tinted glass strips)
            drawSolidBox( width * 0.51f, height * 0.65f, 0.0f, 0.04f, height * 0.30f, length * 0.80f, 0.12f, 0.20f, 0.28f, 32.0f);
            drawSolidBox(-width * 0.51f, height * 0.65f, 0.0f, 0.04f, height * 0.30f, length * 0.80f, 0.12f, 0.20f, 0.28f, 32.0f);
            // 6 Heavy Coach Wheels (1 Front axle + 2 Dual Rear axles)
            float wheelRadius = height * 0.24f;
            float wheelWidth  = 0.20f;
            float wheelXOff   = width * 0.52f;
            float zOffsets[3] = { length * 0.33f, -length * 0.18f, -length * 0.38f };
            for (int side = -1; side <= 1; side += 2) {
                for (int w = 0; w < 3; w++) {
                    glPushMatrix();
                    glTranslatef(side * wheelXOff, wheelRadius, zOffsets[w]);
                    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
                    setMaterialColor(0.08f, 0.08f, 0.08f, 8.0f);
                    glutSolidTorus(wheelWidth * 0.45, wheelRadius * 0.75, 8, 16);
                    glPopMatrix();
                }
            }
            // Headlights & Taillights
            drawSolidSphere(-width * 0.36f, height * 0.28f, length * 0.51f, 0.14f, 1.0f, 1.0f, 0.8f, 0.9f);
            drawSolidSphere( width * 0.36f, height * 0.28f, length * 0.51f, 0.14f, 1.0f, 1.0f, 0.8f, 0.9f);
            drawSolidSphere(-width * 0.36f, height * 0.28f, -length * 0.51f, 0.12f, 1.0f, 0.1f, 0.05f, 0.7f);
            drawSolidSphere( width * 0.36f, height * 0.28f, -length * 0.51f, 0.12f, 1.0f, 0.1f, 0.05f, 0.7f);
        } else if (type == "Bike") {
            // Sleek Motorcycle Chassis
            drawSolidBox(0.0f, height * 0.42f, 0.0f, width * 0.35f, height * 0.45f, length * 0.70f, r, g, b, 24.0f);
            // Fuel Tank
            drawSolidBox(0.0f, height * 0.60f, length * 0.10f, width * 0.40f, height * 0.28f, length * 0.35f, r * 1.1f, g * 1.1f, b * 1.1f, 32.0f);
            // Handlebars
            drawSolidBox(0.0f, height * 0.78f, length * 0.32f, width * 0.95f, 0.06f, 0.06f, 0.1f, 0.1f, 0.12f);
            // Rider Silhouette
            drawSolidSphere(0.0f, height * 0.82f, -length * 0.12f, 0.24f, 0.15f, 0.18f, 0.22f); // Jacket
            drawSolidSphere(0.0f, height * 1.12f, -length * 0.10f, 0.18f, 0.95f, 0.2f, 0.1f);   // Helmet
            // 2 Inline Wheels (Front & Rear)
            float wheelRadius = height * 0.30f;
            float wheelWidth  = 0.10f;
            float zW[2] = { length * 0.40f, -length * 0.38f };
            for (int w = 0; w < 2; w++) {
                glPushMatrix();
                glTranslatef(0.0f, wheelRadius, zW[w]);
                glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
                setMaterialColor(0.08f, 0.08f, 0.08f, 8.0f);
                glutSolidTorus(wheelWidth * 0.45, wheelRadius * 0.75, 8, 16);
                glPopMatrix();
            }
            // Front Headlight & Rear Taillight
            drawSolidSphere(0.0f, height * 0.55f, length * 0.50f, 0.13f, 1.0f, 1.0f, 0.85f, 0.9f);
            drawSolidSphere(0.0f, height * 0.55f, -length * 0.48f, 0.10f, 1.0f, 0.1f, 0.05f, 0.7f);
        } else {
            // Sedan / Hatchback Passenger Car
            // Lower Chassis Body
            drawSolidBox(0.0f, height * 0.32f, 0.0f, width, height * 0.40f, length, r, g, b, 24.0f);
            // Passenger Cabin & Roof
            drawSolidBox(0.0f, height * 0.68f, -length * 0.06f, width * 0.84f, height * 0.38f, length * 0.55f, r * 0.9f, g * 0.9f, b * 0.9f, 24.0f);
            // Windshield (Front Slanted Glass)
            drawSolidBox(0.0f, height * 0.68f, length * 0.22f, width * 0.80f, height * 0.32f, 0.06f, 0.15f, 0.22f, 0.32f, 32.0f);
            // Rear Windshield
            drawSolidBox(0.0f, height * 0.68f, -length * 0.34f, width * 0.80f, height * 0.30f, 0.06f, 0.15f, 0.22f, 0.32f, 32.0f);
            // 4 Wheels
            float wheelRadius = height * 0.24f;
            float wheelWidth  = 0.16f;
            float wheelZOff   = length * 0.30f;
            float wheelXOff   = width * 0.51f;
            for (int side = -1; side <= 1; side += 2) {
                for (int end = -1; end <= 1; end += 2) {
                    glPushMatrix();
                    glTranslatef(side * wheelXOff, wheelRadius, end * wheelZOff);
                    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
                    setMaterialColor(0.08f, 0.08f, 0.08f, 8.0f);
                    glutSolidTorus(wheelWidth * 0.45, wheelRadius * 0.75, 8, 16);
                    glPopMatrix();
                }
            }
            // Headlights & Taillights
            drawSolidSphere(-width * 0.32f, height * 0.32f, length * 0.51f, 0.12f, 1.0f, 1.0f, 0.85f, 0.85f);
            drawSolidSphere( width * 0.32f, height * 0.32f, length * 0.51f, 0.12f, 1.0f, 1.0f, 0.85f, 0.85f);
            drawSolidSphere(-width * 0.32f, height * 0.32f, -length * 0.51f, 0.10f, 1.0f, 0.1f, 0.05f, 0.65f);
            drawSolidSphere( width * 0.32f, height * 0.32f, -length * 0.51f, 0.10f, 1.0f, 0.1f, 0.05f, 0.65f);
        }

        glPopMatrix();
    }
};

// ------------------------------------------------------------------------------
// DERIVED EMERGENCY VEHICLE CLASS
// Demonstrating Inheritance & Specialized Emergency Preemption
// ------------------------------------------------------------------------------
class EmergencyVehicle : public Vehicle {
private:
    std::string emergencySubtype; // "Ambulance" or "FireTruck"

public:
    EmergencyVehicle(int vId, std::string subType, int vWait, int vLane)
        : Vehicle(vId, subType, vWait, vLane,
                  (subType == "FireTruck" ? 4.2f : 2.7f), // Length
                  (subType == "FireTruck" ? 1.6f : 1.35f), // Width
                  (subType == "FireTruck" ? 1.7f : 1.45f), // Height
                  (subType == "FireTruck" ? 0.92f : 0.95f), // R
                  (subType == "FireTruck" ? 0.12f : 0.95f), // G
                  (subType == "FireTruck" ? 0.10f : 0.92f), // B
                  true),
          emergencySubtype(subType) {
        maxSpeed = 0.15f;
    }

    // Polymorphism: Emergency Priority is Waiting Time + 100 Bonus (from PSOOP / Review 1)
    int getPriority() const override {
        return waitTime + 100;
    }

    void draw() const override {
        glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);

        if (emergencySubtype == "FireTruck") {
            // Heavy Engine Chassis
            drawSolidBox(0.0f, height * 0.42f, 0.0f, width, height * 0.65f, length, 0.88f, 0.08f, 0.08f, 24.0f);
            // Driver Cabin
            drawSolidBox(0.0f, height * 0.75f, length * 0.24f, width * 0.92f, height * 0.48f, length * 0.35f, 0.82f, 0.05f, 0.05f);
            // Windshield
            drawSolidBox(0.0f, height * 0.82f, length * 0.42f, width * 0.80f, height * 0.30f, 0.08f, 0.2f, 0.3f, 0.4f, 32.0f);
            // Equipment Ladder on Roof
            drawSolidBox(0.0f, height * 1.08f, -length * 0.10f, width * 0.55f, 0.10f, length * 0.65f, 0.85f, 0.85f, 0.88f);
        } else {
            // Ambulance Chassis
            drawSolidBox(0.0f, height * 0.35f, 0.0f, width, height * 0.50f, length, 0.95f, 0.95f, 0.92f, 24.0f);
            // Medical Patient Compartment
            drawSolidBox(0.0f, height * 0.72f, -length * 0.08f, width * 0.92f, height * 0.52f, length * 0.70f, 0.92f, 0.92f, 0.90f);
            // Red Medical Cross Symbol on Sides
            drawSolidBox( width * 0.47f, height * 0.72f, -length * 0.08f, 0.04f, 0.38f, 0.12f, 0.9f, 0.05f, 0.05f);
            drawSolidBox( width * 0.47f, height * 0.72f, -length * 0.08f, 0.04f, 0.12f, 0.38f, 0.9f, 0.05f, 0.05f);
            drawSolidBox(-width * 0.47f, height * 0.72f, -length * 0.08f, 0.04f, 0.38f, 0.12f, 0.9f, 0.05f, 0.05f);
            drawSolidBox(-width * 0.47f, height * 0.72f, -length * 0.08f, 0.04f, 0.12f, 0.38f, 0.9f, 0.05f, 0.05f);
        }

        // 6 Heavy Wheels for Trucks / 4 for Ambulance
        float wheelRadius = height * 0.26f;
        int wheelPairs = (emergencySubtype == "FireTruck") ? 3 : 2;
        for (int side = -1; side <= 1; side += 2) {
            for (int w = 0; w < wheelPairs; w++) {
                float wz = -length * 0.35f + w * (length * 0.70f / (wheelPairs - 1));
                glPushMatrix();
                glTranslatef(side * width * 0.52f, wheelRadius, wz);
                glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
                setMaterialColor(0.06f, 0.06f, 0.06f, 8.0f);
                glutSolidTorus(0.09, wheelRadius * 0.72, 8, 16);
                glPopMatrix();
            }
        }

        // ----------------------------------------------------------------------
        // BLUE FLASHING BEACON ON ROOF (Critical Requirement 1 & 4)
        // ----------------------------------------------------------------------
        float beaconY = (emergencySubtype == "FireTruck") ? height * 1.15f : height * 1.04f;
        float strobeEmission = beaconState ? 1.0f : 0.15f;

        // Left Beacon: Blue Strobe
        drawSolidBox(-width * 0.25f, beaconY, length * 0.15f, 0.22f, 0.16f, 0.22f,
                     0.05f, 0.45f, 1.0f, 32.0f, strobeEmission);
        // Right Beacon: Alternating Blue / Red Strobe
        drawSolidBox( width * 0.25f, beaconY, length * 0.15f, 0.22f, 0.16f, 0.22f,
                     beaconState ? 0.05f : 1.0f, beaconState ? 0.45f : 0.05f, beaconState ? 1.0f : 0.05f,
                     32.0f, strobeEmission);

        // Halo / Point Glow above siren
        if (beaconState) {
            drawSolidSphere(0.0f, beaconY + 0.10f, length * 0.15f, 0.18f, 0.2f, 0.6f, 1.0f, 1.0f);
        }

        // Headlights
        drawSolidSphere(-width * 0.35f, height * 0.35f, length * 0.51f, 0.15f, 1.0f, 1.0f, 0.85f, 0.9f);
        drawSolidSphere( width * 0.35f, height * 0.35f, length * 0.51f, 0.15f, 1.0f, 1.0f, 0.85f, 0.9f);

        glPopMatrix();
    }
};

// Global Vehicle Fleet
std::vector<Vehicle*> activeVehicles;

// ------------------------------------------------------------------------------
// SCENE ENVIRONMENT: ROADS, MARKINGS, SIDEWALKS, BUILDINGS, TREES
// ------------------------------------------------------------------------------

void drawRoads() {
    // Ground Base (Grass terrain)
    drawSolidBox(0.0f, -0.22f, 0.0f, 110.0f, 0.35f, 110.0f, 0.18f, 0.38f, 0.18f, 4.0f);

    // East-West Asphalt Road
    drawSolidBox(0.0f, 0.0f, 0.0f, ROAD_LENGTH * 2.0f, 0.06f, ROAD_WIDTH, 0.14f, 0.15f, 0.17f, 8.0f);

    // North-South Asphalt Road
    drawSolidBox(0.0f, 0.01f, 0.0f, ROAD_WIDTH, 0.06f, ROAD_LENGTH * 2.0f, 0.14f, 0.15f, 0.17f, 8.0f);

    // Intersection Junction Center Clearway
    drawSolidBox(0.0f, 0.02f, 0.0f, ROAD_WIDTH, 0.06f, ROAD_WIDTH, 0.16f, 0.17f, 0.19f, 8.0f);
}

void drawLaneMarkings() {
    // Center Yellow Double Dividers & Dashed White Lane Lines
    for (float d = STOP_LINE_DIST + 1.5f; d < ROAD_LENGTH - 2.0f; d += 3.2f) {
        // East-West Road Markings
        drawSolidBox(-d, 0.05f, 0.0f, 1.6f, 0.02f, 0.14f, 0.95f, 0.85f, 0.15f); // Double Yellow Divider
        drawSolidBox( d, 0.05f, 0.0f, 1.6f, 0.02f, 0.14f, 0.95f, 0.85f, 0.15f);
        // White Lane Dashes
        drawSolidBox(-d, 0.05f, -LANE_WIDTH, 1.4f, 0.02f, 0.12f, 0.92f, 0.92f, 0.92f);
        drawSolidBox(-d, 0.05f,  LANE_WIDTH, 1.4f, 0.02f, 0.12f, 0.92f, 0.92f, 0.92f);
        drawSolidBox( d, 0.05f, -LANE_WIDTH, 1.4f, 0.02f, 0.12f, 0.92f, 0.92f, 0.92f);
        drawSolidBox( d, 0.05f,  LANE_WIDTH, 1.4f, 0.02f, 0.12f, 0.92f, 0.92f, 0.92f);

        // North-South Road Markings
        drawSolidBox(0.0f, 0.05f, -d, 0.14f, 0.02f, 1.6f, 0.95f, 0.85f, 0.15f);
        drawSolidBox(0.0f, 0.05f,  d, 0.14f, 0.02f, 1.6f, 0.95f, 0.85f, 0.15f);
        // White Lane Dashes
        drawSolidBox(-LANE_WIDTH, 0.05f, -d, 0.12f, 0.02f, 1.4f, 0.92f, 0.92f, 0.92f);
        drawSolidBox( LANE_WIDTH, 0.05f, -d, 0.12f, 0.02f, 1.4f, 0.92f, 0.92f, 0.92f);
        drawSolidBox(-LANE_WIDTH, 0.05f,  d, 0.12f, 0.02f, 1.4f, 0.92f, 0.92f, 0.92f);
        drawSolidBox( LANE_WIDTH, 0.05f,  d, 0.12f, 0.02f, 1.4f, 0.92f, 0.92f, 0.92f);
    }

    // Solid White Stop Lines on All 4 Approaches
    drawSolidBox(-STOP_LINE_DIST, 0.05f, -HALF_ROAD / 2.0f, 0.35f, 0.02f, HALF_ROAD, 0.95f, 0.95f, 0.95f);
    drawSolidBox( STOP_LINE_DIST, 0.05f,  HALF_ROAD / 2.0f, 0.35f, 0.02f, HALF_ROAD, 0.95f, 0.95f, 0.95f);
    drawSolidBox( HALF_ROAD / 2.0f, 0.05f, -STOP_LINE_DIST, HALF_ROAD, 0.02f, 0.35f, 0.95f, 0.95f, 0.95f);
    drawSolidBox(-HALF_ROAD / 2.0f, 0.05f,  STOP_LINE_DIST, HALF_ROAD, 0.02f, 0.35f, 0.95f, 0.95f, 0.95f);

    // Zebra Crossings on All 4 Approaches
    float zDist = STOP_LINE_DIST - 1.1f;
    for (int i = 0; i < 9; i++) {
        float stripePos = -HALF_ROAD + 0.6f + i * 1.3f;
        // East-West Approaches
        drawSolidBox(-zDist, 0.05f, stripePos, 1.2f, 0.02f, 0.55f, 0.92f, 0.92f, 0.95f);
        drawSolidBox( zDist, 0.05f, stripePos, 1.2f, 0.02f, 0.55f, 0.92f, 0.92f, 0.95f);
        // North-South Approaches
        drawSolidBox(stripePos, 0.05f, -zDist, 0.55f, 0.02f, 1.2f, 0.92f, 0.92f, 0.95f);
        drawSolidBox(stripePos, 0.05f,  zDist, 0.55f, 0.02f, 1.2f, 0.92f, 0.92f, 0.95f);
    }
}

void drawSidewalks() {
    float swWidth  = 4.5f;
    float swHeight = 0.22f;
    float swOffset = HALF_ROAD + swWidth / 2.0f; // 6.0 + 2.25 = 8.25

    // Concrete Sidewalk Pavement Blocks in 4 Quadrants
    drawSolidBox(-28.0f, swHeight / 2.0f, -28.0f, 44.0f - swWidth, swHeight, 44.0f - swWidth, 0.48f, 0.50f, 0.52f, 8.0f);
    drawSolidBox( 28.0f, swHeight / 2.0f, -28.0f, 44.0f - swWidth, swHeight, 44.0f - swWidth, 0.48f, 0.50f, 0.52f, 8.0f);
    drawSolidBox(-28.0f, swHeight / 2.0f,  28.0f, 44.0f - swWidth, swHeight, 44.0f - swWidth, 0.48f, 0.50f, 0.52f, 8.0f);
    drawSolidBox( 28.0f, swHeight / 2.0f,  28.0f, 44.0f - swWidth, swHeight, 44.0f - swWidth, 0.48f, 0.50f, 0.52f, 8.0f);

    // Curb Edges
    float curbColor = 0.65f;
    drawSolidBox(-ROAD_LENGTH / 2.0f, swHeight * 0.6f, -HALF_ROAD - 0.12f, ROAD_LENGTH, swHeight, 0.24f, curbColor, curbColor, curbColor);
    drawSolidBox( ROAD_LENGTH / 2.0f, swHeight * 0.6f, -HALF_ROAD - 0.12f, ROAD_LENGTH, swHeight, 0.24f, curbColor, curbColor, curbColor);
    drawSolidBox(-ROAD_LENGTH / 2.0f, swHeight * 0.6f,  HALF_ROAD + 0.12f, ROAD_LENGTH, swHeight, 0.24f, curbColor, curbColor, curbColor);
    drawSolidBox( ROAD_LENGTH / 2.0f, swHeight * 0.6f,  HALF_ROAD + 0.12f, ROAD_LENGTH, swHeight, 0.24f, curbColor, curbColor, curbColor);

    drawSolidBox(-HALF_ROAD - 0.12f, swHeight * 0.6f, -ROAD_LENGTH / 2.0f, 0.24f, swHeight, ROAD_LENGTH, curbColor, curbColor, curbColor);
    drawSolidBox(-HALF_ROAD - 0.12f, swHeight * 0.6f,  ROAD_LENGTH / 2.0f, 0.24f, swHeight, ROAD_LENGTH, curbColor, curbColor, curbColor);
    drawSolidBox( HALF_ROAD + 0.12f, swHeight * 0.6f, -ROAD_LENGTH / 2.0f, 0.24f, swHeight, ROAD_LENGTH, curbColor, curbColor, curbColor);
    drawSolidBox( HALF_ROAD + 0.12f, swHeight * 0.6f,  ROAD_LENGTH / 2.0f, 0.24f, swHeight, ROAD_LENGTH, curbColor, curbColor, curbColor);
}

void drawBuildings() {
    // Quadrant 1: North-West Corner (Office Tower & Retail)
    drawSolidBox(-22.0f, 5.5f, -22.0f, 14.0f, 11.0f, 14.0f, 0.22f, 0.32f, 0.45f, 32.0f); // High-rise
    drawSolidBox(-13.0f, 2.5f, -24.0f,  8.0f,  5.0f,  8.0f, 0.55f, 0.50f, 0.45f);

    // Window Rows on NW Tower
    for (float floor = 2.0f; floor < 10.0f; floor += 2.0f) {
        for (float wx = -27.0f; wx <= -17.0f; wx += 2.5f) {
            drawSolidBox(wx, floor, -14.95f, 1.2f, 1.0f, 0.12f, 0.85f, 0.92f, 0.98f, 64.0f, 0.4f);
        }
    }

    // Quadrant 2: North-East Corner (Commercial Plaza)
    drawSolidBox( 22.0f, 7.0f, -22.0f, 16.0f, 14.0f, 12.0f, 0.40f, 0.25f, 0.25f, 16.0f);
    drawSolidBox( 13.0f, 3.0f, -22.0f,  6.0f,  6.0f,  8.0f, 0.70f, 0.65f, 0.55f);

    // Quadrant 3: South-West Corner (City Hospital / Emergency Center)
    drawSolidBox(-22.0f, 6.0f,  22.0f, 15.0f, 12.0f, 15.0f, 0.82f, 0.82f, 0.85f, 16.0f);
    // Hospital Red Cross on Facade
    drawSolidBox(-14.45f, 8.5f, 22.0f, 0.12f, 2.2f, 0.7f, 0.9f, 0.05f, 0.05f, 16.0f, 0.6f);
    drawSolidBox(-14.45f, 8.5f, 22.0f, 0.12f, 0.7f, 2.2f, 0.9f, 0.05f, 0.05f, 16.0f, 0.6f);

    // Quadrant 4: South-East Corner (Civic Center)
    drawSolidBox( 22.0f, 4.5f,  22.0f, 14.0f,  9.0f, 14.0f, 0.30f, 0.40f, 0.35f, 24.0f);
    drawSolidBox( 24.0f, 8.0f,  24.0f,  8.0f,  7.0f,  8.0f, 0.25f, 0.35f, 0.40f);
}

void drawTree(float x, float z, float scale = 1.0f) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glScalef(scale, scale, scale);
    // Trunk
    drawSolidCylinder(0.0f, 0.0f, 0.0f, 0.22f, 1.8f, 0.35f, 0.22f, 0.12f);
    // Conical Foliage (3 Layers)
    drawSolidCone(0.0f, 1.4f, 0.0f, 1.35f, 2.2f, 0.12f, 0.45f, 0.15f);
    drawSolidCone(0.0f, 2.4f, 0.0f, 1.10f, 1.8f, 0.15f, 0.52f, 0.18f);
    drawSolidCone(0.0f, 3.2f, 0.0f, 0.80f, 1.5f, 0.18f, 0.58f, 0.22f);
    glPopMatrix();
}

void drawStreetlight(float x, float z, float facingAngle) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(facingAngle, 0.0f, 1.0f, 0.0f);

    // Vertical Steel Pole
    drawSolidCylinder(0.0f, 0.0f, 0.0f, 0.10f, 4.8f, 0.25f, 0.28f, 0.30f);
    // Curved Overhang Arm
    drawSolidBox(0.0f, 4.8f, 0.65f, 0.10f, 0.10f, 1.30f, 0.25f, 0.28f, 0.30f);
    // Lamp Fixture
    drawSolidBox(0.0f, 4.75f, 1.25f, 0.25f, 0.12f, 0.40f, 0.20f, 0.22f, 0.24f);
    // Glowing Light Bulb
    drawSolidSphere(0.0f, 4.68f, 1.25f, 0.14f, 1.0f, 0.95f, 0.8f, 0.9f);
    glPopMatrix();
}

void drawTreesAndStreetlights() {
    // Trees planted along sidewalks in all 4 corners
    drawTree(-9.5f, -14.0f, 1.1f);
    drawTree(-9.5f, -20.0f, 1.2f);
    drawTree(-14.0f, -9.5f, 1.0f);
    drawTree(-20.0f, -9.5f, 1.15f);

    drawTree( 9.5f, -14.0f, 1.05f);
    drawTree( 9.5f, -20.0f, 1.1f);
    drawTree( 14.0f, -9.5f, 1.2f);
    drawTree( 20.0f, -9.5f, 1.0f);

    drawTree(-9.5f,  14.0f, 1.15f);
    drawTree(-9.5f,  20.0f, 1.05f);
    drawTree(-14.0f,  9.5f, 1.1f);
    drawTree(-20.0f,  9.5f, 1.25f);

    drawTree( 9.5f,  14.0f, 1.1f);
    drawTree( 9.5f,  20.0f, 1.2f);
    drawTree( 14.0f,  9.5f, 1.05f);
    drawTree( 20.0f,  9.5f, 1.15f);

    // Streetlights at approaches
    drawStreetlight(-7.8f, -12.0f, 90.0f);
    drawStreetlight( 7.8f,  12.0f, 270.0f);
    drawStreetlight(-12.0f,  7.8f, 0.0f);
    drawStreetlight( 12.0f, -7.8f, 180.0f);
}

// ------------------------------------------------------------------------------
// CRITICAL REQUIREMENT 1 & 2: FLAT 2D TRAFFIC SIGNAL BOARD (STRICTLY NON-ROTATING)
// - The signal itself remains a simple, flat 2D traffic light.
// - Clean rectangular signal housing with clearly visible RED, YELLOW, GREEN circular lamps.
// - Fixed and upright at designated corners facing oncoming lane.
// - DEDICATED SEPARATE BLUE EMERGENCY INDICATOR mounted on top (NOT a 4th lamp inside the housing).
// - Activates/strobes ONLY when an emergency vehicle is approaching / preemption active.
// ------------------------------------------------------------------------------
void drawTrafficSignal(float x, float z, float facingAngle,
                       bool redOn, bool yellowOn, bool greenOn, bool blueBeaconOn) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(facingAngle, 0.0f, 1.0f, 0.0f); // Fixed static orientation towards incoming lane

    // Upright Signal Post (Fixed vertical steel pole)
    drawSolidCylinder(0.0f, 0.0f, 0.0f, 0.09f, 3.85f, 0.22f, 0.24f, 0.26f);

    // Flat 2D Rectangular Signal Housing (Thin dark plate facing +Z)
    // Housing Board: Width = 0.68f, Height = 1.95f, Depth = 0.04f, centered at y = 4.60f
    drawSolidBox(0.0f, 4.60f, 0.0f, 0.68f, 1.95f, 0.04f, 0.08f, 0.09f, 0.10f, 8.0f);
    // Outer protective border trim
    drawSolidBox(0.0f, 4.60f, -0.01f, 0.72f, 1.99f, 0.02f, 0.15f, 0.16f, 0.18f);

    // --------------------------------------------------------------------------
    // EXACTLY THREE CIRCULAR LAMPS INSIDE THE FLAT 2D HOUSING (Red, Yellow, Green)
    // --------------------------------------------------------------------------
    // 1. Red Lamp (Top)
    float redEmiss = redOn ? 1.0f : 0.05f;
    drawFlatCircle2D(0.0f, 5.18f, 0.025f, 0.21f,
                     redOn ? 1.0f : 0.30f, 0.04f, 0.04f, redEmiss);
    // Small 2D visor hood over red lamp
    drawSolidBox(0.0f, 5.40f, 0.10f, 0.48f, 0.04f, 0.18f, 0.06f, 0.06f, 0.07f);

    // 2. Yellow Lamp (Middle)
    float yellowEmiss = yellowOn ? 1.0f : 0.05f;
    drawFlatCircle2D(0.0f, 4.60f, 0.025f, 0.21f,
                     yellowOn ? 1.0f : 0.32f, yellowOn ? 0.85f : 0.22f, 0.04f, yellowEmiss);
    // Small 2D visor hood over yellow lamp
    drawSolidBox(0.0f, 4.82f, 0.10f, 0.48f, 0.04f, 0.18f, 0.06f, 0.06f, 0.07f);

    // 3. Green Lamp (Bottom)
    float greenEmiss = greenOn ? 1.0f : 0.05f;
    drawFlatCircle2D(0.0f, 4.02f, 0.025f, 0.21f,
                     0.04f, greenOn ? 1.0f : 0.28f, 0.12f, greenEmiss);
    // Small 2D visor hood over green lamp
    drawSolidBox(0.0f, 4.24f, 0.10f, 0.48f, 0.04f, 0.18f, 0.06f, 0.06f, 0.07f);

    // --------------------------------------------------------------------------
    // DEDICATED SEPARATE BLUE EMERGENCY INDICATOR BEACON
    // Mounted SEPARATELY on top of the signal housing (NOT a 4th normal signal lamp!)
    // Pulsing/strobe activation strictly tied to emergency vehicle preemption
    // --------------------------------------------------------------------------
    // Stalk bracket connecting housing top to beacon
    drawSolidCylinder(0.0f, 5.58f, 0.0f, 0.05f, 0.14f, 0.20f, 0.22f, 0.24f);

    // Emergency beacon fixture
    float emergEmiss = blueBeaconOn ? (beaconState ? 1.0f : 0.35f) : 0.0f;
    float br = blueBeaconOn ? 0.10f : 0.08f;
    float bg = blueBeaconOn ? (beaconState ? 0.70f : 0.40f) : 0.12f;
    float bb = blueBeaconOn ? 1.0f : 0.20f;

    // Small beacon cylindrical housing and dome on top
    drawSolidCylinder(0.0f, 5.72f, 0.0f, 0.11f, 0.18f, br, bg, bb);
    drawSolidSphere(0.0f, 5.90f, 0.0f, 0.11f, br, bg, bb, emergEmiss);

    // Expanding optical strobe halo when beacon is active and flashing
    if (blueBeaconOn && beaconState) {
        drawFlatCircle2D(0.0f, 5.85f, 0.02f, 0.36f, 0.25f, 0.75f, 1.0f, 1.0f);
    }

    glPopMatrix();
}

void drawAllSignals() {
    // Signal state resolution based on active phase or emergency override
    bool ewRed = true, ewYellow = false, ewGreen = false;
    bool nsRed = true, nsYellow = false, nsGreen = false;

    if (emergencyActive) {
        // Emergency route is granted Green wave; conflicting direction held Red
        // Default emergency corridor is Northbound (moves towards -Z) or Eastbound
        ewRed = true;  ewYellow = false; ewGreen = false;
        nsRed = false; nsYellow = false; nsGreen = true;
    } else {
        switch (currentPhase) {
            case PHASE_EW_GREEN:
                ewGreen = true;  ewYellow = false; ewRed = false;
                nsGreen = false; nsYellow = false; nsRed = true;
                break;
            case PHASE_EW_YELLOW:
                ewGreen = false; ewYellow = true;  ewRed = false;
                nsGreen = false; nsYellow = false; nsRed = true;
                break;
            case PHASE_ALL_RED_1:
            case PHASE_ALL_RED_2:
            case PHASE_EMERGENCY_CLEARANCE:
                ewGreen = false; ewYellow = false; ewRed = true;
                nsGreen = false; nsYellow = false; nsRed = true;
                break;
            case PHASE_NS_GREEN:
                ewGreen = false; ewYellow = false; ewRed = true;
                nsGreen = true;  nsYellow = false; nsRed = false;
                break;
            case PHASE_NS_YELLOW:
                ewGreen = false; ewYellow = false; ewRed = true;
                nsGreen = false; nsYellow = true;  nsRed = false;
                break;
        }
    }

    // 4 Fixed Traffic Signals at Designated Road Corners (Non-rotating)
    // Corner 1: South-West (Controls Eastbound traffic, faces -X)
    drawTrafficSignal(-HALF_ROAD - 1.2f,  HALF_ROAD + 1.2f, 90.0f,
                      ewRed, ewYellow, ewGreen, emergencyActive);

    // Corner 2: North-East (Controls Westbound traffic, faces +X)
    drawTrafficSignal( HALF_ROAD + 1.2f, -HALF_ROAD - 1.2f, 270.0f,
                      ewRed, ewYellow, ewGreen, emergencyActive);

    // Corner 3: North-West (Controls Southbound traffic, faces -Z)
    drawTrafficSignal(-HALF_ROAD - 1.2f, -HALF_ROAD - 1.2f, 0.0f,
                      nsRed, nsYellow, nsGreen, emergencyActive);

    // Corner 4: South-East (Controls Northbound traffic, faces +Z)
    drawTrafficSignal( HALF_ROAD + 1.2f,  HALF_ROAD + 1.2f, 180.0f,
                      nsRed, nsYellow, nsGreen, emergencyActive);
}

// ------------------------------------------------------------------------------
// DRAW ALL VEHICLES
// ------------------------------------------------------------------------------
void drawVehicles() {
    for (size_t i = 0; i < activeVehicles.size(); i++) {
        activeVehicles[i]->draw();
    }
}

// ------------------------------------------------------------------------------
// CO.5: COHEN-SUTHERLAND 3D ROAD CLIPPING ALGORITHM VISUALIZER
// ------------------------------------------------------------------------------
void drawCohenSutherlandClipping() {
    if (!clippingDemoActive) return;

    glDisable(GL_LIGHTING);
    glLineWidth(2.5f);

    // 1. Draw Junction Clipping Window Bounding Box (Cyan)
    glColor3f(0.1f, 0.9f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex3f(CLIP_XMIN, 0.15f, CLIP_ZMIN);
        glVertex3f(CLIP_XMAX, 0.15f, CLIP_ZMIN);
        glVertex3f(CLIP_XMAX, 0.15f, CLIP_ZMAX);
        glVertex3f(CLIP_XMIN, 0.15f, CLIP_ZMAX);
    glEnd();

    // 2. Draw Translucent Clipping Volume Floor
    glEnable(GL_BLEND);
    glColor4f(0.0f, 0.5f, 0.8f, 0.15f);
    glBegin(GL_QUADS);
        glVertex3f(CLIP_XMIN, 0.12f, CLIP_ZMIN);
        glVertex3f(CLIP_XMAX, 0.12f, CLIP_ZMIN);
        glVertex3f(CLIP_XMAX, 0.12f, CLIP_ZMAX);
        glVertex3f(CLIP_XMIN, 0.12f, CLIP_ZMAX);
    glEnd();

    // 3. For each active vehicle, test its path using Cohen-Sutherland Algorithm
    for (size_t vi = 0; vi < activeVehicles.size(); vi++) {
        Vehicle* v = activeVehicles[vi];
        float x0 = v->x - v->vx * 18.0f;
        float z0 = v->z - v->vz * 18.0f;
        float x1 = v->x + v->vx * 6.0f;
        float z1 = v->z + v->vz * 6.0f;

        float cx0 = x0, cz0 = z0, cx1 = x1, cz1 = z1;
        int code0 = computeCSOutCode(cx0, cz0);
        int code1 = computeCSOutCode(cx1, cz1);
        bool accept = false;

        for (int iter = 0; iter < 6; iter++) {
            if ((code0 | code1) == 0) { // Trivial Accept
                accept = true;
                break;
            } else if (code0 & code1) { // Trivial Reject
                break;
            } else {
                // Calculate intersection with boundary
                float x = 0.0f, z = 0.0f;
                int outcodeOut = code0 ? code0 : code1;

                if (outcodeOut & CS_TOP) {
                    x = cx0 + (cx1 - cx0) * (CLIP_ZMAX - cz0) / (cz1 - cz0);
                    z = CLIP_ZMAX;
                } else if (outcodeOut & CS_BOTTOM) {
                    x = cx0 + (cx1 - cx0) * (CLIP_ZMIN - cz0) / (cz1 - cz0);
                    z = CLIP_ZMIN;
                } else if (outcodeOut & CS_RIGHT) {
                    z = cz0 + (cz1 - cz0) * (CLIP_XMAX - cx0) / (cx1 - cx0);
                    x = CLIP_XMAX;
                } else if (outcodeOut & CS_LEFT) {
                    z = cz0 + (cz1 - cz0) * (CLIP_XMIN - cx0) / (cx1 - cx0);
                    x = CLIP_XMIN;
                }

                if (outcodeOut == code0) {
                    cx0 = x; cz0 = z;
                    code0 = computeCSOutCode(cx0, cz0);
                } else {
                    cx1 = x; cz1 = z;
                    code1 = computeCSOutCode(cx1, cz1);
                }
            }
        }

        // Draw Full Unclipped Path Line in Dark Red (Rejected Portion)
        glColor3f(0.85f, 0.2f, 0.2f);
        glBegin(GL_LINES);
            glVertex3f(x0, 0.18f, z0);
            glVertex3f(x1, 0.18f, z1);
        glEnd();

        // If line intersects/crosses window, draw Accepted Clipped Line in Bright Green
        if (accept) {
            glColor3f(0.15f, 1.0f, 0.35f);
            glBegin(GL_LINES);
                glVertex3f(cx0, 0.22f, cz0);
                glVertex3f(cx1, 0.22f, cz1);
            glEnd();

            // Intersection Boundary Points (Yellow dots)
            drawSolidSphere(cx0, 0.25f, cz0, 0.20f, 1.0f, 0.85f, 0.1f, 0.8f);
            drawSolidSphere(cx1, 0.25f, cz1, 0.20f, 1.0f, 0.85f, 0.1f, 0.8f);
        }
    }

    glLineWidth(1.0f);
    glEnable(GL_LIGHTING);
}

// ------------------------------------------------------------------------------
// 2D ON-SCREEN TELEMETRY & HUD OVERLAY
// ------------------------------------------------------------------------------
void renderBitmapString(float x, float y, void* font, const std::string& str) {
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(font, c);
    }
}

void drawHUDOverlay() {
    // Switch to 2D Orthographic Mode for UI rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Top Header Banner
    glColor4f(0.04f, 0.06f, 0.12f, 0.88f);
    glBegin(GL_QUADS);
        glVertex2i(15, windowHeight - 15);
        glVertex2i(windowWidth - 15, windowHeight - 15);
        glVertex2i(windowWidth - 15, windowHeight - 85);
        glVertex2i(15, windowHeight - 85);
    glEnd();

    // Top Border Line
    glColor3f(0.27f, 0.90f, 1.0f); // Electric Cyan
    glLineWidth(2.0f);
    glBegin(GL_LINES);
        glVertex2i(15, windowHeight - 15);
        glVertex2i(windowWidth - 15, windowHeight - 15);
        glVertex2i(15, windowHeight - 85);
        glVertex2i(windowWidth - 15, windowHeight - 85);
    glEnd();

    // Title Text
    glColor3f(1.0f, 1.0f, 1.0f);
    renderBitmapString(30, windowHeight - 40, GLUT_BITMAP_HELVETICA_18,
                       "SMART TRAFFIC 3D - EMERGENCY VEHICLE PRIORITY CONTROL CENTER");

    // Phase Status & Priority Mode Text
    std::string phaseStr = "";
    if (emergencyActive) {
        glColor3f(1.0f, 0.25f, 0.30f); // Bright Red
        phaseStr = "MODE: [!] EMERGENCY PRIORITY OVERRIDE ACTIVE | UNIT: " + emergencyUnitName + " | CORRIDOR: NORTH-SOUTH GREEN WAVE";
    } else {
        glColor3f(0.25f, 0.92f, 0.55f); // Green
        std::ostringstream ss;
        ss << "MODE: AUTO SIGNAL CYCLING | ";
        if (currentPhase == PHASE_EW_GREEN)  ss << "EAST-WEST: GREEN (" << (int)phaseTimer << "s) | NORTH-SOUTH: RED";
        else if (currentPhase == PHASE_EW_YELLOW) ss << "EAST-WEST: YELLOW (" << (int)phaseTimer << "s) | NORTH-SOUTH: RED";
        else if (currentPhase == PHASE_NS_GREEN)  ss << "NORTH-SOUTH: GREEN (" << (int)phaseTimer << "s) | EAST-WEST: RED";
        else if (currentPhase == PHASE_NS_YELLOW) ss << "NORTH-SOUTH: YELLOW (" << (int)phaseTimer << "s) | EAST-WEST: RED";
        else ss << "ALL-RED SAFETY INTERVAL (" << (int)phaseTimer << "s)";
        phaseStr = ss.str();
    }
    renderBitmapString(30, windowHeight - 65, GLUT_BITMAP_HELVETICA_12, phaseStr);

    // Right Side Telemetry Stats
    std::ostringstream statStream;
    statStream << "FLEET: " << activeVehicles.size() << " | PRIORITY: " << (emergencyActive ? 150 : 25)
               << " | AVG WAIT: " << avgWaitTimeSec << "s";
    glColor3f(0.95f, 0.75f, 0.25f); // Amber
    renderBitmapString(windowWidth - 360, windowHeight - 40, GLUT_BITMAP_HELVETICA_12, statStream.str());

    // Emergency Status Tag
    if (emergencyActive) {
        glColor3f(0.2f, 0.6f, 1.0f); // Blue
        renderBitmapString(windowWidth - 360, windowHeight - 65, GLUT_BITMAP_HELVETICA_12,
                           "BLUE STROBE ACTIVE | PREEMPTION INTERRUPT ENGAGED");
    } else {
        glColor3f(0.6f, 0.65f, 0.75f);
        renderBitmapString(windowWidth - 360, windowHeight - 65, GLUT_BITMAP_HELVETICA_12,
                           "STANDBY: Press 'E' for Ambulance | 'F' for Fire Truck");
    }

    // CO.5 Clipping Telemetry Sub-banner
    if (clippingDemoActive) {
        glColor3f(0.1f, 0.95f, 1.0f); // Cyan
        renderBitmapString(30, windowHeight - 80, GLUT_BITMAP_HELVETICA_10,
                           "CO.5 CLIPPING: COHEN-SUTHERLAND 4-BIT OUTCODE ACTIVE [Window: -16m to +16m | Green: Accepted | Red: Clipped]");
    }
    if (hardwareClipPlaneActive) {
        glColor3f(1.0f, 0.75f, 0.2f); // Amber
        renderBitmapString(windowWidth - 420, windowHeight - 80, GLUT_BITMAP_HELVETICA_10,
                           "CO.5 HARDWARE CLIP: GL_CLIP_PLANE0 (x <= 16.0) ENGAGED");
    }

    // Bottom Controls Banner
    glColor4f(0.04f, 0.06f, 0.12f, 0.85f);
    glBegin(GL_QUADS);
        glVertex2i(15, 60);
        glVertex2i(windowWidth - 15, 60);
        glVertex2i(windowWidth - 15, 15);
        glVertex2i(15, 15);
    glEnd();

    glColor3f(0.27f, 0.90f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2i(15, 60);
        glVertex2i(windowWidth - 15, 60);
        glVertex2i(windowWidth - 15, 15);
        glVertex2i(15, 15);
    glEnd();

    glColor3f(0.85f, 0.90f, 0.95f);
    renderBitmapString(30, 32, GLUT_BITMAP_HELVETICA_12,
        "[E]: Ambulance | [F]: Fire Truck | [SPACE]: Pause | [S]: Manual Signal | [C]: Cohen-Sutherland Clip | [P]: Hardware Clip | [V]: View | [Arrows]: Orbit | [ESC]: Exit");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ------------------------------------------------------------------------------
// EMERGENCY DISPATCH & CONTROL LOGIC
// ------------------------------------------------------------------------------
void dispatchEmergencyVehicle(std::string type) {
    if (emergencyActive) return; // Already active

    emergencyActive = true;
    emergencyPassed = false;
    emergencyUnitName = (type == "FireTruck") ? "FIRE TRUCK #104" : "AMBULANCE #103";
    preEmergencyPhase = currentPhase;
    activePriorityScore = 150;

    // Push event to system stack (LIFO)
    systemEventStack.push_back("PREEMPTION ENGAGED: " + emergencyUnitName);

    // Create and add the emergency vehicle approaching on Northbound lane (moves towards -Z)
    int emergId = (type == "FireTruck") ? 104 : 103;
    EmergencyVehicle* ev = new EmergencyVehicle(emergId, type, 25, 3);
    ev->z = ROAD_LENGTH - 4.0f; // Starts approach from South
    activeVehicles.push_back(ev);
    activeEmergencyId = emergId;

    std::cout << "[EMERGENCY DISPATCH] " << emergencyUnitName
              << " deployed. Priority Score = " << ev->getPriority() << std::endl;
}

void resetSimulation() {
    emergencyActive = false;
    emergencyPassed = false;
    emergencyUnitName = "NONE";
    currentPhase = PHASE_EW_GREEN;
    phaseTimer = DURATION_GREEN;

    // Clean up vehicles
    for (Vehicle* v : activeVehicles) {
        delete v;
    }
    activeVehicles.clear();

    // Re-initialize standard traffic fleet with diverse vehicle models
    // Lane 0: Eastbound (+X)
    activeVehicles.push_back(new Vehicle(101, "Car", 10, 0, 2.2f, 1.2f, 0.9f, 0.2f, 0.5f, 0.9f));
    activeVehicles.back()->x = -24.0f;

    activeVehicles.push_back(new Vehicle(102, "Bike", 14, 0, 1.4f, 0.7f, 0.8f, 0.95f, 0.55f, 0.1f));
    activeVehicles.back()->x = -38.0f;

    // Lane 1: Westbound (-X)
    activeVehicles.push_back(new Vehicle(105, "Bus", 18, 1, 4.2f, 1.4f, 1.4f, 0.65f, 0.25f, 0.85f));
    activeVehicles.back()->x = 26.0f;

    activeVehicles.push_back(new Vehicle(106, "Car", 12, 1, 2.2f, 1.2f, 0.9f, 0.85f, 0.15f, 0.2f));
    activeVehicles.back()->x = 40.0f;

    // Lane 2: Southbound (+Z)
    activeVehicles.push_back(new Vehicle(107, "Car", 14, 2, 2.2f, 1.2f, 0.9f, 0.15f, 0.75f, 0.3f));
    activeVehicles.back()->z = -22.0f;

    activeVehicles.push_back(new Vehicle(108, "Bike", 6, 2, 1.4f, 0.7f, 0.8f, 0.9f, 0.85f, 0.2f));
    activeVehicles.back()->z = -36.0f;

    // Lane 3: Northbound (-Z)
    activeVehicles.push_back(new Vehicle(109, "Car", 8, 3, 2.2f, 1.2f, 0.9f, 0.75f, 0.75f, 0.8f));
    activeVehicles.back()->z = 24.0f;

    std::cout << "[SIMULATION RESET] 4-way intersection reset with 7 standard vehicles." << std::endl;
}

// ------------------------------------------------------------------------------
// REAL VEHICLE MOVEMENT & COLLISION LOGIC
// ------------------------------------------------------------------------------
void updateTraffic(int value) {
    if (!simulationPaused) {
        float dt = 0.016f; // ~60 FPS update
        simulationTime += dt;

        // Beacon strobe oscillation (Alternates every 120ms)
        beaconFlashTimer += dt;
        if (beaconFlashTimer > 0.12f) {
            beaconState = !beaconState;
            beaconFlashTimer = 0.0f;
        }

        // Signal Phase State Machine
        if (!emergencyActive) {
            phaseTimer -= dt;
            if (phaseTimer <= 0.0f) {
                switch (currentPhase) {
                    case PHASE_EW_GREEN:
                        currentPhase = PHASE_EW_YELLOW;
                        phaseTimer = DURATION_YELLOW;
                        break;
                    case PHASE_EW_YELLOW:
                        currentPhase = PHASE_ALL_RED_1;
                        phaseTimer = DURATION_ALL_RED;
                        break;
                    case PHASE_ALL_RED_1:
                        currentPhase = PHASE_NS_GREEN;
                        phaseTimer = DURATION_GREEN;
                        break;
                    case PHASE_NS_GREEN:
                        currentPhase = PHASE_NS_YELLOW;
                        phaseTimer = DURATION_YELLOW;
                        break;
                    case PHASE_NS_YELLOW:
                        currentPhase = PHASE_ALL_RED_2;
                        phaseTimer = DURATION_ALL_RED;
                        break;
                    case PHASE_ALL_RED_2:
                    default:
                        currentPhase = PHASE_EW_GREEN;
                        phaseTimer = DURATION_GREEN;
                        break;
                }
            }
        }

        // Vehicle Movement & Collision Handling
        for (size_t i = 0; i < activeVehicles.size(); i++) {
            Vehicle* v = activeVehicles[i];

            // 1. Check Traffic Signal for this Vehicle's Corridor
            bool signalAllowsGo = false;
            if (emergencyActive) {
                // Emergency vehicle moves freely; conflicting traffic held
                signalAllowsGo = (v->isEmergency() || (v->lane == 2 || v->lane == 3));
                if (!v->isEmergency() && (v->lane == 0 || v->lane == 1)) signalAllowsGo = false;
            } else {
                if (v->lane == 0 || v->lane == 1) { // East-West
                    signalAllowsGo = (currentPhase == PHASE_EW_GREEN);
                } else { // North-South
                    signalAllowsGo = (currentPhase == PHASE_NS_GREEN);
                }
            }

            // 2. Check Distance to Stop Line
            float distToStop = 999.0f;
            if (v->lane == 0) distToStop = -STOP_LINE_DIST - v->x; // Moving +X
            else if (v->lane == 1) distToStop = v->x - STOP_LINE_DIST; // Moving -X
            else if (v->lane == 2) distToStop = -STOP_LINE_DIST - v->z; // Moving +Z
            else if (v->lane == 3) distToStop = v->z - STOP_LINE_DIST; // Moving -Z

            // 3. Collision Avoidance (Lead Vehicle Detection)
            float leadDistance = 999.0f;
            for (size_t j = 0; j < activeVehicles.size(); j++) {
                if (i == j) continue;
                Vehicle* other = activeVehicles[j];
                if (v->lane != other->lane) continue;

                float gap = 999.0f;
                if (v->lane == 0 && other->x > v->x) gap = (other->x - v->x) - (other->getLength() / 2.0f + v->getLength() / 2.0f);
                else if (v->lane == 1 && other->x < v->x) gap = (v->x - other->x) - (other->getLength() / 2.0f + v->getLength() / 2.0f);
                else if (v->lane == 2 && other->z > v->z) gap = (other->z - v->z) - (other->getLength() / 2.0f + v->getLength() / 2.0f);
                else if (v->lane == 3 && other->z < v->z) gap = (v->z - other->z) - (other->getLength() / 2.0f + v->getLength() / 2.0f);

                if (gap > 0.0f && gap < leadDistance) {
                    leadDistance = gap;
                }
            }

            // 4. Compute Desired Speed
            float targetSpeed = v->isEmergency() ? 0.16f : 0.085f;
            const float safeFollowGap = 3.5f;

            if (leadDistance < safeFollowGap) {
                targetSpeed = 0.0f; // Stop to prevent rear-end collision
            } else if (!v->hasCrossed && distToStop > 0.0f && distToStop < 7.0f && !signalAllowsGo) {
                targetSpeed = 0.0f; // Stop behind stop line on red signal
            } else if (leadDistance < safeFollowGap + 4.0f) {
                targetSpeed *= (leadDistance - safeFollowGap) / 4.0f; // Decelerate smoothly
            }

            // Smooth Acceleration / Braking
            if (v->speed < targetSpeed) v->speed = std::min(targetSpeed, v->speed + 0.006f);
            else if (v->speed > targetSpeed) v->speed = std::max(targetSpeed, v->speed - 0.015f);

            // Position Update
            v->x += v->vx * v->speed;
            v->z += v->vz * v->speed;

            // Check if vehicle crossed intersection center
            if (!v->hasCrossed) {
                if (v->lane == 0 && v->x > STOP_LINE_DIST) v->hasCrossed = true;
                else if (v->lane == 1 && v->x < -STOP_LINE_DIST) v->hasCrossed = true;
                else if (v->lane == 2 && v->z > STOP_LINE_DIST) v->hasCrossed = true;
                else if (v->lane == 3 && v->z < -STOP_LINE_DIST) v->hasCrossed = true;
            }

            // Recycle vehicles when they exit the road bounds
            if (v->x > ROAD_LENGTH + 5.0f && v->lane == 0) {
                if (!v->isEmergency()) { v->x = -ROAD_LENGTH - 5.0f; v->hasCrossed = false; }
            } else if (v->x < -ROAD_LENGTH - 5.0f && v->lane == 1) {
                if (!v->isEmergency()) { v->x = ROAD_LENGTH + 5.0f; v->hasCrossed = false; }
            } else if (v->z > ROAD_LENGTH + 5.0f && v->lane == 2) {
                if (!v->isEmergency()) { v->z = -ROAD_LENGTH - 5.0f; v->hasCrossed = false; }
            } else if (v->z < -ROAD_LENGTH - 5.0f && v->lane == 3) {
                if (!v->isEmergency()) { v->z = ROAD_LENGTH + 5.0f; v->hasCrossed = false; }
            }

            // Check if Emergency Vehicle has cleared the junction completely
            if (v->isEmergency() && v->z < -HALF_ROAD - 8.0f && !emergencyPassed) {
                emergencyPassed = true;
                std::cout << "[EMERGENCY CLEARED] Emergency vehicle " << v->getType()
                          << " cleared intersection. Resuming normal traffic." << std::endl;
            }
        }

        // Resume Normal Signals After Emergency Clears
        if (emergencyActive && emergencyPassed) {
            // Find and delete emergency vehicle once off-screen
            for (auto it = activeVehicles.begin(); it != activeVehicles.end();) {
                if ((*it)->isEmergency() && (*it)->z < -ROAD_LENGTH) {
                    delete *it;
                    it = activeVehicles.erase(it);
                } else {
                    ++it;
                }
            }
            emergencyActive = false;
            currentPhase = preEmergencyPhase;
            phaseTimer = DURATION_GREEN;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, updateTraffic, 0); // 60 FPS timer
}

// ------------------------------------------------------------------------------
// OPENGL DISPLAY FUNCTION
// ------------------------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Camera Configuration
    if (viewMode == 0) { // Default Elevated 3D Perspective (Attractive complete view)
        float yawRad   = cameraYaw * PI / 180.0f;
        float pitchRad = cameraPitch * PI / 180.0f;
        float camX = cameraDistance * std::cos(pitchRad) * std::sin(yawRad);
        float camY = cameraDistance * std::sin(pitchRad);
        float camZ = cameraDistance * std::cos(pitchRad) * std::cos(yawRad);
        gluLookAt(camX, camY, camZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    } else if (viewMode == 1) { // 2D Overhead Top-Down Orthogonal View
        gluLookAt(0.01f, 52.0f, 0.01f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
    } else { // Driver Chase Cam (Follows first active vehicle)
        if (!activeVehicles.empty()) {
            Vehicle* lead = activeVehicles[0];
            gluLookAt(lead->x - lead->vx * 7.0f, 3.2f, lead->z - lead->vz * 7.0f,
                      lead->x + lead->vx * 8.0f, 1.0f, lead->z + lead->vz * 8.0f,
                      0.0f, 1.0f, 0.0f);
        } else {
            gluLookAt(36.0f, 28.0f, 36.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        }
    }

    // Dynamic Sunlight Positioning
    GLfloat sunPosition[] = { 30.0f, 45.0f, 35.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);

    // CO.5: Optional Hardware Clip Plane (Demonstrating OpenGL clipping plane)
    if (hardwareClipPlaneActive) {
        double clipEquation[] = { 1.0, 0.0, 0.0, 16.0 }; // Clips anything with x > 16
        glEnable(GL_CLIP_PLANE0);
        glClipPlane(GL_CLIP_PLANE0, clipEquation);
    }

    // Draw Complete 3D World
    drawRoads();
    drawLaneMarkings();
    drawSidewalks();
    drawBuildings();
    drawTreesAndStreetlights();
    drawAllSignals(); // Fixed non-rotating traffic signals
    drawVehicles();   // 3D vehicles with movement and beacons

    if (hardwareClipPlaneActive) {
        glDisable(GL_CLIP_PLANE0);
    }

    // CO.5: Cohen-Sutherland Line Clipping Visualizer Overlay
    if (clippingDemoActive) {
        drawCohenSutherlandClipping();
    }

    // Draw On-Screen Telemetry & HUD Overlay
    drawHUDOverlay();

    glutSwapBuffers();
}

// ------------------------------------------------------------------------------
// RESHAPE CALLBACK
// ------------------------------------------------------------------------------
void reshape(int w, int h) {
    windowWidth  = std::max(1, w);
    windowHeight = std::max(1, h);
    glViewport(0, 0, windowWidth, windowHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(48.0, (double)windowWidth / (double)windowHeight, 1.0, 250.0);
    glMatrixMode(GL_MODELVIEW);
}

// ------------------------------------------------------------------------------
// USER KEYBOARD CONTROLS
// ------------------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'e':
        case 'E':
            dispatchEmergencyVehicle("Ambulance");
            break;
        case 'f':
        case 'F':
            dispatchEmergencyVehicle("FireTruck");
            break;
        case 'c':
        case 'C':
            clippingDemoActive = !clippingDemoActive;
            std::cout << "[CO.5 CLIPPING] Cohen-Sutherland Line Clipping Visualizer: "
                      << (clippingDemoActive ? "ACTIVATED" : "DEACTIVATED") << std::endl;
            break;
        case 'p':
        case 'P':
            hardwareClipPlaneActive = !hardwareClipPlaneActive;
            std::cout << "[CO.5 CLIPPING] OpenGL GL_CLIP_PLANE0: "
                      << (hardwareClipPlaneActive ? "ENGAGED" : "DISABLED") << std::endl;
            break;
        case ' ':
            simulationPaused = !simulationPaused;
            std::cout << "[PAUSE] Traffic " << (simulationPaused ? "PAUSED" : "RESUMED") << std::endl;
            break;
        case 's':
        case 'S':
            // Manually advance signal phase
            if (!emergencyActive) {
                phaseTimer = 0.0f; // Triggers immediate phase transition
                std::cout << "[MANUAL] Advanced traffic signal phase." << std::endl;
            }
            break;
        case 'r':
        case 'R':
            resetSimulation();
            break;
        case 'v':
        case 'V':
            viewMode = (viewMode + 1) % 3;
            std::cout << "[VIEW MODE] Switched to mode: " << viewMode << std::endl;
            break;
        case '+':
        case '=':
            cameraDistance = std::max(16.0f, cameraDistance - 2.5f);
            break;
        case '-':
        case '_':
            cameraDistance = std::min(70.0f, cameraDistance + 2.5f);
            break;
        case '0':
            cameraYaw = defaultCameraYaw;
            cameraPitch = defaultCameraPitch;
            cameraDistance = defaultCameraDistance;
            viewMode = 0;
            std::cout << "[CAMERA] Reset to default elevated perspective." << std::endl;
            break;
        case 27: // ESC key
            std::cout << "[EXIT] Closing Smart Traffic 3D..." << std::endl;
            for (Vehicle* v : activeVehicles) delete v;
            activeVehicles.clear();
            std::exit(0);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            cameraYaw -= 4.0f;
            break;
        case GLUT_KEY_RIGHT:
            cameraYaw += 4.0f;
            break;
        case GLUT_KEY_UP:
            cameraPitch = std::min(78.0f, cameraPitch + 3.0f);
            break;
        case GLUT_KEY_DOWN:
            cameraPitch = std::max(12.0f, cameraPitch - 3.0f);
            break;
        case GLUT_KEY_PAGE_UP:
            cameraDistance = std::max(16.0f, cameraDistance - 2.0f);
            break;
        case GLUT_KEY_PAGE_DOWN:
            cameraDistance = std::min(70.0f, cameraDistance + 2.0f);
            break;
    }
    glutPostRedisplay();
}

// ------------------------------------------------------------------------------
// OPENGL INITIALIZATION
// ------------------------------------------------------------------------------
void initGL() {
    glClearColor(0.55f, 0.72f, 0.90f, 1.0f); // Sky Blue Background

    // Depth Testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Smooth Shading & Normalization
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // Alpha Blending for Translucent Overlays
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Lighting Setup
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[]  = { 0.38f, 0.40f, 0.44f, 1.0f };
    GLfloat diffuseLight[]  = { 0.95f, 0.92f, 0.88f, 1.0f };
    GLfloat specularLight[] = { 0.60f, 0.60f, 0.60f, 1.0f };

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    // Material Color Tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Initialize Initial Fleet
    resetSimulation();
}

// ------------------------------------------------------------------------------
// MAIN ENTRY POINT
// ------------------------------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(80, 50);
    glutCreateWindow("Smart Traffic Light System - 3D Emergency Priority (C++ / OpenGL)");

    std::cout << "================================================================" << std::endl;
    std::cout << " SMART TRAFFIC LIGHT SYSTEM WITH EMERGENCY VEHICLE PRIORITY     " << std::endl;
    std::cout << " 3D OpenGL FreeGLUT Simulation                                   " << std::endl;
    std::cout << "================================================================" << std::endl;
    std::cout << " Controls:" << std::endl;
    std::cout << "   [E / e]     : Dispatch Ambulance (Blue Beacon + Priority)" << std::endl;
    std::cout << "   [F / f]     : Dispatch Fire Truck" << std::endl;
    std::cout << "   [SPACE]     : Pause / Resume Traffic" << std::endl;
    std::cout << "   [S / s]     : Advance Signal Phase manually" << std::endl;
    std::cout << "   [R / r]     : Reset Simulation" << std::endl;
    std::cout << "   [Arrow Keys]: Orbit camera around intersection" << std::endl;
    std::cout << "   [+ / -]     : Zoom In / Out" << std::endl;
    std::cout << "   [V / v]     : Switch View Mode (3D Perspective / 2D / Chase)" << std::endl;
    std::cout << "   [0 / Home]  : Reset camera to default elevated perspective" << std::endl;
    std::cout << "   [ESC]       : Exit Application" << std::endl;
    std::cout << "================================================================" << std::endl;

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, updateTraffic, 0);

    glutMainLoop();
    return 0;
}