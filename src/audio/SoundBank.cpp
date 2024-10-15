#include <audio/SoundBank.h>
#include <audio/AudioException.h>
#include <ranges>

namespace arch::audio{
	void SoundBank::loadGroup(int group){
		auto entryFound = _groups.find(group);
		if(entryFound == _groups.end()){
			throw AudioException("Can't find sound bank group with id " + std::to_string(group));
		}
		if(_isLoaded[entryFound->first]){
			throw AudioException("Can't load sound bank group: " + std::to_string(group) + " - it's already loaded");
		}
		std::set<std::string> paths = entryFound->second;
		for(auto& path : paths){
			auto clipFound = _clips.find(path);
			if(clipFound == _clips.end()){
				throw AudioException("Can't find clip with path " + path);
			}
			clipFound->second.load();
		}
		_isLoaded[entryFound->first] = true;
		Logger::info("Audio system: loaded sound bank group with id {}", std::to_string(group));
	}

	void SoundBank::unloadGroup(int group){
		auto entryFound = _groups.find(group);
		if(entryFound == _groups.end()){
			throw AudioException("Can't find sound bank group with id " + std::to_string(group));
		}
		if(!_isLoaded[group]){
			throw AudioException("Can't unload sound bank group: " + std::to_string(group) + " - it's not loaded");
		}

		std::set<std::string> paths = entryFound->second;
		for(auto& path : paths){
			auto clipFound = _clips.find(path);
			if(clipFound == _clips.end()){
				throw AudioException("Can't find clip with path " + path);
			}
			clipFound->second.unload();
		}
		_isLoaded[entryFound->first] = false;
		Logger::info("Audio system: unloaded sound bank group with id {}", std::to_string(group));
	}

	void SoundBank::loadInitialGroups(){
		for(auto& group : _initialGroups){
			loadGroup(group);
		}
		Logger::info("Audio system: loaded initial sound bank groups");
	}

	void SoundBank::unloadInitialGroups(){
		for(auto& group : _initialGroups){
			unloadGroup(group);
		}
		Logger::info("Audio system: unloaded initial sound bank groups");
	}

	void SoundBank::loadAllUnloadedGroups() {
		for(auto& group : std::views::keys(_groups)){
			if(!_isLoaded[group]) {
				loadGroup(group);
			}
		}
		Logger::info("Audio system: loaded all unloaded sound bank groups");
	}

	void SoundBank::unloadAllLoadedGroups(){
		for(auto&& group : std::views::keys(_groups)){
			if(_isLoaded[group]) {
				unloadGroup(group);
			}
		}
		Logger::info("Audio system: unloaded all loaded sound bank groups");
	}

	Clip& SoundBank::getClip(const std::string& path){
		auto foundEntry = _clips.find(path);
		if(foundEntry == _clips.end()){
			throw AudioException("Clip with path " + path + " not found");
		}
		return foundEntry->second;
	}

	void SoundBank::addClip(const std::string& path, int group){
		auto foundEntry = _clips.find(path);
		if(_isLoaded[group]){
			throw AudioException("Can't modify sound bank group " + std::to_string(group) + " - it's loaded");
		}
		if(foundEntry != _clips.end()){
			throw AudioException("Can't add clip: " + path + " it's already in the bank");
		}
		_clips.try_emplace(path, path);
		_addToGroup(path, group);
		Logger::info("Audio system: added clip {} to sound bank group {}", path, std::to_string(group));
	}

	void SoundBank::removeClip(const std::string& path){
		int group = findClipGroup(path);
		if(_isLoaded[group]){
			throw AudioException("Can't modify sound bank group " + std::to_string(group) + " - it's loaded");
		}
		if(_clips.erase(path) == 0){
			throw AudioException("Can't remove clip: " + path + " it's not in the bank");
		}
		_removeFromGroup(path, group);
		Logger::info("Audio system: removed clip {} from the sound bank", path);
	}

