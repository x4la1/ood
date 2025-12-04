#include "gtest/gtest.h"
#include "Image.h"

class ImageTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		ASSERT_EQ(Tile::GetInstanceCount(), 0);
	}
};

TEST_F(ImageTest, TileCounterAndConstructorTest)
{
	{
		Tile t1;
		EXPECT_EQ(Tile::GetInstanceCount(), 1);
		EXPECT_EQ(t1.GetPixel({ 0, 0 }), ' ');

		Tile t2('A');
		EXPECT_EQ(Tile::GetInstanceCount(), 2);
		EXPECT_EQ(t2.GetPixel({ 0, 0 }), 'A');
	}
	EXPECT_EQ(Tile::GetInstanceCount(), 0);
}

TEST_F(ImageTest, TilePixelsAccess)
{
	Tile tile('X');
	tile.SetPixel({ 3, 4 }, 'Z');
	EXPECT_EQ(tile.GetPixel({ 3, 4 }), 'Z');

	EXPECT_EQ(tile.GetPixel({ Tile::SIZE, Tile::SIZE }), ' ');
	EXPECT_EQ(tile.GetPixel({ -1, -1 }), ' ');

	tile.SetPixel({ Tile::SIZE, 0 }, 'Y');
	EXPECT_EQ(tile.GetPixel({ 0, 0 }), 'X');
}

TEST_F(ImageTest, TileCopyConstructor)
{
	Tile t1('C');
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	Tile t2 = t1;
	EXPECT_EQ(Tile::GetInstanceCount(), 2);

	t2.SetPixel({ 0, 0 }, 'D');
	EXPECT_EQ(t1.GetPixel({ 0, 0 }), 'C');
	EXPECT_EQ(t2.GetPixel({ 0, 0 }), 'D');
}

TEST_F(ImageTest, ImageInvalidConstructor)
{
	ASSERT_THROW(Image({ -10, 10 }, '*'), std::out_of_range);
	ASSERT_THROW(Image({ 10, -10 }, '*'), std::out_of_range);
	ASSERT_THROW(Image({ 0, 0 }, '*'), std::out_of_range);
}

TEST_F(ImageTest, ImageCopyOnWrite)
{
	Image img1({ 16, 16 }, 'A');
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	Image img2 = img1;
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	img2.SetPixel({ 0, 0 }, 'B');
	EXPECT_EQ(Tile::GetInstanceCount(), 2);

	EXPECT_EQ(img1.GetPixel({ 0, 0 }), 'A');
	EXPECT_EQ(img2.GetPixel({ 0, 0 }), 'B');
}

TEST_F(ImageTest, ImageMultipleTileWrites)
{
	Image img1({ 16, 16 }, 'X');
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	Image img2 = img1;
	EXPECT_EQ(Tile::GetInstanceCount(), 1);

	img2.SetPixel({ 1, 1 }, 'A');
	EXPECT_EQ(Tile::GetInstanceCount(), 2);

	img2.SetPixel({ 2, 2 }, 'B');
	EXPECT_EQ(Tile::GetInstanceCount(), 2);

	img2.SetPixel({ 10, 10 }, 'C');
	EXPECT_EQ(Tile::GetInstanceCount(), 3);

	EXPECT_EQ(img1.GetPixel({ 1, 1 }), 'X');
	EXPECT_EQ(img1.GetPixel({ 10, 10 }), 'X');

	EXPECT_EQ(img2.GetPixel({ 1, 1 }), 'A');
	EXPECT_EQ(img2.GetPixel({ 2, 2 }), 'B');
	EXPECT_EQ(img2.GetPixel({ 10, 10 }), 'C');
}