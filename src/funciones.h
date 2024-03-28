#include<iostream>
#include<vector>
#include<cmath>
#include <cstdlib>
#include <algorithm>
#include <time.h>
#include <queue>
#include <stack>
using namespace std;
struct punto{
    int x;
    int y;
    punto(int x=0, int y=0): x(x), y(y) {}
};
float distancia(int x1, int x2, int y1, int y2) {
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
void cpuntos(vector<punto> &puntos){
    for(int i=0;i<=200;i+=10){
        for(int j=0;j<=200;j+=10){
            punto temp(i,j);
            puntos.push_back(temp);
        }
    }    
}
void print_g(vector<vector<int>> grafico, vector<punto> puntos, vector<int> puntos_v, int x1, int y1, int x0, int y0){
    for(int i=0;i<puntos_v.size();i++){
        if(puntos_v[i]==1){
            grafico[puntos[i].y/10][puntos[i].x/10]=1;
        }
        else if(puntos_v[i]==-1){
            grafico[puntos[i].y/10][puntos[i].x/10]=-1;
        }        
    }
    for(int i=20;i>=0;i--){
        for(int j=0;j<21;j++){
            if(i==y1/10 && j==x1/10){
                cout << "F";
            }
            else if(i==y0/10 && j==x0/10){
                cout << "I";
            }
            else if(grafico[i][j]==0){
                cout << "O"; 
            }
            else if(grafico[i][j]==-1){
                cout << "-";
            }
            else{
                cout << "X";
            }
        }
        cout << endl;
    }
}
struct nivel{
    int x0,y0;
    float dist;
    vector<int> puntos_v;
    nivel(int x1, int y1, float dist1,vector<int> puntos_v1){
        x0=x1;
        y0=y1;
        dist=dist1;
        puntos_v=puntos_v1;
    }
    nivel(const nivel& tmp){
        x0=tmp.x0;
        y0=tmp.y0;
        dist=tmp.dist;
        puntos_v=tmp.puntos_v;
    }
};