#pragma once
#include "BaseGame.h"
#include "LevelManager.h"
#include "Button.h"
#include "SoundBarSetting.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	//Variables
	bool m_Playing{false};
	bool m_MenuScreen{true};
	bool m_PauseScreen{false};
	bool m_SettingsScreen{ false };
	bool m_MenuToSettings{ false };
	bool m_UpReset{ true };
	bool m_DownReset{ true };
	bool m_SpacebarReset{ true };

	Texture* m_ptrMenuScreen;
	Texture* m_ptrMenuScott;
	Texture* m_ptrSettingsScreen;
	Texture* m_ptrSettingsText;
	Texture* m_ptrPauseScreen;
	

	std::vector<Button*> m_ptrMenuButtons;
	std::vector<Button*> m_ptrSettingsButtons;
	std::vector<Button*> m_ptrPauseButtons;

	enum class ButtonType
	{
		Play, Settings, Music, SFX, Exit, Menu, Resume
	};

	ButtonType m_ButtonType{};

	//Sound
	bool m_BackgroundMusicIsPlaying{ false };
	int m_BackgroundVolume{ 10 };
	int m_SFXVolume{ 70 };
	SoundStream* m_ptrLevel1Music;
	SoundStream* m_ptrMenuMusic;
	SoundStream* m_ptrPauseMusic;
	SoundEffects* m_ptrSoundEffects;
	SoundBarSetting* m_ptrBackgroundVolume;
	SoundBarSetting* m_ptrSFXVolume;

	//Managers
	LevelManager* m_ptrLevelManager;
};