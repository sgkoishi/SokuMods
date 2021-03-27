//
// Created by PinkySmile on 18/02/2021.
//
#include <SokuLib.hpp>
#include "Gui.hpp"
#include "Moves.hpp"
#include "State.hpp"
#include "Dummy.hpp"

namespace Practice
{
	tgui::Gui gui;
	std::array<std::string, SokuLib::CHARACTER_RANDOM> names{
		"reimu",
		"marisa",
		"sakuya",
		"alice",
		"patchouli",
		"youmu",
		"remilia",
		"yuyuko",
		"yukari",
		"suika",
		"reisen",
		"aya",
		"komachi",
		"iku",
		"tenshi",
		"sanae",
		"cirno",
		"meiling",
		"utsuho",
		"suwako",
	};
	std::vector<sf::Texture> skillsTextures;
	static tgui::Panel::Ptr panel;
	static tgui::Tabs::Ptr tab;

	inline void getSkillMap(SokuLib::Skill *skillMap, char (&skills)[5], SokuLib::Character character)
	{
		char nbSkills = 4 + (character == SokuLib::CHARACTER_PATCHOULI);

		memset(skills, 0, sizeof(skills));
		for (char i = nbSkills; i < 3 * nbSkills; i++) {
			if (!skillMap[i].notUsed)
				skills[i % nbSkills] = i / nbSkills;
		}
	}

	void init(LPCSTR profilePath)
	{
#ifdef NDEBUG
		if (!skillsTextures.empty())
#endif
			return;
		skillsTextures.resize(SokuLib::CHARACTER_RANDOM);
		for (int i = 0; i < SokuLib::CHARACTER_RANDOM; i++) {
			printf("Loading file %s\n", (std::string(profilePath) + "/assets/skills/" + names[i] + "Skills.png").c_str());
			skillsTextures[i].loadFromFile(std::string(profilePath) + "/assets/skills/" + names[i] + "Skills.png");
		}
		puts("Init done");
	}

	static bool makeFakeCard(SokuLib::CharacterManager &manager, unsigned short id)
	{
		printf("Insert card %i\n", id);
		auto card = manager.addCard(id);

		if (card) {
			card->cost = 1;
			return true;
		}
		printf("Cannot insert card %i !", id);
		return false;
	}

