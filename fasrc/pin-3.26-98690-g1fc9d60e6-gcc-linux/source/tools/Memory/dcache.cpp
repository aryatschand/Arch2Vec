/*
 * Copyright (C) 2004-2021 Intel Corporation.
 * SPDX-License-Identifier: MIT
 */

/*! @file
 *  This file contains an ISA-portable cache simulator
 *  data cache hierarchies, modified to map all memory addresses
 *  to a fixed address range, making cache simulations deterministic.
 */

#include "pin.H"

#include <iostream>
#include <fstream>
#include <cassert>
#include <map>

#include "cache.H"
#include "pin_profile.H"

using std::cerr;
using std::endl;

/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */

KNOB< std::string > KnobOutputFile(KNOB_MODE_WRITEONCE, "pintool", "o", "dcache.out", "specify dcache file name");
KNOB< BOOL > KnobTrackLoads(KNOB_MODE_WRITEONCE, "pintool", "tl", "0", "track individual loads -- increases profiling time");
KNOB< BOOL > KnobTrackStores(KNOB_MODE_WRITEONCE, "pintool", "ts", "0", "track individual stores -- increases profiling time");
KNOB< UINT32 > KnobThresholdHit(KNOB_MODE_WRITEONCE, "pintool", "rh", "100", "only report memops with hit count above threshold");
KNOB< UINT32 > KnobThresholdMiss(KNOB_MODE_WRITEONCE, "pintool", "rm", "100",
                                 "only report memops with miss count above threshold");
KNOB< UINT32 > KnobCacheSize(KNOB_MODE_WRITEONCE, "pintool", "c", "32", "cache size in kilobytes");
KNOB< UINT32 > KnobLineSize(KNOB_MODE_WRITEONCE, "pintool", "b", "32", "cache block size in bytes");
KNOB< UINT32 > KnobAssociativity(KNOB_MODE_WRITEONCE, "pintool", "a", "4", "cache associativity (1 for direct mapped)");

/* ===================================================================== */
INT32 Usage()
{
    cerr << "This tool represents a cache simulator.\n"
         << "\n";

    cerr << KNOB_BASE::StringKnobSummary();
    cerr << endl;

    return -1;
}

/* ===================================================================== */
/* Global Variables */
/* ===================================================================== */

// wrap configuation constants into their own name space to avoid name clashes
namespace DL1
{
const UINT32 max_sets                          = KILO; // cacheSize / (lineSize * associativity);
const UINT32 max_associativity                 = 256;  // associativity;
const CACHE_ALLOC::STORE_ALLOCATION allocation = CACHE_ALLOC::STORE_ALLOCATE;

typedef CACHE_ROUND_ROBIN(max_sets, max_associativity, allocation) CACHE;
} // namespace DL1

DL1::CACHE* dl1 = NULL;

typedef enum
{
    COUNTER_MISS = 0,
    COUNTER_HIT  = 1,
    COUNTER_NUM
} COUNTER;

typedef COUNTER_ARRAY< UINT64, COUNTER_NUM > COUNTER_HIT_MISS;

// holds the counters with misses and hits
// conceptually this is an array indexed by instruction address
COMPRESSOR_COUNTER< ADDRINT, UINT32, COUNTER_HIT_MISS > profile;

/* ===================================================================== */
/* Address Remapping Globals and Function */
/* ===================================================================== */

/*!
 * We map real addresses to a "fixed" range, so that the cache sees
 * deterministic addresses across runs. We start at 0x10000000 and
 * increment by 64 bytes (for example) each time we see a new address.
 */
static std::map<ADDRINT, ADDRINT> addrMap;
static ADDRINT baseAddr = 0x10000000; // start of our artificial address space

/*!
 * Return a fixed address for any real address.
 * If we've seen the real address before, return the mapped address.
 * Otherwise, assign a new address from [baseAddr, baseAddr+64, ...].
 */
