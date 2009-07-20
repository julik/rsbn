// Overrides XPLMGetDirectorySeparator() to return the POSIX / instead of : for paths on OS X
std::string getDirSeparator();

// Get either a POSIX or a Windows path to the datafile having this specific name, relative to the
// plugin installation directory
void detectDatabasePath(char *dataPath, char *datafileName);

// Convert a Mac-style path with double colons to a POSIX path. Google for "FSRef to POSIX path"
// if you want to know the ass-backwards way of doing it properly, but we won't link to Carbon
// only for this mmkay?
std::string carbonPathToPosixPath(const std::string &carbonPath);