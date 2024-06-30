#include "FileFilterIndex.h"

#include "utils/StringUtil.h"
#include "views/UIModeController.h"
#include "FileData.h"
#include "Log.h"
#include "Settings.h"

#define UNKNOWN_LABEL "UNKNOWN"
#define INCLUDE_UNKNOWN false;

FileFilterIndex::FileFilterIndex()
	: filterByFavorites(false), filterByGenre(false), filterByHidden(false), filterByKidGame(false), filterByPlayers(false), filterByPubDev(false), filterByRatings(false),
	filterByFranchise(false), filterBySubsystem(false), filterByRegion(false), filterByLanguage(false), filterByRate(false), filterByInput(false), filterByAspect(false)
{
	clearAllFilters();
	FilterDataDecl filterDecls[] = {
		//type 				//allKeys 				//filteredBy 		//filteredKeys 				//primaryKey 	//hasSecondaryKey 	//secondaryKey 	//menuLabel
		{ FAVORITES_FILTER, &favoritesIndexAllKeys, &filterByFavorites,	&favoritesIndexFilteredKeys,"favorite",		false,				"",				"FAVORITES"	},
		{ GENRE_FILTER, 	&genreIndexAllKeys, 	&filterByGenre,		&genreIndexFilteredKeys, 	"genre",		true,				"genre",		"GENRE"	},
		{ PLAYER_FILTER, 	&playersIndexAllKeys, 	&filterByPlayers,	&playersIndexFilteredKeys, 	"players",		false,				"",				"PLAYERS"	},
		{ PUBDEV_FILTER, 	&pubDevIndexAllKeys, 	&filterByPubDev,	&pubDevIndexFilteredKeys, 	"developer",	true,				"publisher",	"PUBLISHER / DEVELOPER"	},
		{ RATINGS_FILTER, 	&ratingsIndexAllKeys, 	&filterByRatings,	&ratingsIndexFilteredKeys, 	"rating",		false,				"",				"RATING"	},
		{ KIDGAME_FILTER, 	&kidGameIndexAllKeys, 	&filterByKidGame,	&kidGameIndexFilteredKeys, 	"kidgame",		false,				"",				"KIDGAME" },
		{ HIDDEN_FILTER, 	&hiddenIndexAllKeys, 	&filterByHidden,	&hiddenIndexFilteredKeys, 	"hidden",		false,				"",				"HIDDEN" },
		{ FRANCHISE_FILTER,	&franchiseIndexAllKeys,	&filterByFranchise,	&franchiseIndexFilteredKeys,"franchise",	true,				"franchise",	"FRANCHISE" },
		{ SUBSYSTEM_FILTER,	&subsystemIndexAllKeys,	&filterBySubsystem,	&subsystemIndexFilteredKeys,"subsystem",	false,				"",				"SUBSYSTEM" },
		{ REGION_FILTER,	&regionIndexAllKeys, 	&filterByRegion,	&regionIndexFilteredKeys, 	"region",		false,				"",				"REGION" },
		{ LANGUAGE_FILTER,	&languageIndexAllKeys, 	&filterByLanguage,	&languageIndexFilteredKeys, "language",		false,				"",				"LANGUAGE" },
		{ RATE_FILTER,		&rateIndexAllKeys, 		&filterByRate,		&rateIndexFilteredKeys, 	"rate",			false,				"",				"AGE CLASSIFICATION" },
		{ INPUT_FILTER,		&inputIndexAllKeys, 	&filterByInput,		&inputIndexFilteredKeys, 	"input",		false,				"",				"INPUT DEVICE" },
		{ ASPECT_FILTER,	&aspectIndexAllKeys, 	&filterByAspect,	&aspectIndexFilteredKeys, 	"aspect",		false,				"",				"ASPECT RATIO" }
	};

	filterDataDecl = std::vector<FilterDataDecl>(filterDecls, filterDecls + sizeof(filterDecls) / sizeof(filterDecls[0]));
}

FileFilterIndex::~FileFilterIndex()
{
	resetIndex();
}

std::vector<FilterDataDecl>& FileFilterIndex::getFilterDataDecls()
{
	return filterDataDecl;
}

