#include "pch.h"
#include "Game.h"


Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	//Initialize Sound
	m_ptrLevel1Music = new SoundStream("Sounds/AnotherWinter(Mixed).mp3");
	m_ptrMenuMusic = new SoundStream("Sounds/ScottPilgrimAnthem.mp3");
	m_ptrPauseMusic = new SoundStream("Sounds/PauseMusic.mp3");
	m_ptrSoundEffects = new SoundEffects();

	//Initialize Managers
	m_ptrLevelManager = new LevelManager(GetViewPort(), m_ptrSoundEffects);

	//Initialize Menu
	m_ptrMenuScott = new Texture("MenuScott_Sprite.png");
	m_ptrMenuScreen = new Texture("MenuScreen_Sprite.png");

	m_ptrMenuButtons.push_back(new Button(Point2f(0.f, 330.f), 170.f, 80.f, "PLAY", true));
	m_ptrMenuButtons.push_back(new Button(Point2f(105.f, 230.f), 390.f, 80.f, "HELP AND OPTIONS"));

	//Initialize PauseScreen
	m_ptrPauseScreen = new Texture("PauseScreen_Sprite.png");

	m_ptrPauseButtons.push_back(new Button(Point2f(40.f, 370.f), 170.f, 80.f, "RESUME", true));
	m_ptrPauseButtons.push_back(new Button(Point2f(105.f, 290.f), 390.f, 80.f, "HELP AND OPTIONS"));
	m_ptrPauseButtons.push_back(new Button(Point2f(0.f, 210.f), 170.f, 80.f, "MENU"));

	//Initialize Settings
	m_ptrSettingsScreen = new Texture("SettingsScreen_Sprite.png");
	m_ptrSettingsText = new Texture("SettingsScreenText_Sprite.png");
	m_ptrBackgroundVolume = new SoundBarSetting(Point2f(1010.f, 440.f), 200.f, 50.f, m_BackgroundVolume);
	m_ptrSFXVolume = new SoundBarSetting(Point2f(1010.f, 340.f), 200.f, 50.f, m_SFXVolume);

	m_ptrSettingsButtons.push_back(new Button(Point2f(215.f, 430.f), 350.f, 85.f, "MUSIC VOLUME"));
	m_ptrSettingsButtons.push_back(new Button(Point2f(175.f, 330.f), 280.f, 85.f, "SFX VOLUME"));
	m_ptrSettingsButtons.push_back(new Button(Point2f(110.f, 230.f), 190.f, 85.f, "GO BACK"));
}

void Game::Cleanup( )
{
	// Delete Managers
	delete m_ptrLevelManager;
	m_ptrLevelManager = nullptr;


	// Delete Menu
	delete m_ptrMenuScott;
	m_ptrMenuScott = nullptr;
	delete m_ptrMenuScreen;
	m_ptrMenuScreen = nullptr;

	for(Button* Buttons: m_ptrMenuButtons)
	{
		delete Buttons;
		Buttons = nullptr;
	}

	//Delete Settings
	delete m_ptrSettingsScreen;
	m_ptrSettingsScreen = nullptr;
	delete m_ptrSettingsText;
	m_ptrSettingsText = nullptr;

	for (Button* Buttons : m_ptrSettingsButtons)
	{
		delete Buttons;
		Buttons = nullptr;
	}

	delete m_ptrBackgroundVolume;
	m_ptrBackgroundVolume = nullptr;
	delete m_ptrSFXVolume;
	m_ptrSFXVolume = nullptr;

	//Delete PauseScreen
	delete m_ptrPauseScreen;
	m_ptrPauseScreen = nullptr;

	for (Button* Buttons : m_ptrPauseButtons)
	{
		delete Buttons;
		Buttons = nullptr;
	}

	//Sound
	delete m_ptrLevel1Music;
	m_ptrLevel1Music = nullptr;
	delete m_ptrMenuMusic;
	m_ptrMenuMusic = nullptr;
	delete m_ptrPauseMusic;
	m_ptrPauseMusic = nullptr;
	delete m_ptrSoundEffects;
	m_ptrSoundEffects = nullptr;

}

