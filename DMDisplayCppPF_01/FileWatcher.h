#pragma once
#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

enum class FileStatus { created, modified, erased };

//using namespace std;
class FileWatcher
{
private:
	std::unordered_map<std::string, std::experimental::filesystem::file_time_type> paths_;
	bool running_ = true;
	bool Contains(const std::string& key) {
		auto el = paths_.find(key);
		return el != paths_.end();
	}
public:
	std::string pathToWatch;
	std::chrono::duration<int, std::milli> delay;

	FileWatcher(std::string pathToWatch, std::chrono::duration<int, std::milli>delay) :pathToWatch{ pathToWatch }, delay{ delay }{
		for (auto& file : std::experimental::filesystem::recursive_directory_iterator(pathToWatch)) {
			paths_[file.path().string()] = std::experimental::filesystem::last_write_time(file);
		}
	}


	void Start(const std::function<void(std::string, FileStatus)>& action) {
		while (running_)
		{
			std::this_thread::sleep_for(delay);

			//for(auto it = paths_.begin(); it != paths_end(); )
			auto it = paths_.begin();
			while (it != paths_.end()) {
				if (!std::experimental::filesystem::exists(it->first)) {
					action(it->first, FileStatus::erased);
					it = paths_.erase(it);
				}
				else {
					++it;
				}
			}

			for (auto& file : std::experimental::filesystem::recursive_directory_iterator(pathToWatch)) {
				auto currentFileLastWriteTime = std::experimental::filesystem::last_write_time(file);

				//File Created
				if (!Contains(file.path().string())) {
					paths_[file.path().string()] = currentFileLastWriteTime;
					action(file.path().string(), FileStatus::created);
				}

				//File Modified
				else {
					if (paths_[file.path().string()] != currentFileLastWriteTime) {
						paths_[file.path().string()] = currentFileLastWriteTime;
						action(file.path().string(), FileStatus::modified);
					}
				}
			}
		}
	}
};

