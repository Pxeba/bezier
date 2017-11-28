#include <iostream>
#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include<windows.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <cmath>

class Point {
public: double x;
public: double y;

public:
Point(double x1,double y1){
    x = x1;
    y = y1;
}
};

Point* p0 = new Point(100,200);
Point* p1 = new Point(250,400);
Point* p2 = new Point(400,400);
Point* p3 = new Point(550,200);
Point* pFinal = new Point(100,300);
bool mouseRight = true;



Point* bezier(Point* p0, Point* p1, Point* p2, Point* p3,double t, Point* pFinal ) {
    double x = pow(1-t,3) * p0->x +
               pow(1-t,2) * 3 * t * p1->x +
               (1-t) * 3 * t * t * p2->x +
               t * t * t * p3->x;
    double y = pow(1-t,3) * p0->y +
               pow(1-t,2) * 3 * t * p1->y +
               (1-t) * 3 * t * t * p2->y +
               t * t * t * p3->y;
    // std::cout << t;
    return new Point(x,y);

}

double interpolate(Point* f[], int xi, int n)
{
    double result = 0; // Initialize result

    for (int i=0; i<n; i++)
    {
        // Compute individual terms of above formula
        double term = f[i]->y;
        for (int j=0;j<n;j++)
        {
            if (j!=i)
                term = term*(xi - f[j]->x)/double(f[i]->x - f[j]->x);
        }

        // Add current term to result
        result += term;
    }

    return result;
}

int escolherPonto(int x,int y)
{
    int distancia1,distancia2,distancia3,distancia4;
    distancia1=sqrt(((x - p0->x)*(x - p0->x))+((y - p0->y)*(y - p0->y)));
    distancia2=sqrt(((x - p1->x)*(x - p1->x))+((y - p1->y)*(y - p1->y)));
    distancia3=sqrt(((x - p2->x)*(x - p2->x))+((y - p2->y)*(y - p2->y)));
    distancia4=sqrt(((x - p3->x)*(x - p3->x))+((y - p3->y)*(y - p3->y)));

    if(distancia1 < distancia2)
    {
        if(distancia1 < distancia3)
        {
            if(distancia1 < distancia4)
            {
                return 1;
            }
            else
            {
                return 4;
            }
        }
        else
        {
            if(distancia3 < distancia4)
            {
                return 3;
            }
            else
            {
                return 4;
            }
        }
    }
    else
    {
        if(distancia2 < distancia3)
        {
            if(distancia2 < distancia4)
            {
                return 2;
            }
            else
            {
                return 4;
            }
        }
        else
        {
            if(distancia3 < distancia4)
            {
                return 3;
            }
            else
            {
                return 4;
            }
        }
    }

}

void mouse(int button, int state, int mousex, int mousey)
{

    if(button ==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int ponto = escolherPonto(mousex, 600-mousey);
        switch(ponto)
        {
        case 1:
            p0->x = mousex;
            p0->y = 600 - mousey;
            break;
        case 2:
            p1->x = mousex;glEnd();
            p1->y = 600 - mousey;
            break;
        case 3:
            p2->x = mousex;
            p2->y = 600 - mousey;
            break;
        case 4:
            p3->x = mousex;
            p3->y = 600 - mousey;
            break;
        }
        std::cout << mousex << " " << mousey << "\n";
        glutPostRedisplay();
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        mouseRight = !mouseRight;
    }

}

void desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,800,0,600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
       //std::cout << pFinal->x;
    Point* f[] = {p0, p1, p2, p3};
    pFinal = new Point(p0->x,interpolate(f, p0->x, 4));
    glPointSize(5);

    glBegin(GL_POINTS);
        glVertex2f(p0->x,p0->y);
        glVertex2f(p1->x,p1->y);
        glVertex2f(p2->x,p2->y);
        glVertex2f(p3->x,p3->y);
    glEnd();
     glPointSize(2);
     if(!mouseRight)
     {
        for(double i=p0->x+1;i<p3->x;i=i+1){
        glBegin(GL_POINTS);
            glVertex2f(pFinal->x,pFinal->y);
            pFinal = new Point(i,interpolate(f, i, 4));
            std::cout << pFinal->x << "\t" << pFinal->y << "\n";
            glVertex2f(pFinal->x,pFinal->y);
        glEnd();
        }
     }
     else
     {
         for(double i=0.01;i<1;i=i+0.01){
         glBegin(GL_LINES);
            glVertex2f(pFinal->x,pFinal->y);
            pFinal = bezier(p0,p1,p2,p3,i,pFinal);
            std::cout << pFinal->x;
            glVertex2f(pFinal->x,pFinal->y);
        glEnd();
        }
     }

    glFlush();
}


int main(int argc, char *argv[])
{
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(10,10);
    glutCreateWindow("APLICAÇÂO BEZIER");
    glClearColor(0,0,1,0);
    glClear(GL_COLOR_BUFFER_BIT);

    glutDisplayFunc(desenha);
    glutMouseFunc(mouse);
    glutMainLoop();
}