	void SoundBank::setInitial(int group, bool isInitial){
		if(isInitial){
			_initialGroups.emplace(group);
			Logger::info("Audio system: sound bank group {} set as initial", std::to_string(group));
			return;
		}
		_initialGroups.erase(group);
		Logger::info("Audio system: sound bank group {} unset as initial", std::to_string(group));
	}

	void SoundBank::addGroup(int group, bool isInitial){
		auto entryFound = _groups.find(group);
		if(entryFound != _groups.end()){
			throw AudioException("Can't add sound bank group with id " + std::to_string(group) + " - group already exists");
		}
		_groups.try_emplace(group);
		_isLoaded[group] = false;
		if(isInitial){
			_initialGroups.emplace(group);
			Logger::info("Audio system: added sound bank group {} as initial", std::to_string(group));
		}
		else {
			Logger::info("Audio system: added sound bank group {} as not initial", std::to_string(group));
		}
	}

	void SoundBank::removeGroup(int group){
		if(group == 0) {
			throw AudioException("Can't remove the default sound bank group");
		}
		if(_isLoaded[group]){
			throw AudioException("Can't modify sound bank group " + std::to_string(group) + " - it's loaded");
		}
		auto entryFound = _groups.find(group);
		if(entryFound == _groups.end()){
			throw AudioException("Can't find sound bank group with id " + std::to_string(group));
		}
		std::set<std::string> paths = entryFound->second;
		for(auto& path : paths){
			moveClipToGroup(path, 0);
		}
		_isLoaded.erase(group);
		_groups.erase(group);
		Logger::info("Audio system: removed sound bank group {}", std::to_string(group));
	}

	void SoundBank::moveClipToGroup(const std::string& path, int destinationGroup){
		if(_isLoaded[destinationGroup]){
			throw AudioException("Can't modify sound bank group " + std::to_string(destinationGroup) + " - it's loaded");
		}
		int sourceGroup = findClipGroup(path);
		if(_isLoaded[sourceGroup]){
			throw AudioException("Can't modify sound bank group " + std::to_string(sourceGroup) + " - it's loaded");
		}
		_removeFromGroup(path, sourceGroup);
		_addToGroup(path, destinationGroup);
		Logger::info("Audio system: moved clip {} to sound bank group {}", path, std::to_string(destinationGroup));
	}

	void SoundBank::_addToGroup(const std::string& path, int group){
		auto entryFound = _groups.find(group);
		if(entryFound == _groups.end()){
			addGroup(group, true);
			entryFound = _groups.find(group);
		}
		auto pathFound = entryFound->second.find(path);
		if(pathFound != entryFound->second.end()){
			throw AudioException("Can't add clip: " + path + " to group: " + std::to_string(group) + " - it's already in the group");
		}
		entryFound->second.emplace(path);
	}

	void SoundBank::_removeFromGroup(const std::string& path, int group){
		auto entryFound = _groups.find(group);
		if(entryFound == _groups.end()){
			throw AudioException("Can't remove clip: " + path + " from group: " + std::to_string(group) + " - the group doesn't exist");
		}
		if(entryFound->second.erase(path) == 0){
			throw AudioException("Can't remove clip: " + path + " from group: " + std::to_string(group) + " - the clip doesn't belong to the group");
		}
	}

	int SoundBank::findClipGroup(const std::string& path){
		for(auto&& [group, members] : _groups){
			auto entryFound = members.find(path);
			if(entryFound != members.end()){
				return group;
			}
		}
		throw AudioException("Clip: " + path + " doesn't belong to any group (possibly it doesn't exist)");
	}

	SoundBank::SoundBank(){
		addGroup(0, true);
		Logger::info("Audio system: sound bank initialised");
	}

	SoundBank::~SoundBank(){
		unloadAllLoadedGroups();
		Logger::info("Audio system: sound bank closed");
	}

}