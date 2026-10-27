// SMART TRAFFIC LIGHT SYSTEM WITH EMERGENCY VEHICLE PRIORITY
// 3D OpenGL + FreeGLUT | Windows / MinGW g++
// Controls:
//   E = Spawn an emergency ambulance (blue signal activates automatically near junction)
//   R = Reset emergency vehicle
//   SPACE = Pause / resume vehicle movement
//   Arrow keys = rotate camera
//   + / - = zoom
//   ESC = exit

#include <GL/freeglut.h>S
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

const float PI = 3.14159265f;
int winW = 1200, winH = 800;
float cameraYaw = 45.0f, cameraPitch = 34.0f, cameraDistance = 30.0f;
bool paused = false;
bool emergencyPresent = false;
bool emergencyPassed = false;
float emergencyZ = -18.0f;
float emergencySpeed = 0.10f;
int normalPhase = 0;                 // 0: N/S green, 1: yellow, 2: E/W green, 3: yellow
int phaseTicks = 0;
bool emergencyMode = false;
int lastTime = 0;

struct Vehicle {
    float x, z, speed;
    int direction; // 0 = along X, 1 = along Z
    float r, g, b;
    float length;
};
std::vector<Vehicle> cars;

void setColor(float r, float g, float b) { glColor3f(r, g, b); }

void drawCube(float x, float y, float z, float sx, float sy, float sz,
              float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    setColor(r, g, b);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawCylinder(float x, float y, float z, float radius, float height,
                  float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(-90, 1, 0, 0);
    setColor(r, g, b);
    GLUquadric* q = gluNewQuadric();
    gluCylinder(q, radius, radius, height, 16, 4);
    gluDisk(q, 0, radius, 16, 1);
    glTranslatef(0, 0, height);
    gluDisk(q, 0, radius, 16, 1);
    gluDeleteQuadric(q);
    glPopMatrix();
}

void drawRoadMarking(float x1, float z1, float x2, float z2, float width) {
    glBegin(GL_QUADS);
    glVertex3f(x1-width/2, 0.025f, z1-width/2);
    glVertex3f(x2-width/2, 0.025f, z2-width/2);
    glVertex3f(x2+width/2, 0.025f, z2+width/2);
    glVertex3f(x1+width/2, 0.025f, z1+width/2);
    glEnd();
}

void drawGroundAndRoads() {
    // Grass / ground
    drawCube(0, -0.22f, 0, 42, 0.35f, 42, 0.20f, 0.48f, 0.20f);
    // Two intersecting asphalt roads
    drawCube(0, 0.0f, 0, 10.5f, 0.08f, 42, 0.17f, 0.18f, 0.20f);
    drawCube(0, 0.01f, 0, 42, 0.08f, 10.5f, 0.17f, 0.18f, 0.20f);

    // Dashed centre lane markings, kept away from the centre box
    setColor(0.95f, 0.88f, 0.30f);
    for (float p=-20; p<=20; p+=2.8f) {
        if (std::fabs(p) < 6.0f) continue;
        drawRoadMarking(-0.10f, p, 0.10f, p+1.45f, 0.10f);
        drawRoadMarking(p, -0.10f, p+1.45f, 0.10f, 0.10f);
    }

    // Stop lines and zebra crossings on all four approaches
    setColor(0.94f, 0.94f, 0.90f);
    for (int i=0; i<7; ++i) {
        float lane = -3.0f + i;
        // North and south crossings (stripes across X)
        drawRoadMarking(lane, -6.7f, lane+0.42f, -6.7f, 0.95f);
        drawRoadMarking(lane,  6.7f, lane+0.42f,  6.7f, 0.95f);
        // East and west crossings (stripes across Z)
        drawRoadMarking(-6.7f, lane, -6.7f, lane+0.42f, 0.95f);
        drawRoadMarking( 6.7f, lane,  6.7f, lane+0.42f, 0.95f);
    }
    drawCube(0, 0.06f, -7.45f, 10.5f, 0.035f, 0.13f, 0.95f, 0.95f, 0.90f);
    drawCube(0, 0.06f,  7.45f, 10.5f, 0.035f, 0.13f, 0.95f, 0.95f, 0.90f);
    drawCube(-7.45f, 0.06f, 0, 0.13f, 0.035f, 10.5f, 0.95f, 0.95f, 0.90f);
    drawCube( 7.45f, 0.06f, 0, 0.13f, 0.035f, 10.5f, 0.95f, 0.95f, 0.90f);

    // Pavement blocks around corners
    drawCube(-8.2f, 0.04f, -8.2f, 5.0f, 0.12f, 5.0f, 0.55f, 0.55f, 0.52f);
    drawCube( 8.2f, 0.04f, -8.2f, 5.0f, 0.12f, 5.0f, 0.55f, 0.55f, 0.52f);
    drawCube(-8.2f, 0.04f,  8.2f, 5.0f, 0.12f, 5.0f, 0.55f, 0.55f, 0.52f);
    drawCube( 8.2f, 0.04f,  8.2f, 5.0f, 0.12f, 5.0f, 0.55f, 0.55f, 0.52f);
}

void drawLamp(float y, float r, float g, float b, bool active) {
    glPushMatrix();
    glTranslatef(0.0f, y, 0.43f);
    setColor(0.025f, 0.025f, 0.03f);
    glutSolidTorus(0.035, 0.225, 10, 24);
    glTranslatef(0, 0, 0.025f);
    if (active) {
        GLfloat emission[] = {r*0.65f, g*0.65f, b*0.65f, 1.0f};
        GLfloat noEmission[] = {0,0,0,1};
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
        setColor(r,g,b);
        glutSolidSphere(0.18, 20, 16);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noEmission);
    } else {
        setColor(r*0.15f, g*0.15f, b*0.15f);
        glutSolidSphere(0.18, 20, 16);
    }
    glPopMatrix();
}

