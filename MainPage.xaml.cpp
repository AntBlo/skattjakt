//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <WindowsNumerics.h>
#include <string>
#include "World.h"

using namespace Skattjakt;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

MainPage::MainPage()
{
	InitializeComponent();
	StartTimerAndRegisterHandler();
}

void MainPage::CanvasControl_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender,
	Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args)
{
	using namespace Windows::UI;
	using namespace Microsoft::Graphics::Canvas::UI::Xaml;
	const World &theWorld = theGame.theWorld;

	// Make sure whole map fits in canvas, max scale is 1.5, transform offset to
	// make map in center
	auto height = canvas->Size.Height;
	auto width = canvas->Size.Width;
	float scale = 1.5f;  // default max
	float x_needed = theWorld.world_size_x * 32.0f;
	float y_needed = theWorld.world_size_y * 32.0f;
	if (x_needed * scale > width) {
		scale = width / x_needed;
	}
	if (y_needed * scale > height) {
		scale = height / y_needed;
	}
	scale *= base_scale;
	float x_offset = (width - x_needed * scale) / 2;
	float y_offset = (height - y_needed * scale) / 2;
	args->DrawingSession->Transform = make_float3x2_scale(scale) * make_float3x2_translation(x_offset,y_offset);

	if (!m_tiles_loaded)
		return;

	// Draw world, empty cells, walls, treasures, bananas (traps)
	for (int y = 0; y < theWorld.world_size_y ; y++)
		for (int x = 0; x < theWorld.world_size_x; x++) {
			float xcoord = x * 32.0f;
			float ycoord = y * 32.0f;
			if (theWorld.getCell(x, y) == Cell_content::WALL) {
				args->DrawingSession->DrawImage(tiles[Bitmapnames::wall], xcoord, ycoord);
			}
			else {
				args->DrawingSession->DrawImage(tiles[Bitmapnames::empty], xcoord, ycoord);
			}
			if (theWorld.getCell(x, y) == Cell_content::TREASURE) {
				args->DrawingSession->DrawImage(tiles[Bitmapnames::treasure], xcoord, ycoord);
			}
			if (theWorld.getCell(x, y) == Cell_content::TRAP) {
				args->DrawingSession->DrawImage(tiles[Bitmapnames::banana], xcoord, ycoord);
			}
		}
	// Draw robots
	args->DrawingSession->DrawImage(tiles[Bitmapnames::player1], theGame.robot1.pos_x * 32.0f, theGame.robot1.pos_y * 32.0f);
	args->DrawingSession->DrawImage(tiles[Bitmapnames::player2], theGame.robot2.pos_x * 32.0f, theGame.robot2.pos_y * 32.0f);
	// Draw grid lines
	for (int y = 1; y < theWorld.world_size_y - 1; y++) {
		float ycoord = y * 32.0f;
		args->DrawingSession->DrawLine(0, ycoord, (theWorld.world_size_x - 1) * 32.0f, ycoord,
			Windows::UI::Colors::Black);
	}
	for (int x = 1; x < theWorld.world_size_x - 1; x++) {
		float xcoord = x * 32.0f;
		args->DrawingSession->DrawLine(xcoord, 0, xcoord, (theWorld.world_size_y - 1) * 32.0f,
			Windows::UI::Colors::Black);
	}
}

// Return small 32x32 tile on row "tile_row" and column "tile_col" from larger tileset "m_tiles"

Microsoft::Graphics::Canvas::CanvasBitmap^ MainPage::CreateSmallTile(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender,
	int tile_row, int tile_col)
{
	using namespace Microsoft::Graphics::Canvas;
	return CanvasBitmap::CreateFromColors(sender,
		tiles[Bitmapnames::alltiles]->GetPixelColors(32 * tile_col, 32 * tile_row, 32, 32), 32, 32);
}

