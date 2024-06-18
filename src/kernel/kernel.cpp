#include <drivers/vga.hpp>
#include <kernel/arch/platform.hpp>
#include <lib/integers.hpp>

using Multiboot = void *;
using CtorFunc = void (*)();

extern "C" CtorFunc start_ctors[];
extern "C" CtorFunc end_ctors[];

drivers::VgaWriter writer;

/**
 * Required run-time function for driving C++ constructors.
 */
extern "C" void call_ctors()
{
    writer.write_string("CALL_CTORS\n");
    for (CtorFunc *ctor = start_ctors; ctor != end_ctors; ctor++)
    {
        writer.write_string("CALL_CTORS_LOOP\n");
        (*ctor)();
    }
}

// TODO: Not sure if this function can replace `call_ctors`
// [[gnu::constructor]] void global_ctors()
// {
//     writer.write_string("GLOBAL_CTORS\n");
// }

/**
 * Entry point of the kernel.
 */
extern "C" void _kmain(Multiboot /* multiboot */, u32 /* magic */)
{
    const char *arch_msg;
    if (ARCH(I386))
    {
        arch_msg = "Architecture: i386";
    }
    else
    {
        arch_msg = "Architecture: unknown";
    }

    writer.write_string("Welcome to WYOOS\n");
    writer.write_string(arch_msg);

    // Infinite loop
    while (true) {}
}