void drawSignal(float x, float z, int facing, bool green, bool yellow, bool red) {
    glPushMatrix();
    glTranslatef(x, 0, z);
    glRotatef((float)facing, 0, 1, 0);
    drawCylinder(0, 0, 0, 0.13f, 3.25f, 0.24f, 0.24f, 0.25f);
    drawCube(0, 3.55f, 0.0f, 0.85f, 2.45f, 0.62f, 0.08f, 0.09f, 0.10f);
    drawLamp(4.20f, 1.0f, 0.04f, 0.03f, red);
    drawLamp(3.55f, 1.0f, 0.62f, 0.02f, yellow);
    drawLamp(2.90f, 0.02f, 0.9f, 0.06f, green);
    // Blue priority lamp: ONLY active when the emergency vehicle is detected.
    drawLamp(2.28f, 0.02f, 0.20f, 1.0f, emergencyMode);
    glPopMatrix();
}

void drawAllSignals() {
    bool nsGreen = emergencyMode || normalPhase == 0;
    bool nsYellow = !emergencyMode && normalPhase == 1;
    bool nsRed = !nsGreen && !nsYellow;

    bool ewGreen = !emergencyMode && normalPhase == 2;
    bool ewYellow = !emergencyMode && normalPhase == 3;
    bool ewRed = !ewGreen && !ewYellow;

    // Four signal heads; their facing directions are rotated toward the road.
    drawSignal(-6.2f, -6.2f, 45, nsGreen, nsYellow, nsRed);
    drawSignal( 6.2f,  6.2f, 225, nsGreen, nsYellow, nsRed);
    drawSignal(-6.2f,  6.2f, 135, ewGreen, ewYellow, ewRed);
    drawSignal( 6.2f, -6.2f, 315, ewGreen, ewYellow, ewRed);
}

void drawCarBody(float x, float z, float r, float g, float b, bool alongZ, float length, bool ambulance) {
    glPushMatrix();
    glTranslatef(x, 0.18f, z);
    // The car's long axis is local Z, so keep it unrotated for Z-lane traffic.
    if (!alongZ) glRotatef(90, 0, 1, 0);
    float carLength = length;
    drawCube(0, 0.25f, 0, 1.15f, 0.48f, carLength, r, g, b);
    drawCube(0, 0.61f, -0.05f, 0.78f, 0.42f, carLength*0.48f,
             ambulance ? 0.82f : 0.28f, ambulance ? 0.88f : 0.36f, ambulance ? 0.90f : 0.42f);
    // Wheels
    for (int side=-1; side<=1; side+=2) {
        for (int end=-1; end<=1; end+=2) {
            glPushMatrix();
            glTranslatef(side*0.59f, -0.02f, end*carLength*0.34f);
            glRotatef(90, 0, 1, 0);
            setColor(0.035f,0.035f,0.04f);
            glutSolidTorus(0.07, 0.12, 8, 12);
            glPopMatrix();
        }
    }
    if (ambulance) {
        // Red/blue roof beacons; they flash only while emergency mode is active.
        float t = glutGet(GLUT_ELAPSED_TIME) / 180.0f;
        bool flash = ((int)t % 2) == 0;
        drawCube(-0.20f, 0.90f, 0, 0.20f, 0.12f, 0.22f,
                 flash ? 0.05f : 0.95f, 0.04f, flash ? 0.95f : 0.05f);
        drawCube( 0.20f, 0.90f, 0, 0.20f, 0.12f, 0.22f,
                 flash ? 0.95f : 0.05f, 0.04f, flash ? 0.05f : 0.95f);
        // Ambulance cross symbol
        drawCube(0, 0.54f, -carLength*0.24f, 0.12f, 0.34f, 0.035f, 0.85f, 0.05f, 0.05f);
        drawCube(0, 0.54f, -carLength*0.24f, 0.32f, 0.11f, 0.035f, 0.85f, 0.05f, 0.05f);
    }
    glPopMatrix();
}

void drawVehicles() {
    for (const Vehicle& v : cars)
        drawCarBody(v.x, v.z, v.r, v.g, v.b, v.direction == 1, v.length, false);

    if (emergencyPresent) {
        // Ambulance approaches the junction from the south along the northbound lane.
        drawCarBody(2.0f, emergencyZ, 0.92f, 0.92f, 0.86f, true, 2.0f, true);
    }
}

