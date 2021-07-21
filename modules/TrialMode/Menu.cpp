//
// Created by PinkySmile on 19/07/2021.
//

#include "Menu.hpp"
#include "Pack.hpp"

static unsigned currentPack = 0;
static unsigned currentEntry = 0;
static bool loaded = false;
static SokuLib::DrawUtils::Sprite packContainer;
static SokuLib::DrawUtils::Sprite previewContainer;

SokuLib::SWRFont defaultFont8;
SokuLib::SWRFont defaultFont12;
SokuLib::SWRFont defaultFont16;
HMODULE myModule;
char profilePath[1024 + MAX_PATH];
char profileFolderPath[1024 + MAX_PATH];

void loadFont()
{
	SokuLib::FontDescription desc;

	desc.r1 = 255;
	desc.r2 = 255;
	desc.g1 = 255;
	desc.g2 = 255;
	desc.b1 = 255;
	desc.b2 = 255;
	desc.height = 8;
	desc.weight = FW_BOLD;
	desc.italic = 0;
	desc.shadow = 2;
	desc.bufferSize = 1000000;
	desc.charSpaceX = 0;
	desc.charSpaceY = 0;
	desc.offsetX = 0;
	desc.offsetY = 0;
	desc.useOffset = 0;
	strcpy(desc.faceName, SokuLib::defaultFontName);
	desc.weight = FW_REGULAR;
	defaultFont8.create();
	defaultFont8.setIndirect(desc);

	desc.height = 12;
	defaultFont12.create();
	defaultFont12.setIndirect(desc);

	desc.height = 16;
	defaultFont16.create();
	defaultFont16.setIndirect(desc);
}

void menuLoadAssets()
{
	if (loaded)
		return;
	loaded = true;
	puts("Loading assets");

	previewContainer.texture.loadFromGame("data/menu/profile_list_seat.bmp");
	previewContainer.rect = {
		0, 0,
		static_cast<int>(previewContainer.texture.getSize().x),
		static_cast<int>(previewContainer.texture.getSize().y),
	};
	previewContainer.setPosition({310, 92});
	previewContainer.setSize({365, 345});

	packContainer.texture.loadFromResource(myModule, MAKEINTRESOURCE(4));
	packContainer.rect = {
		0, 0,
		static_cast<int>(packContainer.texture.getSize().x),
		static_cast<int>(packContainer.texture.getSize().y),
	};
	packContainer.setSize(packContainer.texture.getSize() - 1);
	loadFont();
	loadPacks();
	std::sort(loadedPacks.begin(), loadedPacks.end(), [](std::shared_ptr<Pack> pack1, std::shared_ptr<Pack> pack2){
		if (pack1->error.texture.hasTexture() != pack2->error.texture.hasTexture())
			return pack2->error.texture.hasTexture();
		return pack1->category < pack2->category;
	});
}

void menuUnloadAssets()
{
	if (!loaded)
		return;
	loaded = false;
	puts("Unloading assets");

	defaultFont8.destruct();
	defaultFont12.destruct();
	defaultFont16.destruct();
	previewContainer.texture.destroy();
	packContainer.texture.destroy();
	loadedPacks.clear();
}

int menuOnProcess(SokuLib::MenuResult *This)
{
	auto keys = reinterpret_cast<SokuLib::KeyManager *>(0x89A394);

	menuLoadAssets();
	if (keys->keymapManager->input.b) {
		SokuLib::playSEWaveBuffer(0x29);
		return 0;
	}
	return 1;
}

void renderOnePackBack(Pack &pack, SokuLib::Vector2<float> &pos, bool deployed)
{
	packContainer.setPosition({
		static_cast<int>(pos.x),
		static_cast<int>(pos.y)
	});
	packContainer.draw();
	pos.y += 40;
}

void renderOnePack(Pack &pack, SokuLib::Vector2<float> &pos, bool deployed)
{
	if (pack.icon) {
		pack.icon->sprite.setPosition(SokuLib::Vector2i{
			static_cast<int>(pos.x + 4),
			static_cast<int>(pos.y + 2)
		} + pack.icon->translate);
		pack.icon->sprite.draw();
	}
	pack.name.setPosition({
		static_cast<int>(pos.x + 74),
		static_cast<int>(pos.y + 3)
	});
	pack.name.draw();
	pos.y += 40;
}

void menuOnRender(SokuLib::MenuResult *This)
{
	SokuLib::Vector2<float> pos{16, 116};

	if (!loaded)
		return;

	for (int i = 0; i < loadedPacks.size(); i++)
		renderOnePackBack(*loadedPacks[i], pos, i == currentPack);
	pos = {16, 116};
	for (int i = 0; i < loadedPacks.size(); i++)
		renderOnePack(*loadedPacks[i], pos, i == currentPack);
	previewContainer.draw();
}