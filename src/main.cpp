
#include <GL/freeglut.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
// Función para convertir una variable a string
template <typename T>
std::string toString(T val) {
  std::ostringstream oss;
  oss << val;
  return oss.str();
}
// Estructuras punto
struct punto;
float distancia(int, int, int, int);
// Estructura arista
struct arista {
  punto* origen;
  punto* destino;
  float distancia;
  arista(punto* origen, punto* destino, float distancia)
      : origen(origen), destino(destino), distancia(distancia) {}
};
// Estructura punto
struct punto {
  int x;
  int y;
  punto(int x = 0, int y = 0) : x(x), y(y) {}
};

// Funciones
float distancia(int x1, int x2, int y1, int y2) {
  float pt1 = pow(x1 - x2, 2);
  float pt2 = pow(y1 - y2, 2);
  float temp = sqrt(pt1 + pt2);
  return temp;
}

// Inicializar puntos
void cpuntos(vector<punto>& puntos) {
  for (int i = 0; i <= 200; i += 10) {
    for (int j = 0; j <= 200; j += 10) {
      punto temp(i, j);
      puntos.push_back(temp);
    }
  }
}

// Si nodo tiene aristas disponibles
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

// Seleccionar aristas
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

// Marcar puntos
bool marcar_p(vector<punto> puntos, vector<int>& puntos_v, int x, int y) {
  for (int i = 0; i < puntos.size(); i++) {
    if (puntos[i].x == x && puntos[i].y == y) {
      puntos_v[i] = 1;
      i = puntos.size();
    }
  }
  return true;
}

// Hill Climbing
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
vector<int> aristas_v(1640);
vector<int> puntos_v(441);
int x0 = 40, y0 = 0;
int x1 = 130, y1 = 150;
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
float colors[3][3] = {
    {1.0f, 1.0f, 1.0f},  // Blanco
    {0.0f, 0.0f, 0.0f},  // Negro
    {1.0f, 0.0f, 0.0f}   // Rojo
};
bool buttonPressed = false;
bool button2Pressed = false;
bool button3Pressed = false;

float buttonColor[3] = {0.5f, 0.5f, 0.5f};  // Gris
float lineColor[3] = {0.5f, 0.5f, 0.5f};    // Gris claro
// Coordenadas del botón
int buttonWidth = 100;
int buttonHeight = 50;
int buttonX = 50;
int buttonY = windowHeight / 2 - buttonHeight / 2;

// Tamaño y posición del segundo botón
int button2Width = 100;
int button2Height = 50;
int button2X = 50;
int button2Y = windowHeight / 2 - buttonHeight / 2 - buttonHeight - 20;

// Tamaño y posición del tercer botón
int button3Width = 100;
int button3Height = 50;
int button3X = 50;
int button3Y = windowHeight / 2 - buttonHeight / 2 - 2 * (buttonHeight + 20);

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
bool submitButtonPressed = false;

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
void drawLine(float x1, float y1, float x2, float y2) {
  glColor3fv(colors[0]);
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

// Función para dibujar el botón
void drawButton(int x, int y, int width, int height, float color[]) {
  if (buttonPressed) {
    srand(time(NULL));
    for (int i = 0; i < 441 * cant / 100;) {
      int temp = rand() % 440;
      bool posib1 = puntos[temp].x == x0 && puntos[temp].y == y0;
      bool posib2 = puntos[temp].x == x1 && puntos[temp].y == y1;
      if (!posib1 && !posib2 &&
          find(temp2.begin(), temp2.end(), temp) == temp2.end()) {
        puntos_v[temp] = -1;
        temp2.push_back(temp);
        selec_aristas(aristas, aristas_v, puntos[temp].x, puntos[temp].y);
        i++;
      }
    }
    buttonPressed = !buttonPressed;
  }
  if (button2Pressed) {
    float dist = 0;
    HillClimbing(puntos, aristas, puntos_v, aristas_v, dist, x0, y0, x1, y1);
    button2Pressed = !button2Pressed;
  }
  if (button3Pressed) {
    puntos.clear();
    aristas.clear();
    temp2.clear();
    cpuntos(puntos);
    aristas_v.assign(1640, 0);
    puntos_v.assign(441, 0);
    grafico.assign(21, vector<int>(21, 0));
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
    button3Pressed = !button3Pressed;
  }
  if (submitButtonPressed) {
    cant = stoi(percentageInput);
    submitButtonPressed = !submitButtonPressed;
  }
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
  // Dibujar botón 1
  drawButton(buttonX, buttonY, buttonWidth, buttonHeight, buttonColor);

  // Dibujar botón 2
  drawButton(button2X, button2Y, button2Width, button2Height, colors[2]);

  // Dibujar botón 3
  drawButton(button3X, button3Y, button3Width, button3Height, buttonColor);

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
      float x = j * stepX + stepX / 2.0f +
                windowWidth / 3;  // Ubica los puntos a la derecha
      float y = windowHeight - (i * stepY + stepY / 2.0f);
      if (grafico[i][j] == 0 || grafico[i][j] == 1) {
        if (j < cols - 1 && grafico[i][j + 1] != -1)  // Derecha
          drawLine(x, y, x + stepX, y);
        if (i < rows && i != 0 && grafico[i - 1][j] != -1)  // Arriba
          drawLine(x, y, x, y + stepY);
        // Diagonalmente
        if (j < cols - 1 && i < rows && i != 0 &&
            grafico[i - 1][j + 1] != -1)  // Arriba-derecha
          drawLine(x, y, x + stepX, y + stepY);
        if (j > 0 && i < rows && i != 0 &&
            grafico[i - 1][j - 1] != -1)  // Arriba-izquierda
          drawLine(x, y, x - stepX, y + stepY);
      }
      if (grafico[i][j] == 0) {
        drawPoint(x, y, colors[0]);  // Blanco
      } else if (grafico[i][j] == -1) {
        drawPoint(x, y, colors[1]);  // Negro
      } else {
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
        y <= buttonY + buttonHeight) {
      buttonPressed = !buttonPressed;
      glutPostRedisplay();
    } else if (x >= button2X && x <= button2X + button2Width && y >= button2Y &&
               y <= button2Y + button2Height) {
      button2Pressed = !button2Pressed;
      glutPostRedisplay();
    } else if (x >= button3X && x <= button3X + button3Width && y >= button3Y &&
               y <= button3Y + button3Height) {
      button3Pressed = !button3Pressed;
      glutPostRedisplay();
    } else if (x >= submitButtonX - submitButtonWidth + 15 &&
               x <= submitButtonX + 15 && y >= submitButtonY &&
               y <= submitButtonY + submitButtonHeight) {
      submitButtonPressed = !submitButtonPressed;
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
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouseClick);
  glutMainLoop();

  return 0;
}
