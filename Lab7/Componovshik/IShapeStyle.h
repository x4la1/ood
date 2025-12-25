#pragma once
#include <functional>
#include <optional>

class IShapeStyle
{
public:
	virtual void EnableLine() = 0;
	virtual void DisableLine() = 0;
	virtual void EnableFill() = 0;
	virtual void DisableFill() = 0;
	virtual std::optional<bool> IsFillEnable() = 0;
	virtual std::optional<bool> IsLineEnable() = 0;

	virtual std::optional<uint32_t> GetLineColor() const = 0;
	virtual std::optional<uint32_t> GetFillColor() const = 0;
	virtual std::optional<double> GetLineWidth() const = 0;

	virtual void SetLineColor(const uint32_t& color) = 0;
	virtual void SetFillColor(const uint32_t& color) = 0;
	virtual void SetLineWidth(const double& color) = 0;

	~IShapeStyle() = default;
};

class SimpleShapeStyle : public IShapeStyle
{
public:
	void EnableLine() override
	{
		m_isLineEnable = true;
	}

	void DisableLine() override
	{
		m_isLineEnable = false;
	}

	void EnableFill() override
	{
		m_isFillEnable = true;
	}

	void DisableFill() override
	{
		m_isFillEnable = false;
	}

	std::optional<bool> IsFillEnable() override
	{
		return m_isFillEnable;
	}

	std::optional<bool> IsLineEnable() override
	{
		return m_isLineEnable;
	}

	std::optional<uint32_t> GetLineColor() const override
	{
		return m_lineColor;
	}

	std::optional<uint32_t> GetFillColor() const override
	{
		return m_fillColor;
	}

	std::optional<double> GetLineWidth() const override
	{
		return m_lineWidth;
	}

	void SetLineColor(const uint32_t& lineColor) override
	{
		m_lineColor = lineColor;
	}

	void SetFillColor(const uint32_t& fillColor) override
	{
		m_fillColor = fillColor;
	}

	// max line width = 5
	void SetLineWidth(const double& lineWidth) override
	{
		if (lineWidth <= 5.0)
		{
			m_lineWidth = lineWidth;
		}
	}

private:
	bool m_isLineEnable = true;
	bool m_isFillEnable = true;
	uint32_t m_lineColor = 0x000000FF;
	uint32_t m_fillColor = 0xFFFFFFFF;
	double m_lineWidth = 1;
};

class IShapeStyleEnumarator
{
public:
	virtual void EnumarateAll(const std::function<void(IShapeStyle&)>& callback) = 0;
	~IShapeStyleEnumarator() = default;
};

class CompositeShapeStyle : public IShapeStyle
{
public:
	CompositeShapeStyle(IShapeStyleEnumarator& enumerator)
		: m_shapeStyleEnum(&enumerator)
	{
	}

	void EnableLine() override
	{
		m_shapeStyleEnum->EnumarateAll(
			[](IShapeStyle& shapeStyle) {
				shapeStyle.EnableLine();
			});
	}

	void DisableLine() override
	{
		m_shapeStyleEnum->EnumarateAll(
			[](IShapeStyle& shapeStyle) {
				shapeStyle.DisableLine();
			});
	}

	void EnableFill() override
	{
		m_shapeStyleEnum->EnumarateAll(
			[](IShapeStyle& shapeStyle) {
				shapeStyle.EnableFill();
			});
	}

	void DisableFill() override
	{
		m_shapeStyleEnum->EnumarateAll(
			[](IShapeStyle& shapeStyle) {
				shapeStyle.DisableFill();
			});
	}

	std::optional<bool> IsFillEnable() override
	{
		std::optional<bool> isFillEnable = std::nullopt;
		bool isEqual = true;
		m_shapeStyleEnum->EnumarateAll(
			[&](IShapeStyle& shapeStyle) {
				auto isFillEnable = shapeStyle.IsFillEnable();

				if (!isFillEnable)
				{
					isFillEnable = std::nullopt;
					isEqual = false;
				}

				if (!isFillEnable)
				{
					isFillEnable = isFillEnable;
				}
				else if (*isFillEnable != *isFillEnable)
				{
					isFillEnable = std::nullopt;
					isEqual = false;
				}
			});

		if (!isEqual)
		{
			return std::nullopt;
		}

		return isFillEnable;
	}

