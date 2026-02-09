#include <graphics.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

struct Punct
{
    int x, y;
};

int main()
{
    double alfa, v0;
    const double g = 9.81;
    const double PI = 3.14159265;

    cout << "Introduceti unghiul alfa (grade): "; cin >> alfa;
    cout << "Introduceti viteza initiala (m/s): "; cin >> v0;

    double rad = alfa * PI / 180.0;

    // Calcul fizic teoretic
    double R = (v0 * v0 * sin(2 * rad)) / g;
    double H = (v0 * v0 * pow(sin(rad), 2)) / (2 * g);

    // Zoom adaptiv imbunatatit
    if (R < 10) R = 10;
    if (H < 10) H = 10;
    double zoomX = 850.0 / R;
    double zoomY = 450.0 / H;
    double zoom = (zoomX < zoomY) ? zoomX : zoomY;

    initwindow(1000, 600, "Simulare Profesionala Proiectil");

    double t = 0;
    double pas_timp = 0.015;
    vector<Punct> traiectorie;

    int pagina = 0;

    while (true)
    {
        double fX = v0 * cos(rad) * t;
        double fY = v0 * sin(rad) * t - (0.5 * g * t * t);

        int dX = 50 + (int)(fX * zoom);
        int dY = 550 - (int)(fY * zoom);

        if (dY >= 550 && t > 0.05) {
            dY = 550;
            traiectorie.push_back({dX, dY});
            break;
        }

        traiectorie.push_back({dX, dY});

        // --- TEHNICA DOUBLE BUFFERING (pentru fluiditate maxima) ---
        setactivepage(1 - pagina);
        cleardevice();

        // Solul
        setcolor(WHITE);
        line(0, 550, 1000, 550);

        // Traiectorie
        setlinestyle(DOTTED_LINE, 0, 1);
        setcolor(8);
        for (size_t i = 1; i < traiectorie.size(); i++) {
            line(traiectorie[i-1].x, traiectorie[i-1].y, traiectorie[i].x, traiectorie[i].y);
        }

        // Obiectul
        setlinestyle(SOLID_LINE, 0, 1);
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(dX, dY, 6, 6);

        setvisualpage(1 - pagina);
        pagina = 1 - pagina;

        delay(15);
        t += pas_timp;

        if (dX > 1000) break; // Siguranta sa nu mearga la infinit in dreapta
    }

    // Afisare finala pe ultima pagina activa
    setcolor(YELLOW);
    char buf[200];
    sprintf(buf, "Viteza: %.1f m/s | Unghi: %.1f grade | Distanta: %.2f m | Timp: %.2f s", v0, alfa, v0 * cos(rad) * t, t);
    outtextxy(20, 20, buf);
    outtextxy(20, 40, (char*)"Simulare incheiata. Apasa o tasta.");

    getch();
    closegraph();
    return 0;
}