	static void populateCharacterPanel(const std::string &profilePath, tgui::Panel::Ptr panel, SokuLib::CharacterManager &manager, SokuLib::Character character, CharacterState &state)
	{
		// Levels
		char nbSkills = 4;

		panel->loadWidgetsFromFile(profilePath + "/assets/chr.gui");

		auto spePic = panel->get<tgui::Picture>("SpecialPic");
		auto speLvl = panel->get<tgui::ComboBox>("SpecialLevel");

		auto dollPic = panel->get<tgui::Picture>("DollPic");
		auto dollLvl = panel->get<tgui::ComboBox>("DollLvl");

		auto dropPic = panel->get<tgui::Picture>("DropPic");
		auto dropLvl = panel->get<tgui::ComboBox>("DropLvl");

		auto rodPic = panel->get<tgui::Picture>("RodPic");
		auto rodLvl = panel->get<tgui::ComboBox>("RodLvl");

		auto grimPic = panel->get<tgui::Picture>("GrimPic");
		auto grimLvl = panel->get<tgui::ComboBox>("GrimLvl");

		auto fanPic = panel->get<tgui::Picture>("FanPic");
		auto fanLvl = panel->get<tgui::ComboBox>("FanLvl");

		char skills[5];

		getSkillMap(state.skillMap, skills, character);
		if (character == SokuLib::CHARACTER_PATCHOULI) {
			nbSkills = 5;
			panel->get<tgui::Widget>("Skill4Lvl")->setVisible(true);
			panel->get<tgui::Widget>("Skill4Img")->setVisible(true);
			panel->get<tgui::Widget>("Skill4Id")->setVisible(true);
		}
		if (character == SokuLib::CHARACTER_REISEN) {
			spePic->setVisible(true);
			speLvl->setVisible(true);
			spePic->getRenderer()->setTexture(profilePath + "/assets/stats/elixir.png");
			speLvl->removeItem("MAX");
		}
		if (character == SokuLib::CHARACTER_YUYUKO) {
			spePic->setVisible(true);
			speLvl->setVisible(true);
			spePic->getRenderer()->setTexture(profilePath + "/assets/stats/butterflies.png");
		}

		dollPic->getRenderer()->setTexture(profilePath + "/assets/stats/doll.png");
		dropPic->getRenderer()->setTexture(profilePath + "/assets/stats/drop.png");
		rodPic->getRenderer()->setTexture(profilePath + "/assets/stats/rod.png");
		grimPic->getRenderer()->setTexture(profilePath + "/assets/stats/grimoire.png");
		fanPic->getRenderer()->setTexture(profilePath + "/assets/stats/fan.png");
		dollLvl->setSelectedItemByIndex(state.dollLevel);
		dropLvl->setSelectedItemByIndex(state.dropLevel);
		rodLvl->setSelectedItemByIndex(state.rodLevel);
		grimLvl->setSelectedItemByIndex(state.grimoireLevel);
		fanLvl->setSelectedItemByIndex(state.fanLevel);
		speLvl->setSelectedItemByIndex(state.specialValue);
		dollLvl->connect("ItemSelected", [&state](int item){
			state.dollLevel = item;
		});
		dropLvl->connect("ItemSelected", [&state](int item){
			state.dropLevel = item;
		});
		rodLvl->connect("ItemSelected", [&state](int item){
			state.rodLevel = item;
		});
		grimLvl->connect("ItemSelected", [&state](int item){
			state.grimoireLevel = item;
		});
		fanLvl->connect("ItemSelected", [&state](int item){
			state.fanLevel = item;
		});
		speLvl->connect("ItemSelected", [&state](int item){
			state.specialValue = item;
		});

		for (int i = 0; i < nbSkills; i++) {
			auto img = panel->get<tgui::Picture>("Skill" + std::to_string(i) + "Img");
			auto lvl = panel->get<tgui::ComboBox>("Skill" + std::to_string(i) + "Lvl");
			auto id = panel->get<tgui::ComboBox>("Skill" + std::to_string(i) + "Id");

			for (int j = 0; j < 3; j++)
				id->addItem(movesNames[character][i + j * nbSkills]);
			id->setSelectedItemByIndex(skills[i]);
			id->connect("ItemSelected", [lvl, img, &state, nbSkills, i, character](std::weak_ptr<tgui::ComboBox> skill, int item){
				lvl->disconnectAll("ItemSelected");
				lvl->removeAllItems();
				for (int j = item != 0; j < 5; j++)
					lvl->addItem(j == 4 ? "MAX" : std::to_string(j));
				lvl->connect("ItemSelected", [&state, i, nbSkills, skill](int item){
					auto index = skill.lock()->getSelectedItemIndex();

					for (int j = 0; j < 3; j++) {
						if (j == index) {
							state.skillMap[j * nbSkills + i].notUsed = false;
							state.skillMap[j * nbSkills + i].level = item + (index != 0);
						} else {
							state.skillMap[j * nbSkills + i].notUsed = true;
							state.skillMap[j * nbSkills + i].level = 0x7F;
						}
					}
				});
				lvl->setSelectedItemByIndex(0);

				img->getRenderer()->setTexture(
					tgui::Texture(
						skillsTextures[character],
						sf::IntRect((item * nbSkills + i) * 32, 0, 32, 32)
					)
				);
			}, std::weak_ptr<tgui::ComboBox>(id));

			lvl->disconnectAll("ItemSelected");
			lvl->removeAllItems();
			for (int j = skills[i] != 0; j < 5; j++)
				lvl->addItem(j == 4 ? "MAX" : std::to_string(j));
			lvl->setSelectedItemByIndex(state.skillMap[skills[i] * nbSkills + i].level - (skills[i] != 0));
			lvl->connect("ItemSelected", [&state, i, nbSkills](std::weak_ptr<tgui::ComboBox> skill, int item){
				auto index = skill.lock()->getSelectedItemIndex();

				for (int j = 0; j < 3; j++) {
					if (j == index) {
						state.skillMap[j * nbSkills + i].notUsed = false;
						state.skillMap[j * nbSkills + i].level = item + (index != 0);
					} else {
						state.skillMap[j * nbSkills + i].notUsed = true;
						state.skillMap[j * nbSkills + i].level = 0x7F;
					}
				}
			}, std::weak_ptr<tgui::ComboBox>(id));

			img->getRenderer()->setTexture(
				tgui::Texture(
					skillsTextures[character],
					sf::IntRect((skills[i] * nbSkills + i) * 32, 0, 32, 32)
				)
			);
		}

		panel->get<tgui::Button>("Button1")->connect("Clicked", [&manager, character]{
			unsigned last = 100 + 3 * (4 + (character == SokuLib::CHARACTER_PATCHOULI));
			const char *brokenNames[] = {
				"reimu", "marisa", "sakuya", "alice", "patchouli", "youmu", "remilia", "yuyuko", "yukari",
				"suika", "udonge", "aya", "komachi", "iku", "tenshi", "sanae", "chirno", "meirin", "utsuho", "suwako"
			};
			std::vector<unsigned short> cards;

			for (int i = 0; i < 21; i++)
				cards.push_back(i);
			for (int i = 100; i < last; i++)
				cards.push_back(i);

			auto &entry = characterSpellCards[brokenNames[character]];

			for (auto &card : entry)
				cards.push_back(card);
			makeFakeCard(manager, cards[rand() % cards.size()]);
		});

		// HP and SP
		auto HP = panel->get<tgui::Slider>("HP");
		auto SP = panel->get<tgui::Slider>("SP");
		auto brokenOrbs = panel->get<tgui::Slider>("BrokenOrbs");
		HP->connect("ValueChanged", [&state](float newValue) { state.hp = newValue; });
		SP->connect("ValueChanged", [&state](float newValue) { state.maxCurrentSpirit = newValue; });
		brokenOrbs->connect("ValueChanged", [&state](float newValue) { state.brokenOrbs = newValue; });
		HP->setValue(state.hp);
		SP->setValue(state.maxCurrentSpirit);
		brokenOrbs->setValue(state.brokenOrbs);

		panel->get<tgui::CheckBox>("HPInstantRegen")->setChecked(state.HPInstantRegen);
		panel->get<tgui::CheckBox>("SPInstantRegen")->setChecked(state.SPInstantRegen);
	}

