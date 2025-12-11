#pragma once
#include <iomanip>
#include <iostream>
#include <sstream>

namespace graphics_lib
{
// Холст для рисования
class ICanvas
{
public:
	// Установка цвета в формате 0xRRGGBB
	virtual void SetColor(uint32_t rgbColor) = 0;
	// Ставит "перо" в точку x, y
	virtual void MoveTo(int x, int y) = 0;
	// Рисует линию с текущей позиции, передвигая перо в точку x,y
	virtual void LineTo(int x, int y) = 0;
	virtual ~ICanvas() = default;
};

// Реализация холста для рисования
class Canvas : public ICanvas
{
public:
	void SetColor(uint32_t rgbColor) override
	{
		std::stringstream ss{};
		ss << "#" << std::hex << std::uppercase << std::setfill('0') << std::setw(6) << (rgbColor & 0xFFFFFF);
		std::cout << "SetColor(" + ss.str() + ")" << "\n";
	}
	void MoveTo(int x, int y) override
	{
		std::cout << "MoveTo (" << x << ", " << y << ")" << std::endl;
	}
	void LineTo(int x, int y) override
	{
		std::cout << "LineTo (" << x << ", " << y << ")" << std::endl;
	}
};
} // namespace graphics_lib