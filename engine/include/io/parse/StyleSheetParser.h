#pragma once
#include <string>
#include "../../ui/StyleManager.h"
#include "FileParser.h"

namespace slam::io {

  class StyleSheetParser final : public FileParser {
  public:
    explicit StyleSheetParser(ui::StyleManager &styleManager) : _styleManager(styleManager) {}
    void Parse(const std::string &filePath) override;
    void Write(const std::string &filePath) override;

    template<typename T>
    T ParseValue(const std::string &value);

    static Color ParseColor(const std::string &value);
    static Vector2 ParseVector2(const std::string &value);
    static float ParseFloat(const std::string &value);
    static int ParseInt(const std::string &value);
    static bool ParseBool(const std::string &value);
    static ui::FlexDirection ParseFlexDirection(const std::string &value);
    static ui::JustifyContent ParseJustifyContent(const std::string &value);
    static ui::AlignItems ParseAlignItems(const std::string &value);

  private:
    ui::StyleManager &_styleManager;
    void ParseProperty(const std::string &selector, const std::string &line, ui::StyleState state = ui::StyleState::Default);
    static std::pair<std::string, std::string> SplitProperty(const std::string &line);
  };

} // namespace slam::io
