#include "gtest/gtest.h"
#include "ISlide.h"
#include <memory>

testing::AssertionResult FramesEqual(const std::optional<Frame>& expected, const std::optional<Frame>& actual, float epsilon = 1e-5)
{
	if (!expected.has_value() && !actual.has_value())
		return ::testing::AssertionSuccess();
	if (expected.has_value() != actual.has_value())
		return ::testing::AssertionFailure() << "optional mismatch: expected " << expected.has_value() << ", actual: " << actual.has_value();
	if (std::abs(expected->GetTopLeft().x - actual->GetTopLeft().x) > epsilon ||
		std::abs(expected->GetTopLeft().y - actual->GetTopLeft().y) > epsilon ||
		std::abs(expected->GetWidth() - actual->GetWidth()) > epsilon ||
		std::abs(expected->GetHeight() - actual->GetHeight()) > epsilon)
	{
		return ::testing::AssertionFailure() << "frames differ";
	}
	return ::testing::AssertionSuccess();
}

TEST(GroupShapeTest, EmptyGroupShouldReturnEmptyValues)
{
	GroupShape group;

	EXPECT_FALSE(group.GetFillColor().has_value());
	EXPECT_FALSE(group.GetFrame().has_value());
	EXPECT_FALSE(group.GetOutlineColor().has_value());
	EXPECT_FALSE(group.GetOutlineThickness().has_value());
	EXPECT_TRUE(group.GetShapesCount() == 0);
}

TEST(GroupShapeTest, GetShapeByIndexInOutOfRange)
{
	GroupShape group;

	EXPECT_THROW(group.GetShapeByIndex(0), std::out_of_range);
}

TEST(GroupShapeTest, SingleShape)
{
	GroupShape group;
	auto rect = std::make_unique<Rectangle>(Frame({ 10, 20 }, 30, 40), OutlineStyle(0xFF0000FF, 2.0f), FillStyle(0x00FF00FF));
	group.AddShape(std::move(rect));

	auto frame = group.GetFrame();
	ASSERT_TRUE(frame.has_value());
	EXPECT_FLOAT_EQ(frame->GetTopLeft().x, 10);
	EXPECT_FLOAT_EQ(frame->GetTopLeft().y, 20);
	EXPECT_FLOAT_EQ(frame->GetWidth(), 30);
	EXPECT_FLOAT_EQ(frame->GetHeight(), 40);

	auto outlineColor = group.GetOutlineColor();
	ASSERT_TRUE(outlineColor.has_value());
	EXPECT_EQ(*outlineColor, 0xFF0000FF);

	auto fillColor = group.GetFillColor();
	ASSERT_TRUE(fillColor.has_value());
	EXPECT_EQ(*fillColor, 0x00FF00FF);

	auto thickness = group.GetOutlineThickness();
	ASSERT_TRUE(thickness.has_value());
	EXPECT_FLOAT_EQ(*thickness, 2.0f);
}

TEST(GroupShapeTest, GroupShapesReturnCorrectFrame)
{
	GroupShape group;
	OutlineStyle outlineStyle(0xFF0000FF, 1.0f);
	FillStyle fillStyle(0x00FF00FF);
	group.AddShape(std::make_unique<Rectangle>(Frame({ 0,0 }, 10, 10), outlineStyle, fillStyle));
	group.AddShape(std::make_unique<Ellipse>(Frame({ 20,20 }, 15, 15), outlineStyle, fillStyle));

	auto frame = group.GetFrame();
	ASSERT_TRUE(frame.has_value());
	EXPECT_FLOAT_EQ(frame->GetTopLeft().x, 0);
	EXPECT_FLOAT_EQ(frame->GetTopLeft().y, 0);
	EXPECT_FLOAT_EQ(frame->GetWidth(), 35);
	EXPECT_FLOAT_EQ(frame->GetHeight(), 35);
}

TEST(GroupShapeTest, GroupShapesWithSameStyle)
{
	GroupShape group;
	OutlineStyle outlineStyle(0xFF0000FF, 1.0f);
	FillStyle fillStyle(0x00FF00FF);
	group.AddShape(std::make_unique<Rectangle>(Frame({ 0,0 }, 10, 10), outlineStyle, fillStyle));
	group.AddShape(std::make_unique<Ellipse>(Frame({ 20,20 }, 15, 15), outlineStyle, fillStyle));

	EXPECT_EQ(group.GetOutlineColor(), 0xFF0000FF);
	EXPECT_EQ(group.GetFillColor(), 0x00FF00FF);
	EXPECT_EQ(group.GetOutlineThickness(), 1.0f);
}

TEST(GroupShapeTest, GroupShapesWithDifferentStylesAndFrames)
{
	GroupShape group;
	group.AddShape(std::make_unique<Rectangle>(Frame({ 0,0 }, 10, 10), OutlineStyle(0xFF0000FF, 1.0f), FillStyle(0x00FF00FF)));
	group.AddShape(std::make_unique<Ellipse>(Frame({ 20,20 }, 15, 15), OutlineStyle(0x0000FFFF, 2.0f), FillStyle(0x00FF00FF)));

	EXPECT_FALSE(group.GetOutlineColor().has_value());
	EXPECT_EQ(group.GetFillColor(), 0x00FF00FF);
	EXPECT_FALSE(group.GetOutlineThickness().has_value());
}

