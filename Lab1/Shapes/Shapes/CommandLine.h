#include "ICanvas.h"
#include "Picture.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace cli
{

enum class CommandType
{
	AddShape,
	MoveShape,
	MovePicture,
	DeleteShape,
	ChangeColor,
	ChangeShape,
	DrawShape,
	DrawPicture,
	List,
};

class CommandLine
{
public:
	CommandLine(gfx::ICanvas& canvas);
	void HandleCommand(const std::string& command);

private:
	double ParseDouble(const std::string& line) const;
	uint32_t ParseColor(const std::string& line) const;

	void HandleAddShape(const std::vector<std::string>& tokens);
	void HandleMovePicture(const std::vector<std::string>& tokens);
	void HandleMoveShape(const std::vector<std::string>& tokens);
	void HandleDeleteShape(const std::vector<std::string>& tokens);
	void HandleChangeColor(const std::vector<std::string>& tokens);
	void HandleChangeShape(const std::vector<std::string>& tokens);
	void HandleDrawShape(const std::vector<std::string>& tokens) const;
	void HandleDrawPicture() const;
	void HandleList() const;

	CommandType GetCommandType(const std::string& command) const;
	std::unique_ptr<shapes::IShapeStrategy> CreateStrategy(const std::string& type, std::vector<std::string>& params) const;

	gfx::ICanvas& m_canvas;
	shapes::Picture m_picture;

	const static std::unordered_map<std::string, CommandType> s_commandTypes;
};

} // namespace cli