//
// Created by PinkySmile on 15/01/2021.
//

#include "MyChrFullNameString.hpp"

#include "vars.hpp"
#include <SokuLib.hpp>

std::string MyChrFullNameString::getString() const {
	std::map<unsigned int, std::pair<std::string, std::string>>::iterator it;

	switch (SokuLib::mainMode) {
	case SokuLib::BATTLE_MODE_VSSERVER:
		it = charactersNames.find(SokuLib::rightChar);
		break;
	default:
		it = charactersNames.find(SokuLib::leftChar);
	}
	return (it == charactersNames.end() ? "Unknown character" : it->second.second);
}