void drawText3DOverlay() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, winW, 0, winH);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING);
    glColor3f(1,1,1);
    std::string title = "SMART TRAFFIC LIGHT - 3D EMERGENCY PRIORITY";
    glRasterPos2i(20, winH-32);
    for (char c : title) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    std::string status = emergencyMode ? "EMERGENCY DETECTED: BLUE LAMP ON | NORTH-SOUTH GREEN"
                                       : "NORMAL MODE | Press E to send ambulance";
    glRasterPos2i(20, winH-58);
    for (char c : status) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    std::string controls = "E: spawn ambulance   R: reset   SPACE: pause   Arrows: camera   +/-: zoom   ESC: exit";
    glRasterPos2i(20, 22);
    for (char c : controls) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float yaw = cameraYaw * PI / 180.0f;
    float pitch = cameraPitch * PI / 180.0f;
    float cx = cameraDistance * std::cos(pitch) * std::sin(yaw);
    float cy = cameraDistance * std::sin(pitch);
    float cz = cameraDistance * std::cos(pitch) * std::cos(yaw);
    gluLookAt(cx, cy, cz, 0, 0, 0, 0, 1, 0);

    GLfloat lightPos[] = { -8.0f, 18.0f, 8.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    drawGroundAndRoads();
    drawAllSignals();
    drawVehicles();
    drawText3DOverlay();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    winW = std::max(1, w); winH = std::max(1, h);
    glViewport(0, 0, winW, winH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(48.0, (double)winW / (double)winH, 0.1, 150.0);
    glMatrixMode(GL_MODELVIEW);
}

void resetEmergency() {
    emergencyPresent = false;
    emergencyPassed = false;
    emergencyMode = false;
    emergencyZ = -18.0f;
}

void update(int) {
    if (!paused) {
        phaseTicks++;
        if (!emergencyMode && phaseTicks > 220) {
            normalPhase = (normalPhase + 1) % 4;
            phaseTicks = 0;
        }

        // Regular traffic moves through the junction in both directions.
        for (Vehicle& v : cars) {
            if (v.direction == 0) {
                v.x += v.speed;
                if (v.speed >= 0 && v.x > 22) v.x = -22;
                if (v.speed < 0 && v.x < -22) v.x = 22;
            } else {
                v.z += v.speed;
                if (v.speed >= 0 && v.z > 22) v.z = -22;
                if (v.speed < 0 && v.z < -22) v.z = 22;
            }
        }

        if (emergencyPresent && !emergencyPassed) {
            emergencyZ += emergencySpeed;
            // Detection zone near the intersection. Blue light is not manually toggled:
            // it activates when the ambulance approaches the junction.
            if (emergencyZ >= -9.0f && emergencyZ <= 5.0f) emergencyMode = true;
            if (emergencyZ > 8.5f) {
                emergencyMode = false;
                emergencyPassed = true;
            }
            if (emergencyZ > 23.0f) resetEmergency();
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'e': case 'E':
            if (!emergencyPresent || emergencyPassed) {
                emergencyPresent = true;
                emergencyPassed = false;
                emergencyMode = false;
                emergencyZ = -18.0f;
            }
            break;
        case 'r': case 'R': resetEmergency(); break;
        case ' ': paused = !paused; break;
        case '+': case '=': cameraDistance = std::max(15.0f, cameraDistance - 1.5f); break;
        case '-': case '_': cameraDistance = std::min(55.0f, cameraDistance + 1.5f); break;
        case 27: std::exit(0);
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    if (key == GLUT_KEY_LEFT) cameraYaw -= 5;
    if (key == GLUT_KEY_RIGHT) cameraYaw += 5;
    if (key == GLUT_KEY_UP) cameraPitch = std::min(65.0f, cameraPitch + 3);
    if (key == GLUT_KEY_DOWN) cameraPitch = std::max(15.0f, cameraPitch - 3);
    glutPostRedisplay();
}

void initScene() {
    glClearColor(0.48f, 0.70f, 0.88f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_NORMALIZE);
    GLfloat ambient[] = {0.42f,0.42f,0.42f,1};
    GLfloat diffuse[] = {0.95f,0.95f,0.95f,1};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    // Starting traffic around the junction
    cars.push_back({-18, -2.0f, 0.075f, 0, 0.80f, 0.12f, 0.10f, 1.7f});
    cars.push_back({-10,  2.0f, 0.060f, 0, 0.10f, 0.35f, 0.88f, 1.6f});
    cars.push_back({ 17,  3.0f, -0.065f, 0, 0.92f, 0.55f, 0.08f, 1.7f});
    cars.push_back({-2.0f, -19, 0.070f, 1, 0.72f, 0.18f, 0.76f, 1.6f});
    cars.push_back({ 3.0f, -12, 0.055f, 1, 0.12f, 0.72f, 0.38f, 1.7f});
    cars.push_back({-3.0f,  18, -0.060f, 1, 0.75f, 0.75f, 0.78f, 1.7f});
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("Smart Traffic Light System - 3D Emergency Priority");
    initScene();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}