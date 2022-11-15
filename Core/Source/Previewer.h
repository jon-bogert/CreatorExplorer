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
};