void FileFilterIndex::importIndex(FileFilterIndex* indexToImport)
{
	struct IndexImportStructure
	{
		std::map<std::string, int>* destinationIndex;
		std::map<std::string, int>* sourceIndex;
	};

	IndexImportStructure indexStructDecls[] = {
		{ &genreIndexAllKeys, &(indexToImport->genreIndexAllKeys) },
		{ &playersIndexAllKeys, &(indexToImport->playersIndexAllKeys) },
		{ &pubDevIndexAllKeys, &(indexToImport->pubDevIndexAllKeys) },
		{ &ratingsIndexAllKeys, &(indexToImport->ratingsIndexAllKeys) },
		{ &favoritesIndexAllKeys, &(indexToImport->favoritesIndexAllKeys) },
		{ &hiddenIndexAllKeys, &(indexToImport->hiddenIndexAllKeys) },
		{ &kidGameIndexAllKeys, &(indexToImport->kidGameIndexAllKeys) },
		{ &franchiseIndexAllKeys, &(indexToImport->franchiseIndexAllKeys) },
		{ &subsystemIndexAllKeys, &(indexToImport->subsystemIndexAllKeys) },
		{ &regionIndexAllKeys, &(indexToImport->regionIndexAllKeys) },
		{ &languageIndexAllKeys, &(indexToImport->languageIndexAllKeys) },
		{ &rateIndexAllKeys, &(indexToImport->rateIndexAllKeys) },
		{ &inputIndexAllKeys, &(indexToImport->inputIndexAllKeys) },
		{ &aspectIndexAllKeys, &(indexToImport->aspectIndexAllKeys) }
	};

	std::vector<IndexImportStructure> indexImportDecl = std::vector<IndexImportStructure>(indexStructDecls, indexStructDecls + sizeof(indexStructDecls) / sizeof(indexStructDecls[0]));

	for (std::vector<IndexImportStructure>::const_iterator indexesIt = indexImportDecl.cbegin(); indexesIt != indexImportDecl.cend(); ++indexesIt )
	{
		for (std::map<std::string, int>::const_iterator sourceIt = (*indexesIt).sourceIndex->cbegin(); sourceIt != (*indexesIt).sourceIndex->cend(); ++sourceIt )
		{
			if ((*indexesIt).destinationIndex->find((*sourceIt).first) == (*indexesIt).destinationIndex->cend())
			{
				// entry doesn't exist
				(*((*indexesIt).destinationIndex))[(*sourceIt).first] = (*sourceIt).second;
			}
			else
			{
				(*((*indexesIt).destinationIndex))[(*sourceIt).first] += (*sourceIt).second;
			}
		}
	}
}
void FileFilterIndex::resetIndex()
{
	clearAllFilters();
	clearIndex(genreIndexAllKeys);
	clearIndex(playersIndexAllKeys);
	clearIndex(pubDevIndexAllKeys);
	clearIndex(ratingsIndexAllKeys);
	clearIndex(favoritesIndexAllKeys);
	clearIndex(hiddenIndexAllKeys);
	clearIndex(kidGameIndexAllKeys);
	clearIndex(franchiseIndexAllKeys);
	clearIndex(subsystemIndexAllKeys);
	clearIndex(regionIndexAllKeys);
	clearIndex(languageIndexAllKeys);
	clearIndex(rateIndexAllKeys);
	clearIndex(inputIndexAllKeys);
	clearIndex(aspectIndexAllKeys);
}

