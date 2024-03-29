
#include <GL/freeglut.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "funciones.h"
using namespace std;

// Función para convertir una variable a string
template <typename T>
std::string toString(T val) {
  std::ostringstream oss;
  oss << val;
  return oss.str();
}

// Hill Climbing
void HillClimbing(vector<punto> puntos, vector<int>& puntos_v, float& dist,
                  int x0, int y0, int x1, int y1) {
  int sw = 1;
  while (sw && (y0 != y1 || x0 != x1)) {
    float dist_temp = INT_MAX, i_temp, x_temp, y_temp, suma = 0;
    sw = 0;
    for (int i = 0; i < puntos.size(); i++) {
      bool casox = abs(puntos[i].x - x0) <= 10;
      bool casoy = abs(puntos[i].y - y0) <= 10;
      bool casoz = puntos[i].x == x0 && puntos[i].y == y0;
      bool casod = distancia(x1, puntos[i].x, puntos[i].y, y1) < dist_temp;
      if (puntos_v[i] == 0 && casox && casoy && !casoz && casod) {
        i_temp = i;
        dist_temp = distancia(x1, puntos[i].x, puntos[i].y, y1);
        suma = distancia(x0, puntos[i].x, puntos[i].y, y0);
        x_temp = puntos[i].x;
        y_temp = puntos[i].y;
        sw = 1;
      }
    }
    x0 = x_temp;
    y0 = y_temp;
    puntos_v[i_temp] = 1;
    dist += suma;
  }
}

// Variables globales
vector<vector<int>> grafico(21, vector<int>(21, 0));
vector<punto> puntos;
vector<int> puntos_v(441);
int x0 = -1, y0 = -1;
int x1 = -1, y1 = -1;
int cant = 20;
vector<int> temp2;

// Dimensiones de la ventana
int windowWidth = 1600;
int windowHeight = 900;

// Texto ingresado por el usuario
std::string userInput = "";

// Número de filas y columnas
int rows = 20;
int cols = 20;

// Tamaño de los puntos
int point_radius = 10.0f;
int puntos_cant = 0;
// Colores
float colors[5][3] = {
    {1.0f, 1.0f, 1.0f},  // Blanco
    {0.0f, 0.0f, 0.0f},  // Negro
    {1.0f, 0.0f, 0.0f},  // Rojo
    {0.0f, 0.0f, 1.0f},  // Azul
    {0.0f, 1.0f, 0.0f}   // Verde
};

// Tamaño de un botón
int buttonWidth = 100;
int buttonHeight = 50;
float buttonColor[3] = {0.5f, 0.5f, 0.5f};  // Gris

// Color arista
float lineColor[3] = {0.5f, 0.5f, 0.5f};  // Gris

// Coordenadas del botón 1
int buttonX = 50;
int buttonY = windowHeight / 2 - buttonHeight / 2;

// Coordenadas del botón 2
int button2X = 50;
int button2Y = windowHeight / 2 - buttonHeight / 2 - buttonHeight - 20;

// Coordenadas del botón 3
int button3X = 50;
int button3Y = windowHeight / 2 - buttonHeight / 2 - 2 * (buttonHeight + 20);

// Caja de texto
std::string percentageInput = "";

// Función para dibujar texto en la pantalla
void drawText(float x, float y, std::string text) {
  glRasterPos2f(x, y);
  for (char& c : text) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
  }
}

// Función para dibujar un punto
void drawPoint(float x, float y, float color[]) {
  glColor3fv(color);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(x, y);
  for (int i = 0; i <= 100; ++i) {
    float angle = 2.0f * 3.14159265358979323846f * float(i) / 100.0f;
    float dx = point_radius * cosf(angle);
    float dy = point_radius * sinf(angle);
    glVertex2f(x + dx, y + dy);
  }
  glEnd();
}

