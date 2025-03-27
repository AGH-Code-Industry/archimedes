#pragma once
#include <filesystem>
#include <map>
#include <set>
#include <string>

#include <audio/Clip.h>

namespace fs = std::filesystem;

namespace arch::audio {
/// @brief SoundBank stores a map of Clip objects.
/// Thanks to this, you can save each audio file just once for all Sources,
/// even if some play the same sound.
class SoundBank {
	fs::path soundsDirectory;

	/// @brief A map storing all Clip objects.
	/// Each clip is referenced by its sound file's path on disk.
	std::map<std::string, Clip> _clips;

	/// @brief A map storing all Clip groups.
	/// Each groups is a set of paths to sound files which belong to Clips in the group.
	/// You can use it to load/unload multiple Clips at once.
	std::map<int, std::set<std::string>> _groups;

	/// @brief A set storing all groups that should be loaded during the game's startup.
	std::set<int> _initialGroups;

	/// @brief A map used for checking if a group has been loaded.
	std::map<int, bool> _isLoaded;

	/// @brief Add a clip to a group. Create the group if it doesn't exist.
	/// @param path Path of the clip's sound file.
	/// @param group The group you want to add the clip to.
	/// @throws AudioException if the clip is already in the group.
	void _addToGroup(const std::string& path, int group);

	/// @brief Remove a clip from a group.
	/// @param path Path of the clip's sound file.
	/// @param group The group you want to remove the clip from.
	/// @throws AudioException if the group doesn't exist or the clip doesn't belong to it.
	void _removeFromGroup(const std::string& path, int group);

public:

	/// @brief Constructor. Adds a default empty group with id=0.
	/// The group will be loaded at the game's startup.
	SoundBank();

	/// @brief Destructor. Unloads all groups that are still loaded.
	~SoundBank();

	/// @brief Loads all clips in a group.
	/// @param group Number of the group.
	/// @throws AudioException if the group doesn't exist, it's already loaded,
	/// or a clip from the group doesn't exist.
	void loadGroup(int group);

	/// @brief Loads all clips in a group.
	/// @param group Number of the group.
	/// @throws AudioException if the group doesn't exist, it's not loaded
	/// or a clip from the group doesn't exist.
	void unloadGroup(int group);

	/// @brief Loads all groups marked to load at the game's startup.
	/// @throws AudioException if some of the groups are already loaded.
	void loadInitialGroups();

	/// @brief Unloads all groups marked to load at the game's startup.
	/// @throws AudioException if some of the groups are already unloaded.
	void unloadInitialGroups();

	/// @brief Loads all groups that are not loaded.
	void loadAllUnloadedGroups();

	/// @brief Unloads all groups that are loaded.
	void unloadAllLoadedGroups();

	/// @brief Returns a Clip with a given sound file.
	/// @param path Path to the sound file.
	/// @throws AudioException if the Clip is not found.
	Clip& getClip(const std::string& path);

	/// @brief Finds a group containing a Clip with the given sound file.
	/// @param path Path to the sound file.
	/// @throws AudioException if the Clip doesn't belong to any group (so it doesn't exist).
	int findClipGroup(const std::string& path);

	/// @brief Moves a Clip to a group.
	/// Creates the destination group if it doesn't exist.
	/// @param path Path to the sound file.
	/// @param destinationGroup The group you want to insert the Clip into.
	/// @throws AudioException if any of the modified groups is loaded.
	void moveClipToGroup(const std::string& path, int destinationGroup);

	/// @brief Adds a Clip to the SoundBank, storing it in one of the groups.
	/// Creates the group if it doesn't exist.
	/// @param path Path to the sound file.
	/// @param group The group you want to add the Clip to.
	/// @throws AudioException if the group is loaded or the clip is already in the bank.
	void addClip(const std::string& path, size_t size = 2'048 / 4, int group = 0);

	/// @brief Removes a Clip from the SoundBank.
	/// @param path Path to the sound file.
	/// @throws AudioException if the group is loaded or if the clip doesn't belong to the bank.
	void removeClip(const std::string& path);

	/// @brief Marks or unmarks a group as started at the game's startup.
	/// @param group Group to be marked.
	/// @param isInitial Set it to true if you want to mark the group and false if you want to unmark it.
	void setInitial(int group, bool isInitial);

	/// @brief Adds a group to the bank.
	/// @param group Group to be added.
	/// @param isInitial Set it to true if you want to mark the group as loaded at the game's startup.
	/// @throws AudioException if the group already exists.
	void addGroup(int group, bool isInitial);

	/// @brief Removes a group from the bank.
	/// Moves all its Clips to the default group (id=0).
	/// @param group The group to remove.
	/// @throws AudioException if you try to remove the default group, the group is loaded,
	/// or it doesn't exist.
	void removeGroup(int group);
};
} // namespace arch::audio
