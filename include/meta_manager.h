#pragma once

#include <switch.h>

#include <string>

namespace MetaManager {
  
  /**
   * Formats a u64 title ID into a hexidecimal string
   */
  std::string getHexTitleId(const u64& titleId);

  /**
   * Parses the name of an entity from a folder name
   */
  std::string parseName(const std::string& folderName);

  /**
   * Parses a rating of a mod from a folder name
   */
  u8 parseRating(const std::string& folderName);
}