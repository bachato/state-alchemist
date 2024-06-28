#include "meta_manager.h"
#include "constants.h"
#include <string.h>

/**
 * Formats a u64 title ID into a hexidecimal string
 */
std::string MetaManager::getHexTitleId(const u64& titleId) {
  u64 idCopy = titleId;
  std::string strId;

  // Copilot gave me this; converts to hex
  do {
    strId.insert(strId.begin(), "0123456789abcdef"[idCopy % 16]);
    idCopy >>= 4;
  } while (idCopy != 0);

  // Uppercase letter characters
  for (char& c : strId) {
    c = std::toupper(c);
  }

  // Pad 0s to the left to make it the proper length
  do {
    strId.insert(0, "0");
  } while (strId.size() < 16);

  return strId;
}

/**
 * Parses the name of an entity from a folder name
 */
std::string MetaManager::parseName(const std::string& folderName) {
  u8 rating = parseRating(folderName);

  if (rating != 100) {
    return folderName.substr(0, folderName.length() - RATING_DELIMITER.length() - 2);
  }

  return folderName;
}

/**
 * Parses a rating of a mod from a folder name
 */
u8 MetaManager::parseRating(const std::string& folderName) {
  if (folderName.length() > RATING_DELIMITER.length() + 2) {
    std::string possibleRating = folderName.substr(folderName.length() - 2);
    std::string possibleDelim = folderName.substr(
      folderName.length() - RATING_DELIMITER.length() - 2,
      RATING_DELIMITER.length()
    );

    if (std::isdigit(possibleRating[0]) && std::isdigit(possibleRating[1]) && possibleDelim == RATING_DELIMITER) {
      return std::stoi(possibleRating);
    }
  }
  return 100;
}

/**
 * Builds a folder name from a mod name and rating
 */
std::string MetaManager::buildFolderName(const std::string& modName, const u8& rating) {
  if (rating == 100) {
    return modName;
  }
  return modName + RATING_DELIMITER + std::to_string(rating);
}

/**
 * Checks if the entity name belongs to a folder name
 */
bool MetaManager::namesMatch(char* folderName, const std::string& entityName) {
  std::string folderNameStr(folderName);
  return parseName(folderNameStr) == entityName;
}