ADDRINT GetFixedAddress(ADDRINT realAddr)
{
    // Check if realAddr is already mapped
    std::map<ADDRINT, ADDRINT>::iterator it = addrMap.find(realAddr);
    if (it != addrMap.end())
    {
        return it->second;
    }
    else
    {
        ADDRINT assigned = baseAddr;
        addrMap[realAddr] = assigned;

        // Increment by a stride (could use line size or something else)
        baseAddr += 64;
        return assigned;
    }
}

/* ===================================================================== */
/* Instrumentation Functions (Modified to use GetFixedAddress) */
/* ===================================================================== */

// For multi-byte loads
VOID LoadMulti(ADDRINT addr, UINT32 size, UINT32 instId)
{
    ADDRINT fixedAddr = GetFixedAddress(addr);

    // first level D-cache with fixedAddr
    const BOOL dl1Hit = dl1->Access(fixedAddr, size, CACHE_BASE::ACCESS_TYPE_LOAD);

    const COUNTER counter = dl1Hit ? COUNTER_HIT : COUNTER_MISS;
    profile[instId][counter]++;
}

// For multi-byte stores
VOID StoreMulti(ADDRINT addr, UINT32 size, UINT32 instId)
{
    ADDRINT fixedAddr = GetFixedAddress(addr);

    const BOOL dl1Hit = dl1->Access(fixedAddr, size, CACHE_BASE::ACCESS_TYPE_STORE);

    const COUNTER counter = dl1Hit ? COUNTER_HIT : COUNTER_MISS;
    profile[instId][counter]++;
}

// For single-word loads
VOID LoadSingle(ADDRINT addr, UINT32 instId)
{
    ADDRINT fixedAddr = GetFixedAddress(addr);

    const BOOL dl1Hit = dl1->AccessSingleLine(fixedAddr, CACHE_BASE::ACCESS_TYPE_LOAD);

    const COUNTER counter = dl1Hit ? COUNTER_HIT : COUNTER_MISS;
    profile[instId][counter]++;
}

// For single-word stores
VOID StoreSingle(ADDRINT addr, UINT32 instId)
{
    ADDRINT fixedAddr = GetFixedAddress(addr);

    const BOOL dl1Hit = dl1->AccessSingleLine(fixedAddr, CACHE_BASE::ACCESS_TYPE_STORE);

    const COUNTER counter = dl1Hit ? COUNTER_HIT : COUNTER_MISS;
    profile[instId][counter]++;
}

// For multi-byte loads (fast)
VOID LoadMultiFast(ADDRINT addr, UINT32 size)
{
    ADDRINT fixedAddr = GetFixedAddress(addr);
    dl1->Access(fixedAddr, size, CACHE_BASE::ACCESS_TYPE_LOAD);
}

// For multi-byte stores (fast)
VOID StoreMultiFast(ADDRINT addr, UINT32 size)
{
    ADDRINT fixedAddr = GetFixedAddress(addr);
    dl1->Access(fixedAddr, size, CACHE_BASE::ACCESS_TYPE_STORE);
}

// For single-word loads (fast)
VOID LoadSingleFast(ADDRINT addr)
{
    ADDRINT fixedAddr = GetFixedAddress(addr);
    dl1->AccessSingleLine(fixedAddr, CACHE_BASE::ACCESS_TYPE_LOAD);
}

// For single-word stores (fast)
VOID StoreSingleFast(ADDRINT addr)
{
    ADDRINT fixedAddr = GetFixedAddress(addr);
    dl1->AccessSingleLine(fixedAddr, CACHE_BASE::ACCESS_TYPE_STORE);
}

/* ===================================================================== */
/* Instruction Callback */
/* ===================================================================== */

