#include "openCLFunctions.h"
#include "enumerate.h"

void displayDeviceType(cl_device_id device)
{
    cl_device_type type;
    clGetDeviceInfo(device, CL_DEVICE_TYPE, sizeof(cl_device_type), &type, nullptr);
    std::cout << "Device type ";// << data.data() << std::endl;
    switch (type)
    {
    case CL_DEVICE_TYPE_CPU:
        std::cout << "CPU" << std::endl;
        break;

    case CL_DEVICE_TYPE_GPU:
        std::cout << "GPU" << std::endl;
        break;

    case CL_DEVICE_TYPE_ACCELERATOR:
        std::cout << "Accelerator" << std::endl;
        break;

    default:
        std::cout << "Unknown" << std::endl;
        break;
    }
}

void displayMemorySize(cl_device_id device, const cl_device_info info, const std::string& title)
{
    cl_ulong allocSize = 0;
    clGetDeviceInfo(device, info, sizeof(cl_ulong), &allocSize, nullptr);


    constexpr size_t kilo = 1024;
    constexpr size_t mega = 1024 * kilo;
    constexpr size_t giga = 1024 * mega;

    if (allocSize > giga)
    {
        std::cout << title << " " << static_cast<double>(allocSize) / static_cast<double>(giga) << " Gb" << std::endl;
    }
    else if (allocSize > mega)
    {
        std::cout << title << " " << static_cast<double>(allocSize) / static_cast<double>(mega) << " Mb" << std::endl;
    }
    else if (allocSize > kilo)
    {
        std::cout << title << " " << static_cast<double>(allocSize) / static_cast<double>(kilo) << " Kb" << std::endl;
    }
    else
    {
        std::cout << title << " " << allocSize << " bytes" << std::endl;
    }
}

void displayDeviceInfo(cl_device_id device)
{
    displayDeviceType(device);
    displayDeviceCLInfo<cl_uint>(device, CL_DEVICE_MAX_CLOCK_FREQUENCY, "Max clock frequency", "MHz");
    displayDeviceCLInfo<cl_uint>(device, CL_DEVICE_MAX_COMPUTE_UNITS, "Max compute units");
    displayDeviceCLInfo<cl_uint>(device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, "Max work item dim");
    displayDeviceCLInfo<cl_uint>(device, CL_DEVICE_MAX_WORK_GROUP_SIZE, "Max workgroup size");
    displayDeviceCLInfo<cl_uint>(device, CL_DEVICE_MAX_WORK_ITEM_SIZES, "Max workitem size");
    displayMemorySize(device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, "Max memory allocation size");
    displayMemorySize(device, CL_DEVICE_LOCAL_MEM_SIZE, "Device local memory size");
}

void displayPlaformInfo(cl_platform_id platform)
{
    static const char* attributeNames[5] = { "Name", "Vendor", "Version", "Profile", "Extensions" };
    static const cl_platform_info attributeTypes[5] = {
        CL_PLATFORM_NAME,
        CL_PLATFORM_VENDOR,
        CL_PLATFORM_VERSION,
        CL_PLATFORM_PROFILE,
        CL_PLATFORM_EXTENSIONS };

    static const cl_device_type tableOfDevType[] = {
        CL_DEVICE_TYPE_CPU,
        CL_DEVICE_TYPE_GPU,
        CL_DEVICE_TYPE_ACCELERATOR
    };

    int attibuteIndex = 0;
    size_t infoSize = 0;
    std::vector<char> data;
    for (const auto attrib : attributeTypes)
    {
        // get platform attribute value size
        clGetPlatformInfo(platform, attrib, 0, nullptr, &infoSize);
        data.reserve(infoSize);

        // get platform attribute value
        clGetPlatformInfo(platform, attrib, infoSize, data.data(), nullptr);
        std::cout << attributeNames[attibuteIndex++] << " " << data.data() << std::endl;
    }

    for (cl_device_type type : tableOfDevType)
    {
        cl_uint numDevices;
        clGetDeviceIDs(platform, type, 0, nullptr, &numDevices);

        if (numDevices == 1)
        {
            cl_device_id device;
            clGetDeviceIDs(platform, type, numDevices, &device, &numDevices);
            displayDeviceInfo(device);
        }
        else if (numDevices > 1)
        {
            std::vector< cl_device_id> devices;
            devices.reserve(numDevices);
            clGetDeviceIDs(platform, type, numDevices, devices.data(), &numDevices);
            for(auto device : devices)
                displayDeviceInfo(device);
        }
    }
}

void displayOpenCLInfo()
{
    std::vector<cl_platform_id> platformList;
    enumerate(clGetPlatformIDs, platformList);
    for (cl_platform_id platform : platformList)
        displayPlaformInfo(platform);
}