	static void displaySkillsPanel(const std::string &profile)
	{
		panel->loadWidgetsFromFile(profile + "/assets/skills.gui");

		tgui::Panel::Ptr leftPanel = panel->get<tgui::Panel>("Left");
		tgui::Panel::Ptr rightPanel = panel->get<tgui::Panel>("Right");

		populateCharacterPanel(profile, leftPanel,  SokuLib::getBattleMgr().leftCharacterManager,  SokuLib::leftChar,  settings.nonSaved.leftState);
		populateCharacterPanel(profile, rightPanel, SokuLib::getBattleMgr().rightCharacterManager, SokuLib::rightChar, settings.nonSaved.rightState);
		updateGuiState();
	}

	static void updateDummyPanel()
	{
		settings.controlDummy = panel->get<tgui::CheckBox>("Control")->isChecked();
	}

	static void displayDummyPanel(const std::string &profile)
	{
		panel->loadWidgetsFromFile(profile + "/assets/dummy.gui");
		panel->get<tgui::CheckBox>("Control")->setChecked(settings.controlDummy);

		auto block = panel->get<tgui::ComboBox>("Block");
		auto airtech = panel->get<tgui::ComboBox>("Airtech");
		auto tech = panel->get<tgui::ComboBox>("Tech");
		auto breaking = panel->get<tgui::Panel>("Breaking");
		auto normal = panel->get<tgui::CheckBox>("Normal");
		auto x = panel->get<tgui::Slider>("X");
		auto y = panel->get<tgui::Slider>("Y");
		auto xen = panel->get<tgui::CheckBox>("XEnabled");
		auto fct = [breaking]{
			for (auto &widget : breaking->getWidgets())
				widget->setEnabled(settings.nonSaved.activated);
		};

		panel->get<tgui::Button>("Button1")->connect("Clicked", []{
			//static int i = 0;
			//static const char *s[] = {"d 2d", "d 1d", "d 4d", "d 6d"};

			//addInputSequence(s[i]);
			//i = (i + 1) % 4;
			addInputSequence("623b");
		});

		x->setValue(settings.posX != 0 ? settings.posX : 40);
		x->setEnabled(settings.posX != 0);
		xen->connect("Changed", [x](bool b){
			x->setEnabled(b);
			settings.posX = b ? x->getValue() : 0;
		});
		xen->setChecked(settings.posX != 0);

		y->setValue(settings.posY != 0 ? settings.posY : 0);
		y->setEnabled(settings.posY != 0);

		x->connect("ValueChanged", [](float v){
			settings.posX = v;
		});
		y->connect("ValueChanged", [](float v){
			settings.posY = v;
		});

		block->setSelectedItemByIndex(settings.block);
		airtech->setSelectedItemByIndex(settings.airtech);
		tech->setSelectedItemByIndex(settings.tech);
		block->connect("ItemSelected", [](int item){
			settings.block = static_cast<BlockingSide>(item);
		});
		airtech->connect("ItemSelected", [](int item){
			settings.airtech = static_cast<AirTechDirection>(item);
		});
		tech->connect("ItemSelected", [](int item){
			settings.tech = static_cast<TechDirection>(item);
		});
		fct();
		normal->setChecked(!settings.nonSaved.activated);
		normal->connect("Changed", [fct](bool b){
			if (b)
				removeHooks();
			else
				placeHooks();
			fct();
		});
	}

