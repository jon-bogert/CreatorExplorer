#pragma once
class Item;
class Previewer
{
	std::shared_ptr<sf::RenderWindow> window = nullptr;
	bool isActive = false;

	std::shared_ptr<Item> item = nullptr;

	sf::Text loadingText;
	sf::Color bgColor = { sf::Color::Black };

	//Image
	std::shared_ptr<sf::Texture> texture = nullptr;
	std::shared_ptr<sf::Sprite> sprite = nullptr;

	sf::Vector2f defSize = { 400.f, 225.f };
	sf::Vector2f maxSize = { 1920.f, 1080.f };

	//Audio
	bool isAudioStreaming = false;
	float streamLimit = 2.f;

	Music* audioLong = nullptr;
	Sound* audioShort = nullptr;

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
};