	std::optional<bool> IsLineEnable() override
	{
		std::optional<bool> isLineEnable = std::nullopt;
		bool isEqual = true;
		m_shapeStyleEnum->EnumarateAll(
			[&](IShapeStyle& shapeStyle) {
				auto isLineEnable = shapeStyle.IsLineEnable();

				if (!isLineEnable)
				{
					isLineEnable = std::nullopt;
					isEqual = false;
				}

				if (!isLineEnable)
				{
					isLineEnable = isLineEnable;
				}
				else if (*isLineEnable != *isLineEnable)
				{
					isLineEnable = std::nullopt;
					isEqual = false;
				}
			});

		if (!isEqual)
		{
			return std::nullopt;
		}

		return isLineEnable;
	}

	std::optional<uint32_t> GetLineColor() const override
	{
		std::optional<uint32_t> lineColor = std::nullopt;
		bool isEqual = true;
		m_shapeStyleEnum->EnumarateAll(
			[&](IShapeStyle& shapeStyle) {
				auto currentColor = shapeStyle.GetLineColor();

				if (!currentColor)
				{
					lineColor = std::nullopt;
					isEqual = false;
				}

				if (!lineColor)
				{
					lineColor = currentColor;
				}
				else if (*currentColor != *lineColor)
				{
					lineColor = std::nullopt;
					isEqual = false;
				}
			});

		if (!isEqual)
		{
			return std::nullopt;
		}

		return lineColor;
	}

	std::optional<uint32_t> GetFillColor() const override
	{
		std::optional<uint32_t> fillColor = std::nullopt;
		bool isEqual = true;
		m_shapeStyleEnum->EnumarateAll(
			[&](IShapeStyle& shapeStyle) {
				auto currentColor = shapeStyle.GetLineColor();

				if (!currentColor)
				{
					fillColor = std::nullopt;
					isEqual = false;
				}

				if (!fillColor)
				{
					fillColor = currentColor;
				}
				else if (*currentColor != *fillColor)
				{
					fillColor = std::nullopt;
					isEqual = false;
				}
			});

		if (!isEqual)
		{
			return std::nullopt;
		}

		return fillColor;
	}

	std::optional<double> GetLineWidth() const override
	{
		std::optional<uint32_t> lineWidth = std::nullopt;
		bool isEqual = true;
		m_shapeStyleEnum->EnumarateAll(
			[&](IShapeStyle& shapeStyle) {
				auto currentWidth = shapeStyle.GetLineColor();

				if (!currentWidth)
				{
					lineWidth = std::nullopt;
					isEqual = false;
				}

				if (!lineWidth)
				{
					lineWidth = currentWidth;
				}
				else if (*currentWidth != *lineWidth)
				{
					lineWidth = std::nullopt;
					isEqual = false;
				}
			});

		if (!isEqual)
		{
			return std::nullopt;
		}

		return lineWidth;
	}

	void SetLineColor(const uint32_t& lineColor) override
	{
		m_shapeStyleEnum->EnumarateAll(
			[&](IShapeStyle& shapeStyle) {
				shapeStyle.SetLineColor(lineColor);
			});
	}

	void SetFillColor(const uint32_t& fillColor) override
	{
		m_shapeStyleEnum->EnumarateAll(
			[&](IShapeStyle& shapeStyle) {
				shapeStyle.SetFillColor(fillColor);
			});
	}

	void SetLineWidth(const double& lineWidth) override
	{
		m_shapeStyleEnum->EnumarateAll(
			[&](IShapeStyle& shapeStyle) {
				shapeStyle.SetLineWidth(lineWidth);
			});
	}

private:
	IShapeStyleEnumarator* m_shapeStyleEnum;
};