	static void updateStatePanel()
	{
	}

	static void setNewWeather(SokuLib::Weather weather)
	{
		if (weather == SokuLib::WEATHER_CLEAR) {
			settings.weatherResetRequest = true;
			return;
		}
		SokuLib::displayedWeather = weather;
		SokuLib::activeWeather = SokuLib::WEATHER_CLEAR;
		SokuLib::weatherCounter = 999;
	}
	
	static void displayStatePanel(const std::string &profile)
	{
		panel->loadWidgetsFromFile(profile + "/assets/state.gui");

		auto checkFct = [](bool &val, bool b){
			val = b;
		};
		auto force = panel->get<tgui::CheckBox>("ForceWeather");
		auto weather = panel->get<tgui::ComboBox>("Weather");
		auto editTimer = panel->get<tgui::EditBox>("EditTimer");
		auto counter = panel->get<tgui::ComboBox>("Counter");
		auto frameRate = panel->get<tgui::EditBox>("FPS");
		auto frameStep = panel->get<tgui::Button>("Step");
		auto leftHurtBoxes        = panel->get<tgui::CheckBox>("LeftHurtBoxes");
		auto leftHitBoxes         = panel->get<tgui::CheckBox>("LeftHitBoxes");
		auto leftPosBox           = panel->get<tgui::CheckBox>("LeftPosBox");
		auto leftCollisionBox     = panel->get<tgui::CheckBox>("LeftCollisionBox");
		auto leftBuff             = panel->get<tgui::CheckBox>("LeftBuff");
		auto leftHitProperties    = panel->get<tgui::CheckBox>("LeftHitProperties");
		auto leftSubObjHurtBoxes  = panel->get<tgui::CheckBox>("LeftSubObjHurtBoxes");
		auto leftSubObjHitBoxes   = panel->get<tgui::CheckBox>("LeftSubObjHitBoxes");
		auto leftSubObjPosBox     = panel->get<tgui::CheckBox>("LeftSubObjPosBox");
		auto leftSubObjProperties = panel->get<tgui::CheckBox>("LeftSubObjProperties");
		auto rightHurtBoxes       = panel->get<tgui::CheckBox>("RightHurtBoxes");
		auto rightHitBoxes        = panel->get<tgui::CheckBox>("RightHitBoxes");
		auto rightPosBox          = panel->get<tgui::CheckBox>("RightPosBox");
		auto rightCollisionBox    = panel->get<tgui::CheckBox>("RightCollisionBox");
		auto rightBuff            = panel->get<tgui::CheckBox>("RightBuff");
		auto rightHitProperties   = panel->get<tgui::CheckBox>("RightHitProperties");
		auto rightSubObjHurtBoxes = panel->get<tgui::CheckBox>("RightSubObjHurtBoxes");
		auto rightSubObjHitBoxes  = panel->get<tgui::CheckBox>("RightSubObjHitBoxes");
		auto rightSubObjPosBox    = panel->get<tgui::CheckBox>("RightSubObjPosBox");
		auto rightSubObjProperties= panel->get<tgui::CheckBox>("RightSubObjProperties");

		leftHurtBoxes->setChecked(settings.leftHitboxSettings.showHurtboxes);
		leftHitBoxes->setChecked(settings.leftHitboxSettings.showHitboxes);
		leftPosBox->setChecked(settings.leftHitboxSettings.showPosition);
		leftCollisionBox->setChecked(settings.leftHitboxSettings.showCollisionBox);
		leftBuff->setChecked(settings.leftHitboxSettings.showBuffProperties);
		leftHitProperties->setChecked(settings.leftHitboxSettings.showHitProperties);
		leftSubObjHurtBoxes->setChecked(settings.leftHitboxSettings.showSubObjectHurtboxes);
		leftSubObjHitBoxes->setChecked(settings.leftHitboxSettings.showSubObjectHitboxes);
		leftSubObjPosBox->setChecked(settings.leftHitboxSettings.showSubObjectPosition);
		leftSubObjProperties->setChecked(settings.leftHitboxSettings.showSubObjectProperties);
		rightHurtBoxes->setChecked(settings.rightHitboxSettings.showHurtboxes);
		rightHitBoxes->setChecked(settings.rightHitboxSettings.showHitboxes);
		rightPosBox->setChecked(settings.rightHitboxSettings.showPosition);
		rightCollisionBox->setChecked(settings.rightHitboxSettings.showCollisionBox);
		rightBuff->setChecked(settings.rightHitboxSettings.showBuffProperties);
		rightHitProperties->setChecked(settings.rightHitboxSettings.showHitProperties);
		rightSubObjHurtBoxes->setChecked(settings.rightHitboxSettings.showSubObjectHurtboxes);
		rightSubObjHitBoxes->setChecked(settings.rightHitboxSettings.showSubObjectHitboxes);
		rightSubObjPosBox->setChecked(settings.rightHitboxSettings.showSubObjectPosition);
		rightSubObjProperties->setChecked(settings.rightHitboxSettings.showSubObjectProperties);

		leftHurtBoxes->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showHurtboxes));
		leftHitBoxes->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showHitboxes));
		leftPosBox->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showPosition));
		leftCollisionBox->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showCollisionBox));
		leftBuff->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showBuffProperties));
		leftHitProperties->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showHitProperties));
		leftSubObjHurtBoxes->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showSubObjectHurtboxes));
		leftSubObjHitBoxes->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showSubObjectHitboxes));
		leftSubObjPosBox->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showSubObjectPosition));
		leftSubObjProperties->connect("Changed", checkFct, std::ref(settings.leftHitboxSettings.showSubObjectProperties));
		rightHurtBoxes->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showHurtboxes));
		rightHitBoxes->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showHitboxes));
		rightPosBox->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showPosition));
		rightCollisionBox->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showCollisionBox));
		rightBuff->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showBuffProperties));
		rightHitProperties->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showHitProperties));
		rightSubObjHurtBoxes->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showSubObjectHurtboxes));
		rightSubObjHitBoxes->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showSubObjectHitboxes));
		rightSubObjPosBox->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showSubObjectPosition));
		rightSubObjProperties->connect("Changed", checkFct, std::ref(settings.rightHitboxSettings.showSubObjectProperties));

		frameRate->setText(std::to_string(settings.requestedFrameRate));
		auto fct = [](std::weak_ptr<tgui::EditBox> boxWeak){
			auto box = boxWeak.lock();

			if (box->getText().isEmpty())
				return;
			settings.requestedFrameRate = std::stoul(box->getText().toAnsiString());
		};
		frameRate->connect("TextChanged", fct, std::weak_ptr<tgui::EditBox>(frameRate));
		frameRate->connect("ReturnKeyPressed", fct, std::weak_ptr<tgui::EditBox>(frameRate));
		frameStep->connect("Clicked", [frameRate]{
			extern float frameCounter;
			if (frameRate->getText() != "0")
				frameRate->setText("0");
			else
				frameCounter++;
		});

		std::string editTimerStr = std::to_string(settings.weatherTime / 10) + "." + std::to_string(settings.weatherTime % 10);
		editTimer->setText(editTimerStr);

		editTimer->connect("TextChanged", [](std::string time) {
			settings.weatherTime = !time.empty() ? atof(time.c_str()) * 10 : 999;
		});

		counter->setSelectedItemByIndex(settings.counter);
		counter->connect("ItemSelected", [](int item){
			settings.counter = static_cast<Counter>(item);
		});

		force->setChecked(settings.forceWeather);
		force->connect("Changed", [weather](bool newValue){
			settings.forceWeather = newValue;
			weather->setEnabled(newValue);
			if (newValue)
				setNewWeather(static_cast<SokuLib::Weather>(weather->getSelectedItemIndex()));
		});
		weather->setEnabled(settings.forceWeather);
		if (settings.forceWeather && SokuLib::activeWeather != SokuLib::WEATHER_CLEAR)
			weather->setSelectedItemByIndex(SokuLib::displayedWeather);
		else
			weather->setSelectedItemByIndex(SokuLib::WEATHER_CLEAR);
		weather->connect("ItemSelected", [](int item){
			setNewWeather(static_cast<SokuLib::Weather>(item));
		});
	}

	static void displayMiscPanel(const std::string &profile)
	{
		panel->loadWidgetsFromFile(profile + "/assets/misc.gui");

		auto leftBox = panel->get<tgui::CheckBox>("LeftInputs");
		auto rightBox = panel->get<tgui::CheckBox>("RightInputs");
		auto leftJoypad = panel->get<tgui::CheckBox>("LeftJoypad");
		auto rightJoypad = panel->get<tgui::CheckBox>("RightJoypad");
		auto rawBox = panel->get<tgui::CheckBox>("RawInputs");
		auto emptyBox = panel->get<tgui::CheckBox>("Empty");

		leftBox->setChecked(settings.showLeftInputBox);
		leftBox->connect("Changed", [](bool b){
			settings.showLeftInputBox = b;
		});

		rightBox->setChecked(settings.showRightInputBox);
		rightBox->connect("Changed", [](bool b){
			settings.showRightInputBox = b;
		});

		leftJoypad->setChecked(settings.showLeftJoypad);
		leftJoypad->connect("Changed", [](bool b){
			settings.showLeftJoypad = b;
		});

		rightJoypad->setChecked(settings.showRightJoypad);
		rightJoypad->connect("Changed", [](bool b){
			settings.showRightJoypad = b;
		});

		rawBox->setChecked(settings.showRawInputs);
		rawBox->connect("Changed", [](bool b){
			settings.showRawInputs = b;
		});

		emptyBox->setChecked(settings.showEmptyInputs);
		emptyBox->connect("Changed", [](bool b){
			settings.showEmptyInputs = b;
		});
	}

	static void showMacroInputs(const std::string &profile, std::weak_ptr<tgui::ComboBox> macrosWeak, tgui::EditBox::Ptr name, tgui::ScrollablePanel::Ptr inputPanel, tgui::ComboBox::Ptr character)
	{
		auto macros = macrosWeak.lock();
		auto &allMacros = settings.nonSaved.macros.macros[character->getSelectedItemIndex()];

		if (macros->getSelectedItemIndex() < 0 || macros->getSelectedItemIndex() >= allMacros.size())
			return;

		auto &selected = allMacros[macros->getSelectedItemIndex()];
		auto &elems = selected.macroElems;
		int i = 0;

		name->setText(selected.name);
		inputPanel->removeAllWidgets();
		for (auto &elem : elems) {
			auto pan = tgui::Panel::create({615, 22});

			pan->loadWidgetsFromFile(profile + "/assets/macroElem.gui");

			auto A = pan->get<tgui::CheckBox>("A");
			auto B = pan->get<tgui::CheckBox>("B");
			auto C = pan->get<tgui::CheckBox>("C");
			auto D = pan->get<tgui::CheckBox>("D");
			auto SC = pan->get<tgui::CheckBox>("SC");
			auto CH = pan->get<tgui::CheckBox>("CH");
			auto H = pan->get<tgui::Slider>("H");
			auto V = pan->get<tgui::Slider>("V");
			auto duration = pan->get<tgui::EditBox>("Duration");
			auto remove = pan->get<tgui::Button>("Remove");

			A->setChecked(elem.first.a);
			B->setChecked(elem.first.b);
			C->setChecked(elem.first.c);
			D->setChecked(elem.first.d);
			SC->setChecked(elem.first.spellcard);
			CH->setChecked(elem.first.changeCard);
			H->setValue(elem.first.horizontalAxis);
			V->setValue(elem.first.verticalAxis);
			duration->setText(std::to_string(elem.second));

			A->connect("Changed", [&elem](bool b){
				elem.first.a = b;
			});
			B->connect("Changed", [&elem](bool b){
				elem.first.b = b;
			});
			C->connect("Changed", [&elem](bool b){
				elem.first.c = b;
			});
			D->connect("Changed", [&elem](bool b){
				elem.first.d = b;
			});
			SC->connect("Changed", [&elem](bool b){
				elem.first.spellcard = b;
			});
			CH->connect("Changed", [&elem](bool b){
				elem.first.changeCard = b;
			});
			H->connect("ValueChanged", [&elem](float v){
				elem.first.horizontalAxis = v;
			});
			V->connect("ValueChanged", [&elem](float v){
				elem.first.verticalAxis = v;
			});
			duration->connect("TextChanged", [&elem](const std::string &text){
				if (text.empty())
					return;
				try {
					elem.second = std::stoul(text);
				} catch (...) {}
			});
			remove->connect("Clicked", [profile, macros, name, inputPanel, character, i, &elems]{
				elems.erase(elems.begin() + i);
				showMacroInputs(profile, macros, name, inputPanel, character);
			});

			pan->setPosition(0, i * 30);
			i++;
			inputPanel->add(pan);
		}
		auto lab = tgui::Label::create();

		lab->setSize({20, 20});
		lab->setPosition(0, i * 30);
		inputPanel->add(lab);
	}

	void displayMacroPanel(const std::string &profile)
	{
		panel->loadWidgetsFromFile(profile + "/assets/macro.gui");

		auto macros = panel->get<tgui::ComboBox>("Macros");
		auto character = panel->get<tgui::ComboBox>("Character");
		auto name = panel->get<tgui::EditBox>("Name");
		auto inputPanel = panel->get<tgui::ScrollablePanel>("InputPanel");
		auto newMacro = panel->get<tgui::Button>("New");
		auto deleteMacro = panel->get<tgui::Button>("Delete");
		auto play = panel->get<tgui::Button>("Play");
		auto record1 = panel->get<tgui::Button>("Record1");
		auto addInput = panel->get<tgui::Button>("AddInput");
		auto record2 = panel->get<tgui::Button>("Record2");
		auto loadMacros = [macros, name, deleteMacro, play, record1, addInput, record2](int character){
			const auto &allMacros = settings.nonSaved.macros.macros[character];
			auto isEmpty = allMacros.empty();

			macros->removeAllItems();
			for (auto &macro : allMacros)
				macros->addItem(macro.name);
			macros->setSelectedItemByIndex(0);
			if (!isEmpty)
				name->setText(allMacros.front().name);
			name->setEnabled(!isEmpty);
			deleteMacro->setEnabled(!isEmpty);
			play->setEnabled(!isEmpty);
			record1->setEnabled(!isEmpty);
			addInput->setEnabled(!isEmpty);
			record2->setEnabled(!isEmpty);
		};

		character->setSelectedItemByIndex(SokuLib::rightChar);
		character->connect("ItemSelected", loadMacros);
		loadMacros(SokuLib::rightChar);
		showMacroInputs(profile, macros, name, inputPanel, character);

		macros->connect("ItemSelected", showMacroInputs, profile, std::weak_ptr<tgui::ComboBox>(macros), name, inputPanel, character);
		newMacro->connect("Clicked", [name, inputPanel, macros, character, deleteMacro, play, record1, addInput, record2]{
			auto &allMacros = settings.nonSaved.macros.macros[character->getSelectedItemIndex()];
			auto macroName = "Untitled macro " + std::to_string(allMacros.size() + 1);

			allMacros.push_back({macroName, {}});
			macros->addItem(macroName);
			macros->setSelectedItemByIndex(allMacros.size() - 1);
			name->setText(allMacros.front().name);
			name->setEnabled(true);
			deleteMacro->setEnabled(true);
			play->setEnabled(true);
			record1->setEnabled(true);
			addInput->setEnabled(true);
			record2->setEnabled(true);
			inputPanel->removeAllWidgets();
		});
		name->connect("TextChanged", [character, macros, profile, name, inputPanel](std::string newName){
			auto &allMacros = settings.nonSaved.macros.macros[character->getSelectedItemIndex()];
			auto selected = macros->getSelectedItemIndex();
			auto &changedMacro = allMacros[selected];

			changedMacro.name.swap(newName);
			macros->disconnectAll();
			macros->removeAllItems();
			for (auto &macro : allMacros)
				macros->addItem(macro.name);
			macros->setSelectedItemByIndex(selected);
			macros->connect("ItemSelected", showMacroInputs, profile, std::weak_ptr<tgui::ComboBox>(macros), name, inputPanel, character);
		});
		deleteMacro->connect("Clicked", [macros, character, name, play, record1, addInput, record2](std::weak_ptr<tgui::Button> self){
			auto &allMacros = settings.nonSaved.macros.macros[character->getSelectedItemIndex()];
			auto index = macros->getSelectedItemIndex();
			auto isEmpty = allMacros.size() == 1;

			name->setEnabled(!isEmpty);
			self.lock()->setEnabled(!isEmpty);
			play->setEnabled(!isEmpty);
			record1->setEnabled(!isEmpty);
			addInput->setEnabled(!isEmpty);
			record2->setEnabled(!isEmpty);
			macros->removeItemByIndex(index);
			allMacros.erase(allMacros.begin() + index);
		}, std::weak_ptr<tgui::Button>(deleteMacro));
		play->connect("Clicked", [character, macros]{
			auto &macro = settings.nonSaved.macros.macros[character->getSelectedItemIndex()][macros->getSelectedItemIndex()];
			std::vector<SokuLib::KeyInput> sequence;

			for (auto &elem : macro.macroElems)
				for (int i = 0; i < elem.second; i++)
					sequence.push_back(elem.first);
			addInputSequence(sequence);
		});
		//record1->;
		//record2->;
		addInput->connect("Clicked", [character, profile, macros, name, inputPanel]{
			auto &macro = settings.nonSaved.macros.macros[character->getSelectedItemIndex()][macros->getSelectedItemIndex()];

			if (macro.macroElems.empty())
				macro.macroElems.emplace_back();
			else
				macro.macroElems.push_back(macro.macroElems.back());
			macro.macroElems.back().second = 1;
			showMacroInputs(profile, macros, name, inputPanel, character);
		});
	}

	void loadAllGuiElements(LPCSTR profilePath)
	{
		puts("Loading GUI...");

		std::string profile = profilePath;

		gui.loadWidgetsFromFile(profile + "/assets/main.gui");
		panel = gui.get<tgui::Panel>("Panel");
		tab = gui.get<tgui::Tabs>("Tabs");
		tab->connect("TabSelected", [profile]{
			switch (tab->getSelectedIndex()) {
			case 0:
				return displaySkillsPanel(profile);
			case 1:
				return displayDummyPanel(profile);
			case 2:
				return displayStatePanel(profile);
			case 3:
				return displayMacroPanel(profile);
			case 4:
				return displayMiscPanel(profile);
			default:
				return panel->removeAllWidgets();
			}
		});
		puts("Display skills");
		displaySkillsPanel(profile);
		puts("GUI loaded");
	}

	static void updateCharacterPanel(tgui::Panel::Ptr panel, SokuLib::CharacterManager &manager, SokuLib::Character character, CharacterState &state)
	{
		state.HPInstantRegen = panel->get<tgui::CheckBox>("HPInstantRegen")->isChecked();
		state.SPInstantRegen = panel->get<tgui::CheckBox>("SPInstantRegen")->isChecked();
	}

	static void updateMiscPanel()
	{
	}

	void updateGuiState()
	{
		return;
		switch (tab->getSelectedIndex()) {
		case 0:
			updateCharacterPanel(panel->get<tgui::Panel>("Left"),  SokuLib::getBattleMgr().leftCharacterManager,  SokuLib::leftChar,  settings.nonSaved.leftState);
			updateCharacterPanel(panel->get<tgui::Panel>("Right"), SokuLib::getBattleMgr().rightCharacterManager, SokuLib::rightChar, settings.nonSaved.rightState);
			break;
		case 1:
			updateDummyPanel();
			break;
		case 2:
			updateStatePanel();
			break;
		case 4:
			updateMiscPanel();
			break;
		default:
			break;
		}
	}
}