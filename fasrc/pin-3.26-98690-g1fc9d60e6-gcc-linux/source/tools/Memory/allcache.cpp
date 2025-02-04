#include <iostream>
#include <fstream>
#include <string>
#include "pin.H"

// ----------------------------------------------------------------------
// IMPORTANT: Define CACHE_STATS BEFORE including "pin_cache.H" 
// so that pin_cache.H knows what type to use for its stats.
// ----------------------------------------------------------------------
typedef UINT64 CACHE_STATS; // or UINT32 if you prefer

#include "pin_cache.H"

// ----------------------------------------------------------------------
// Command-Line Knobs
// ----------------------------------------------------------------------

// Output file
KNOB<std::string> KnobOutputFile(
    KNOB_MODE_WRITEONCE, "pintool", "o", "allcache.out",
    "Specify output file name for cache simulation results"
);

// ITLB knobs
KNOB<UINT32> KnobITLBSizeKB(
    KNOB_MODE_WRITEONCE, "pintool", "itlb_size_kb", "128",
    "ITLB size in KB"
);
KNOB<UINT32> KnobITLBLineSize(
    KNOB_MODE_WRITEONCE, "pintool", "itlb_line_size", "4096",
    "ITLB line size (bytes) [page size]"
);
KNOB<UINT32> KnobITLBAssoc(
    KNOB_MODE_WRITEONCE, "pintool", "itlb_assoc", "32",
    "ITLB associativity"
);

// DTLB knobs
KNOB<UINT32> KnobDTLBSizeKB(
    KNOB_MODE_WRITEONCE, "pintool", "dtlb_size_kb", "128",
    "DTLB size in KB"
);
KNOB<UINT32> KnobDTLBLineSize(
    KNOB_MODE_WRITEONCE, "pintool", "dtlb_line_size", "4096",
    "DTLB line size (bytes) [page size]"
);
KNOB<UINT32> KnobDTLBAssoc(
    KNOB_MODE_WRITEONCE, "pintool", "dtlb_assoc", "32",
    "DTLB associativity"
);

// L1 Instruction Cache knobs
KNOB<UINT32> KnobIL1SizeKB(
    KNOB_MODE_WRITEONCE, "pintool", "il1_size_kb", "32",
    "L1 instruction cache size in KB"
);
KNOB<UINT32> KnobIL1LineSize(
    KNOB_MODE_WRITEONCE, "pintool", "il1_line_size", "32",
    "L1 instruction cache line size in bytes"
);
KNOB<UINT32> KnobIL1Assoc(
    KNOB_MODE_WRITEONCE, "pintool", "il1_assoc", "32",
    "L1 instruction cache associativity"
);

// L1 Data Cache knobs
KNOB<UINT32> KnobDL1SizeKB(
    KNOB_MODE_WRITEONCE, "pintool", "dl1_size_kb", "32",
    "L1 data cache size in KB"
);
KNOB<UINT32> KnobDL1LineSize(
    KNOB_MODE_WRITEONCE, "pintool", "dl1_line_size", "32",
    "L1 data cache line size in bytes"
);
KNOB<UINT32> KnobDL1Assoc(
    KNOB_MODE_WRITEONCE, "pintool", "dl1_assoc", "32",
    "L1 data cache associativity"
);

// L2 Unified Cache knobs
KNOB<UINT32> KnobUL2SizeKB(
    KNOB_MODE_WRITEONCE, "pintool", "ul2_size_kb", "2048",
    "L2 unified cache size in KB (default 2MB = 2048KB)"
);
KNOB<UINT32> KnobUL2LineSize(
    KNOB_MODE_WRITEONCE, "pintool", "ul2_line_size", "64",
    "L2 unified cache line size in bytes"
);
KNOB<UINT32> KnobUL2Assoc(
    KNOB_MODE_WRITEONCE, "pintool", "ul2_assoc", "1",
    "L2 unified cache associativity (1 = direct-mapped)"
);

// L3 Unified Cache knobs
KNOB<UINT32> KnobUL3SizeKB(
    KNOB_MODE_WRITEONCE, "pintool", "ul3_size_kb", "16384",
    "L3 unified cache size in KB (default 16MB = 16384KB)"
);
KNOB<UINT32> KnobUL3LineSize(
    KNOB_MODE_WRITEONCE, "pintool", "ul3_line_size", "64",
    "L3 unified cache line size in bytes"
);
KNOB<UINT32> KnobUL3Assoc(
    KNOB_MODE_WRITEONCE, "pintool", "ul3_assoc", "1",
    "L3 unified cache associativity (1 = direct-mapped)"
);

// ----------------------------------------------------------------------
// Cache Declaration
// ----------------------------------------------------------------------

// Example: ITLB 
namespace ITLB
{
    // Provide large-enough compile-time bounds:
    const UINT32 max_sets          = 1024;
    const UINT32 max_associativity = 1024;
    // Typically TLB is store-allocate, but you can change if you want.
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}
static ITLB::CACHE* itlb = nullptr;

