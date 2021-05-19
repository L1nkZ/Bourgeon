#include "ui/text.h"

#include "imgui.h"

namespace ui {

const std::string &Text::text() const { return text_; }

void Text::set_text(std::string value) { text_ = std::move(value); }

void Text::Show() { ImGui::TextUnformatted(text_.c_str()); }

}  // namespace ui