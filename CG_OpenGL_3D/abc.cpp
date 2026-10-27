#include <GL/glut.h>
#include <cstdlib>

// ---------------- TRANSFORMATION VARIABLES ----------------
float posX = 0.0f;
float posZ = 0.0f;
float rotation = 0.0f;
float scaleValue = 1.0f;

// ---------------- CLIPPING PLANE ----------------
double clipPlane[] = {1.0, 0.0, 0.0, 4.0};


// ================= CO1: OPENGL PRIMITIVES =================

// Draw the road using 3D rectangles
void drawRoad()
{
    glColor3f(0.12f, 0.12f, 0.12f);

    // Horizontal road
    glBegin(GL_QUADS);

        glVertex3f(-12, 0, -3);
        glVertex3f( 12, 0, -3);
        glVertex3f( 12, 0,  3);
        glVertex3f(-12, 0,  3);

    glEnd();

    // Vertical road
    glBegin(GL_QUADS);

        glVertex3f(-3, 0, -12);
        glVertex3f( 3, 0, -12);
        glVertex3f( 3, 0,  12);
        glVertex3f(-3, 0,  12);

    glEnd();

    // Road markings
    glColor3f(1, 1, 1);

    glBegin(GL_LINES);

        for (int i = -12; i <= 12; i += 2)
        {
            glVertex3f(i, 0.05f, -0.15f);
            glVertex3f(i + 1, 0.05f, -0.15f);
        }

    glEnd();
}


// 3D traffic signal
void drawTrafficSignal()
{
    // Pole
    glColor3f(0.3f, 0.3f, 0.3f);

    glPushMatrix();

        glTranslatef(5, 2, 5);
        glScalef(0.3f, 4.0f, 0.3f);

        glutSolidCube(1);

    glPopMatrix();


    // Signal body
    glColor3f(0.05f, 0.05f, 0.05f);

    glPushMatrix();

        glTranslatef(5, 5, 5);
        glScalef(1.2f, 3.0f, 1.0f);

        glutSolidCube(1);

    glPopMatrix();


    // Red light
    glColor3f(1, 0, 0);

    glPushMatrix();

        glTranslatef(5, 6, 5.5f);
        glutSolidSphere(0.35, 20, 20);

    glPopMatrix();


    // Yellow light
    glColor3f(1, 1, 0);

    glPushMatrix();

        glTranslatef(5, 5, 5.5f);
        glutSolidSphere(0.35, 20, 20);

    glPopMatrix();


    // Green light
    glColor3f(0, 1, 0);

    glPushMatrix();

        glTranslatef(5, 4, 5.5f);
        glutSolidSphere(0.35, 20, 20);

    glPopMatrix();
}


// 3D emergency vehicle
void drawEmergencyVehicle()
{
    glPushMatrix();

        // CO2: Translation + Rotation + Scaling
        glTranslatef(-4 + posX, 0.7f, -4 + posZ);
        glRotatef(rotation, 0, 1, 0);
        glScalef(1.5f * scaleValue,
                 0.7f * scaleValue,
                 2.0f * scaleValue);

        // Vehicle body
        glColor3f(0.9f, 0.1f, 0.1f);

        glutSolidCube(1);

    glPopMatrix();


    // Emergency light
    glColor3f(0.1f, 0.5f, 1.0f);

    glPushMatrix();

        glTranslatef(-4 + posX, 1.3f, -4 + posZ);
        glutSolidSphere(0.2, 20, 20);

    glPopMatrix();
}


// ================= DISPLAY =================

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // 3D camera
    gluLookAt(
        18, 15, 20,
         0,  0,  0,
         0,  1,  0
    );


    // CO3: Clipping
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, clipPlane);


    // Apply transformations
    glPushMatrix();

        drawRoad();
        drawTrafficSignal();
        drawEmergencyVehicle();

    glPopMatrix();


    glDisable(GL_CLIP_PLANE0);

    glutSwapBuffers();
}


// ================= CO5: DYNAMIC VIEWPORT =================

void reshape(int width, int height)
{
    if (height == 0)
        height = 1;

    // Dynamic viewport
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 3D perspective
    gluPerspective(
        45.0,
        (double)width / height,
        1.0,
        100.0
    );

    glMatrixMode(GL_MODELVIEW);
}


// ================= KEYBOARD CONTROLS =================

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // Rotation
        case 'r':
            rotation += 10;
            break;

        // Move vehicle
        case 'a':
            posX -= 0.5f;
            break;

        case 'd':
            posX += 0.5f;
            break;

        case 'w':
            posZ -= 0.5f;
            break;

        case 's':
            posZ += 0.5f;
            break;

        // Scaling
        case '+':
            scaleValue += 0.1f;
            break;

        case '-':
            if (scaleValue > 0.2f)
                scaleValue -= 0.1f;
            break;

        // Reset
        case '0':
            posX = 0;
            posZ = 0;
            rotation = 0;
            scaleValue = 1;
            break;

        // Exit
        case 27:
            exit(0);
    }

    glutPostRedisplay();
}


// ================= INITIALIZATION =================

void init()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(
        0.02f,
        0.04f,
        0.06f,
        1.0f
    );
}


// ================= MAIN =================

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_DEPTH
    );

    glutInitWindowSize(900, 650);

    glutCreateWindow(
        "Smart Traffic Light - Computer Graphics"
    );

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}