// Función para dibujar una línea entre dos puntos
void drawLine(float x1, float y1, float x2, float y2, float color[]) {
  glColor3fv(color);
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

// Función para dibujar el botón
void drawButton(int x, int y, int width, int height, float color[]) {
  glColor3fv(color);
  glBegin(GL_QUADS);
  glVertex2i(x, y);
  glVertex2i(x + width, y);
  glVertex2i(x + width, y + height);
  glVertex2i(x, y + height);
  glEnd();
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 13) {  // ASCII de la tecla Enter
    // Agregar una nueva línea
    percentageInput += "\n";
  } else if (key == 8) {  // ASCII de la tecla Backspace
    // Eliminar el último carácter
    if (!percentageInput.empty()) {
      percentageInput.pop_back();
    }
  } else {
    // Agregar el carácter ingresado
    percentageInput += key;
  }
  glutPostRedisplay();
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

  drawButton(buttonX, buttonY, buttonWidth, buttonHeight, buttonColor);
  drawButton(button2X, button2Y, buttonWidth, buttonHeight, colors[2]);
  drawButton(button3X, button3Y, buttonWidth, buttonHeight, colors[4]);

  // Dibujar caja de texto
  drawText(buttonX + buttonWidth + 10, buttonY + buttonHeight / 3,
           "Borrar: " + toString(cant) + "%");
  drawButton(submitButtonX, submitButtonY, submitButtonWidth,
             submitButtonHeight, buttonColor);
  drawText(textBoxX + 5, textBoxY + 5, "Porcentaje: " + percentageInput);

  // Dibujar puntos
  float stepX = windowWidth / static_cast<float>(cols);
  float stepY = windowHeight / static_cast<float>(rows);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      float x = j * stepX + stepX / 2.0f + windowWidth / 3;
      float y = windowHeight - (i * stepY + stepY / 2.0f);
      if (grafico[i][j] != -1) {
        if (j < cols - 1 && grafico[i][j + 1] != -1) {  // Arista Derecha
          if ((grafico[i][j] == 0 && grafico[i][j + 1] == 1) ||
              (grafico[i][j] == 1 && grafico[i][j + 1] == 0)) {
            drawLine(x, y, x + stepX, y, colors[3]);
          } else if (grafico[i][j] == 1 && grafico[i][j + 1] == 1) {
            drawLine(x, y, x + stepX, y, colors[2]);
          } else {
            drawLine(x, y, x + stepX, y, colors[0]);
          }
        }
        if (i < rows && i != 0 && grafico[i - 1][j] != -1) {  // Arista Arriba
          if ((grafico[i][j] == 0 && grafico[i - 1][j] == 1) ||
              (grafico[i][j] == 1 && grafico[i - 1][j] == 0)) {
            drawLine(x, y, x, y + stepY, colors[3]);
          } else if (grafico[i][j] == 1 && grafico[i - 1][j] == 1) {
            drawLine(x, y, x, y + stepY, colors[2]);
          } else {
            drawLine(x, y, x, y + stepY, colors[0]);
          }
        }
        // Diagonalmente
        if (j < cols - 1 && i < rows && i != 0 &&
            grafico[i - 1][j + 1] != -1) {  // Arista Arriba-derecha
          if ((grafico[i][j] == 0 && grafico[i - 1][j + 1] == 1) ||
              (grafico[i][j] == 1 && grafico[i - 1][j + 1] == 0)) {
            drawLine(x, y, x + stepX, y + stepY, colors[3]);
          } else if (grafico[i][j] == 1 && grafico[i - 1][j + 1] == 1) {
            drawLine(x, y, x + stepX, y + stepY, colors[2]);
          } else {
            drawLine(x, y, x + stepX, y + stepY, colors[0]);
          }
        }
        if (j > 0 && i < rows && i != 0 &&
            grafico[i - 1][j - 1] != -1) {  // Arista Arriba-izquierda
          if ((grafico[i][j] == 0 && grafico[i - 1][j - 1] == 1) ||
              (grafico[i][j] == 1 && grafico[i - 1][j - 1] == 0)) {
            drawLine(x, y, x - stepX, y + stepY, colors[3]);
          } else if (grafico[i][j] == 1 && grafico[i - 1][j - 1] == 1) {
            drawLine(x, y, x - stepX, y + stepY, colors[2]);
          } else {
            drawLine(x, y, x - stepX, y + stepY, colors[0]);
          }
        }
      }
      if ((i == y1 && j == x1) || (i == y0 && j == x0)) {
        drawPoint(x, y, colors[4]);  // Verde
      } else if (grafico[i][j] == 0) {
        drawPoint(x, y, colors[0]);  // Blanco
      } else if (grafico[i][j] == -1) {
        drawPoint(x, y, colors[1]);  // Negro
      } else if (grafico[i][j] == 1) {
        drawPoint(x, y, colors[2]);  // Rojo
      }
    }
  }

  glutSwapBuffers();
}
void mouseClick(int button, int state, int x, int y) {
  y = windowHeight - y;
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Verificar si se hizo clic en el botón
    if (x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY &&
        y <= buttonY + buttonHeight) {  // Botón borrar
      srand(time(NULL));
      for (int i = 0; i < 441 * cant / 100;) {
        int temp = rand() % 441;
        if (find(temp2.begin(), temp2.end(), temp) == temp2.end()) {
          puntos_v[temp] = -1;
          temp2.push_back(temp);
          i++;
        }
      }
      glutPostRedisplay();
    } else if (x >= button2X && x <= button2X + buttonWidth && y >= button2Y &&
               y <= button2Y + buttonHeight) {  // Botón HillClimbing
      float dist = 0;
      HillClimbing(puntos, puntos_v, dist, x0 * 10, y0 * 10, x1 * 10, y1 * 10);
      glutPostRedisplay();
    } else if (x >= button3X && x <= button3X + buttonWidth && y >= button3Y &&
               y <= button3Y + buttonHeight) {  // Botón Reiniciar
      puntos.clear();
      temp2.clear();
      cpuntos(puntos);
      puntos_v.assign(441, 0);
      grafico.assign(21, vector<int>(21, 0));
      puntos_cant = 0;
      x0 = -1, y0 = -1;
      x1 = -1, y1 = -1;
      glutPostRedisplay();
    } else if (x >= submitButtonX - submitButtonWidth + 15 &&
               x <= submitButtonX + 15 && y >= submitButtonY &&
               y <= submitButtonY + submitButtonHeight) {  // Botón de envío
      cant = stoi(percentageInput);
      glutPostRedisplay();
    }
    float stepX = 60;  //  430 - 1570
    float stepY = windowHeight / static_cast<float>(rows);
    // Verificar si se hizo clic en un punto
    if (puntos_cant < 2) {
      for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
          float pointX = 430 + j * stepX;
          float pointY = windowHeight - (i * stepY + stepY / 2.0f);
          if (x >= pointX - point_radius && x <= pointX + point_radius &&
              y >= pointY - point_radius &&
              y <= pointY + point_radius) {  // Definir punto (Inicio-Final)
            grafico[i][j] = 1;
            if (puntos_cant == 0) {
              x0 = j;
              y0 = i;
            } else if (puntos_cant == 1) {
              x1 = j;
              y1 = i;
            }
            puntos_cant++;
            glutPostRedisplay();
            return;
          }
        }
      }
    }
  }
}

// Función para inicializar GLUT
void inicializarGLUT(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(windowWidth, windowHeight);
  glutCreateWindow("HClimbing");

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, windowWidth + windowWidth / 3, 0, windowHeight, -1,
          1);  // Ajusta la matriz de proyección

  glutDisplayFunc(drawScene);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouseClick);
}

// Función principal
int main(int argc, char** argv) {
  // Inicializar puntos
  cpuntos(puntos);

  // Inicializar GLUT
  inicializarGLUT(argc, argv);

  // Iniciar el bucle de eventos de GLUT
  glutMainLoop();

  return 0;
}
