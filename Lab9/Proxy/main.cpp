#include "Drawer.h"
#include "Image.h"
#include <iostream>

void DrawSnowman()
{
	Image image({ 60, 30 }, ' ');

	// Земля
	DrawLine(image, { 0, 25 }, { 59, 25 }, '_');

	// Солнце
	DrawCircle(image, { 49, 7 }, 5, '@');

	// Нижняя часть
	FillCircle(image, { 25, 20 }, 7, 'O');
	// Средняя часть
	FillCircle(image, { 25, 11 }, 5, 'O');
	// Голова
	FillCircle(image, { 25, 5 }, 3, 'o');

	// Глаза
	FillCircle(image, { 23, 4 }, 0, '@');
	FillCircle(image, { 27, 4 }, 0, '@');

	// Нос
	DrawLine(image, { 29, 5 }, { 25, 6 }, '^');

	// Левая рука
	DrawLine(image, { 20, 11 }, { 15, 8 }, '|');
	// Правая рука
	DrawLine(image, { 30, 11 }, { 35, 8 }, '|');

	// Шляпа
	DrawLine(image, { 22, 2 }, { 28, 2 }, '#');
	DrawLine(image, { 23, 1 }, { 27, 1 }, '#');
	DrawLine(image, { 24, 0 }, { 26, 0 }, '#');

	//DrawCircle(image, { 15, 15 }, 4, '#');

	Print(image, std::cout);
}

int main()
{
	DrawSnowman();
	return 0;
}