std::string FileFilterIndex::getIndexableKey(FileData* game, FilterIndexType type, bool getSecondary)
{
	std::string key = "";
	switch(type)
	{
		case GENRE_FILTER:
		{
			key = Utils::String::toUpper(game->metadata.get("genre"));
			key = Utils::String::trim(key);
			if (getSecondary && !key.empty()) {
				std::istringstream f(key);
				std::string newKey;
				getline(f, newKey, '/');
				if (!newKey.empty() && newKey != key)
				{
					key = newKey;
				}
				else
				{
					key = std::string();
				}
			}
			break;
		}
		case PLAYER_FILTER:
		{
			if (getSecondary)
				break;

			key = game->metadata.get("players");
			break;
		}
		case PUBDEV_FILTER:
		{
			key = Utils::String::toUpper(game->metadata.get("publisher"));
			key = Utils::String::trim(key);

			if ((getSecondary && !key.empty()) || (!getSecondary && key.empty()))
				key = Utils::String::toUpper(game->metadata.get("developer"));
			else
				key = Utils::String::toUpper(game->metadata.get("publisher"));
			break;
		}
		case RATINGS_FILTER:
		{
			int ratingNumber = 0;
			if (!getSecondary)
			{
				std::string ratingString = game->metadata.get("rating");
				if (!ratingString.empty()) {
					try {
						ratingNumber = (int)((std::stod(ratingString)*5)+0.5);
						if (ratingNumber < 0)
							ratingNumber = 0;

						key = std::to_string(ratingNumber) + " STARS";
					}
					catch (int e)
					{
						LOG(LogError) << "Error parsing Rating (invalid value, exception nr.): " << ratingString << ", " << e;
					}
				}
			}
			break;
		}
		case FAVORITES_FILTER:
		{
			if (game->getType() != GAME)
				return "FALSE";
			key = Utils::String::toUpper(game->metadata.get("favorite"));
			break;
		}
		case HIDDEN_FILTER:
		{
			if (game->getType() != GAME)
				return "FALSE";
			key = Utils::String::toUpper(game->metadata.get("hidden"));
			break;
		}
		case KIDGAME_FILTER:
		{
			if (game->getType() != GAME)
				return "FALSE";
			key = Utils::String::toUpper(game->metadata.get("kidgame"));
			break;
		}
		case FRANCHISE_FILTER:
		{
			key = Utils::String::toUpper(game->metadata.get("franchise"));
			key = Utils::String::trim(key);
			if (getSecondary && !key.empty()) {
				std::istringstream f(key);
				std::string newKey;
				getline(f, newKey, '/');
				if (!newKey.empty() && newKey != key)
				{
					key = newKey;
				}
				else
				{
					key = std::string();
				}
			}
			break;
		}
		case SUBSYSTEM_FILTER:
		{
			if (getSecondary)
				break;
			key = Utils::String::toUpper(game->metadata.get("subsystem"));
			key = Utils::String::trim(key);
			break;
		}
		case REGION_FILTER:
		{
			if (getSecondary)
				break;
			key = Utils::String::toUpper(game->metadata.get("region"));
			key = Utils::String::trim(key);
			break;
		}
		case LANGUAGE_FILTER:
		{
			if (getSecondary)
				break;
			key = Utils::String::toUpper(game->metadata.get("language"));
			key = Utils::String::trim(key);
			break;
		}
		case RATE_FILTER:
		{
			if (getSecondary)
				break;
			key = Utils::String::toUpper(game->metadata.get("rate"));
			key = Utils::String::trim(key);
			break;
		}
		case INPUT_FILTER:
		{
			if (getSecondary)
				break;
			key = Utils::String::toUpper(game->metadata.get("input"));
			key = Utils::String::trim(key);
			break;
		}
		case ASPECT_FILTER:
		{
			if (getSecondary)
				break;
			key = Utils::String::toUpper(game->metadata.get("aspect"));
			key = Utils::String::trim(key);
			break;
		}
		default:
			LOG(LogWarning) << "Unknown Filter type:" << type;
			break;
	}
	key = Utils::String::trim(key);
	if (key.empty() || (type == RATINGS_FILTER && key == "0 STARS")) {
		key = UNKNOWN_LABEL;
	}
	return key;
}

void FileFilterIndex::addToIndex(FileData* game)
{
	manageGenreEntryInIndex(game);
	managePlayerEntryInIndex(game);
	managePubDevEntryInIndex(game);
	manageRatingsEntryInIndex(game);
	manageFavoritesEntryInIndex(game);
	manageHiddenEntryInIndex(game);
	manageKidGameEntryInIndex(game);
	manageFranchiseEntryInIndex(game);
	manageSubsystemEntryInIndex(game);
	manageRegionEntryInIndex(game);
	manageLanguageEntryInIndex(game);
	manageRateEntryInIndex(game);
	manageInputEntryInIndex(game);
	manageAspectEntryInIndex(game);
}

void FileFilterIndex::removeFromIndex(FileData* game)
{
	manageGenreEntryInIndex(game, true);
	managePlayerEntryInIndex(game, true);
	managePubDevEntryInIndex(game, true);
	manageRatingsEntryInIndex(game, true);
	manageFavoritesEntryInIndex(game, true);
	manageHiddenEntryInIndex(game, true);
	manageKidGameEntryInIndex(game, true);
	manageFranchiseEntryInIndex(game, true);
	manageSubsystemEntryInIndex(game, true);
	manageRegionEntryInIndex(game, true);
	manageLanguageEntryInIndex(game, true);
	manageRateEntryInIndex(game, true);
	manageInputEntryInIndex(game, true);
	manageAspectEntryInIndex(game, true);
}

