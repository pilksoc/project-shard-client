#pragma once
#ifndef __ENGINETYPES_H__
#define __ENGINETYPES_H__

#include <functional>
#include <memory>
#include <map>

typedef std::function<void()> KeyCallback;
typedef std::function<bool()> HButtonCallback;

#endif // #ifndef __ENGINETYPES_H__