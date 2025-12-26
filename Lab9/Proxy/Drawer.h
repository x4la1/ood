#pragma once

#include "Image.h"

void DrawLine(Image& image, Point from, Point to, char color);

void DrawCircle(Image& image, Point center, int radius, char color);

void FillCircle(Image& image, Point center, int radius, char color);