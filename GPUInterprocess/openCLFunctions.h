#pragma once
#include <iostream>
#include <string>
#include "CL/opencl.hpp"

inline void levelSpace(const int level)
{
	for (int i = 0; i < level; ++i)
		std::cout << "\t";
}


static constexpr int PlatformSpec = 1;
static constexpr int LevelDevice = 1;
static constexpr int LevelDeviceSpec = LevelDevice + 1;


template<typename Type>
void displayDeviceCLInfo(cl_device_id device, const cl_device_info info, const std::string& title, const std::string& unit = "")
{
	Type value;
	clGetDeviceInfo(device, info, sizeof(Type), &value, nullptr);
	levelSpace(LevelDeviceSpec);
	std::cout << title << " " << value << " " << unit << std::endl;
}

void displayDeviceInfo(cl_device_id device);

void displayPlaformInfo(cl_platform_id platform);

void displayOpenCLInfo();