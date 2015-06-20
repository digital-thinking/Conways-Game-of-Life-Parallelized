#ifndef BOARD_DATA
#define BOARD_DATA

#include <vector>
struct BoardData{
	std::vector<unsigned char> data;
	int width;
	int height;

	int size(){
		return width*height;
	}

	BoardData(){
		width = 0;
		height = 0;
		data = std::vector<unsigned char>(0);
	}
};

#endif // BOARD_DATA

#ifndef OCL_ERROR_STRING
#define OCL_ERROR_STRING
const char* oclErrorString(int error)
{
	static const char* errorString[] = {
		"CL_SUCCESS",
		"CL_DEVICE_NOT_FOUND",
		"CL_DEVICE_NOT_AVAILABLE",
		"CL_COMPILER_NOT_AVAILABLE",
		"CL_MEM_OBJECT_ALLOCATION_FAILURE",
		"CL_OUT_OF_RESOURCES",
		"CL_OUT_OF_HOST_MEMORY",
		"CL_PROFILING_INFO_NOT_AVAILABLE",
		"CL_MEM_COPY_OVERLAP",
		"CL_IMAGE_FORMAT_MISMATCH",
		"CL_IMAGE_FORMAT_NOT_SUPPORTED",
		"CL_BUILD_PROGRAM_FAILURE",
		"CL_MAP_FAILURE",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"CL_INVALID_VALUE",
		"CL_INVALID_DEVICE_TYPE",
		"CL_INVALID_PLATFORM",
		"CL_INVALID_DEVICE",
		"CL_INVALID_CONTEXT",
		"CL_INVALID_QUEUE_PROPERTIES",
		"CL_INVALID_COMMAND_QUEUE",
		"CL_INVALID_HOST_PTR",
		"CL_INVALID_MEM_OBJECT",
		"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
		"CL_INVALID_IMAGE_SIZE",
		"CL_INVALID_SAMPLER",
		"CL_INVALID_BINARY",
		"CL_INVALID_BUILD_OPTIONS",
		"CL_INVALID_PROGRAM",
		"CL_INVALID_PROGRAM_EXECUTABLE",
		"CL_INVALID_KERNEL_NAME",
		"CL_INVALID_KERNEL_DEFINITION",
		"CL_INVALID_KERNEL",
		"CL_INVALID_ARG_INDEX",
		"CL_INVALID_ARG_VALUE",
		"CL_INVALID_ARG_SIZE",
		"CL_INVALID_KERNEL_ARGS",
		"CL_INVALID_WORK_DIMENSION",
		"CL_INVALID_WORK_GROUP_SIZE",
		"CL_INVALID_WORK_ITEM_SIZE",
		"CL_INVALID_GLOBAL_OFFSET",
		"CL_INVALID_EVENT_WAIT_LIST",
		"CL_INVALID_EVENT",
		"CL_INVALID_OPERATION",
		"CL_INVALID_GL_OBJECT",
		"CL_INVALID_BUFFER_SIZE",
		"CL_INVALID_MIP_LEVEL",
		"CL_INVALID_GLOBAL_WORK_SIZE",
	};
	const int errorCount = sizeof(errorString) / sizeof(errorString[0]);

	const int index = -error;

	return (index >= 0 && index < errorCount) ? errorString[index] : "";

}

#endif // 

#ifndef EXT_SUPPORTED
#define EXT_SUPPORTED
#include <string.h>

char *strnstr(const char *haystack, const char *needle, size_t len)
{
	int i;
	size_t needle_len;

	/* segfault here if needle is not NULL terminated */
	if (0 == (needle_len = strlen(needle)))
		return (char *)haystack;

	/* Limit the search if haystack is shorter than 'len' */
	len = strnlen(haystack, len);

	for (i = 0; i<(int)(len - needle_len); i++)
	{
		if ((haystack[0] == needle[0]) &&
			(0 == strncmp(haystack, needle, needle_len)))
			return (char *)haystack;

		haystack++;
	}
	return NULL;
}

int IsExtensionSupported(
	const char* support_str, const char* ext_string, size_t ext_buffer_size)
{
	size_t offset = 0;
	const char* space_substr = strnstr(ext_string + offset, " ", ext_buffer_size - offset);
	size_t space_pos = space_substr ? space_substr - ext_string : 0;
	while (space_pos < ext_buffer_size)
		{
		if (strncmp(support_str, ext_string + offset, space_pos) == 0)
			{
			 // Device supports requested extension
				printf("Info: Found extension support ‘%s’\n", support_str);
			return 1;
			}
		 // Keep searching -- skip to next token string
			offset = space_pos + 1;
		space_substr = strnstr(ext_string + offset, " ", ext_buffer_size - offset);
		space_pos = space_substr ? space_substr - ext_string : 0;
		}
	printf("Warning: Extension not supported ‘%s’!\n", support_str);
	return 0;
}


#endif // EXT_SUPPORTED
