// ==========================================================================
//                      Yara - Yet Another Read Aligner
// ==========================================================================
// Copyright (c) 2011-2014, Enrico Siragusa, FU Berlin
// Copyright (c) 2013 NVIDIA Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Enrico Siragusa or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ENRICO SIRAGUSA OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Enrico Siragusa <enrico.siragusa@fu-berlin.de>
// ==========================================================================
// This file contains type overloadings.
// ==========================================================================

#ifndef APP_YARA_MISC_TYPES_H_
#define APP_YARA_MISC_TYPES_H_

using namespace seqan;

// ============================================================================
// Tags
// ============================================================================

// ----------------------------------------------------------------------------
// Tag Limits
// ----------------------------------------------------------------------------

template <typename T1 = void, typename T2 = void>
struct Limits {};

// ----------------------------------------------------------------------------
// Metafunction Value
// ----------------------------------------------------------------------------

namespace seqan
{
template <typename T1, typename T2>
struct Value<Limits<T1, T2>, 1>
{
    typedef T1 Type;
};

template <typename T1, typename T2>
struct Value<Limits<T1, T2>, 2>
{
    typedef T2 Type;
};
}

// ============================================================================
// FMIndex
// ============================================================================

// ----------------------------------------------------------------------------
// FMIndex Config
// ----------------------------------------------------------------------------

template <typename TSize, typename TLen, typename TSum>
struct YaraFMIndexConfig
{
    typedef TSum                              TLengthSum;
    typedef Levels<void, LevelsConfig<TSum> > TBwtSpec;
    typedef Levels<void, LevelsConfig<TSum> > TSentinelsSpec;

    static const unsigned SAMPLING = 10;
};

template <typename TSize, typename TSum>
struct YaraFMIndexConfig<__uint8, TSize, TSum>
{
    typedef TSum                              TLengthSum;
    typedef Levels<void, LevelsConfig<TSum> > TBwtSpec;
    typedef Naive<void, NaiveConfig<TSum> >   TSentinelsSpec;

    static const unsigned SAMPLING = 10;
};

// ----------------------------------------------------------------------------
// FMIndex Contigs
// ----------------------------------------------------------------------------

template <typename TSize, typename TLen, typename TSum, typename TSpec = Alloc<> >
struct YaraFMIndexContigs
{
    typedef YaraFMIndexConfig<TSize, TLen, TSum>        TConfig_;
    typedef Owner<ConcatDirect<TConfig_> >              TSSetSpec_;

    typedef StringSet<String<Dna, TSpec>, TSSetSpec_>   Type;
};

// ----------------------------------------------------------------------------
// FMIndex SAValue
// ----------------------------------------------------------------------------

namespace seqan {
template <typename TValue, typename TSpec, typename TSize, typename TLen, typename TSum>
struct SAValue<StringSet<String<TValue, TSpec>, Owner<ConcatDirect<YaraFMIndexConfig<TSize, TLen, TSum> > > > >
{
    typedef Pair<TSize, TLen, Pack>   Type;
};
}

// ----------------------------------------------------------------------------
// CompressedSA SparseString
// ----------------------------------------------------------------------------
// TODO(esiragusa): remove this crap once the CSA gets refactored.

namespace seqan {
template <typename TText, typename TSpec, typename TSize, typename TLen, typename TSum>
struct Fibre<CompressedSA<TText, TSpec, YaraFMIndexConfig<TSize, TLen, TSum> >, FibreSparseString>
{
    typedef Pair<TSize, TLen, Pack>                         TSAValue_;
    typedef typename DefaultIndexStringSpec<TText>::Type    TSASpec_;
    typedef String<TSAValue_, TSASpec_>                     TSA_;
    typedef YaraFMIndexConfig<TSize, TLen, TSum>            TConfig_;

    typedef SparseString<TSA_, TConfig_>                    Type;
};
}

// ----------------------------------------------------------------------------
// SparseString Size
// ----------------------------------------------------------------------------
// TODO(esiragusa): remove this crap once the CSA gets refactored.

namespace seqan {
template <typename TString, typename TSize, typename TLen, typename TSum>
struct Size<SparseString<TString, YaraFMIndexConfig<TSize, TLen, TSum> > >
{
    typedef TSum Type;
};
}

// ============================================================================
// StringSet
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction LengthSum
// ----------------------------------------------------------------------------

namespace seqan {
template <typename TString, typename TSum>
struct LengthSum<StringSet<TString, Owner<ConcatDirect<Limits<TSum> > > > >
{
    typedef TSum Type;
};
}

// ============================================================================
// SeqStore Config
// ============================================================================

// ----------------------------------------------------------------------------
// Reads SeqsStore Config
// ----------------------------------------------------------------------------

typedef SeqConfig<void>         YaraReadsConfig;

// ----------------------------------------------------------------------------
// Contigs SeqsStore Config
// ----------------------------------------------------------------------------

template <typename TSpec = Alloc<> >
struct YaraContigsConfig
{
    typedef Dna5                                    TAlphabet;
    typedef Packed<TSpec>                           TSeqSpec;
    typedef Owner<ConcatDirect<Limits<__uint64> > > TSeqsSpec;
    typedef Owner<ConcatDirect<Limits<__uint32> > > TSeqNamesSpec;
};

// ----------------------------------------------------------------------------
// SmartFile Context
// ----------------------------------------------------------------------------

namespace seqan {
template <typename TString, typename TSpec, typename TStorageSpec>
struct SmartFileContext<SmartFile<Bam, Output, StringSet<TString, Owner<ConcatDirect<TSpec> > > >, TStorageSpec>
{
    typedef StringSet<CharString, Owner<ConcatDirect<TSpec> > >     TNameStore;
    typedef NameStoreCache<TNameStore>                              TNameStoreCache;
    typedef BamIOContext<TNameStore, TNameStoreCache, TStorageSpec> Type;
};
}

#endif  // #ifndef APP_YARA_MISC_TYPES_H_
