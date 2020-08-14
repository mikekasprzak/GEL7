#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <GEL.h>

#include <vector>
#include <optional>
#include <set>
#include <string>

#define WINDOW_TITLE "Vulkan Test"

class cSystemState {
public:

	/// ***********************************************************************************
	void Init() {
		SDLInit();
		SDLCreateWindow();
		VulkanCreateInstance();
		VulkanPickPhysicalDevice();
		VulkanCreateDevice();


	}

	/// ***********************************************************************************
	void SDLInit() {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
		Log("SDL Initalized");
	}

	/// ***********************************************************************************
	void SDLCreateWindow() {
		// Might be unnecessary because I think SDL_WINDOW_VULKAN may call this
		#ifdef USE_DYNAMIC_VULKAN_LIBRARY
		if (SDL_Vulkan_LoadLibrary(nullptr)) {
			Error();
		}
		Log("Dynamic Vulkan Library Loaded");
		#endif // USE_DYNAMIC_VULKAN_LIBRARY

		int createFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN;
		int windowWidth = 1280;
		int windowHeight = 720;
		window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, createFlags);
		
		Log("SDL Window created: %ix%i", windowWidth, windowHeight);
	}

	/// ***********************************************************************************
	void VulkanCreateInstance() {
		// SDL requires some Vulkan extenions to work 

		// Figure out how many extensions SDL needs
		unsigned int sdlExtensionCount;
		if (!SDL_Vulkan_GetInstanceExtensions(window /*window arg is deprecated*/, &sdlExtensionCount, nullptr)) {
			Error("Failed to count Vulkan extensions that SDL requires");
		}

		// Put any extensions we require here
		std::vector<const char*> extensions = {
			VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
			//VK_EXT_DEBUG_REPORT_EXTENSION_NAME, // Sample additional extension
		};
		// Create additional space to hold the extensions SDL requires
		size_t additional_extension_count = extensions.size();
		extensions.resize(additional_extension_count + sdlExtensionCount);

		// Store the extensions that SDL requires on the end
		if (!SDL_Vulkan_GetInstanceExtensions(window /*window arg is deprecated*/, &sdlExtensionCount, extensions.data() + additional_extension_count)) {
			Error("Failed to get Vulkan extensions that SDL requires");
		}
		
		Log("Vulkan extensions required: %i (required by SDL: %i)", extensions.size(), sdlExtensionCount);

		// TODO: enumerate any optional extensions here
		// vkEnumerateInstanceExtensionProperties

		// Check if validation layers are available
		#ifdef USE_VULKAN_VALIDATION
		Log("Vulkan searching for Validation layers");

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					Log("Vulkan validation layer %s found", layerName);
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				Error();
			}
		}
		#else
		const std::vector<const char*> validationLayers = {};
		#endif // USE_VULKAN_VALIDATION
		
		VkApplicationInfo applicationInfo = {};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pNext = nullptr;
		applicationInfo.pApplicationName = WINDOW_TITLE;
		applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
		applicationInfo.pEngineName = "GEL";
		applicationInfo.engineVersion = VK_MAKE_VERSION(7, 0, 0);
		applicationInfo.applicationVersion = VK_API_VERSION_1_0;
		
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pNext = nullptr;
		createInfo.pApplicationInfo = &applicationInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		#ifdef USE_VULKAN_VALIDATION
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    	createInfo.ppEnabledLayerNames = validationLayers.data();
		#endif // USE_VULKAN_VALIDATION

		// Create Instance
		if (vkCreateInstance(&createInfo, nullptr, &instance)) {
			Error("Failed to create Vulkan instance");
		}
		Log("Vulkan instance created");

		VkDebugUtilsMessengerCreateInfoEXT utilsCreateInfo = {};
		utilsCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		utilsCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		utilsCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		utilsCreateInfo.pfnUserCallback = debugCallback;
		utilsCreateInfo.pUserData = nullptr; // Optional

		if (CreateDebugUtilsMessengerEXT(instance, &utilsCreateInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			Error("Failed to create debug messenger");
		}

		// Create Surface
		if (SDL_Vulkan_CreateSurface(window, instance, &surface)) {
			Error("Failed to create surface");
		}
		Log("Vulkan surface created");

		// NOTE: The drawable dimensions MIGHT not be the same as the window dimensions. Use these
		// dimensions for pipelines and framebuffers
		SDL_Vulkan_GetDrawableSize(window, &surfaceWidth, &surfaceHeight);
		Log("Vulkan surface drawable area: %ix%i", surfaceWidth, surfaceHeight);
	}

	
	/// ***********************************************************************************
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		Log("VK: %s", pCallbackData->pMessage);

		return VK_FALSE;
	}


	/// ***********************************************************************************
	void VulkanPickPhysicalDevice()
	{
		physicalDevice = VK_NULL_HANDLE;

		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0) {
			Error("No devices found");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		// print all devices
		for (const auto& device : devices) {
			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			Log("Vulkan physical device: %s [%i: %s]", deviceProperties.deviceName, deviceProperties.deviceType, (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) ? "discrete" : "other");
		}
		
		// select a device
		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				physicalDevice = device;

				VkPhysicalDeviceProperties deviceProperties;
				vkGetPhysicalDeviceProperties(device, &deviceProperties);

				Log("Vulkan discreet physical device selected: %s", deviceProperties.deviceName);

				break;
			}
		}

		// if no discreet gpu found, use the first (default)
		if (physicalDevice == VK_NULL_HANDLE) {
			physicalDevice = devices[0];

			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(devices[0], &deviceProperties);

			Log("Vulkan default physical device selected: %s", deviceProperties.deviceName);
		}


		// Now that we have a physical device, we need to check if it supports the queues we want
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			Log("Vulkan physical device queue family [%i] Count: %i, Flags: 0x%x", i, queueFamily.queueCount, queueFamily.queueFlags);

			VkBool32 surfaceSupport = true;

			// THIS DIDN'T WORK! CRASH!
			//VkBool32 surfaceSupport = false;
			//vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &surfaceSupport);

			if (surfaceSupport && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				physicalDeviceQueueIndex = i;
			}

			i++;
		}

		Log("Vulkan physical device queue selected: %i", physicalDeviceQueueIndex);
		Log("Vulkan physical device supports swapchain: %s", isDeviceSuitable(physicalDevice) ? "yes" : "no");
	}


	/// ***********************************************************************************
	bool isDeviceSuitable(VkPhysicalDevice device) {
		VkPhysicalDeviceProperties deviceProperties;
		//VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		//vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		// TODO: check if it supports the queue famalies we want

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) && extensionsSupported;
	}


	/// ***********************************************************************************
	void VulkanCreateDevice() 
	{
		VkDeviceQueueCreateInfo queueCreateInfo = {};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.pNext = nullptr;
		queueCreateInfo.queueFamilyIndex = physicalDeviceQueueIndex;
		queueCreateInfo.queueCount = 1;

		float queuePriority = 1.0f;
		queueCreateInfo.pQueuePriorities = &queuePriority;

		VkPhysicalDeviceFeatures deviceFeatures = {};

		const std::vector<const char*> extensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};	

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.pQueueCreateInfos = &queueCreateInfo;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		#ifdef USE_VULKAN_VALIDATION
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		#else // USE_VULKAN_VALIDATION
		createInfo.enabledLayerCount = 0;
		#endif // USE_VULKAN_VALIDATION

		Log("Creating logical device...");
		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
			Error("Failed to create device");
		}
		Log("Vulcan logical device created");

		// fetch the graphics queue at index 0 (we only asked for 1)
		vkGetDeviceQueue(device, physicalDeviceQueueIndex, 0, &graphicsQueue);
		// Since we're requiring that the device queue be the same, this ends up the same.
		// NOTE: this is probably better performance, but a bit inelegant
		vkGetDeviceQueue(device, physicalDeviceQueueIndex, 0, &presentQueue);

		
	


	}

	/// ***********************************************************************************
	bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}


	/// ***********************************************************************************
	void Exit() {
		vkDestroyDevice(device, nullptr);
		vkDestroySurfaceKHR(instance, surface, nullptr);
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		//vkDestroyInstance(instance, nullptr); // SDL cleans this up for us? Commenting this out fixes segfalt

		SDL_DestroyWindow(window);

		#ifdef USE_DYNAMIC_VULKAN_LIBRARY
		SDL_Vulkan_UnloadLibrary();
		#endif // USE_DYNAMIC_VULKAN_LIBRARY

		SDL_Quit();
		
		Log("Done");
	}

	/// ***********************************************************************************
	void Draw() {


	}


	/// ***********************************************************************************
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		} else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}


	/// ***********************************************************************************
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

public:
	SDL_Window* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;	// Manages the debug callback

	VkSurfaceKHR surface;
	int surfaceWidth;
	int surfaceHeight;

	VkPhysicalDevice physicalDevice;
	int physicalDeviceQueueIndex;

	VkDevice device;	// Logical device

	VkQueue graphicsQueue;
	VkQueue presentQueue;
};


int main(int argc, char *argv[])
{
	cSystemState systemState;
	systemState.Init();

	bool running = true;
	while (running) {
		SDL_Event evt;
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) {
				running = 0;
				Log("Quitting...");
			}
		}

		systemState.Draw();
	}

	systemState.Exit();

	return 0; 
}
