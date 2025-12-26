#include "gtest/gtest.h"
#include "ModernClassAdapter.h"
#include "ModernObjectAdapter.h"
#include <format>

std::string MakeExpectedLine(int x1, int y1, int x2, int y2, float r, float g, float b, float a)
{
	return std::format(
		R"(<line fromX="{}" fromY="{}" toX="{}" toY="{}">
  <color r="{:.2f}" g="{:.2f}" b="{:.2f}" a="{:.2f}" />
</line>
)",
x1, y1, x2, y2, r, g, b, a);
}

TEST(ModernObjectAdapterTest, CanDrawSingleLineWithColor)
{
	std::stringstream strm;
	modern_graphics_lib::ModernGraphicsRenderer renderer(strm);
	ModernObjectAdapter adapter(renderer);

	const int startX = 10, startY = 20;
	const int endX = 110, endY = 120;
	const uint32_t rgbColor = 0x112233;

	adapter.SetColor(rgbColor);
	adapter.MoveTo(startX, startY);
	adapter.LineTo(endX, endY);

	std::string output = strm.str();

	EXPECT_TRUE(output.find("<draw>") != std::string::npos);
	EXPECT_TRUE(output.find("</draw>") == std::string::npos);

	std::string expectedLine = MakeExpectedLine(startX, startY, endX, endY,
		17 / 255.0f, 34 / 255.0f, 51 / 255.0f, 1.0f);

	EXPECT_TRUE(output.find(expectedLine) != std::string::npos);
}

TEST(ModernObjectAdapterTest, CanDrawTriangle)
{
	std::stringstream strm;
	modern_graphics_lib::ModernGraphicsRenderer renderer(strm);
	ModernObjectAdapter adapter(renderer);

	const uint32_t color = 0xFF0000;

	adapter.SetColor(color);
	adapter.MoveTo(0, 0);
	adapter.LineTo(10, 0);
	adapter.LineTo(10, 10);
	adapter.LineTo(0, 0);

	std::string output = strm.str();

	std::string line1 = MakeExpectedLine(0, 0, 10, 0, 1.0f, 0.0f, 0.0f, 1.0f);
	std::string line2 = MakeExpectedLine(10, 0, 10, 10, 1.0f, 0.0f, 0.0f, 1.0f);
	std::string line3 = MakeExpectedLine(10, 10, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f);

	EXPECT_TRUE(output.find(line1) != std::string::npos);
	EXPECT_TRUE(output.find(line2) != std::string::npos);
	EXPECT_TRUE(output.find(line3) != std::string::npos);
}

TEST(ModernObjectAdapterTest, ManagesDrawingLifecycleCorrectly)
{
	std::stringstream strm;

	{
		modern_graphics_lib::ModernGraphicsRenderer renderer(strm);
		ModernObjectAdapter adapter(renderer);
	}

	std::string output = strm.str();
	EXPECT_TRUE(output.find("<draw>") != std::string::npos);
	EXPECT_TRUE(output.find("</draw>") != std::string::npos);
}

TEST(ModernObjectAdapterTest, LineToDrawsFromPreviousEndPoint)
{
	std::stringstream strm;
	modern_graphics_lib::ModernGraphicsRenderer renderer(strm);
	ModernObjectAdapter adapter(renderer);

	adapter.MoveTo(1, 1);
	adapter.LineTo(2, 2);
	adapter.LineTo(3, 3);

	std::string output = strm.str();

	std::string line1 = MakeExpectedLine(1, 1, 2, 2, 0.0f, 0.0f, 0.0f, 1.0f);
	std::string line2 = MakeExpectedLine(2, 2, 3, 3, 0.0f, 0.0f, 0.0f, 1.0f);

	EXPECT_TRUE(output.find(line1) != std::string::npos);
	EXPECT_TRUE(output.find(line2) != std::string::npos);
}

TEST(ModernClassAdapterTest, CanDrawSingleLineWithColor)
{
	std::stringstream strm;
	ModernClassAdapter adapter(strm);

	const int startX = 10, startY = 20;
	const int endX = 110, endY = 120;
	const uint32_t rgbColor = 0x112233;

	adapter.SetColor(rgbColor);
	adapter.MoveTo(startX, startY);
	adapter.LineTo(endX, endY);

	std::string output = strm.str();

	EXPECT_TRUE(output.find("<draw>") != std::string::npos);
	EXPECT_TRUE(output.find("</draw>") == std::string::npos);

	std::string expectedLine = MakeExpectedLine(startX, startY, endX, endY,
		17 / 255.0f, 34 / 255.0f, 51 / 255.0f, 1.0f);

	EXPECT_TRUE(output.find(expectedLine) != std::string::npos);
}

TEST(ModernClassAdapterTest, CanDrawTriangle)
{
	std::stringstream strm;
	ModernClassAdapter adapter(strm);

	const uint32_t color = 0xFF0000;

	adapter.SetColor(color);
	adapter.MoveTo(0, 0);
	adapter.LineTo(10, 0);
	adapter.LineTo(10, 10);
	adapter.LineTo(0, 0);

	std::string output = strm.str();

	std::string line1 = MakeExpectedLine(0, 0, 10, 0, 1.0f, 0.0f, 0.0f, 1.0f);
	std::string line2 = MakeExpectedLine(10, 0, 10, 10, 1.0f, 0.0f, 0.0f, 1.0f);
	std::string line3 = MakeExpectedLine(10, 10, 0, 0, 1.0f, 0.0f, 0.0f, 1.0f);

	EXPECT_TRUE(output.find(line1) != std::string::npos);
	EXPECT_TRUE(output.find(line2) != std::string::npos);
	EXPECT_TRUE(output.find(line3) != std::string::npos);
}

TEST(ModernClassAdapterTest, ManagesDrawingLifecycleCorrectly)
{
	std::stringstream strm;

	{
		ModernClassAdapter adapter(strm);
	}

	std::string output = strm.str();
	EXPECT_TRUE(output.find("<draw>") != std::string::npos);
	EXPECT_TRUE(output.find("</draw>") != std::string::npos);
}

TEST(ModernClassAdapterTest, LineToDrawsFromPreviousEndPoint)
{
	std::stringstream strm;
	ModernClassAdapter adapter(strm);

	adapter.MoveTo(1, 1);
	adapter.LineTo(2, 2);
	adapter.LineTo(3, 3);

	std::string output = strm.str();

	std::string line1 = MakeExpectedLine(1, 1, 2, 2, 0.0f, 0.0f, 0.0f, 1.0f);
	std::string line2 = MakeExpectedLine(2, 2, 3, 3, 0.0f, 0.0f, 0.0f, 1.0f);

	EXPECT_TRUE(output.find(line1) != std::string::npos);
	EXPECT_TRUE(output.find(line2) != std::string::npos);
}