#ifndef SYSTEM_TARGET_HPP
#define SYSTEM_TARGET_HPP

#if defined(_WIN32) || defined(__WIN32__) || defined(__MINGW32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define WINDOWS
#endif

#if defined(__linux__)
#define LINUX
#endif

#endif // !SYSTEM_TARGET_HPP