void FileFilterIndex::setFilter(FilterIndexType type, std::vector<std::string>* values)
{
	// test if it exists before setting
	if(type == NONE)
	{
		clearAllFilters();
	}
	else
	{
		for (std::vector<FilterDataDecl>::const_iterator it = filterDataDecl.cbegin(); it != filterDataDecl.cend(); ++it ) {
			if ((*it).type == type)
			{
				FilterDataDecl filterData = (*it);
				*(filterData.filteredByRef) = values->size() > 0;
				filterData.currentFilteredKeys->clear();
				for (std::vector<std::string>::const_iterator vit = values->cbegin(); vit != values->cend(); ++vit ) {
					// check if exists
					if (filterData.allIndexKeys->find(*vit) != filterData.allIndexKeys->cend()) {
						filterData.currentFilteredKeys->push_back(std::string(*vit));
					}
				}
			}
		}
	}
	return;
}

void FileFilterIndex::clearAllFilters()
{
	for (std::vector<FilterDataDecl>::const_iterator it = filterDataDecl.cbegin(); it != filterDataDecl.cend(); ++it )
	{
		FilterDataDecl filterData = (*it);
		*(filterData.filteredByRef) = false;
		filterData.currentFilteredKeys->clear();
	}
	return;
}

void FileFilterIndex::resetFilters()
{
	clearAllFilters();
	setUIModeFilters();
}

void FileFilterIndex::setUIModeFilters()
{
	if(!Settings::getInstance()->getBool("ForceDisableFilters")){
		if (UIModeController::getInstance()->isUIModeKiosk())
		{
			filterByHidden = true;
			std::vector<std::string> val = { "FALSE" };
			setFilter(HIDDEN_FILTER, &val);
		}
		if (UIModeController::getInstance()->isUIModeKid())
		{
			filterByKidGame = true;
			std::vector<std::string> val = { "TRUE" };
			setFilter(KIDGAME_FILTER, &val);
		}
	}
}

void FileFilterIndex::debugPrintIndexes()
{
	LOG(LogInfo) << "Printing Indexes...";
	for (auto x: playersIndexAllKeys) {
		LOG(LogInfo) << "Multiplayer Index: " << x.first << ": " << x.second;
	}
	for (auto x: genreIndexAllKeys) {
		LOG(LogInfo) << "Genre Index: " << x.first << ": " << x.second;
	}
	for (auto x: ratingsIndexAllKeys) {
		LOG(LogInfo) << "Ratings Index: " << x.first << ": " << x.second;
	}
	for (auto x: pubDevIndexAllKeys) {
		LOG(LogInfo) << "PubDev Index: " << x.first << ": " << x.second;
	}
	for (auto x: favoritesIndexAllKeys) {
		LOG(LogInfo) << "Favorites Index: " << x.first << ": " << x.second;
	}
	for (auto x : hiddenIndexAllKeys) {
		LOG(LogInfo) << "Hidden Index: " << x.first << ": " << x.second;
	}
	for (auto x : kidGameIndexAllKeys) {
		LOG(LogInfo) << "KidGames Index: " << x.first << ": " << x.second;
	}
	for (auto x : franchiseIndexAllKeys) {
		LOG(LogInfo) << "Franchise Index: " << x.first << ": " << x.second;
	}
	for (auto x : subsystemIndexAllKeys) {
		LOG(LogInfo) << "Subsystem Index: " << x.first << ": " << x.second;
	}
	for (auto x : regionIndexAllKeys) {
		LOG(LogInfo) << "Region Index: " << x.first << ": " << x.second;
	}
	for (auto x : languageIndexAllKeys) {
		LOG(LogInfo) << "Language Index: " << x.first << ": " << x.second;
	}
	for (auto x : rateIndexAllKeys) {
		LOG(LogInfo) << "Rate Index: " << x.first << ": " << x.second;
	}
	for (auto x : inputIndexAllKeys) {
		LOG(LogInfo) << "Input Index: " << x.first << ": " << x.second;
	}
	for (auto x : aspectIndexAllKeys) {
		LOG(LogInfo) << "Aspect Index: " << x.first << ": " << x.second;
	}
}

