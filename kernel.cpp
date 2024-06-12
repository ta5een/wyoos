using U8 = unsigned char;
using U16 = unsigned short;
using U32 = unsigned int;
using U64 = unsigned long long;

using S8 = signed char;
using S16 = signed short;
using S32 = signed int;
using S64 = signed long long;

using Multiboot = void *;
using Constructor = void (*)();

enum class VgaColor : U8
{
	Black = 0x0,
	Blue = 0x1,
	Green = 0x2,
	Cyan = 0x3,
	Red = 0x4,
	Magenta = 0x5,
	Brown = 0x6,
	LightGrey = 0x7,
	DarkGrey = 0x8,
	LightBlue = 0x9,
	LightGreen = 0xA,
	LightCyan = 0xB,
	LightRed = 0xC,
	LightMegenta = 0xD,
	LightBrown = 0xE,
	White = 0xF,
};

U16 *const VGA_MEMORY = (U16 *)0xB8000;
const U8 VGA_SCREEN_COLOR = (U8)VgaColor::White | ((U8)VgaColor::Black << 4);

extern "C" Constructor start_ctors;
extern "C" Constructor end_ctors;

/**
 * Output a white-on-black character to the VGA memory buffer.
 */
void vga_write_to_buffer(const char *str)
{
	for (U8 i = 0; str[i] != '\0'; i++)
	{
		// Set the highest bits to the default foreground and background
		VGA_MEMORY[i] = (U16)str[i] | (U16)(VGA_SCREEN_COLOR) << 8;
	}
}

/**
 * Required run-time function for driving C++ constructors.
 */
extern "C" void call_ctors()
{
	for (Constructor *ctor = &start_ctors; ctor != &end_ctors; ctor++)
	{
		(*ctor)();
	}
}

/**
 * Entry point of the kernel.
 */
extern "C" void kmain(Multiboot multiboot, U32 magic)
{
	vga_write_to_buffer("Hello, world!");
	while (true)
	{
		// Infinite loop
	}
}
