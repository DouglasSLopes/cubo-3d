#include <iostream>
#include <GL/freeglut.h>
#include <vector>
#include <cmath>

using namespace std;

// ============================================================
//  Tipos básicos — compatível com C++11
// ============================================================
struct vertice {
    double x, y, z;
    vertice(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
};

struct aresta {
    int origem, destino;
    aresta(int o, int d) : origem(o), destino(d) {}
};

typedef vector<vertice> lista_vertices;
typedef vector<aresta>  lista_arestas;

// ============================================================
//  Estrutura do polígono 3-D
// ============================================================
struct Poligono3D {
    vertice posicao;
    double  escala_x, escala_y, escala_z;
    double  rot_x, rot_y, rot_z;
    lista_vertices vertices;
    lista_arestas  arestas;

    Poligono3D() : escala_x(1), escala_y(1), escala_z(1),
        rot_x(0), rot_y(0), rot_z(0) {
    }
};

// ============================================================
//  Protótipos
// ============================================================
Poligono3D criar_cubo(double cx, double cy, double cz, double lado);
void desenhar(const Poligono3D& p);
void movimentar(Poligono3D& p, double dx, double dy, double dz);
void escalar(Poligono3D& p, double ex, double ey, double ez);
void rotacionar(Poligono3D& p, double ax, double ay, double az);
void display();
void redraw(int value);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void mouse(int button, int state, int x, int y);

// ============================================================
//  Variáveis globais
// ============================================================
Poligono3D cubo;
const int DELAY = 16;

// ============================================================
//  main
// ============================================================
int main(int argc, char** argv) {
    cubo = criar_cubo(256, 256, 0, 80);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Cubo 3D - Wireframe");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 512, 0, 512, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_special);
    glutMouseFunc(mouse);
    glutTimerFunc(DELAY, redraw, 0);

    cout << "=== Cubo 3D ===\n";
    cout << "Translacao : setas (X/Y) | PgUp/PgDn (Z)\n";
    cout << "Rotacao    : W/S (X) | A/D (Y) | Q/E (Z)\n";
    cout << "Escala     : +/- (uniforme) | mouse esq(+) / dir(-)\n";
    cout << "ESC        : sair\n";

    glutMainLoop();
    return 0;
}

// ============================================================
//  Criação do cubo (vértices em coordenadas locais)
// ============================================================
Poligono3D criar_cubo(double cx, double cy, double cz, double lado) {
    Poligono3D c;
    c.posicao = vertice(cx, cy, cz);
    c.escala_x = c.escala_y = c.escala_z = 1.0;
    c.rot_x = c.rot_y = c.rot_z = 0.0;

    double h = lado / 2.0;

    // 8 vértices descritos diretamente
    c.vertices.push_back(vertice(-h, -h, -h)); // 0 frente-baixo-esq
    c.vertices.push_back(vertice(h, -h, -h)); // 1 frente-baixo-dir
    c.vertices.push_back(vertice(h, h, -h)); // 2 frente-cima-dir
    c.vertices.push_back(vertice(-h, h, -h)); // 3 frente-cima-esq
    c.vertices.push_back(vertice(-h, -h, h)); // 4 tras-baixo-esq
    c.vertices.push_back(vertice(h, -h, h)); // 5 tras-baixo-dir
    c.vertices.push_back(vertice(h, h, h)); // 6 tras-cima-dir
    c.vertices.push_back(vertice(-h, h, h)); // 7 tras-cima-esq

    // 12 arestas
    c.arestas.push_back(aresta(0, 1)); // face frontal
    c.arestas.push_back(aresta(1, 2));
    c.arestas.push_back(aresta(2, 3));
    c.arestas.push_back(aresta(3, 0));
    c.arestas.push_back(aresta(4, 5)); // face traseira
    c.arestas.push_back(aresta(5, 6));
    c.arestas.push_back(aresta(6, 7));
    c.arestas.push_back(aresta(7, 4));
    c.arestas.push_back(aresta(0, 4)); // conexoes
    c.arestas.push_back(aresta(1, 5));
    c.arestas.push_back(aresta(2, 6));
    c.arestas.push_back(aresta(3, 7));

    return c;
}

// ============================================================
//  Projeção perspectiva manual
// ============================================================
static void projetar(double x, double y, double z, double& px, double& py) {
    const double fov = 400.0;
    const double camZ = 400.0;
    double denom = camZ - z;
    if (denom < 1.0) denom = 1.0;
    px = (x - 256.0) * fov / denom + 256.0;
    py = (y - 256.0) * fov / denom + 256.0;
}

// ============================================================
//  Transformar um vértice local aplicando escala + rotações
// ============================================================
static vertice transformar(const vertice& v, const Poligono3D& p) {
    double x = v.x * p.escala_x;
    double y = v.y * p.escala_y;
    double z = v.z * p.escala_z;

    // Rotação X
    double y2 = y * cos(p.rot_x) - z * sin(p.rot_x);
    double z2 = y * sin(p.rot_x) + z * cos(p.rot_x);
    y = y2; z = z2;

    // Rotação Y
    double x2 = x * cos(p.rot_y) + z * sin(p.rot_y);
    z2 = -x * sin(p.rot_y) + z * cos(p.rot_y);
    x = x2; z = z2;

    // Rotação Z
    x2 = x * cos(p.rot_z) - y * sin(p.rot_z);
    y2 = x * sin(p.rot_z) + y * cos(p.rot_z);
    x = x2; y = y2;

    // Translação
    x += p.posicao.x;
    y += p.posicao.y;
    z += p.posicao.z;

    return vertice(x, y, z);
}

// ============================================================
//  Desenho
// ============================================================
void desenhar(const Poligono3D& p) {
    glColor3f(0.0f, 1.0f, 0.4f);
    glBegin(GL_LINES);

    for (int i = 0; i < (int)p.arestas.size(); i++) {
        vertice v0 = transformar(p.vertices[p.arestas[i].origem], p);
        vertice v1 = transformar(p.vertices[p.arestas[i].destino], p);

        double px0, py0, px1, py1;
        projetar(v0.x, v0.y, v0.z, px0, py0);
        projetar(v1.x, v1.y, v1.z, px1, py1);

        glVertex2f((float)px0, (float)py0);
        glVertex2f((float)px1, (float)py1);
    }

    glEnd();
}

// ============================================================
//  Movimentar, Escalar, Rotacionar
// ============================================================
void movimentar(Poligono3D& p, double dx, double dy, double dz) {
    p.posicao.x += dx;
    p.posicao.y += dy;
    p.posicao.z += dz;
}

void escalar(Poligono3D& p, double ex, double ey, double ez) {
    p.escala_x *= ex;
    p.escala_y *= ey;
    p.escala_z *= ez;
}

void rotacionar(Poligono3D& p, double ax, double ay, double az) {
    p.rot_x += ax;
    p.rot_y += ay;
    p.rot_z += az;
}

// ============================================================
//  Callbacks OpenGL
// ============================================================
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    desenhar(cubo);
    glFlush();
}

