
#include <GL/freeglut.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
using namespace std;

struct punto;
float distancia(int, int, int, int);
struct arista {
  punto* origen;
  punto* destino;
  float distancia;
  arista(punto* origen, punto* destino, float distancia)
      : origen(origen), destino(destino), distancia(distancia) {}
};
struct punto {
  int x;
  int y;
  punto(int x = 0, int y = 0) : x(x), y(y) {}
};

float distancia(int x1, int x2, int y1, int y2) {
  float pt1 = pow(x1 - x2, 2);
  float pt2 = pow(y1 - y2, 2);
  float temp = sqrt(pt1 + pt2);
  return temp;
}
void printp(punto a) { cout << "( " << a.x << "," << a.y << " )"; }
void printar(vector<arista> aristas, int x, int y) {
  for (int i = 0; i < aristas.size(); i++) {
    int xx = x;
    int yy = y;
    bool casox = aristas[i].destino->x == xx && aristas[i].destino->y == yy;
    bool casoy = aristas[i].origen->x == xx && aristas[i].origen->y == yy;
    if (casox || casoy) {
      printp(*aristas[i].origen);
      cout << "   ";
      printp(*aristas[i].destino);
      cout << "   " << aristas[i].distancia << endl;
    }
  }
}
void cpuntos(vector<punto>& puntos) {
  for (int i = 0; i <= 200; i += 10) {
    for (int j = 0; j <= 200; j += 10) {
      punto temp(i, j);
      puntos.push_back(temp);
    }
  }
}
bool aristas_disp(vector<arista> aristas, vector<int> aristas_v, int x, int y) {
  for (int i = 0; i < aristas.size(); i++) {
    bool casox = aristas[i].destino->x == x && aristas[i].destino->y == y;
    bool casoy = aristas[i].origen->x == x && aristas[i].origen->y == y;
    if (aristas_v[i] == 0 && (casox || casoy)) {
      return true;
    }
  }
  return false;
}
void selec_aristas(vector<arista> aristas, vector<int>& aristas_v, int x,
                   int y) {
  for (int i = 0; i < aristas.size(); i++) {
    bool casox = aristas[i].destino->x == x && aristas[i].destino->y == y;
    bool casoy = aristas[i].origen->x == x && aristas[i].origen->y == y;
    if (casox || casoy) {
      aristas_v[i] = -1;
    }
  }
}
bool marcar_p(vector<punto> puntos, vector<int>& puntos_v, int x, int y) {
  for (int i = 0; i < puntos.size(); i++) {
    if (puntos[i].x == x && puntos[i].y == y) {
      puntos_v[i] = 1;
      i = puntos.size();
    }
  }
  return true;
}

void HillClimbing(vector<punto> puntos, vector<arista> aristas,
                  vector<int>& puntos_v, vector<int>& aristas_v, float& dist,
                  int x0, int y0, int x1, int y1) {
  while (marcar_p(puntos, puntos_v, x0, y0) && (y0 != y1 || x0 != x1) &&
         aristas_disp(aristas, aristas_v, x0, y0)) {
    float dist_temp = INT_MAX, i_temp, x_temp, y_temp, suma = -1;
    for (int i = 0; i < aristas_v.size(); i++) {
      bool casox = aristas[i].destino->x == x0 && aristas[i].destino->y == y0;
      bool casoy = aristas[i].origen->x == x0 && aristas[i].origen->y == y0;
      if (aristas_v[i] == 0 && casox &&
          distancia(x1, aristas[i].origen->x, aristas[i].origen->y, y1) <
              dist_temp) {
        dist_temp =
            distancia(x1, aristas[i].origen->x, aristas[i].origen->y, y1);
        i_temp = i;
        suma = aristas[i].distancia;
        x_temp = aristas[i].origen->x;
        y_temp = aristas[i].origen->y;
      } else if (aristas_v[i] == 0 && casoy &&
                 distancia(x1, aristas[i].destino->x, aristas[i].destino->y,
                           y1) < dist_temp) {
        dist_temp =
            distancia(x1, aristas[i].destino->x, aristas[i].destino->y, y1);
        i_temp = i;
        suma = aristas[i].distancia;
        x_temp = aristas[i].destino->x;
        y_temp = aristas[i].destino->y;
      }
    }
    x0 = x_temp;
    y0 = y_temp;
    aristas_v[i_temp] = 1;
    dist += suma;
  }
}

