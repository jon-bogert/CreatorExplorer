#include "Pch.h"
#include "Item.h"
#include "AssetManager.h"

const std::vector<std::string> TXT_EXT =
{
    ".txt", ".TXT",
    ".h", ".H",
    ".c", ".C",
    ".hpp", ".HPP",
    ".cpp", ".CPP",
    ".cs", ".CS",
    ".py", ".PY",
    ".bat", ".BAT",
    ".xml", ".XML",
    ".dat", ".DAT",
    ".config", ".CONFIG",
    ".properties", ".PROPERTIES",
    ".gitignore", ".GITIGNORE",
    ".gitattributes", ".GITATTRIBUTES",
    ".md", ".MD"
};
const std::vector<std::string> IMG_EXT =
{
    ".jpg", ".JPG",
    ".png", ".PNG"
};
const std::vector<std::string> AUD_EXT =
{
    ".mp3", ".MP3",
    ".wav", ".WAV",
    ".ogg", ".OGG",
};
const std::vector<std::string> VID_EXT =
{
    ".mov", ".MOV",
    ".mp4", ".MP4",
    ".mkv", ".MKV",
    ".avi", ".AVI"
};

Item::Item(std::string name, std::string extention)
    :name(name), extention(extention)
{
    text.setFont(AssetManager::GetFont());
    text.setCharacterSize(textSize);
    text.setFillColor(sf::Color::White);

    if (extention != "")
    {
        type = ItemType::File;
        for (std::string ext : TXT_EXT)
        {
            if (ext == extention)
                type = ItemType::Text;
        }
        for (std::string ext : IMG_EXT)
        {
            if (ext == extention)
                type = ItemType::Image;
        }
        for (std::string ext : AUD_EXT)
        {
            if (ext == extention)
                type = ItemType::Audio;
        }
        //for (std::string ext : VID_EXT)
        //{
        //    if (ext == extention)
        //        type = ItemType::Video;
        //}
    }

    text.setString(name);

    border.setSize({ boxLength, textOffset.y * 2 + textSize });
    border.setFillColor({ 50, 50, 50, 255 });

    //Icon

}

sf::Text& Item::GetText()
{
    return text;
}

void Item::SetIsSelected(bool onOff)
{
    isSelected = onOff;
}

bool Item::GetIsFolder()
{
    return (type == ItemType::Folder);
}

std::string Item::GetName()
{
    return name;
}

ItemType Item::GetType() const
{
    return type;
}

void Item::Draw(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f position)
{
    if (isSelected)
    {
        border.setPosition(position);
        window->draw(border);
    }
    text.setPosition({ position.x + 5.f, position.y + 5.f });
    window->draw(text);
}