// Example: DTLB
namespace DTLB
{
    const UINT32 max_sets          = 1024;
    const UINT32 max_associativity = 1024;
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}
static DTLB::CACHE* dtlb = nullptr;

// Example: IL1
namespace IL1
{
    const UINT32 max_sets          = 1024;
    const UINT32 max_associativity = 1024;
    // Usually instruction caches are "no-allocate" on stores
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_NO_ALLOCATE;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}
static IL1::CACHE* il1 = nullptr;

// Example: DL1
namespace DL1
{
    const UINT32 max_sets          = 1024;
    const UINT32 max_associativity = 1024;
    // We use "no-allocate" for demonstration, though you can choose otherwise
    const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_NO_ALLOCATE;

    typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
}
static DL1::CACHE* dl1 = nullptr;

// For UL2 and UL3, we want to choose DM or RR at runtime -> store in CACHE_BASE*:
namespace UL2
{
    const UINT32 max_sets = 65536;
    // For associativity == 1 => direct mapped
    typedef CACHE_DIRECT_MAPPED(max_sets, CACHE_ALLOC::STORE_ALLOCATE) CACHE_DM;
    // Else => round robin
    typedef CACHE_ROUND_ROBIN(max_sets, 1024, CACHE_ALLOC::STORE_ALLOCATE) CACHE_RR;
}
static CACHE_BASE* ul2 = nullptr; // base pointer

namespace UL3
{
    const UINT32 max_sets = 65536;
    typedef CACHE_DIRECT_MAPPED(max_sets, CACHE_ALLOC::STORE_ALLOCATE) CACHE_DM;
    typedef CACHE_ROUND_ROBIN(max_sets, 1024, CACHE_ALLOC::STORE_ALLOCATE) CACHE_RR;
}
static CACHE_BASE* ul3 = nullptr; // base pointer

// ----------------------------------------------------------------------
// Instrumentation and Access Routines
// ----------------------------------------------------------------------

// Access L2 (and possibly L3)
static VOID Ul2Access(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType)
{
    if (!ul2) return; // safety check
    BOOL hit = ul2->Access(addr, size, accessType);

    if (!hit && ul3)
    {
        ul3->Access(addr, size, accessType);
    }
}

// Instruction fetch path
static VOID InsRef(ADDRINT addr)
{
    const UINT32 size                        = 1; // typically a single-line fetch
    const CACHE_BASE::ACCESS_TYPE accessType = CACHE_BASE::ACCESS_TYPE_LOAD;

    // ITLB
    if (itlb) itlb->AccessSingleLine(addr, accessType);

    // IL1
    if (il1)
    {
        BOOL il1Hit = il1->AccessSingleLine(addr, accessType);
        if (!il1Hit)
        {
            // go to L2 (then L3)
            Ul2Access(addr, size, accessType);
        }
    }
}

// Multi-byte data access
static VOID MemRefMulti(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType)
{
    // DTLB
    if (dtlb) dtlb->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_LOAD);

    // DL1
    if (dl1)
    {
        BOOL dl1Hit = dl1->Access(addr, size, accessType);
        if (!dl1Hit)
        {
            Ul2Access(addr, size, accessType);
        }
    }
}

// Single-line data access
static VOID MemRefSingle(ADDRINT addr, UINT32 size, CACHE_BASE::ACCESS_TYPE accessType)
{
    // DTLB
    if (dtlb) dtlb->AccessSingleLine(addr, CACHE_BASE::ACCESS_TYPE_LOAD);

    // DL1
    if (dl1)
    {
        BOOL dl1Hit = dl1->AccessSingleLine(addr, accessType);
        if (!dl1Hit)
        {
            Ul2Access(addr, size, accessType);
        }
    }
}

// Instrument instructions
static VOID Instruction(INS ins, VOID* v)
{
    // All instruction fetches: InsRef
    INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)InsRef, IARG_INST_PTR, IARG_END);

    // For standard memory ops:
    if (!INS_IsStandardMemop(ins) || INS_MemoryOperandCount(ins) == 0)
        return;

    UINT32 readSize  = 0;
    UINT32 writeSize = 0;

    // We only look at the first read operand
    for (UINT32 opIdx = 0; opIdx < INS_MemoryOperandCount(ins); opIdx++)
    {
        if (INS_MemoryOperandIsRead(ins, opIdx))
        {
            readSize = INS_MemoryOperandSize(ins, opIdx);
            break; // or remove break if you want to handle multiple
        }
    }
    // We only look at the first write operand
    for (UINT32 opIdx = 0; opIdx < INS_MemoryOperandCount(ins); opIdx++)
    {
        if (INS_MemoryOperandIsWritten(ins, opIdx))
        {
            writeSize = INS_MemoryOperandSize(ins, opIdx);
            break; // or remove break if you want to handle multiple
        }
    }

    if (readSize > 0)
    {
        AFUNPTR fn = (readSize <= 4) ? (AFUNPTR)MemRefSingle : (AFUNPTR)MemRefMulti;
        INS_InsertPredicatedCall(ins, IPOINT_BEFORE, fn,
                                 IARG_MEMORYREAD_EA,
                                 IARG_MEMORYREAD_SIZE,
                                 IARG_UINT32, CACHE_BASE::ACCESS_TYPE_LOAD,
                                 IARG_END);
    }

    if (writeSize > 0)
    {
        AFUNPTR fn = (writeSize <= 4) ? (AFUNPTR)MemRefSingle : (AFUNPTR)MemRefMulti;
        INS_InsertPredicatedCall(ins, IPOINT_BEFORE, fn,
                                 IARG_MEMORYWRITE_EA,
                                 IARG_MEMORYWRITE_SIZE,
                                 IARG_UINT32, CACHE_BASE::ACCESS_TYPE_STORE,
                                 IARG_END);
    }
}

