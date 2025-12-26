#pragma once
#include <iomanip>
#include <iostream>
#include <sstream>

namespace graphics_lib
{
// ����� ��� ���������
class ICanvas
{
public:
	// ��������� ����� � ������� 0xRRGGBB
	virtual void SetColor(uint32_t rgbColor) = 0;
	// ������ "����" � ����� x, y
	virtual void MoveTo(int x, int y) = 0;
	// ������ ����� � ������� �������, ���������� ���� � ����� x,y
	virtual void LineTo(int x, int y) = 0;
	virtual ~ICanvas() = default;
};

// ���������� ������ ��� ���������
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