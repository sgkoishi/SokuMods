//
// Created by PC_volt on 31/07/2021.
//
#include <SokuLib.hpp>
#include <DrawUtils.hpp>

#define FONT_HEIGHT 20
#define TEXTURE_SIZE 0x100

namespace PunishDisplay
{
static SokuLib::SWRFont font;
static int yOffset = 70;
static int timer1 = 0;
static int timer2 = 0;

static float getAlpha(unsigned timer)
{
	if (timer < 5)
	{
		return timer / 5.f;
	}
	if (timer > 180)
	{
		return 1 - (timer - 180) / 60.f;
	}
	return 1.f;
}

void showBox(int xOffset, float alpha, SokuLib::DrawUtils::DxSokuColor color)
{
	if (alpha == 0)
		return;

	SokuLib::DrawUtils::Sprite sprite;
	int text;
	char buffer[] = "Punish!";
	if (!SokuLib::textureMgr.createTextTexture(&text, buffer, font, TEXTURE_SIZE, FONT_HEIGHT + 18, nullptr, nullptr))
	{
		return;
	}

	font.create();
	sprite.setPosition({xOffset, yOffset});
	sprite.texture.setHandle(text, {TEXTURE_SIZE, FONT_HEIGHT + 18});
	sprite.rect = {0, 0, TEXTURE_SIZE, FONT_HEIGHT + 18};
	sprite.tint = color * alpha;
	sprite.draw();
}

static int (SokuLib::CharacterManager::*original_onHit)(int param);
bool isHit(SokuLib::CharacterManager &character)
{
	return (character.*original_onHit)(param);
}

#define canBlock objectBase.frameData.frameFlags.guardAvailable;
void checkPunish()
{
	bool p1CanBlock = SokuLib::getBattleMgr().leftCharacterManager.canBlock;
	bool p2CanBlock = SokuLib::getBattleMgr().rightCharacterManager.canBlock;

	if (!p1CanBlock)
	{
		if(isHit(SokuLib::getBattleMgr().leftCharacterManager))
		{
			showBox(40, getAlpha(timer1), SokuLib::DrawUtils::DxSokuColor::White);
		}
	}

	if (!p2CanBlock)
	{
		if (isHit(SokuLib::getBattleMgr().leftCharacterManager))
		{
			showBox(600, getAlpha(timer2), SokuLib::DrawUtils::DxSokuColor::White);
		}
	}

	++timer1;
	++timer2;
}

void placeHooks()
{
	DWORD old;
	VirtualProtect((PVOID)0x47c5aa, 4, PAGE_EXECUTE_WRITECOPY, &old);
	original_onHit = SokuLib::union_cast<int (SokuLib::CharacterManager::*)(int)>(SokuLib::TamperNearJmpOpr(0x47c5a9, reinterpret_cast<DWORD>(isHit)));
	VirtualProtect((PVOID)0x47c5aa, 4, old, &old);
}

extern "C" __declspec(dllexport) bool CheckVersion(const BYTE hash[16])
{
	return true;
}

extern "C" __declspec(dllexport) bool Initialize(HMODULE hMyModule, HMODULE hParentModule)
{
	placeHooks();
	checkPunish();
	return true;
}

extern "C" int APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	return true;
}
}