bool FileFilterIndex::showFile(FileData* game)
{
	// this shouldn't happen, but just in case let's get it out of the way
	if (!isFiltered())
		return true;

	// if folder, needs further inspection - i.e. see if folder contains at least one element
	// that should be shown
	if (game->getType() == FOLDER) {
		std::vector<FileData*> children = game->getChildren();
		// iterate through all of the children, until there's a match

		for (std::vector<FileData*>::const_iterator it = children.cbegin(); it != children.cend(); ++it ) {
			if (showFile(*it))
			{
				return true;
			}
		}
		return false;
	}

	bool keepGoing = false;

	for (std::vector<FilterDataDecl>::const_iterator it = filterDataDecl.cbegin(); it != filterDataDecl.cend(); ++it ) {
		FilterDataDecl filterData = (*it);
		if(*(filterData.filteredByRef))
		{
			// try to find a match
			std::string key = getIndexableKey(game, filterData.type, false);
			keepGoing = isKeyBeingFilteredBy(key, filterData.type);

			// if we didn't find a match, try for secondary keys - i.e. publisher and dev, or first genre
			if (!keepGoing)
			{
				if (!filterData.hasSecondaryKey)
				{
					return false;
				}
				std::string secKey = getIndexableKey(game, filterData.type, true);
				if (secKey != UNKNOWN_LABEL)
				{
					keepGoing = isKeyBeingFilteredBy(secKey, filterData.type);
				}
			}
			// if still nothing, then it's not a match
			if (!keepGoing)
				return false;

		}

	}

	return keepGoing;
}

bool FileFilterIndex::isKeyBeingFilteredBy(std::string key, FilterIndexType type)
{
	const FilterIndexType filterTypes[14] = {
		FAVORITES_FILTER,
		GENRE_FILTER,
		PLAYER_FILTER,
		PUBDEV_FILTER,
		RATINGS_FILTER,
		HIDDEN_FILTER,
		KIDGAME_FILTER,
		FRANCHISE_FILTER,
		SUBSYSTEM_FILTER,
		REGION_FILTER,
		LANGUAGE_FILTER,
		RATE_FILTER,
		INPUT_FILTER,
		ASPECT_FILTER
	};
	std::vector<std::string> filterKeysList[14] = {
		favoritesIndexFilteredKeys,
		genreIndexFilteredKeys,
		playersIndexFilteredKeys,
		pubDevIndexFilteredKeys,
		ratingsIndexFilteredKeys,
		hiddenIndexFilteredKeys,
		kidGameIndexFilteredKeys,
		franchiseIndexFilteredKeys,
		subsystemIndexFilteredKeys,
		regionIndexFilteredKeys,
		languageIndexFilteredKeys,
		rateIndexFilteredKeys,
		inputIndexFilteredKeys,
		aspectIndexFilteredKeys
	};

	for (int i = 0; i < 14; i++)
	{
		if (filterTypes[i] == type)
		{
			for (std::vector<std::string>::const_iterator it = filterKeysList[i].cbegin(); it != filterKeysList[i].cend(); ++it )
			{
				if (key == (*it))
				{
					return true;
				}
			}
			return false;
		}
	}

	return false;
}

void FileFilterIndex::manageFranchiseEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, FRANCHISE_FILTER, false);
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		return;
	}
	manageIndexEntry(&franchiseIndexAllKeys, key, remove);
	key = getIndexableKey(game, FRANCHISE_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&franchiseIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::manageSubsystemEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, SUBSYSTEM_FILTER, false);
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		return;
	}
	manageIndexEntry(&subsystemIndexAllKeys, key, remove);
	key = getIndexableKey(game, SUBSYSTEM_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&subsystemIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::manageRegionEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, REGION_FILTER, false);
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		return;
	}
	manageIndexEntry(&regionIndexAllKeys, key, remove);
	key = getIndexableKey(game, REGION_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&regionIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::manageLanguageEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, LANGUAGE_FILTER, false);
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		return;
	}
	manageIndexEntry(&languageIndexAllKeys, key, remove);
	key = getIndexableKey(game, LANGUAGE_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&languageIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::manageRateEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, RATE_FILTER, false);
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		return;
	}
	manageIndexEntry(&rateIndexAllKeys, key, remove);
	key = getIndexableKey(game, RATE_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&rateIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::manageInputEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, INPUT_FILTER, false);
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		return;
	}
	manageIndexEntry(&inputIndexAllKeys, key, remove);
	key = getIndexableKey(game, INPUT_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&inputIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::manageAspectEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, ASPECT_FILTER, false);
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		return;
	}
	manageIndexEntry(&aspectIndexAllKeys, key, remove);
	key = getIndexableKey(game, ASPECT_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&aspectIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::manageGenreEntryInIndex(FileData* game, bool remove)
{

	std::string key = getIndexableKey(game, GENRE_FILTER, false);

	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;

	// only add unknown in pubdev IF both dev and pub are empty
	if (!includeUnknown && (key == UNKNOWN_LABEL || key == "BIOS")) {
		// no valid genre info found
		return;
	}

	manageIndexEntry(&genreIndexAllKeys, key, remove);

	key = getIndexableKey(game, GENRE_FILTER, true);
	if (!includeUnknown && key == UNKNOWN_LABEL)
	{
		manageIndexEntry(&genreIndexAllKeys, key, remove);
	}
}

void FileFilterIndex::managePlayerEntryInIndex(FileData* game, bool remove)
{
	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;
	std::string key = getIndexableKey(game, PLAYER_FILTER, false);

	// only add unknown in pubdev IF both dev and pub are empty
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		// no valid player info found
		return;
	}

	manageIndexEntry(&playersIndexAllKeys, key, remove);
}

