#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>


#define A( s ) #s
#define OPTION(type, var, val) Var<type> var = {A(var), val}

template <typename T = bool>
class Var {
public:
	std::string name;
	std::shared_ptr<T> value;
	int32_t size;
	Var(std::string name, T v) : name(name) {
		value = std::make_shared<T>(v);
		size = sizeof(T);
	}
	operator T() { return *value; }
	operator T* () { return &*value; }
	operator T() const { return *value; }
	//operator T*() const { return value; }
};

class Options
{
public:
	// 
	// ESP
	// 
	

	// 
	// GLOW
	// 
	

	//
	// CHAMS
	//
	

	//
	// MISC
	//
	OPTION(bool, bhop, false);
	OPTION(bool, misc_no_hands, false);
	OPTION(bool, misc_thirdperson, false);
	OPTION(bool, misc_showranks, true);
	OPTION(bool, misc_watermark, true);
	OPTION(float, misc_thirdperson_dist, 50.f);
	OPTION(int, viewmodel_fov, 68);
	OPTION(float, mat_ambient_light_r, 0.0f);
	OPTION(float, mat_ambient_light_g, 0.0f);
	OPTION(float, mat_ambient_light_b, 0.0f);

	// 
	// COLORS
	// 
	
};

inline Options g_Options;
inline bool   g_Unload;