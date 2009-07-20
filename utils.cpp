#include "libs.h"
using std::string;

// Overrides XPLMGetDirectorySeparator() to return the POSIX / instead of : for paths on OS X
std::string getDirSeparator()
{
    string sep = XPLMGetDirectorySeparator();
    return (sep == string(":") ?  string("/") : sep);
}

/// Convert a Mac-style path with double colons to a POSIX path. Google for "FSRef to POSIX path"
// if you want to know the ass-backwards way of doing it properly, but we won't link to Carbon
// only for this mmkay?
std::string carbonPathToPosixPath(const std::string &carbonPath)
{
    // Check if we are on a Mac first, could be also #ifdef APL
    std::string sep = XPLMGetDirectorySeparator();
    if(sep != std::string(":")) return carbonPath;
    
    // Prepend the "Volumes" superdir to the path
    std::string posixPath = std::string("/Volumes/") + carbonPath;
    // Replace dots with slashes
    for(unsigned int i = 0; i < posixPath.length(); i++) {
        if(posixPath[i] == ':') posixPath[i] = '/';
    }
    return posixPath;
}

// Get either a POSIX or a Windows path to the datafile having this specific name, relative to the
// plugin installation directory
void detectDatabasePath(char *dataPath, char *datafileName)
{
    char myPath[1024];
    XPLMGetPluginInfo(XPLMGetMyID(), NULL, myPath, NULL, NULL);
    XPLMExtractFileAndPath(myPath);
    
    string normalizedPath = carbonPathToPosixPath(string(myPath));
    string datafilePathS = normalizedPath + getDirSeparator() + "data" + getDirSeparator() + datafileName;
    strcpy(dataPath, datafilePathS.c_str());
}