void MainPage::CanvasControl_CreateResources(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender,
	Microsoft::Graphics::Canvas::UI::CanvasCreateResourcesEventArgs^ args)
{
	using namespace Microsoft::Graphics::Canvas;
	// Nevanda tileset from: https://github.com/coolwanglu/BrowserHack/blob/master/web/Nevanda_32.png
	// (BrowserHack: NetHack ported to the Web by Lu Wang)
	concurrency::create_task(CanvasBitmap::LoadAsync(sender, "Assets/Nevanda_32-alpha.png")).then([this, sender](CanvasBitmap^ map) {
		tiles[Bitmapnames::alltiles] = map;
		tiles[Bitmapnames::wall] = CreateSmallTile(sender, 26, 8);
		tiles[Bitmapnames::empty] = CreateSmallTile(sender, 27, 8);
		tiles[Bitmapnames::treasure] = CreateSmallTile(sender, 9, 34);
		tiles[Bitmapnames::banana] = CreateSmallTile(sender, 16, 8);
		auto row = theGame.pRobotcontrol1->robot_bitmap_row;
		auto col = theGame.pRobotcontrol1->robot_bitmap_col;
		tiles[Bitmapnames::player1] = CreateSmallTile(sender, row, col);
		row = theGame.pRobotcontrol2->robot_bitmap_row;
		col = theGame.pRobotcontrol2->robot_bitmap_col;
		tiles[Bitmapnames::player2] = CreateSmallTile(sender, row, col);
		m_tiles_loaded = true;
		canvas->Invalidate();
		p1image->Invalidate();
		p2image->Invalidate();
		Update_info();
	});
}

// Every ??? ms, run next robot

void MainPage::StartTimerAndRegisterHandler() {
	auto timer = ref new Windows::UI::Xaml::DispatcherTimer();
	TimeSpan ts;
	ts.Duration = 20 * 10000; // 100 ns units, 10'000 = 1 ms
	timer->Interval = ts;
	timer->Start();
	auto registrationtoken = timer->Tick += ref new EventHandler<Object^>(this, &MainPage::OnTick);
}

void MainPage::OnTick(Object^ sender, Object^ e) {
	// initial appear animation
	if (base_scale < 1.0f) {
		base_scale += (1.0f-base_scale)*0.1f;
		if (base_scale > 0.998f)
			base_scale = 1.0f;
		canvas->Invalidate();
	}
	// Run rest in half speed
	static int count = 0;
	if (++count < 2)
		return;
	count = 0;
	// autorun (run 10 rounds)
	if (autorun == 0) {
		next10button->IsEnabled = true;
		nextbutton->IsEnabled = true;
		return;
	}
	--autorun;
	theGame.run_next_robot();
	Update_info();
	canvas->Invalidate();
}


void Skattjakt::MainPage::Nextbutton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	theGame.run_next_robot();
	Update_info();
	canvas->Invalidate();
}

void Skattjakt::MainPage::Next10button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	next10button->IsEnabled = false;
	nextbutton->IsEnabled = false;
	autorun = 20;
}

void MainPage::Update_info()
{
	String^ name1 = ref new String(theGame.pRobotcontrol1->team_name.c_str());
	String^ name2 = ref new String(theGame.pRobotcontrol2->team_name.c_str());

	infoblock->Text = "Runda: " + theGame.round;
	player1block->Text = "Lag 1: " + name1 + "\n\n" +
		infotext(theGame.robot1);
	player2block->Text = "Lag 2: " + name2 + "\n\n" +
		infotext(theGame.robot2);
}

String^ MainPage::infotext(const RobotState &state)
{
	String^ dirtxt[] = { " N"," E"," S"," W","NE","SE","SW","NW" };
	auto &com = state.last_command;
	String^ action = (com.action == Action::PASS ? "Pass" :
		(com.action == Action::PLACE_TRAP ? "Trap " : "Step ") +
		dirtxt[static_cast<int>(com.step_dir)]);
	String^ res;
	switch (state.last_event) {
	case Event::COLLISION:
		res = "Kollision!";
		break;
	case Event::OK:
		res = "Ok";
		break;
	case Event::TREASURE_CAPTURED:
		res = "Hittade skatt!";
		break;
	case Event::TRAPPED:
		res = "Fångad (" + state.trapped_rounds + ")";
		break;
	}
	return "  Action: " + action + "\n" +
		"  Resultat: " + res + "\n" +
		"  Bananskal: " + state.no_traps_remaining + "\n" +
		"  Skatter: " + state.no_treasures_captured;
}


void Skattjakt::MainPage::P1image_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args)
{
	if (m_tiles_loaded) {
		args->DrawingSession->Transform = make_float3x2_scale(2.0f);
		args->DrawingSession->DrawImage(tiles[Bitmapnames::player1]);
	}
}


void Skattjakt::MainPage::P2image_Draw(Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl^ sender, Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs^ args)
{
	if (m_tiles_loaded) {
		args->DrawingSession->Transform = make_float3x2_scale(2.0f);
		args->DrawingSession->DrawImage(tiles[Bitmapnames::player2]);
	}
}