// CODE
vector<vector<int>> grafico(21, vector<int>(21, 0));
vector<punto> puntos;
vector<arista> aristas;
vector<int> aristas_v(aristas.size());
vector<int> puntos_v(puntos.size());
int x0 = 40, y0 = 0;
int x1 = 130, y1 = 150;
int cant = 20;
vector<int> temp2;
// Dimensiones de la ventana
int windowWidth = 1600;
int windowHeight = 900;

// Número de filas y columnas
int rows = 20;
int cols = 20;

// Tamaño de los puntos
float pointSize = 6.0f;

// Colores
float colors[3][3] = {
    {1.0f, 1.0f, 1.0f},  // Blanco
    {0.0f, 0.0f, 0.0f},  // Negro
    {1.0f, 0.0f, 0.0f}   // Rojo
};
bool buttonPressed = false;
float buttonColor[3] = {0.5f, 0.5f, 0.5f};  // Gris
// Coordenadas del botón
int buttonWidth = 100;
int buttonHeight = 50;
int buttonX = 50;
int buttonY = windowHeight / 2 - buttonHeight / 2;
// Función para dibujar un punto
void drawPoint(float x, float y, float color[]) {
  glColor3fv(color);
  glPointSize(pointSize);
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();
}

// Función para dibujar el botón
void drawButton() {
  if (buttonPressed) {
    glColor3fv(colors[2]);
  } else {
    glColor3fv(buttonColor);
  }
  glBegin(GL_QUADS);
  glVertex2i(buttonX, buttonY);
  glVertex2i(buttonX + buttonWidth, buttonY);
  glVertex2i(buttonX + buttonWidth, buttonY + buttonHeight);
  glVertex2i(buttonX, buttonY + buttonHeight);
  glEnd();
}

// Función para dibujar la escena
void drawScene() {
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < puntos_v.size(); i++) {
    if (puntos_v[i] == 1) {
      grafico[puntos[i].y / 10][puntos[i].x / 10] = 1;
    } else if (puntos_v[i] == -1) {
      grafico[puntos[i].y / 10][puntos[i].x / 10] = -1;
    }
  }
  // Dibujar botón
  drawButton();
  // Dibujar puntos
  float stepX = windowWidth / static_cast<float>(cols);
  float stepY = windowHeight / static_cast<float>(rows);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      float x = j * stepX + stepX / 2.0f +
                windowWidth / 3;  // Ubica los puntos a la derecha
      float y = windowHeight - (i * stepY + stepY / 2.0f);
      if (grafico[i][j] == 0) {
        drawPoint(x, y, colors[0]);
      } else if (grafico[i][j] == -1) {
        drawPoint(x, y, colors[1]);
      } else {
        drawPoint(x, y, colors[2]);
      }
    }
  }

  glutSwapBuffers();
}
void mouseClick(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Verificar si se hizo clic en el botón
    if (x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY &&
        y <= buttonY + buttonHeight) {
      buttonPressed = !buttonPressed;
      glutPostRedisplay();
    }
  }
}
// Función principal
int main(int argc, char** argv) {
  cpuntos(puntos);
  // crear aristas
  for (int i = 0; i < puntos.size(); i++) {
    for (int j = i + 1; j < puntos.size(); j++) {
      bool casox = abs(puntos[i].x - puntos[j].x) <= 10;
      bool casoy = abs(puntos[i].y - puntos[j].y) <= 10;
      if (casox && casoy) {
        arista temp(
            &puntos[i], &puntos[j],
            distancia(puntos[i].x, puntos[j].x, puntos[i].y, puntos[j].y));
        aristas.push_back(temp);
      }
    }
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("HClimbing");

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, windowWidth + windowWidth / 3, 0, windowHeight, -1,
          1);  // Ajusta la matriz de proyección

  glutDisplayFunc(drawScene);
  glutMouseFunc(mouseClick);
  glutMainLoop();

  return 0;
}
