#pragma once

#include <lib/integers.hpp>

namespace kernel
{

class GlobalDescriptorTable
{
  public:
    /**
     * An entry in a Global Descriptor Table.
     */
    class [[gnu::packed]] SegmentDescriptor
    {
      public:
        enum EmptyTag
        {
            Empty
        };
        enum WithOptionsTag
        {
            WithOptions
        };

        struct Options
        {
          public:
            u32 base{0};
            u32 limit{0};
            u8 access_byte{0};
        };

        /**
         * Construct an empty SegmentDescriptor.
         */
        explicit SegmentDescriptor(EmptyTag /*unused*/);

        /**
         * Construct a SegmentDescriptor with a targeted base (offset), a
         * targeted limit (size), and provided access byte flags.
         */
        explicit SegmentDescriptor(WithOptionsTag, Options options);

        /**
         * The decoded linear address where this segment begins.
         */
        u32 base() const;

        /**
         * The decoded limit of this segment, respecting the page granularity
         * such that it value ranges from 1 byte to 4 GiB.
         */
        u32 limit() const;

      private:
        SegmentDescriptor();

        u16 m_limit_0_15{0};
        u16 m_base_0_15{0};
        u8 m_base_16_23{0};
        u8 m_access_byte{0};
        u8 m_flags_limit_16_19{0};
        u8 m_base_24_31{0};
    };

    /**
     * Construct a GlobalDescriptorTable with predefined segments.
     */
    GlobalDescriptorTable();

    /**
     * TODO: No-op.
     */
    ~GlobalDescriptorTable() = default;

    /**
     * Load the GlobalDescriptorTable by calling the LGDT instruction.
     */
    void load();

    /**
     * The raw value of the kernel code segment selector.
     */
    u16 code_segment_selector() const
    {
        return (u8 *)&m_code_segment_selector - (u8 *)this;
    }

    /**
     * The raw value of the kernel data segment selector.
     */
    u16 data_segment_selector() const
    {
        return (u8 *)&m_data_segment_selector - (u8 *)this;
    }

    // TODO: I'm not sure what the purpose of "m_unused_segment_selector" is
    // for. Most online sources follow the kernel segments directly after the
    // null segment.
  private:
    SegmentDescriptor m_null_segment_selector;
    SegmentDescriptor m_unused_segment_selector;
    SegmentDescriptor m_code_segment_selector;
    SegmentDescriptor m_data_segment_selector;
};

} // namespace kernel
