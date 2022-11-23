#pragma once
class Item;
class Previewer
{
	std::shared_ptr<sf::RenderWindow> window = nullptr;
	bool isActive = false;

	std::shared_ptr<Item> item = nullptr;

	sf::Text loadingText;
	sf::Color bgColor = { 30, 30, 30, 255 };
	sf::Sprite icon;

	sf::Text fileNameText;
	sf::Vector2f fileNamePos = { 250, 175 };
	sf::Text fileTypeText;
	sf::Vector2f fileTypePos = { 250, 215 };

	//Image
	std::shared_ptr<sf::Texture> texture = nullptr;
	std::shared_ptr<sf::Sprite> sprite = nullptr;

	sf::Vector2f defSize = { 700.f, 400.f };
	sf::Vector2f maxSize = { 1280.f, 720.f };

	//Audio
	bool isAudioStreaming = false;
	float streamLimit = 2.f;

	rl::Music* audioLong = nullptr;
	rl::Sound* audioShort = nullptr;

	sf::RectangleShape playheadBg;
	sf::RectangleShape playheadMain;
	sf::Color playheadBgColor = { 127, 127, 127, 255 };
	sf::Color playheadMainColor = { 200, 200, 200, 255 };
	sf::Vector2f playheadDim = { 300, 10 };
	sf::Vector2f playheadPos = { 250, 300 };

	//Text
	sf::Text textContents;

public:
	static Previewer& Get();

private:
	Previewer();

public:
	~Previewer();

	bool ToggleWindow();
	void Update();
	void DefaultUpdate();
	void ImageUpdate();
	void AudioUpdate();
	void TextUpdate();

	void LoadFile();
	
	bool GetIsActive();

	std::shared_ptr<sf::RenderWindow> GetWindow();

private:
	void Activate();
	void Deactivate();
	void Reset();

	void LoadRoot();
	void LoadDefault();
	void LoadImage();
	void LoadAudio();
	void LoadText();

	bool InPlayheadBounds();
};

