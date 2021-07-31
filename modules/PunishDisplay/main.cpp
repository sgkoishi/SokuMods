//
// Created by PC_volt on 31/07/2021.
//
#include <SokuLib.hpp>
#include <DrawUtils.hpp>

namespace PunishDisplay {

void drawPunish(SokuLib::CharacterManager &chr, int xOffset)
{
	SokuLib::DrawUtils::Texture punishText;
	punishText.createFromText("Punish!", null, new SokuLib::Vector2<unsigned>(), new SokuLib::Vector2<unsigned>());

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
	bool p1IsHit = false;
	bool p2IsHit = false;

	if (!p1CanBlock)
	{
		p1IsHit = isHit(SokuLib::getBattleMgr().leftCharacterManager);
	}
	if (!p2CanBlock)
	{
		p2IsHit = isHit(SokuLib::getBattleMgr().leftCharacterManager);
	}

	if (p1IsHit)
	{
		drawPunish(SokuLib::getBattleMgr().leftCharacterManager, 100);
	}
	if (p2IsHit)
	{
		drawPunish(SokuLib::getBattleMgr().rightCharacterManager, 500);
	}

	isHit(SokuLib::getBattleMgr().rightCharacterManager);
}

void placeHooks()
{
	DWORD old;
	VirtualProtect((PVOID)0x47c5aa, 4, PAGE_EXECUTE_WRITECOPY, &old);
	original_onHit = SokuLib::union_cast<int (SokuLib::CharacterManager::*)(int)>(SokuLib::TamperNearJmpOpr(0x47c5a9, reinterpret_cast<DWORD>(isHit)));
	VirtualProtect((PVOID)0x47c5aa, 4, old, &old);

}

void removeHooks() // is it necessary?
{
	DWORD old;
	VirtualProtect((PVOID)0x47c5aa, 4, PAGE_EXECUTE_WRITECOPY, &old);
	SokuLib::TamperNearJmpOpr(0x47c5a9, SokuLib::union_cast<DWORD>(original_onHit));
	VirtualProtect((PVOID)0x47c5aa, 4, old, &old);
}

extern "C" __declspec(dllexport) bool CheckVersion(const BYTE hash[16])
{
	return true;
}

extern "C" __declspec(dllexport) bool Initialize(HMODULE hMyModule, HMODULE hParentModule)
{
	placeHooks();
	return true;
}

extern "C" int APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	return true;
}
}