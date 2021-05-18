//
// MainPage.xaml.h
// 
// Main page of the Skattjakt app.
//
// Code for competition in c++ course at University West, Trollhättan Sweden, 2019.
//
// Thomas Lundqvist, May 2019, open source but attributions welcome!

#pragma once

#include "MainPage.g.h"
#include "Game.h"

namespace Skattjakt
{
	public ref class MainPage sealed
	{
	public:
		MainPage();
	private:
		Game theGame;

		enum class Bitmapnames {alltiles,wall,empty,treasure,banana,player1,player2};
		std::map<Bitmapnames, Microsoft::Graphics::Canvas::CanvasBitmap^> tiles;
		bool m_tiles_loaded = false;
		float base_scale = 0.1f; // for initial "appear" animation, 1.0 means final scale
		int autorun = 0;        // if positive, OnTick runs round automatically

		// Event handler functions:
		void CanvasControl_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args);
		void CanvasControl_CreateResources(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender, Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs^ args);
		void Nextbutton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Next10button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void P1image_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args);
		void P2image_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args);

		// Help functions and timer functions
		Microsoft::Graphics::Canvas::CanvasBitmap^ CreateSmallTile(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl ^ sender, int tile_row, int tile_col);
		void Update_info();
		Platform::String^ infotext(const RobotState &state);
		void StartTimerAndRegisterHandler();
		void OnTick(Object ^ sender, Object ^ e);
	};
}