void redraw(int value) {
    (void)value;
    glutPostRedisplay();
    glutTimerFunc(DELAY, redraw, 0);
}

void keyboard(unsigned char key, int x, int y) {
    (void)x; (void)y;
    const double pr = 0.05;
    const double pe = 1.1;
    switch (key) {
    case 27:             exit(0);                              break;
    case 'w': case 'W': rotacionar(cubo, pr, 0, 0);        break;
    case 's': case 'S': rotacionar(cubo, -pr, 0, 0);        break;
    case 'a': case 'A': rotacionar(cubo, 0, pr, 0);        break;
    case 'd': case 'D': rotacionar(cubo, 0, -pr, 0);        break;
    case 'q': case 'Q': rotacionar(cubo, 0, 0, pr);       break;
    case 'e': case 'E': rotacionar(cubo, 0, 0, -pr);       break;
    case '+': case '=': escalar(cubo, pe, pe, pe);            break;
    case '-': case '_': escalar(cubo, 1.0 / pe, 1.0 / pe, 1.0 / pe); break;
    }
}

void keyboard_special(int key, int x, int y) {
    (void)x; (void)y;
    const double pm = 10.0;
    switch (key) {
    case GLUT_KEY_UP:        movimentar(cubo, 0, pm, 0); break;
    case GLUT_KEY_DOWN:      movimentar(cubo, 0, -pm, 0); break;
    case GLUT_KEY_RIGHT:     movimentar(cubo, pm, 0, 0); break;
    case GLUT_KEY_LEFT:      movimentar(cubo, -pm, 0, 0); break;
    case GLUT_KEY_PAGE_UP:   movimentar(cubo, 0, 0, pm); break;
    case GLUT_KEY_PAGE_DOWN: movimentar(cubo, 0, 0, -pm); break;
    }
}

void mouse(int button, int state, int x, int y) {
    (void)x; (void)y;
    if (state != GLUT_DOWN) return;
    const double f = 1.15;
    if (button == GLUT_LEFT_BUTTON)  escalar(cubo, f, f, f);
    else if (button == GLUT_RIGHT_BUTTON) escalar(cubo, 1.0 / f, 1.0 / f, 1.0 / f);
}