void Game::Update(float elapsedSec)
{
	//Update Level
	if(m_Playing) m_ptrLevelManager->Update(elapsedSec);

	//Update Menu
	if(m_MenuScreen)
	{
		for (Button* Buttons : m_ptrMenuButtons)
		{
			Buttons->Update(elapsedSec);
		}
	}
	else if(m_SettingsScreen)
	{
		for (Button* Buttons : m_ptrSettingsButtons)
		{
			Buttons->Update(elapsedSec);
		}
	}
	else if (m_ptrPauseScreen)
	{
		for (Button* Buttons : m_ptrPauseButtons)
		{
			Buttons->Update(elapsedSec);
		}
	}

	//Sound
	if(m_BackgroundMusicIsPlaying == false)
	{
		if (m_Playing)
		{
			if (m_ptrMenuMusic->IsPlaying()) m_ptrMenuMusic->Stop();
			m_ptrLevel1Music->SetVolume(m_BackgroundVolume);
			m_ptrLevel1Music->Play(true);
			m_ptrSoundEffects->SetVolume(m_SFXVolume);
		}
		else if (m_MenuScreen)
		{
			if (m_ptrLevel1Music->IsPlaying()) m_ptrLevel1Music->Stop();
			m_ptrMenuMusic->SetVolume(m_BackgroundVolume);
			m_ptrMenuMusic->Play(true);
			m_ptrSoundEffects->SetVolume(m_SFXVolume);
		}
		else if(m_PauseScreen)
		{
			if (m_ptrLevel1Music->IsPlaying()) m_ptrLevel1Music->Stop();
			m_ptrPauseMusic->SetVolume(m_BackgroundVolume);
			m_ptrPauseMusic->Play(true);
			m_ptrSoundEffects->SetVolume(m_SFXVolume);
		}
		m_BackgroundMusicIsPlaying = true;
	}

}