TEST(GroupShapeTest, GroupShapesSetFrameScalesChildren)
{
	GroupShape group;
	OutlineStyle outlineStyle(0xFF0000FF, 1.0f);
	FillStyle fillStyle(0x00FF00FF);
	group.AddShape(std::make_unique<Rectangle>(Frame({ 0,0 }, 50, 40), outlineStyle, fillStyle));
	group.AddShape(std::make_unique<Rectangle>(Frame({ 50,0 }, 50, 40), outlineStyle, fillStyle));

	Frame newFrame({ 0,0 }, 200, 80);
	group.SetFrame(newFrame);

	auto frame0 = group.GetShapeByIndex(0)->GetFrame();
	ASSERT_TRUE(frame0.has_value());
	EXPECT_FLOAT_EQ(frame0->GetTopLeft().x, 0);
	EXPECT_FLOAT_EQ(frame0->GetTopLeft().y, 0);
	EXPECT_FLOAT_EQ(frame0->GetWidth(), 100);
	EXPECT_FLOAT_EQ(frame0->GetHeight(), 80);

	auto frame1 = group.GetShapeByIndex(1)->GetFrame();
	ASSERT_TRUE(frame1.has_value());
	EXPECT_FLOAT_EQ(frame1->GetTopLeft().x, 100);
	EXPECT_FLOAT_EQ(frame1->GetTopLeft().y, 0);
	EXPECT_FLOAT_EQ(frame1->GetWidth(), 100);
	EXPECT_FLOAT_EQ(frame1->GetHeight(), 80);
}

TEST(GroupShapeTest, SetStylesPropagatesToChildren)
{
	GroupShape group;
	OutlineStyle outlineStyle(0xFF0000FF, 1.0f);
	FillStyle fillStyle(0x00FF00FF);
	group.AddShape(std::make_unique<Rectangle>(Frame({ 0,0 }, 50, 40), outlineStyle, fillStyle));
	group.AddShape(std::make_unique<Rectangle>(Frame({ 50,0 }, 50, 40), outlineStyle, fillStyle));

	group.SetOutlineColor(0x0000FFFF);
	group.SetFillColor(0xFFFFFFFF);
	group.SetOutlineThickness(3.0f);

	for (size_t i = 0; i < group.GetShapesCount(); ++i)
	{
		auto* shape = group.GetShapeByIndex(i);
		EXPECT_EQ(shape->GetOutlineColor(), 0x0000FFFF);
		EXPECT_EQ(shape->GetFillColor(), 0xFFFFFFFF);
		EXPECT_EQ(shape->GetOutlineThickness(), 3.0f);
	}
}

TEST(GroupShapeTest, CloneCreatesIndependentCopy)
{
	GroupShape original;
	original.AddShape(std::make_unique<Rectangle>(Frame({ 0,0 }, 10, 10), OutlineStyle(0xFF0000FF, 1.0f), FillStyle(0x00FF00FF)));

	auto clonePtr = original.Clone();
	auto* clone = dynamic_cast<GroupShape*>(clonePtr.get());
	ASSERT_NE(clone, nullptr);

	original.GetShapeByIndex(0)->SetFrame(Frame({ 5,5 }, 20, 20));
	original.SetOutlineColor(0x0000FFFF);

	auto originalFrame = original.GetShapeByIndex(0)->GetFrame();
	auto cloneFrame = clone->GetShapeByIndex(0)->GetFrame();
	EXPECT_FALSE(FramesEqual(originalFrame, cloneFrame));

	EXPECT_NE(original.GetOutlineColor(), clone->GetOutlineColor());
}

TEST(GroupShapeTest, GroupShapesByIndexes)
{
	GroupShape group;
	group.AddShape(std::make_unique<Rectangle>(Frame({ 0,0 }, 10, 10), OutlineStyle(0xFF0000FF, 1.0f), FillStyle(0x00FF00FF)));
	group.AddShape(std::make_unique<Rectangle>(Frame({ 20,0 }, 10, 10), OutlineStyle(0xFF0000FF, 1.0f), FillStyle(0x00FF00FF)));
	group.AddShape(std::make_unique<Rectangle>(Frame({ 40,0 }, 10, 10), OutlineStyle(0xFF0000FF, 1.0f), FillStyle(0x00FF00FF)));

	group.GroupShapesByIndexes({ 0, 2 });

	EXPECT_EQ(group.GetShapesCount(), 2);

	auto* shape = group.GetShapeByIndex(0);
	EXPECT_EQ(dynamic_cast<IGroupShapes*>(shape), nullptr);

	auto* maybeGroup = dynamic_cast<IGroupShapes*>(group.GetShapeByIndex(1));
	ASSERT_NE(maybeGroup, nullptr);
	EXPECT_EQ(maybeGroup->GetShapesCount(), 2);
}