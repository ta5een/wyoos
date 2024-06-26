#pragma once

#include <kernel/arch/x86/gdt.hpp>
#include <kernel/arch/x86/port.hpp>
#include <lib/integers.hpp>

namespace kernel
{

class InterruptManager;

class InterruptHandler
{
  public:
    virtual u32 handle_interrupt(u32 esp);

  protected:
    InterruptHandler(u8 interrupt_number, InterruptManager *interrupt_manager);
    ~InterruptHandler();

  private:
    u8 m_interrupt_number{0};
    InterruptManager *m_interrupt_manager{nullptr};
};

class InterruptManager
{
    friend class InterruptHandler;

  protected:
    static InterruptManager *active_interrupt_manager;
    InterruptHandler *m_handlers[256]{};

    struct [[gnu::packed]] GateDescriptor
    {
        u16 handler_address_lo;
        u16 gdt_code_segment_selector;
        u8 reserved;
        u8 access;
        u16 handler_address_hi;
    };

    struct [[gnu::packed]] InterruptDescriptorTablePointer
    {
        u16 size;
        u32 base;
    };

    static GateDescriptor interrupt_descriptor_table[256];
    static void set_interrupt_descriptor_table_entry(
        u8 interrupt_number,
        u16 code_segment_selector_offset,
        void (*handler)(),
        u8 descriptor_privilege_level,
        u8 descriptor_type
    );

    Port8BitSlow pic_master_command;
    Port8BitSlow pic_master_data;
    Port8BitSlow pic_slave_command;
    Port8BitSlow pic_slave_data;

  public:
    InterruptManager(GlobalDescriptorTable *gdt);
    ~InterruptManager() = default;

    static u32 handle_interrupt(u8 interrupt_number, u32 esp);
    u32 do_handle_interrupt(u8 interrupt_number, u32 esp);

    static void ignore_interrupt_request();
    static void handle_interrupt_request0x00();
    static void handle_interrupt_request0x01();

    void activate();
    void deactivate();
};

} // namespace kernel