void Game::Draw( ) const
{
	ClearBackground( );

	//Draw Level
	if (m_Playing || m_PauseScreen) m_ptrLevelManager->Draw();

	//Draw Menu
	if (m_MenuScreen)
	{
		m_ptrMenuScreen->Draw();

		for (Button* Buttons : m_ptrMenuButtons)
		{
			Buttons->Draw();
		}

		m_ptrMenuScott->Draw(Rectf{ 0.f, 0.f, m_ptrMenuScott->GetWidth() * 3.1f, m_ptrMenuScott->GetHeight() * 3.1f });
	}
	else if(m_SettingsScreen)
	{
		m_ptrSettingsScreen->Draw();

		for (Button* Buttons : m_ptrSettingsButtons)
		{
			Buttons->Draw();
		}

		m_ptrBackgroundVolume->Draw();
		m_ptrSFXVolume->Draw();

		m_ptrSettingsText->Draw();
	}
	else if(m_PauseScreen)
	{
		m_ptrPauseScreen->Draw();

		for (Button* Buttons : m_ptrPauseButtons)
		{
			Buttons->Draw();
		}
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	//Level
	if (m_Playing)
	{
		m_ptrLevelManager->ProcessKeyDownEvent(e);

		if (e.keysym.sym == SDLK_ESCAPE)
		{
			m_PauseScreen = true;
			m_Playing = false;
			m_ButtonType = ButtonType::Resume;
			m_ptrPauseButtons[0]->SetIsActivated(true);
			m_BackgroundMusicIsPlaying = false;
		}
	}

	//Menu
	if (m_MenuScreen)
	{
		if (e.keysym.sym == SDLK_s && m_DownReset)
		{
			m_DownReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Play:
				m_ButtonType = ButtonType::Settings;
				m_ptrMenuButtons[int(ButtonType::Settings)]->SetIsActivated(true);
				m_ptrMenuButtons[int(ButtonType::Play)]->SetIsActivated(false);
				break;
			case Game::ButtonType::Settings:
				m_ButtonType = ButtonType::Play;
				m_ptrMenuButtons[int(ButtonType::Play)]->SetIsActivated(true);
				m_ptrMenuButtons[int(ButtonType::Settings)]->SetIsActivated(false);
				break;
			}
		}
		else if ((e.keysym.sym == SDLK_z || e.keysym.sym == SDLK_w ) && m_UpReset)
		{
			m_UpReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Play:
				m_ButtonType = ButtonType::Settings;
				m_ptrMenuButtons[int(ButtonType::Settings)]->SetIsActivated(true);
				m_ptrMenuButtons[int(ButtonType::Play)]->SetIsActivated(false);
				break;
			case Game::ButtonType::Settings:
				m_ButtonType = ButtonType::Play;
				m_ptrMenuButtons[int(ButtonType::Play)]->SetIsActivated(true);
				m_ptrMenuButtons[int(ButtonType::Settings)]->SetIsActivated(false);
				break;
			}
		}
		else if ((e.keysym.sym == SDLK_SPACE || e.keysym.sym == SDLK_RETURN) && m_SpacebarReset)
		{
			m_SpacebarReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Play:
				m_MenuScreen = false;
				m_Playing = true;
				m_BackgroundMusicIsPlaying = false;
				break;
			case Game::ButtonType::Settings:
				m_ButtonType = ButtonType::Music;
				m_MenuScreen = false;
				m_SettingsScreen = true;
				m_ptrSettingsButtons[int(ButtonType::Music) - 2]->SetIsActivated(true);
				m_ptrBackgroundVolume->SetIsActivated(true);
				m_MenuToSettings = true;
				break;
			}
		}
	}

	//Settings
	if (m_SettingsScreen)
	{
		if (e.keysym.sym == SDLK_s && m_DownReset)
		{
			m_DownReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Music:
				m_ButtonType = ButtonType::SFX;
				m_ptrSettingsButtons[int(ButtonType::SFX) - 2]->SetIsActivated(true);
				m_ptrSettingsButtons[int(ButtonType::Music) - 2]->SetIsActivated(false);
				m_ptrBackgroundVolume->SetIsActivated(false);
				m_ptrSFXVolume->SetIsActivated(true);
				break;
			case Game::ButtonType::SFX:
				m_ButtonType = ButtonType::Exit;
				m_ptrSettingsButtons[int(ButtonType::Exit) - 2]->SetIsActivated(true);
				m_ptrSettingsButtons[int(ButtonType::SFX) - 2]->SetIsActivated(false);
				m_ptrSFXVolume->SetIsActivated(false);
				break;
			case Game::ButtonType::Exit:
				m_ButtonType = ButtonType::Music;
				m_ptrSettingsButtons[int(ButtonType::Music) - 2]->SetIsActivated(true);
				m_ptrSettingsButtons[int(ButtonType::Exit) - 2]->SetIsActivated(false);
				m_ptrBackgroundVolume->SetIsActivated(true);
				break;
			}
		}
		else if ((e.keysym.sym == SDLK_z || e.keysym.sym == SDLK_w) && m_UpReset)
		{
			m_UpReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Music:
				m_ButtonType = ButtonType::Exit;
				m_ptrSettingsButtons[int(ButtonType::Exit) - 2]->SetIsActivated(true);
				m_ptrSettingsButtons[int(ButtonType::Music) - 2]->SetIsActivated(false);
				m_ptrBackgroundVolume->SetIsActivated(false);
				break;
			case Game::ButtonType::SFX:
				m_ButtonType = ButtonType::Music;
				m_ptrSettingsButtons[int(ButtonType::Music) - 2]->SetIsActivated(true);
				m_ptrSettingsButtons[int(ButtonType::SFX) - 2]->SetIsActivated(false);
				m_ptrBackgroundVolume->SetIsActivated(true);
				m_ptrSFXVolume->SetIsActivated(false);
				break;
			case Game::ButtonType::Exit:
				m_ButtonType = ButtonType::SFX;
				m_ptrSettingsButtons[int(ButtonType::SFX) - 2]->SetIsActivated(true);
				m_ptrSettingsButtons[int(ButtonType::Exit) - 2]->SetIsActivated(false);
				m_ptrSFXVolume->SetIsActivated(true);
				break;
			}
		}
		else if ((e.keysym.sym == SDLK_SPACE || e.keysym.sym == SDLK_RETURN) && m_SpacebarReset)
		{
			m_SpacebarReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Music:
				if(m_BackgroundVolume < 100) m_BackgroundVolume += 10;
				m_ptrMenuMusic->SetVolume(m_BackgroundVolume);
				m_ptrLevel1Music->SetVolume(m_BackgroundVolume);
				m_ptrBackgroundVolume->Update(m_BackgroundVolume);
				break;
			case Game::ButtonType::SFX:
				if (m_SFXVolume < 100) m_SFXVolume += 10;
				m_ptrSoundEffects->SetVolume(m_SFXVolume);
				m_ptrSFXVolume->Update(m_SFXVolume);
				m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::CoinPickUp);
				break;
			case Game::ButtonType::Exit:
				if(m_MenuToSettings)
				{
					m_ButtonType = ButtonType::Settings;
					m_MenuScreen = true;
					m_MenuToSettings = false;
				}
				else
				{
					m_ButtonType = ButtonType::Settings;
					m_PauseScreen = true;
				}
				m_SettingsScreen = false;
				m_ptrSettingsButtons[int(ButtonType::Exit) - 2]->SetIsActivated(false);
				break;
			}
		}
		else if (e.keysym.sym == SDLK_q || e.keysym.sym == SDLK_a)
		{
			switch (m_ButtonType)
			{
			case Game::ButtonType::Music:
				if (m_BackgroundVolume > 0) m_BackgroundVolume -= 10;
				m_ptrMenuMusic->SetVolume(m_BackgroundVolume);
				m_ptrLevel1Music->SetVolume(m_BackgroundVolume);
				m_ptrBackgroundVolume->Update(m_BackgroundVolume);
				break;
			case Game::ButtonType::SFX:
				if (m_SFXVolume > 0) m_SFXVolume -= 10;
				m_ptrSoundEffects->SetVolume(m_SFXVolume);
				m_ptrSFXVolume->Update(m_SFXVolume);
				m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::CoinPickUp);
				break;
			}
		}
		else if (e.keysym.sym == SDLK_d)
		{
			switch (m_ButtonType)
			{
			case Game::ButtonType::Music:
				if (m_BackgroundVolume < 100) m_BackgroundVolume += 10;
				m_ptrMenuMusic->SetVolume(m_BackgroundVolume);
				m_ptrLevel1Music->SetVolume(m_BackgroundVolume);
				m_ptrBackgroundVolume->Update(m_BackgroundVolume);
				break;
			case Game::ButtonType::SFX:
				if (m_SFXVolume < 100) m_SFXVolume += 10;
				m_ptrSoundEffects->SetVolume(m_SFXVolume);
				m_ptrSFXVolume->Update(m_SFXVolume);
				m_ptrSoundEffects->Play(SoundEffects::SoundEffectType::CoinPickUp);
				break;
			}
		}
	}

	//PauseScreen
	if(m_PauseScreen)
	{
		if (e.keysym.sym == SDLK_s && m_DownReset)
		{
			m_DownReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Resume:
				m_ButtonType = ButtonType::Settings;
				m_ptrPauseButtons[1]->SetIsActivated(true);
				m_ptrPauseButtons[0]->SetIsActivated(false);
				break;
			case Game::ButtonType::Settings:
				m_ButtonType = ButtonType::Menu;
				m_ptrPauseButtons[2]->SetIsActivated(true);
				m_ptrPauseButtons[1]->SetIsActivated(false);
				break;
			case Game::ButtonType::Menu:
				m_ButtonType = ButtonType::Resume;
				m_ptrPauseButtons[0]->SetIsActivated(true);
				m_ptrPauseButtons[2]->SetIsActivated(false);
				break;
			}
		}
		else if ((e.keysym.sym == SDLK_z || e.keysym.sym == SDLK_w) && m_UpReset)
		{
			m_UpReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Resume:
				m_ButtonType = ButtonType::Menu;
				m_ptrPauseButtons[2]->SetIsActivated(true);
				m_ptrPauseButtons[0]->SetIsActivated(false);
				break;
			case Game::ButtonType::Settings:
				m_ButtonType = ButtonType::Resume;
				m_ptrPauseButtons[0]->SetIsActivated(true);
				m_ptrPauseButtons[1]->SetIsActivated(false);
				break;
			case Game::ButtonType::Menu:
				m_ButtonType = ButtonType::Settings;
				m_ptrPauseButtons[1]->SetIsActivated(true);
				m_ptrPauseButtons[2]->SetIsActivated(false);
				break;
			}
		}
		else if ((e.keysym.sym == SDLK_SPACE || e.keysym.sym == SDLK_RETURN) && m_SpacebarReset)
		{
			m_SpacebarReset = false;
			switch (m_ButtonType)
			{
			case Game::ButtonType::Resume:
				m_PauseScreen = false;
				m_Playing = true;
				m_BackgroundMusicIsPlaying = false;
				break;
			case Game::ButtonType::Settings:
				m_ButtonType = ButtonType::Music;
				m_PauseScreen = false;
				m_SettingsScreen = true;
				m_ptrSettingsButtons[int(ButtonType::Music) - 2]->SetIsActivated(true);
				m_ptrBackgroundVolume->SetIsActivated(true);
				break;
			case Game::ButtonType::Menu:
				m_ButtonType = ButtonType::Play;
				m_ptrPauseButtons[2]->SetIsActivated(false);
				m_PauseScreen = false;
				m_MenuScreen = true;
				m_BackgroundMusicIsPlaying = false;
				m_ptrLevelManager->ResetLevel();
				break;
			}
		}
	}
}


void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//Level
	if (m_Playing) m_ptrLevelManager->ProcessKeyUpEvent(e);

	//Menu Buttons
	if (m_UpReset == false) m_UpReset = true;
	if (m_DownReset == false) m_DownReset = true;
	if (m_SpacebarReset == false) m_SpacebarReset = true;
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