// ----------------------------------------------------------------------
// Fini function: print statistics
// ----------------------------------------------------------------------
static VOID Fini(int code, VOID* v)
{
    std::ofstream out(KnobOutputFile.Value().c_str());
    if (!out.is_open())
    {
        std::cerr << "Could not open output file: "
                  << KnobOutputFile.Value() << std::endl;
        return;
    }

    // Print each cache's stats if allocated:
    if (itlb) out << *itlb << std::endl;
    if (dtlb) out << *dtlb << std::endl;
    if (il1)  out << *il1  << std::endl;
    if (dl1)  out << *dl1  << std::endl;
    if (ul2)  out << *ul2  << std::endl; // recall ul2 is a CACHE_BASE*
    if (ul3)  out << *ul3  << std::endl;

    out.close();
}

// ----------------------------------------------------------------------
// Usage
// ----------------------------------------------------------------------
static INT32 Usage()
{
    std::cerr << "This tool models a multi-level (ITLB,DTLB,L1,L2,L3) cache hierarchy.\n"
              << KNOB_BASE::StringKnobSummary() << std::endl;
    return -1;
}

// ----------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------
int main(int argc, char* argv[])
{
    // Initialize Pin
    if (PIN_Init(argc, argv))
    {
        return Usage();
    }

    // ---------------- ITLB config ----------------
    {
        UINT32 szBytes = KnobITLBSizeKB.Value() * 1024;
        UINT32 lnBytes = KnobITLBLineSize.Value();
        UINT32 assoc   = KnobITLBAssoc.Value();
        itlb = new ITLB::CACHE("ITLB", szBytes, lnBytes, assoc);
    }

    // ---------------- DTLB config ----------------
    {
        UINT32 szBytes = KnobDTLBSizeKB.Value() * 1024;
        UINT32 lnBytes = KnobDTLBLineSize.Value();
        UINT32 assoc   = KnobDTLBAssoc.Value();
        dtlb = new DTLB::CACHE("DTLB", szBytes, lnBytes, assoc);
    }

    // ---------------- IL1 config ----------------
    {
        UINT32 szBytes = KnobIL1SizeKB.Value() * 1024;
        UINT32 lnBytes = KnobIL1LineSize.Value();
        UINT32 assoc   = KnobIL1Assoc.Value();
        il1 = new IL1::CACHE("L1 Instruction Cache", szBytes, lnBytes, assoc);
    }

    // ---------------- DL1 config ----------------
    {
        UINT32 szBytes = KnobDL1SizeKB.Value() * 1024;
        UINT32 lnBytes = KnobDL1LineSize.Value();
        UINT32 assoc   = KnobDL1Assoc.Value();
        dl1 = new DL1::CACHE("L1 Data Cache", szBytes, lnBytes, assoc);
    }

    // ---------------- UL2 config ----------------
    {
        UINT32 szBytes = KnobUL2SizeKB.Value() * 1024;
        UINT32 lnBytes = KnobUL2LineSize.Value();
        UINT32 assoc   = KnobUL2Assoc.Value();

        // If associativity == 1 => direct mapped
        if (assoc == 1)
        {
            ul2 = new UL2::CACHE_DM("L2 Unified Cache", szBytes, lnBytes, assoc);
        }
        else
        {
            ul2 = new UL2::CACHE_RR("L2 Unified Cache", szBytes, lnBytes, assoc);
        }
    }

    // ---------------- UL3 config ----------------
    {
        UINT32 szBytes = KnobUL3SizeKB.Value() * 1024;
        UINT32 lnBytes = KnobUL3LineSize.Value();
        UINT32 assoc   = KnobUL3Assoc.Value();

        if (assoc == 1)
        {
            ul3 = new UL3::CACHE_DM("L3 Unified Cache", szBytes, lnBytes, assoc);
        }
        else
        {
            ul3 = new UL3::CACHE_RR("L3 Unified Cache", szBytes, lnBytes, assoc);
        }
    }

    // Instrument instructions
    INS_AddInstrumentFunction(Instruction, 0);

    // Add finalization callback
    PIN_AddFiniFunction(Fini, 0);

    // Start program (never returns)
    PIN_StartProgram();

    return 0;
}
