#include <time.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;
struct punto {
  int x;
  int y;
  punto(int x = 0, int y = 0) : x(x), y(y) {}
};
float distancia(int x1, int x2, int y1, int y2) {
  return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}
void cpuntos(vector<punto>& puntos) {
  for (int i = 0; i <= 200; i += 10) {
    for (int j = 0; j <= 200; j += 10) {
      punto temp(i, j);
      puntos.push_back(temp);
    }
  }
}
struct nivel {
  int x0, y0;
  float dist;
  vector<int> puntos_v;
  nivel(int x1, int y1, float dist1, vector<int> puntos_v1) {
    x0 = x1;
    y0 = y1;
    dist = dist1;
    puntos_v = puntos_v1;
  }
  nivel(const nivel& tmp) {
    x0 = tmp.x0;
    y0 = tmp.y0;
    dist = tmp.dist;
    puntos_v = tmp.puntos_v;
  }
};
void BCiegaA(vector<punto> puntos, vector<int>& puntos_f, float& dist, int x0,
             int y0, int x1, int y1) {
  vector<int> puntos_v = puntos_f;
  nivel ini(x0, y0, 0, puntos_v);
  queue<nivel> camino;
  camino.push(ini);
  while (!camino.empty()) {
    nivel tmp = camino.front();
    camino.pop();
    if (tmp.x0 == x1 && tmp.y0 == y1) {
      dist = tmp.dist;
      puntos_f = tmp.puntos_v;
      return;
    }
    for (int i = 0; i < puntos.size(); i++) {
      bool casox = abs(puntos[i].x - tmp.x0) <= 10;
      bool casoy = abs(puntos[i].y - tmp.y0) <= 10;
      bool casoz = puntos[i].x == tmp.x0 && puntos[i].y == tmp.y0;
      if (puntos_f[i] == 0 && casox && casoy && !casoz) {
        float dist_temp =
            tmp.dist + distancia(puntos[i].x, tmp.x0, puntos[i].y, tmp.y0);
        vector<int> tmpp = tmp.puntos_v;
        puntos_f[i] = 1;
        tmpp[i] = 1;
        nivel tmp2(puntos[i].x, puntos[i].y, dist_temp, tmpp);
        camino.push(tmp2);
      }
    }
  }
}
void AA(vector<punto> puntos, vector<int>& puntos_v, float& dist, int x0,
        int y0, int x1, int y1) {
  int sw = 1;
  while (sw && (y0 != y1 || x0 != x1)) {
    float dist_temp = INT_MAX, i_temp, x_temp, y_temp, suma = 0;
    sw = 0;
    for (int i = 0; i < puntos.size(); i++) {
      bool casox = abs(puntos[i].x - x0) <= 10;
      bool casoy = abs(puntos[i].y - y0) <= 10;
      bool casoz = puntos[i].x == x0 && puntos[i].y == y0;
      float distt = distancia(x0, puntos[i].x, puntos[i].y, y0);
      bool casod =
          distancia(x1, puntos[i].x, puntos[i].y, y1) + distt < dist_temp;
      if (puntos_v[i] == 0 && casox && casoy && !casoz && casod) {
        i_temp = i;
        dist_temp = distancia(x1, puntos[i].x, puntos[i].y, y1) + distt;
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
void BCiegaP(vector<punto>& puntos, vector<int>& puntos_f, float& dist, int x0,
             int y0, int x1, int y1) {
  vector<int> puntos_v = puntos_f;
  nivel ini(x0, y0, 0, puntos_v);
  stack<nivel> camino;
  camino.push(ini);
  while (!camino.empty()) {
    nivel tmp = camino.top();
    camino.pop();
    if (tmp.x0 == x1 && tmp.y0 == y1) {
      dist = tmp.dist;
      puntos_f = tmp.puntos_v;
      return;
    }
    for (int i = 0; i < puntos.size(); i++) {
      bool casox = abs(puntos[i].x - tmp.x0) <= 10;
      bool casoy = abs(puntos[i].y - tmp.y0) <= 10;
      bool casoz = puntos[i].x == tmp.x0 && puntos[i].y == tmp.y0;
      if (puntos_f[i] == 0 && casox && casoy && !casoz) {
        float dist_temp =
            tmp.dist + distancia(puntos[i].x, tmp.x0, puntos[i].y, tmp.y0);
        vector<int> tmpp = tmp.puntos_v;
        puntos_f[i] = 1;
        tmpp[i] = 1;
        nivel tmp2(puntos[i].x, puntos[i].y, dist_temp, tmpp);
        camino.push(tmp2);
      }
    }
  }
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