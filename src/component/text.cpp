#include "cos_graphics/component/text.h"
#include "cos_graphics/game.h"

void CGText::UpdateTextImage()
{
    if (texture != nullptr)
        CGFree(texture);
    if (text[0] == (CGChar)('\0'))
        return;
    if (font_rk[0] == (CGChar)('\0'))
        texture = CGCreateTextVisualImageRaw(text.c_str(), NULL, CGTextProperty(), CGGame::GetInstance()->GetGameWindow(), CG_FALSE);
    else
        texture = CGCreateTextVisualImage(text.c_str(), font_rk.c_str(), CGTextProperty(), CGGame::GetInstance()->GetGameWindow(), CG_FALSE);
}

CGText::CGText(const CGString& p_text, const CGTextProperty& p_text_property, bool p_is_text_rk) 
    : text(p_text), font_rk(CGSTR("")), text_property(p_text_property), CGTextureContainer()
{
    CGChar* string_data = p_is_text_rk ? (CGChar*)CGLoadResource(p_text.c_str(), NULL, NULL) : (CGChar*)p_text.c_str();
    font_rk = CGSTR("");
    if (p_is_text_rk)
    {
        text = CGString(string_data);
        free(string_data);
    }
    else
        text = p_text;
    
    texture = CGCreateTextVisualImageRaw(p_text.c_str(), NULL, p_text_property, CGGame::GetInstance()->GetGameWindow(), CG_FALSE);
}

CGText::CGText(const CGString& p_text, const CGString& p_font_rk, const CGTextProperty& p_text_property, bool p_is_text_rk)
    : text(p_text), font_rk(p_font_rk), text_property(p_text_property), CGTextureContainer()
{
    CGChar* string_data = p_is_text_rk ? (CGChar*)CGLoadResource(p_text.c_str(), NULL, NULL) : (CGChar*)p_text.c_str();
    font_rk = p_font_rk;
    if (p_is_text_rk)
    {
        text = CGString(string_data);
        free(string_data);
    }
    else
        text = p_text;
    texture = CGCreateTextVisualImage(p_text.c_str(), p_font_rk.c_str(), p_text_property, CGGame::GetInstance()->GetGameWindow(), CG_FALSE);
}

CGText::CGText(const CGText& other) : CGTextureContainer(other)
{
    text = other.text;
    font_rk = other.font_rk;
    text_property = other.text_property;
    UpdateTextImage();
}

CGText::CGText(CGText&& other) noexcept : CGTextureContainer(std::move(other))
{
    text = std::move(other.text);
    font_rk = std::move(other.font_rk);
    text_property = std::move(other.text_property);
    UpdateTextImage();
}

CGText::~CGText()
{
    
}

void CGText::Draw(float p_delta)
{
    if (texture == nullptr)
        return;
    CGDrawVisualImage(texture, GetRenderProperty(), CGGame::GetInstance()->GetGameWindow());
}

void CGText::SetText(const CGString& p_text_rk)
{
    auto string_data = (CGChar*)CGLoadResource(p_text_rk.c_str(), NULL, NULL);
    text = CGString(string_data);
    CGFree(string_data);
    UpdateTextImage();
}

const CGString& CGText::GetText() const noexcept
{
    return text;
}

void CGText::SetFont(const CGString& p_font_rk)
{
    font_rk = p_font_rk;
    UpdateTextImage();
}

const CGTextProperty& CGText::GetTextProperty() const noexcept
{
    return text_property;
}

void CGText::SetTextProperty(const CGTextProperty& p_property)
{
    text_property = p_property;
    UpdateTextImage();
}
