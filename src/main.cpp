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

// Variables globales
vector<vector<int>> grafico(21, vector<int>(21, 0));
vector<punto> puntos;
vector<int> puntos_v(441);
int x0 = -1, y0 = -1;
int x1 = -1, y1 = -1;
int cant = 20;
vector<int> temp2;
float dist = 0;
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
int buttonWidth = 170;
int buttonHeight = 50;
float buttonColor[3] = {0.5f, 0.5f, 0.5f};  // Gris

// Color arista
float lineColor[3] = {0.5f, 0.5f, 0.5f};  // Gris

// Coordenadas del botón 1 Borrar
int buttonX = 50;
int buttonY = windowHeight / 2 - buttonHeight / 2;

// Coordenadas del botón 2 Algoritmo
int button2X = 50;
int button2Y = windowHeight / 2 - buttonHeight / 2 - buttonHeight - 20;

// Coordenadas del botón 3 Reiniciar
int button3X = 50;
int button3Y = windowHeight / 2 - buttonHeight / 2 - 2 * (buttonHeight + 20);

// Coordenadas de los botones de búsqueda

int buttonBX = 50;
int buttonBY = windowHeight - 50 - buttonHeight;

int buttonB2X = 50;
int buttonB2Y = buttonBY - buttonHeight - 15;

int buttonB3X = 50;
int buttonB3Y = buttonB2Y - buttonHeight - 15;

int buttonB4X = 50;
int buttonB4Y = buttonB3Y - buttonHeight - 15;

int buttonVal = 0;
// Colores de los botones
float buttonColorB[4][3] = {{1.0f, 0.0f, 0.0f},   // Rojo
                            {1.0f, 0.5f, 0.0f},   // Naranja
                            {1.0f, 1.0f, 0.0f},   // Amarillo
                            {1.0f, 1.0f, 1.0f}};  // Blanco

// Caja de texto
std::string percentageInput = "";
int textBoxX = 10;
int textBoxY = 10;
int textBoxWidth = 200;
int textBoxHeight = 30;

// Boton de envío
int submitButtonX = textBoxX + textBoxWidth + 10;
int submitButtonY = textBoxY;
int submitButtonWidth = 100;
int submitButtonHeight = textBoxHeight;

// Función para dibujar texto en la pantalla
void drawText(float x, float y, std::string text, float color[3]) {
  glColor3fv(color);
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
  drawButton(buttonBX, buttonBY, buttonWidth * 3 / 2, buttonHeight,
             buttonColorB[0]);
  drawText(buttonBX + 10, buttonBY + buttonHeight / 3, "BusquedaCiegaA",
           colors[1]);
  drawButton(buttonB2X, buttonB2Y, buttonWidth * 3 / 2, buttonHeight,
             buttonColorB[1]);
  drawText(buttonB2X + 10, buttonB2Y + buttonHeight / 3, "BusquedaCiegaP",
           colors[1]);
  drawButton(buttonB3X, buttonB3Y, buttonWidth * 3 / 2, buttonHeight,
             buttonColorB[2]);
  drawText(buttonB3X + 10, buttonB3Y + buttonHeight / 3, "HillClimbing",
           colors[1]);
  drawButton(buttonB4X, buttonB4Y, buttonWidth * 3 / 2, buttonHeight,
             buttonColorB[3]);
  drawText(buttonB4X + 10, buttonB4Y + buttonHeight / 3, "A*", colors[1]);

  drawButton(buttonX, buttonY, buttonWidth, buttonHeight, buttonColor);
  drawButton(button2X, button2Y, buttonWidth, buttonHeight,
             buttonColorB[buttonVal]);
  drawButton(button3X, button3Y, buttonWidth, buttonHeight, buttonColor);

  // Dibujar caja de texto
  drawText(buttonX + 10, buttonY + buttonHeight / 3,
           "Borrar " + toString(cant) + "%", colors[0]);
  drawText(button3X + 10, button3Y + buttonHeight / 3, "Reiniciar", colors[0]);
  drawButton(submitButtonX, submitButtonY, submitButtonWidth,
             submitButtonHeight, buttonColor);
  drawText(textBoxX + 5, textBoxY + 5, "Porcentaje: " + percentageInput,
           buttonColor);
  drawText(button2X + buttonWidth + 5, button2Y + 18,
           "Distancia: " + toString(dist), colors[0]);
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
    if (x >= buttonBX && x <= buttonBX + buttonWidth * 3 / 2 && y >= buttonBY &&
        y <= buttonBY + buttonHeight) {
      buttonVal = 0;
    } else if (x >= buttonB2X && x <= buttonB2X + buttonWidth * 3 / 2 &&
               y >= buttonB2Y && y <= buttonB2Y + buttonHeight) {
      buttonVal = 1;
    } else if (x >= buttonB3X && x <= buttonB3X + buttonWidth * 3 / 2 &&
               y >= buttonB3Y && y <= buttonB3Y + buttonHeight) {
      buttonVal = 2;
    } else if (x >= buttonB4X && x <= buttonB4X + buttonWidth * 3 / 2 &&
               y >= buttonB4Y && y <= buttonB4Y + buttonHeight) {
      buttonVal = 3;
    } else if (x >= buttonX && x <= buttonX + buttonWidth && y >= buttonY &&
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
      if (buttonVal == 0) {
        BCiegaA(puntos, puntos_v, dist, x0 * 10, y0 * 10, x1 * 10, y1 * 10);
      } else if (buttonVal == 1) {
        BCiegaP(puntos, puntos_v, dist, x0 * 10, y0 * 10, x1 * 10, y1 * 10);
      } else if (buttonVal == 2) {
        HillClimbing(puntos, puntos_v, dist, x0 * 10, y0 * 10, x1 * 10,
                     y1 * 10);
      } else if (buttonVal == 3) {
        AA(puntos, puntos_v, dist, x0 * 10, y0 * 10, x1 * 10, y1 * 10);
      }
      glutPostRedisplay();
    } else if (x >= button3X && x <= button3X + buttonWidth && y >= button3Y &&
               y <= button3Y + buttonHeight) {  // Botón Reiniciar
      dist = 0;
      puntos.clear();
      temp2.clear();
      cpuntos(puntos);
      puntos_v.assign(441, 0);
      grafico.assign(21, vector<int>(21, 0));
      puntos_cant = 0;
      x0 = -1, y0 = -1;
      x1 = -1, y1 = -1;
      glutPostRedisplay();
    } else if (x >= submitButtonX - 50 &&
               x <= submitButtonX - 50 + submitButtonWidth &&
               y >= submitButtonY && y <= submitButtonY + submitButtonHeight) {
      cant = stoi(percentageInput);
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
