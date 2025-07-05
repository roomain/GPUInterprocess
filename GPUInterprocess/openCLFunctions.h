#pragma once
#include <iostream>
#include <string>
#include "CL/opencl.hpp"

template<typename Type>
void displayDeviceCLInfo(cl_device_id device, const cl_device_info info, const std::string& title, const std::string& unit = "")
{
	Type value;
	clGetDeviceInfo(device, info, sizeof(Type), &value, nullptr);
	std::cout << title << " " << value << " " << unit << std::endl;
}

void displayDeviceInfo(cl_device_id device);

void displayPlaformInfo(cl_platform_id platform);

void displayOpenCLInfo();