void FileFilterIndex::managePubDevEntryInIndex(FileData* game, bool remove)
{
	std::string pub = getIndexableKey(game, PUBDEV_FILTER, false);
	std::string dev = getIndexableKey(game, PUBDEV_FILTER, true);

	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;
	bool unknownPub = false;
	bool unknownDev = false;

	if (pub == UNKNOWN_LABEL) {
		unknownPub = true;
	}
	if (dev == UNKNOWN_LABEL) {
		unknownDev = true;
	}

	if (!includeUnknown && unknownDev && unknownPub) {
		// no valid rating info found
		return;
	}

	if (unknownDev && unknownPub) {
		// if no info at all
		manageIndexEntry(&pubDevIndexAllKeys, pub, remove);
	}
	else
	{
		if (!unknownDev) {
			// if no info at all
			manageIndexEntry(&pubDevIndexAllKeys, dev, remove);
		}
		if (!unknownPub) {
			// if no info at all
			manageIndexEntry(&pubDevIndexAllKeys, pub, remove);
		}
	}
}

void FileFilterIndex::manageRatingsEntryInIndex(FileData* game, bool remove)
{
	std::string key = getIndexableKey(game, RATINGS_FILTER, false);

	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;

	if (!includeUnknown && key == UNKNOWN_LABEL) {
		// no valid rating info found
		return;
	}

	manageIndexEntry(&ratingsIndexAllKeys, key, remove);
}

void FileFilterIndex::manageFavoritesEntryInIndex(FileData* game, bool remove)
{
	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;
	std::string key = getIndexableKey(game, FAVORITES_FILTER, false);
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		// no valid favorites info found
		return;
	}

	manageIndexEntry(&favoritesIndexAllKeys, key, remove);
}

void FileFilterIndex::manageHiddenEntryInIndex(FileData* game, bool remove)
{
	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;
	std::string key = getIndexableKey(game, HIDDEN_FILTER, false);
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		// no valid hidden info found
		return;
	}

	manageIndexEntry(&hiddenIndexAllKeys, key, remove);
}

void FileFilterIndex::manageKidGameEntryInIndex(FileData* game, bool remove)
{
	// flag for including unknowns
	bool includeUnknown = INCLUDE_UNKNOWN;
	std::string key = getIndexableKey(game, KIDGAME_FILTER, false);
	if (!includeUnknown && key == UNKNOWN_LABEL) {
		// no valid kidgame info found
		return;
	}

	manageIndexEntry(&kidGameIndexAllKeys, key, remove);
}

void FileFilterIndex::manageIndexEntry(std::map<std::string, int>* index, std::string key, bool remove) {
	bool includeUnknown = INCLUDE_UNKNOWN;
	if (!includeUnknown && key == UNKNOWN_LABEL)
		return;
	if (remove) {
		// removing entry
		if (index->find(key) == index->cend())
		{
			// this shouldn't happen
			LOG(LogInfo) << "Couldn't find entry in index! " << key;
		}
		else
		{
			(index->at(key))--;
			if(index->at(key) <= 0) {
				index->erase(key);
			}
		}
	}
	else
	{
		// adding entry
		if (index->find(key) == index->cend())
		{
			(*index)[key] = 1;
		}
		else
		{
			(index->at(key))++;
		}
	}
}

void FileFilterIndex::clearIndex(std::map<std::string, int> indexMap)
{
	indexMap.clear();
}