VOID Instruction(INS ins, void* v)
{
    UINT32 memOperands = INS_MemoryOperandCount(ins);

    // Instrument each memory operand. If the operand is both read and written
    // it will be processed twice.
    for (UINT32 memOp = 0; memOp < memOperands; memOp++)
    {
        const UINT32 size = INS_MemoryOperandSize(ins, memOp);
        const BOOL single = (size <= 4);

        if (INS_MemoryOperandIsRead(ins, memOp))
        {
            if (KnobTrackLoads)
            {
                // map sparse INS addresses to dense IDs
                const ADDRINT iaddr = INS_Address(ins);
                const UINT32 instId = profile.Map(iaddr);

                if (single)
                {
                    INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
                                             (AFUNPTR)LoadSingle,
                                             IARG_MEMORYOP_EA, memOp,
                                             IARG_UINT32, instId,
                                             IARG_END);
                }
                else
                {
                    INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
                                             (AFUNPTR)LoadMulti,
                                             IARG_MEMORYOP_EA, memOp,
                                             IARG_UINT32, size,
                                             IARG_UINT32, instId,
                                             IARG_END);
                }
            }
            else
            {
                if (single)
                {
                    INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
                                             (AFUNPTR)LoadSingleFast,
                                             IARG_MEMORYOP_EA, memOp,
                                             IARG_END);
                }
                else
                {
                    INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
                                             (AFUNPTR)LoadMultiFast,
                                             IARG_MEMORYOP_EA, memOp,
                                             IARG_UINT32, size,
                                             IARG_END);
                }
            }
        }

        if (INS_MemoryOperandIsWritten(ins, memOp))
        {
            if (KnobTrackStores)
            {
                const ADDRINT iaddr = INS_Address(ins);
                const UINT32 instId = profile.Map(iaddr);

                if (single)
                {
                    INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
                                             (AFUNPTR)StoreSingle,
                                             IARG_MEMORYOP_EA, memOp,
                                             IARG_UINT32, instId,
                                             IARG_END);
                }
                else
                {
                    INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
                                             (AFUNPTR)StoreMulti,
                                             IARG_MEMORYOP_EA, memOp,
                                             IARG_UINT32, size,
                                             IARG_UINT32, instId,
                                             IARG_END);
                }
            }
            else
            {
                if (single)
                {
                    INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
                                             (AFUNPTR)StoreSingleFast,
                                             IARG_MEMORYOP_EA, memOp,
                                             IARG_END);
                }
                else
                {
                    INS_InsertPredicatedCall(ins, IPOINT_BEFORE,
                                             (AFUNPTR)StoreMultiFast,
                                             IARG_MEMORYOP_EA, memOp,
                                             IARG_UINT32, size,
                                             IARG_END);
                }
            }
        }
    }
}

/* ===================================================================== */
/* Fini Callback */
/* ===================================================================== */

VOID Fini(int code, VOID* v)
{
    std::ofstream out(KnobOutputFile.Value().c_str());

    // print D-cache profile
    out << "PIN:MEMLATENCIES 1.0. 0x0\n";

    out << "#\n"
           "# DCACHE stats\n"
           "#\n";

    out << dl1->StatsLong("# ", CACHE_BASE::CACHE_TYPE_DCACHE);

    if (KnobTrackLoads || KnobTrackStores)
    {
        out << "#\n"
               "# LOAD stats\n"
               "#\n";
        out << profile.StringLong();
    }
    out.close();
}

/* ===================================================================== */
/* Main */
/* ===================================================================== */

int main(int argc, char* argv[])
{
    PIN_InitSymbols();

    if (PIN_Init(argc, argv))
    {
        return Usage();
    }

    // Create the DL1 cache
    dl1 = new DL1::CACHE("L1 Data Cache",
                         KnobCacheSize.Value() * KILO,
                         KnobLineSize.Value(),
                         KnobAssociativity.Value());

    profile.SetKeyName("iaddr          ");
    profile.SetCounterName("dcache:miss        dcache:hit");

    // Set threshold for reporting
    COUNTER_HIT_MISS threshold;
    threshold[COUNTER_HIT]  = KnobThresholdHit.Value();
    threshold[COUNTER_MISS] = KnobThresholdMiss.Value();
    profile.SetThreshold(threshold);

    // Add instrumentation + finalization
    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns
    PIN_StartProgram();

    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
