// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2013, Knut Reinert, FU Berlin
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
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: David Weese <david.weese@fu-berlin.de>
// ==========================================================================

#include <iostream>

#ifndef SEQAN_HEADER_STORE_IO_SAM_H
#define SEQAN_HEADER_STORE_IO_SAM_H

namespace SEQAN_NAMESPACE_MAIN
{


//////////////////////////////////////////////////////////////////////////////
// Parsing functions
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// _parseReadCigar
    
    template <typename TFile, typename TCigarString, typename TChar>
    inline void
    _parseReadCigar(TFile & file, TCigarString & cigar, TChar & c)
    {
//IOREV _nodoc_ _hasCRef_ could be simplified by using other _is or _parse calls
        typedef typename Value<TCigarString>::Type  TCigarElement;
        typedef typename TCigarElement::TCount      TCount;

        clear(cigar);
        
        // if the CIGAR is not set and '*'
        if (c == '*')
        {
            c = _streamGet(file);
            return;
        }
        
        while (!_streamEOF(file)) 
        {
            TCount count = _parseReadNumber(file, c);
            if (c >= 'a' && c <= 'z')
                c = c + 'A' - 'a';
            appendValue(cigar, TCigarElement(c, count));
            
            c = _streamGet(file);
            if (c == ' ' || c == '\t' || c == '\n') break;
        }
    }
    
//////////////////////////////////////////////////////////////////////////////
// _parseReadSamIdentifier
    
    template<typename TFile, typename TString, typename TChar>
    inline void
    _parseReadSamIdentifier(TFile & file, TString & str, TChar& c)
    {
//IOREV _nodoc_ _hasCRef_ _duplicate_ same as generic _parseUntilWhitespace?
        if (c == ' ' || c == '\t' || c == '\n') return;
        appendValue(str, c);
        while (!_streamEOF(file)) 
        {
            c = _streamGet(file);
            if (c == ' ' || c == '\t' || c == '\n') return;
            appendValue(str, c);
        }
    }
    
//////////////////////////////////////////////////////////////////////////////
// _parseIsDna
    
    template<typename TChar>
    inline bool
    _parseIsDna(TChar const & c)
    {
//IOREV _bug_ according to Dna5 doc c is already uppercase, so it should be (c == x) || (c - 'A' + 'a' == x) OR JUST: return tolower(c) == tolower(x);
        char x = TChar(Dna5(c));
        return (c == x) || (c + 'A' - 'a' == x);
    }
    
//////////////////////////////////////////////////////////////////////////////
//_parseReadDnaSeq
    
    template<typename TFile, typename TString, typename TChar>
    inline void
    _parseReadDnaSeq(TFile & file, TString & str, TChar & c)
    {
//IOREV _nodoc_ _hasCRef_ _duplicate_ same as generic _parseUntilWhitespace?
        TChar first = c;
        if (!_streamEOF(file)) 
            c = _streamGet(file);

        if (!_parseIsDna(first))
            return;
        appendValue(str, first, Generous());
        
        for (; !_streamEOF(file) && _parseIsDna(c); c = _streamGet(file))
            appendValue(str, c, Generous());
    }
        
//////////////////////////////////////////////////////////////////////////////
// _parseIsPhredQual
    
    template <typename TChar>
    inline bool
    _parseIsPhredQual(TChar c)
    {
//IOREV _nodoc_ what does the title mean? same as return isprint(c) && c != ' '
        return c >= '!' && c <= '~';
    }
    
//////////////////////////////////////////////////////////////////////////////
// _parseReadSeqQual
//
    
    template<typename TFile, typename TQualString, typename TChar>
    inline void
    _parseReadSeqQual(TFile & file, TQualString & str, TChar & c)
    {
//IOREV _nodoc_ unclearn what this actually does
        typedef typename Size<TQualString>::Type                TSize;
        typedef typename Iterator<TQualString, Standard>::Type  TIter;
        
        if (!_parseIsPhredQual(c)) return;

        TIter itBegin = begin(str, Standard());
        TIter it = itBegin; 
        TSize rest = length(str);
        
        do {
            int q = c - '!';
            if (!_streamEOF(file)) 
                c = _streamGet(file);
            else
                if (rest > 1)
                    rest = 1;
            
            if (q == '*' - '!' && !_parseIsPhredQual(c) && it == itBegin)
                return;
            
            if (rest != 0)
            {
                assignQualityValue(*it, q);
                ++it;
                --rest;
            }
        } while (_parseIsPhredQual(c) && !_streamEOF(file));
    }
    
//////////////////////////////////////////////////////////////////////////////
// _parseReadCharsUntilEndOfLine
//
// Reads all symbols till the next '\n' and writes them in the CharString str
// the c is the first character after the '\n'.
    
    template<typename TFile, typename TChar>
    inline void
    _parseReadCharsUntilEndOfLine(TFile & file, String<char> & str, TChar& c)
    {
//IOREV replace with generic _parseLine() function that ignores '\r' (this calls adds a lonely '\r' to the end of str if file is windows-formatted)
        // read all chars till '\n'
        while (c != '\n')
        {
            appendValue(str, c, Generous());
            if (_streamEOF(file)) return;
            c = _streamGet(file);
        }
        
        // read the first char after the '\n'
        if (!_streamEOF(file))
            c = _streamGet(file);
    }

//////////////////////////////////////////////////////////////////////////////
// appendAlignment
    
    template<typename TSpec, typename TConfig, typename TId, typename TPos, typename TGaps>
    inline typename Size<typename FragmentStore<TSpec, TConfig>::TAlignedReadStore>::Type
    appendAlignment(
        FragmentStore<TSpec, TConfig> & fragStore, 
        TId readId, 
        TId contigId, 
        TPos beginPos, 
        TPos endPos, 
        TGaps const & gaps)
    {
        typedef FragmentStore<TSpec, TConfig> TFragmentStore;
        typedef typename Value<typename TFragmentStore::TAlignedReadStore>::Type TAlignedElement;
        
        TId id = length(fragStore.alignedReadStore);
        TAlignedElement alignedElem = TAlignedElement(id, readId, contigId, beginPos, endPos, gaps);
        appendValue(fragStore.alignedReadStore, alignedElem);
        
        return id;
    }
    


//////////////////////////////////////////////////////////////////////////////
// read functions for Sam
//////////////////////////////////////////////////////////////////////////////

    
//////////////////////////////////////////////////////////////////////////////
// _generatePairMatchIds
//
    template <typename TPos, typename TId>
    struct MatchMateInfo_
    {
        TId     readId;
        TId     contigId;
        TId     pairMatchId;
        TId     matePairId;//:(sizeof(TId)*8-1);
        TPos    beginPos;
        bool    reversed;
    };
    
    template <typename TFragStore>
    struct AlignedMateLess_
    {
        TFragStore &fragStore;
        
        AlignedMateLess_(TFragStore &fragStore_) :
            fragStore(fragStore_) {}

        template <typename TAlignedRead>
        inline bool 
        operator() (TAlignedRead const& a, TAlignedRead const& b) const 
        {
            if (a.contigId < b.contigId) return true;
            if (a.contigId > b.contigId) return false;

            typename TFragStore::TContigPos posA = _min(a.beginPos, a.endPos);
            typename TFragStore::TContigPos posB = _min(b.beginPos, b.endPos);            
            if (posA < posB) return true;
            if (posA > posB) return false;
            
            bool reversedA = (a.endPos < a.beginPos);
            bool reversedB = (b.endPos < b.beginPos);
            if (reversedA != reversedB) return reversedB;

            typedef typename TFragStore::TMatePairStore     TMatePairStore;
            typedef typename Value<TMatePairStore>::Type    TMatePair;
            typename TMatePair::TId matePairIdB = TMatePair::INVALID_ID;

            if (a.readId >= length(fragStore.readStore))
                return false;
                
            if (b.readId < length(fragStore.readStore))
                matePairIdB = fragStore.readStore[b.readId].matePairId;

            return (fragStore.readStore[a.readId].matePairId < matePairIdB);
        }
    };

    struct MatchMateInfoLess_
    {
        template <typename TMInfo>
        inline bool 
        operator() (TMInfo const &a, TMInfo const &b) const 
        {
            if (a.contigId < b.contigId) return true;
            if (a.contigId > b.contigId) return false;
            if (a.beginPos < b.beginPos) return true;
            if (a.beginPos > b.beginPos) return false;
            if (a.reversed != b.reversed) return b.reversed;
            if (a.matePairId < b.matePairId) return true;
            if (a.matePairId > b.matePairId) return false;
            return (a.pairMatchId < b.pairMatchId);
        }
    };
    
    template <typename TAlignedRead, typename TMInfo, typename TFragStore>
    inline int
    _compareAlignedReadAndMateInfo(TAlignedRead const &a, TMInfo const &b, TFragStore const &fragStore)
    {
        if (a.contigId < b.contigId) return -1;
        if (a.contigId > b.contigId) return 1;

        typename TFragStore::TContigPos posA = _min(a.beginPos, a.endPos);
        if (posA < b.beginPos) return -1;
        if (posA > b.beginPos) return 1;
        
        bool reversedA = (a.endPos < a.beginPos);
        if (!reversedA &&  b.reversed) return -1;
        if ( reversedA && !b.reversed) return 1;

        typedef typename TFragStore::TMatePairStore     TMatePairStore;
        typedef typename Value<TMatePairStore>::Type    TMatePair;
        typename TMatePair::TId matePairIdA = TMatePair::INVALID_ID;

        if (a.readId < length(fragStore.readStore))
            matePairIdA = fragStore.readStore[a.readId].matePairId;
            
        if (matePairIdA < b.matePairId) return -1;
        if (matePairIdA > b.matePairId) return 1;
        return 0;
    }

    template<typename TSpec, typename TConfig, typename TMatchMateInfos>
    inline void 
    _generatePairMatchIds (
        FragmentStore<TSpec, TConfig> & fragStore,
        TMatchMateInfos & matchMateInfos)
    {
        typedef FragmentStore<TSpec, TConfig>                           TFragmentStore;        
        typedef typename TFragmentStore::TAlignedReadStore              TAlignedReadStore;
        typedef typename Iterator<TAlignedReadStore, Standard>::Type    TIter;    
        typedef typename Iterator<TMatchMateInfos, Standard>::Type      TMIter;    
                
        TIter it = begin(fragStore.alignedReadStore, Standard());
        TIter itEnd = end(fragStore.alignedReadStore, Standard());
        TMIter mit = begin(matchMateInfos, Standard());
        TMIter mitEnd = end(matchMateInfos, Standard());

        if (it == itEnd || mit == mitEnd) return;

        // sort the aligned read store by: begin position, contig name
        std::sort(it,  itEnd,  AlignedMateLess_<TFragmentStore>(fragStore));
        std::sort(mit, mitEnd, MatchMateInfoLess_());

        while (true)
        {
            int cmp = _compareAlignedReadAndMateInfo(*it, *mit, fragStore);

            if (cmp == 0)   // both are equal -> link them
                if (it->pairMatchId > mit->pairMatchId)     // avoid id swap and instead decide for
                    it->pairMatchId = mit->pairMatchId;     // the smaller of both ids

            if (cmp >= 0)   // MateInfo is less or equal
                if (++mit == mitEnd)
                    return;

            if (cmp <= 0)   // AlignedRead is less or equal
                if (++it == itEnd)
                    return;
        }
    }    

//////////////////////////////////////////////////////////////////////////////
// read

///.Function.read.param.tag.type:Tag.File Format.tag.Sam
    
    struct FragStoreImportFlags
    {
        bool importRead:1;
        bool importReadSeq:1;
        bool importReadName:1;
        bool importReadAlignment:1;
        bool importReadAlignmentQuality:1;
        bool importReadAlignmentTags:1;

        FragStoreImportFlags():
            importRead(true),
            importReadSeq(true),
            importReadName(true),
            importReadAlignment(true),
            importReadAlignmentQuality(true),
            importReadAlignmentTags(true)
        {}
    };

    inline void
    clear(FragStoreImportFlags & flags)
    {
        flags.importRead = false;
        flags.importReadSeq = false;
        flags.importReadName = false;
        flags.importReadAlignment = false;
        flags.importReadAlignmentQuality = false;
        flags.importReadAlignmentTags = false;
    }

    template<typename TStreamOrReader, typename TSpec, typename TConfig, typename TTag>
    inline void 
    _readImpl(TStreamOrReader & streamOrReader,
              FragmentStore<TSpec, TConfig> & fragStore,
              TTag const & tag,
              FragStoreImportFlags const & importFlags)
    {
        typedef FragmentStore<TSpec, TConfig> TFragmentStore;
        typedef typename TFragmentStore::TContigPos TContigPos;
        typedef typename Id<TFragmentStore>::Type TId;
        
        // data structure to temporarily store the gaps that need to be inserted in the contig sequences
        typedef MatchMateInfo_<TContigPos, TId> TMatchMateInfo;
        typedef String<TMatchMateInfo> TMatchMateInfos;
        typedef StringSet<String<typename TFragmentStore::TContigGapAnchor>, Owner<ConcatDirect<> > > TContigAnchorGaps;

        // data structure to temporarily store information about match mates
        TMatchMateInfos matchMateInfos;
        TContigAnchorGaps contigAnchorGaps;

        refresh(fragStore.contigNameStoreCache);

        // Setup a BamIOContext for I/O.
        typedef BamIOContext<typename TFragmentStore::TContigNameStore> TBamIOContext;
        TBamIOContext bamIOContext(fragStore.contigNameStore, fragStore.contigNameStoreCache);

        // Read in the header.  We will subsequently ignore it and use the information indirectly just using the
        // sequence names if any.
        BamHeader bamHeader;
        if (readRecord(bamHeader, bamIOContext, streamOrReader, tag) != 0)
        {
            std::cerr << "ERROR: Problem reading header from SAM file.\n";
            return;
        }

        // fill up contig entries for each contig name that appears in the header
        resize(fragStore.contigStore, length(fragStore.contigNameStore));

        // Read in alignments section
        _readAlignments(streamOrReader, bamIOContext, fragStore, contigAnchorGaps, matchMateInfos, tag, importFlags);
        
        if (importFlags.importReadAlignment)
        {
            // set the match mate IDs using the information stored in matchMateInfos
            _generatePairMatchIds(fragStore, matchMateInfos);

            convertPairWiseToGlobalAlignment(fragStore, contigAnchorGaps);
        }
    }

    template <typename TFile, typename TSpec, typename TConfig>
    inline void
    read(TFile & file,
         FragmentStore<TSpec, TConfig> & fragStore,
         Sam,
         FragStoreImportFlags const & importFlags)
    {
        // Construct a RecordReader from the input file.
        RecordReader<TFile, SinglePass<> > reader(file);
        if (atEnd(reader))
            return;  // Done, file is empty.
        _readImpl(reader, fragStore, Sam(), importFlags);
    }

#if SEQAN_HAS_ZLIB
    template <typename TFile, typename TSpec, typename TConfig>
    inline void
    read(TFile & file,
         FragmentStore<TSpec, TConfig> & fragStore,
         Bam,
         FragStoreImportFlags const & importFlags)
    {
        _readImpl(file, fragStore, Bam(), importFlags);
    }
#endif  // #if SEQAN_HAS_ZLIB

    template <typename TFile, typename TSpec, typename TConfig>
    inline void
    read(
        TFile & file,
        FragmentStore<TSpec, TConfig> & fragStore,
        Sam)
    {
        read(file, fragStore, Sam(), FragStoreImportFlags());
    }

#if SEQAN_HAS_ZLIB
    template <typename TFile, typename TSpec, typename TConfig>
    inline void
    read(
        TFile & file,
        FragmentStore<TSpec, TConfig> & fragStore,
        Bam)
    {
        read(file, fragStore, Bam(), FragStoreImportFlags());
    }
#endif  // #if SEQAN_HAS_ZLIB

//////////////////////////////////////////////////////////////////////////////
// _readAlignments
//
// reads in alignement sections from a Sam file

    template <typename TFragmentStore>
    struct FragStoreSAMContext
    {
        typedef typename Id<TFragmentStore>::Type                                   TId;
        typedef typename Value<typename TFragmentStore::TAlignedReadStore>::Type    TAlignedElement;
        typedef typename TAlignedElement::TGapAnchors                               TReadGapAnchors;
        typedef String<typename TFragmentStore::TContigGapAnchor>                   TContigAnchorGaps;
        typedef typename TFragmentStore::TReadSeq                                   TReadSeq;

        TId                 readId;
        TId                 contigId;
        TReadGapAnchors     readGapAnchors;
        TContigAnchorGaps   contigGapAnchors;

        // Buffer for SAM tags.
        CharString tags;
        // Buffer for the read sequence.
        TReadSeq readSeq;

        // Buffer for the current BamAlignmentRecord.
        BamAlignmentRecord bamRecord;
    };


    template <typename TStreamOrReader, typename TNameStore, typename TNameStoreCache, typename TSpec, typename TConfig, typename TContigAnchorGaps, typename TMatchMateInfos, typename TTag>
    inline void 
    _readAlignments(
        TStreamOrReader & streamOrReader,
        BamIOContext<TNameStore, TNameStoreCache> & bamIOContext,
        FragmentStore<TSpec, TConfig> & fragStore,
        TContigAnchorGaps & contigAnchorGaps,   
        TMatchMateInfos & matchMateInfos,
        TTag const & tag,
        FragStoreImportFlags const & importFlags)
    {
//IOREV _nodoc_ docusmentation in code, but unclear
        // create dummy entries in Sam specific aligned read quality store and aligned read tag store
        // is needed so the ID in the aligned store can be use to access the other stores
        // even if there exists previous entries without
        typedef FragmentStore<TSpec, TConfig> TFragmentStore;
        typedef typename TFragmentStore::TAlignQualityStore TAlignQualityStore;
        typedef typename TFragmentStore::TReadSeqStore TReadSeqStore;
        typedef typename Size<TReadSeqStore>::Type TReadSeqStoreSize;
        typedef typename Value<TAlignQualityStore>::Type TAlignQuality;
        
        TAlignQuality q;
        q.score = maxValue(q.score);
        int diff = length(fragStore.alignedReadStore) - length(fragStore.alignQualityStore);
        for(int i = 0; i < diff; ++i)
            appendValue(fragStore.alignQualityStore, q, Generous());
        
        diff = length(fragStore.alignedReadStore) - length(fragStore.alignedReadTagStore);
        for(int i = 0; i < diff; ++i)
            appendValue(fragStore.alignedReadTagStore, "", Generous());
        
        // read in alignments
        FragStoreSAMContext<TFragmentStore> contextSAM;
//        refresh(fragStore.contigNameStoreCache);  // was done for the BamIOContext already
        refresh(fragStore.readNameStoreCache);

        while (!atEnd(streamOrReader))
            _readOneAlignment(streamOrReader, bamIOContext, fragStore, contigAnchorGaps, matchMateInfos, contextSAM,
                              importFlags, tag);

        if (importFlags.importReadSeq)
        {
            TReadSeqStoreSize emptyReads = 0;
            for(TReadSeqStoreSize i = 0; i < length(fragStore.alignedReadStore); ++i)
                if (empty(fragStore.readSeqStore[fragStore.alignedReadStore[i].readId]))
                {
                    ++emptyReads;
    //                std::cerr << "Read sequence empty for " << fragStore.readNameStore[fragStore.alignedReadStore[i].readId] << std::endl;
                }
            if (emptyReads != 0)
                std::cerr << "Warning: " << emptyReads << " read sequences are empty." << std::endl;
        }
    }
    
        
    template <typename TReadSeq, typename TCigar, typename TPos, typename TId, typename TFragmentStore>
    inline void
    _samAppendAlignment(
        TFragmentStore &fragStore,
        TReadSeq const &readSeq,
        TCigar &cigar,
        TPos &beginPos, TPos &endPos,
        TId &pairMatchId,
        FragStoreSAMContext<TFragmentStore> & contextSAM)
    {
        typedef typename TFragmentStore::TAlignedReadStore                      TAlignedReadStore;
        typedef typename Value<TAlignedReadStore>::Type                         TAlignedRead;
        typedef Gaps<TReadSeq, AnchorGaps<typename TAlignedRead::TGapAnchors> > TReadGaps;

        // insert alignment gaps
        clear(contextSAM.readGapAnchors);
        TReadGaps readGaps(readSeq, contextSAM.readGapAnchors);
        unsigned beginGaps = cigarToGapAnchorRead(cigar, readGaps);

        // adapt start or end (on reverse strand) position if alignment begins with gaps
        if (beginPos > endPos)
            endPos += beginGaps;
        else
            beginPos += beginGaps;

        // create a new entry in the aligned read store
        pairMatchId = appendAlignment(fragStore, contextSAM.readId, contextSAM.contigId, beginPos, endPos, contextSAM.readGapAnchors);
    }

    template <typename TCigar, typename TPos, typename TId, typename TFragmentStore>
    inline void
    _samAppendAlignmentWithoutSeq(
        TFragmentStore &fragStore,
        TCigar &cigar,
        TPos &beginPos, TPos &endPos,
        TId &pairMatchId,
        FragStoreSAMContext<TFragmentStore> & contextSAM)
    {
        typedef typename TFragmentStore::TAlignedReadStore                      TAlignedReadStore;
        typedef typename Value<TAlignedReadStore>::Type                         TAlignedRead;
        typedef Gaps<Nothing, AnchorGaps<typename TAlignedRead::TGapAnchors> >  TReadGaps;
        Nothing nothing;

        // insert alignment gaps
        clear(contextSAM.readGapAnchors);
        TReadGaps readGaps(nothing, contextSAM.readGapAnchors);
        unsigned beginGaps = cigarToGapAnchorRead(cigar, readGaps);

        // adapt start or end (on reverse strand) position if alignment begins with gaps
        if (beginPos > endPos)
            endPos += beginGaps;
        else
            beginPos += beginGaps;

        // create a new entry in the aligned read store
        pairMatchId = appendAlignment(fragStore, contextSAM.readId, contextSAM.contigId, beginPos, endPos, contextSAM.readGapAnchors);
    }

//////////////////////////////////////////////////////////////////////////////
// _readOneAlignment
//
// reads in one alignement section from a Sam file
    
    template <
        typename TStreamOrReader,
        typename TNameStore,
        typename TNameStoreCache,
        typename TSpec,
        typename TConfig,
        typename TContigAnchorGaps,
        typename TMatchMateInfos,
        typename TFragStore,
        typename TTag>
    inline void
    _readOneAlignment(
        TStreamOrReader & streamOrReader,
        BamIOContext<TNameStore, TNameStoreCache> & bamIOContext,
        FragmentStore<TSpec, TConfig> & fragStore,
        TContigAnchorGaps & contigAnchorGaps,
        TMatchMateInfos & matchMateInfos,
        FragStoreSAMContext<TFragStore> & contextSAM,
        FragStoreImportFlags const & importFlags,
        TTag const & tag)
    {
        // Basic types
        typedef FragmentStore<TSpec, TConfig>                                       TFragmentStore;
        typedef FragStoreSAMContext<TFragStore>                                     TSAMContext;
        typedef typename Id<TFragmentStore>::Type                                   TId;
        //typedef typename Size<TFragmentStore>::Type                                 TSize;

        // All fragment store element types
        //typedef typename Value<typename TFragmentStore::TContigStore>::Type         TContigElement;
        //typedef typename Value<typename TFragmentStore::TLibraryStore>::Type        TLibraryStoreElement;
        //typedef typename Value<typename TFragmentStore::TReadStore>::Type           TReadStoreElement;
        typedef typename Value<typename TFragmentStore::TAlignQualityStore>::Type   TAlignQualityElement;

        // Type for sequence in readstore
        typedef typename TFragmentStore::TReadSeq                                   TReadSeq;

        // Type for gap anchor
        typedef typename TFragmentStore::TContigPos                                 TContigPos;
        typedef Gaps<Nothing, AnchorGaps<typename TSAMContext::TContigAnchorGaps> > TContigGapsPW;

        // Type to temporarily store information about match mates
        typedef typename Value<TMatchMateInfos>::Type                               TMatchMateInfo;

        // Read next BamAlignmentRecord and get shortcut.
        if (readRecord(contextSAM.bamRecord, bamIOContext, streamOrReader, tag) != 0)
        {
//  TODO(weese:) skipLine would be required to continue reading after an invalid SAM line but is not yet implemented for Bgzf stream
//            skipLine(streamOrReader);
            std::cerr << "ERROR: Problem reading SAM/BAM record.\n";
            return;
        }
        BamAlignmentRecord & record = contextSAM.bamRecord;

        // Get element of align quality store.
        TAlignQualityElement mapQ;
        mapQ.score = record.mapQ;

        // Get begin and end position.
        TContigPos beginPos = record.beginPos;
        TContigPos endPos = 0;
        _getLengthInRef(record.cigar, endPos);
        endPos = beginPos + endPos;
        if (hasFlagRC(record))
            std::swap(beginPos, endPos);

        // Put read sequence and qualities into readSeq and reverseComplement if necessary.
        TReadSeq & readSeq = contextSAM.readSeq;
        readSeq = record.seq;
        assignQualities(readSeq, record.qual);
        if (hasFlagRC(record))
            reverseComplement(readSeq);

        // Get tags in SAM format.
        assignTagsBamToSam(contextSAM.tags, record.tags);

        // Check if read sequence is already in the store.  If so get the ID, otherwise create new entries in the read
        // then read name and mate pair store.
        contextSAM.readId = 0;
        if (importFlags.importRead)
        {
            if (!importFlags.importReadName)
                clear(record.qName);

            bool newRead = _storeAppendRead(fragStore, contextSAM.readId, record.qName, readSeq, record.flag,
                                            contextSAM);
            (void)newRead;
            SEQAN_ASSERT_NOT(newRead && empty(readSeq));
        }
        
        // Stop if read is not aligned.
        if (record.rID == BamAlignmentRecord::INVALID_REFID || record.beginPos == BamAlignmentRecord::INVALID_POS)
            return;

        // Check if the contig is already in the store.  Get its ID or create a new one otherwise.
        contextSAM.contigId = 0;
        _storeAppendContig(fragStore, contextSAM.contigId, nameStore(bamIOContext)[record.rID]);
        resize(fragStore.contigStore, length(fragStore.contigNameStore));

        // Stop if no alignment in CIGAR string.
        if (empty(record.cigar))
            return;

        // Handle read alignment import.
        TId pairMatchId = 0;
        if (importFlags.importReadAlignment)
        {
            // generate gap anchor string for the read
            if (importFlags.importReadSeq)
                _samAppendAlignment(fragStore, fragStore.readSeqStore[contextSAM.readId], record.cigar, beginPos, endPos,
                                    pairMatchId, contextSAM);
            else
                _samAppendAlignmentWithoutSeq(fragStore, record.cigar, beginPos, endPos, pairMatchId, contextSAM);

            clear(contextSAM.contigGapAnchors);
            TContigGapsPW contigGaps(contextSAM.contigGapAnchors);
            cigarToGapAnchorContig(record.cigar, contigGaps);
            appendValue(contigAnchorGaps, contextSAM.contigGapAnchors);
        }

        // Import tags and remove some tags.
        if (importFlags.importReadAlignmentTags || importFlags.importReadAlignmentQuality)
        {
            // extract and delete some tags
            if (!empty(contextSAM.tags))
                for (unsigned pos = length(contextSAM.tags), right = length(contextSAM.tags); pos != 0; )
                {
                    --pos;
                    if (pos == 0 || contextSAM.tags[pos] == '\t')
                    {
                        unsigned left = pos;
                        if (contextSAM.tags[left] == '\t') ++left;                    

                        bool remove = false;
                        if (infix(contextSAM.tags, left, left + 2) == "MD")
                            remove = true;
                        
                        if (infix(contextSAM.tags, left, left + 2) == "NM")
                        {
                            int errors = lexicalCast<int>(infix(contextSAM.tags, left + 5, right));  // NM:i:x
                            mapQ.errors = errors;
                            remove = true;
                        }
                        
                        if (remove)
                            erase(contextSAM.tags, left, _min(right + 1, length(contextSAM.tags)));

                        right = pos;
                    }
                }
        }

        // Create entries in Sam specific stores.
        if (importFlags.importReadAlignmentQuality)
            appendValue(fragStore.alignQualityStore, mapQ, Generous());
        
        if (importFlags.importReadAlignmentTags)
            appendValue(fragStore.alignedReadTagStore, contextSAM.tags, Generous());

        // Store additional data about match mate temporarily.
        // used in the end of the read function to generate match mate IDs.
        if (importFlags.importRead && importFlags.importReadAlignment && record.rNextId != BamAlignmentRecord::INVALID_REFID)
        {
            TId mcontigId = contextSAM.contigId;
            if (record.rID != record.rNextId)
            {
                _storeAppendContig(fragStore, mcontigId, nameStore(bamIOContext)[record.rNextId]);
                resize(fragStore.contigStore, length(fragStore.contigNameStore));
            }

            if (contextSAM.readId < length(fragStore.readStore))
            {
                // insert match mate info for the mate record
                TMatchMateInfo matchMateInfo =
                {
                    /* .readId      = */  contextSAM.readId,
                    /* .contigId    = */  mcontigId,
                    /* .pairMatchId = */  pairMatchId,
                    /* .matePairId  = */  fragStore.readStore[contextSAM.readId].matePairId,
                    /* .beginPos    = */  record.pNext,
                    /* .reversed    = */  hasFlagNextRC(record)
                };
                appendValue(matchMateInfos, matchMateInfo);
                back(fragStore.alignedReadStore).pairMatchId = pairMatchId;
            }
        }
    }


//////////////////////////////////////////////////////////////////////////////
// write functions for Sam
//////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------------------
// Function _fillHeader()
// --------------------------------------------------------------------------

template <typename TSpec, typename TConfig>
inline void
_fillHeader(BamHeader & header,
            FragmentStore<TSpec, TConfig> & store)
{
    typedef FragmentStore<TSpec, TConfig>                       TFragmentStore;
    typedef typename TFragmentStore::TLibraryStore              TLibraryStore;
    typedef typename TFragmentStore::TContigStore               TContigStore;
    typedef typename TFragmentStore::TNameStore                 TNameStore;

    typedef typename Value<TContigStore>::Type                  TContig;
    typedef typename Iterator<TLibraryStore, Standard>::Type    TLibraryIter;
    typedef typename Iterator<TContigStore, Standard>::Type     TContigIter;
    typedef typename Iterator<TNameStore, Standard>::Type       TContigNameIter;
    typedef typename Id<TContig>::Type                          TId;

    typedef BamHeader::TSequenceInfo                            TSequenceInfo;
    typedef BamHeaderRecord::TTag                               TTag;

    // Fill first header line.
    BamHeaderRecord firstRecord;
    firstRecord.type = BAM_HEADER_FIRST;
    appendValue(firstRecord.tags, TTag("VN", "1.4"));
    appendValue(firstRecord.tags, TTag("SO", "unsorted"));
    appendValue(header.records, firstRecord);

    // Fill sequence info header line.
    TContigIter it          = begin(store.contigStore, Standard());
    TContigIter itEnd       = end(store.contigStore, Standard());
    TContigNameIter nit     = begin(store.contigNameStore, Standard());
    TContigNameIter nitEnd  = end(store.contigNameStore, Standard());

    for (; it != itEnd && nit != nitEnd; ++it, ++nit)
        appendValue(header.sequenceInfos, TSequenceInfo(*nit, length((*it).seq)));

    // Fill program header line.
    BamHeaderRecord pgRecord;
    pgRecord.type = BAM_HEADER_PROGRAM;
    appendValue(pgRecord.tags, TTag("ID", "SeqAn"));
    appendValue(header.records, pgRecord);

    // Fill library info header line.
    BamHeaderRecord rgRecord;
    rgRecord.type = BAM_HEADER_READ_GROUP;

    TLibraryIter lit    = begin(store.libraryStore, Standard());
    TLibraryIter litEnd = end(store.libraryStore, Standard());

    for (TId id = 0; lit != litEnd; ++lit, ++id)
    {
        appendValue(pgRecord.tags, TTag("ID", id + 1));
        appendValue(pgRecord.tags, TTag("LB", store.libraryNameStore[id]));
        appendValue(pgRecord.tags, TTag("PI", (int)store.libraryStore[id].mean));
        // Sample name needs to be included into fragment store.
        appendValue(pgRecord.tags, TTag("SM", "none"));
    }
}

// --------------------------------------------------------------------------
// Function _writeHeader()
// --------------------------------------------------------------------------

template <typename TStream, typename TSpec, typename TConfig, typename TContext>
inline void _writeHeader(TStream & target,
                         FragmentStore<TSpec, TConfig> & store,
                         TContext & context,
                         Sam)
{
    BamHeader header;

    // Fill header with information from fragment store.
    _fillHeader(header, store);

    // Write header to target.
    write2(target, header, context, Sam());
}

// --------------------------------------------------------------------------
// Function _fillBamSeqAndQual()
// --------------------------------------------------------------------------
template <typename TSeq, typename TQual, typename TRead>
inline void
_fillBamSeqAndQual(TSeq &bamSeq, TQual &bamQual, TRead const &read)
{
    bamSeq = read;

    resize(bamQual, length(read));
    typename Iterator<TQual, Standard>::Type       tIt    = begin(bamQual, Standard());
    typename Iterator<TRead const, Standard>::Type sIt    = begin(read, Standard());
    typename Iterator<TRead const, Standard>::Type sItEnd = end(read, Standard());

    for (; sIt != sItEnd; ++sIt, ++tIt)
        *tIt = (char)(getQualityValue(*sIt) + '!');
}

// --------------------------------------------------------------------------
// Function _fillRecord()
// --------------------------------------------------------------------------

template <typename TSpec, typename TConfig, typename TAlignedRead, typename TAlignQuality, typename TAlignFunctor>
inline void
setPrimaryMatch(BamAlignmentRecord & record,
                FragmentStore<TSpec, TConfig> & store,
                TAlignedRead const & alignedRead,       // read alignment to transform into a BamAlignmentRecord
                TAlignQuality const & alignQuality,     // read alignment quality
                TAlignFunctor & alignFunctor,           // functor that extracts/computes alignment for cigar/md strings
                bool secondaryAlignment)                // is this a secondary alignment?
{
    typedef FragmentStore<TSpec, TConfig>                           TFragmentStore;

    typedef typename TFragmentStore::TContigStore                   TContigStore;
    typedef typename Value<TContigStore>::Type                      TContig;
    typedef typename TContig::TGapAnchors                           TContigGapAnchors;

    typedef Iterator<CharString, Standard>::Type                    TCharStringIterator;

    typedef Gaps<Nothing, AnchorGaps<TContigGapAnchors> >           TContigGaps;

    // Fill QNAME with read name.
    clear(record.qName);
    TCharStringIterator it = begin(store.readNameStore[alignedRead.readId], Standard());
    TCharStringIterator itEnd = end(store.readNameStore[alignedRead.readId], Standard());
    for (; it != itEnd && *it != ' ' && *it != '\t'; ++it)
        appendValue(record.qName, *it);

    // Fill FLAG.
    unsigned short flag = 0;

    if (alignedRead.beginPos > alignedRead.endPos)
        flag |= BAM_FLAG_RC;
    if (secondaryAlignment)
        flag |= BAM_FLAG_SECONDARY;     // we've already output an alignment for this read (this one is secondary)

    signed char mateNo = getMateNo(store, alignedRead.readId);
    if (mateNo == 0)
        flag |= BAM_FLAG_FIRST;        // this read is the first in the pair
    else if (mateNo == 1)
        flag |= BAM_FLAG_LAST;         // this read is the second in the pair
    
    // Set flags regarding mate alignment.
    
    // We disinguish 3 cases:
    //  1) We have no mate (single-end read)
    //  2) We might have a mate, but it wasn't aligned properly
    //  3) We have a mate which was aligned properly
    //
    // wich are encoded as follows:
    //  1) alignedRead and alignedMate have the same address in _fillRecord
    //  2) alignedMate has an invalid contigId (== INVALID_ID)
    //  3) alignedRead and alignedMate have valid readIds

    if (mateNo >= 0)
        flag |=   BAM_FLAG_MULTIPLE | BAM_FLAG_NEXT_UNMAPPED;   // there is a mate
    else
        flag &= ~(BAM_FLAG_MULTIPLE | BAM_FLAG_NEXT_UNMAPPED);  // there is no mate

    record.flag = flag;

    // Fill RNAME by providing contig id.
    record.rID = alignedRead.contigId;

    // Fill POS with start position.
    TContigGaps contigGaps(/*store.contigStore[alignedRead.contigId].seq, */store.contigStore[alignedRead.contigId].gaps);
    record.beginPos = positionGapToSeq(contigGaps, std::min(alignedRead.beginPos, alignedRead.endPos));    

    // Fill MAPQ with mapping quality.
//    if (alignQuality.score != TAlignedReadQualityElement::INVALID_SCORE)
    if (alignQuality.score > 0)
        record.mapQ = alignQuality.score;
    else
        record.mapQ = 255;

//    // Fill CIGAR using aligned read.
//    if (!IsSameType<TAlignFunctor, Nothing>::VALUE)
//        getCigarString(record.cigar, row(align, 0), row(align, 1));

    // Retrieve number of errors from quality store.
    int errors = -1;
    if (alignQuality.errors != MaxValue<unsigned char>::VALUE)
        errors = alignQuality.errors;

    // Use record.qual as a temporary for the md string.
    alignAndGetCigarString(record.cigar, record.qual, store.contigStore[alignedRead.contigId],
            store.readSeqStore[alignedRead.readId], alignedRead, errors, alignFunctor);

    if (alignQuality.errors != MaxValue<unsigned char>::VALUE)
    {
//        if (errors > (int)alignQuality.errors)
//            std::cerr << "WARNING: More errors in the alignment (" << errors << ") than given in NM tag / alignQuality ("
//                      << (int)alignQuality.errors << ") for read " << record.qName << std::endl;

        // To output the same NM value if a SAM file is loaded and saved, we overwrite error with the original value
        // TODO(weese): This can be changed if desired (uncomment the following line) but now even the ex1.sam example
        //              of the SAMtools have NM values that are inconsistent to their CIGAR/MD strings
        errors = alignQuality.errors;
    }

    // Fill tags here to release record.qual 
    clear(record.tags);
    if (errors != -1)
        appendTagValue(record.tags, "NM", errors);
    if (!empty(record.tags))
        appendTagValue(record.tags, "MD", record.qual, 'Z');

    record.rNextId = BamAlignmentRecord::INVALID_REFID;
    record.pNext = BamAlignmentRecord::INVALID_POS;
    record.tLen = BamAlignmentRecord::INVALID_LEN;

    if (!secondaryAlignment)
    {
        // Fill SEQ and QUAL with read sequence and quality values.
        if (alignedRead.beginPos <= alignedRead.endPos)
            _fillBamSeqAndQual(record.seq, record.qual, store.readSeqStore[alignedRead.readId]);
        else
            _fillBamSeqAndQual(record.seq, record.qual, reverseComplementString(store.readSeqStore[alignedRead.readId]));
    }
    else
    {
        clear(record.seq);
        clear(record.qual);
    }
}

template <typename TSpec, typename TConfig, typename TAlignedRead>
inline void
setMateMatch(BamAlignmentRecord & record,
            FragmentStore<TSpec, TConfig> & store,
            TAlignedRead const & alignedRead,       // read alignment to transform into a BamAlignmentRecord
            TAlignedRead const & alignedMate)       // alignment of the mate
{
    typedef FragmentStore<TSpec, TConfig>                           TFragmentStore;

    typedef typename TFragmentStore::TContigStore                   TContigStore;
    typedef typename Value<TContigStore>::Type                      TContig;
    typedef typename TContig::TGapAnchors                           TContigGapAnchors;

    typedef typename TFragmentStore::TReadStore                     TReadStore;
    typedef typename Value<TReadStore>::Type                        TReadStoreElement;

    typedef Gaps<Nothing, AnchorGaps<TContigGapAnchors> >           TContigGaps;


    record.flag &= ~(BAM_FLAG_ALL_PROPER | BAM_FLAG_NEXT_RC | BAM_FLAG_NEXT_UNMAPPED);
    if (alignedMate.contigId != TReadStoreElement::INVALID_ID)
    {
        record.flag |= BAM_FLAG_ALL_PROPER;    // mate is mapped properly
        
        if (alignedMate.beginPos > alignedMate.endPos)
            record.flag |= BAM_FLAG_NEXT_RC;   // mate is mapped on reverse strand
    }
    else
    {
        record.flag |= BAM_FLAG_NEXT_UNMAPPED; // mate is unmapped
    }

    record.rNextId = (alignedMate.contigId != TContig::INVALID_ID)? alignedMate.contigId : BamAlignmentRecord::INVALID_REFID;

    // Fill PNEXT with mate start position
    // Fill TLEN with outer distance of the read alignments
    if (alignedMate.contigId < length(store.contigStore))
    {
        TContigGaps contigGaps(store.contigStore[alignedMate.contigId].gaps);
        record.pNext = positionGapToSeq(contigGaps, std::min(alignedMate.beginPos, alignedMate.endPos));

        if (alignedMate.contigId == alignedRead.contigId)
        {
            if (alignedRead.beginPos < alignedMate.beginPos)
                record.tLen = positionGapToSeq(contigGaps, std::max(alignedMate.beginPos, alignedMate.endPos) - 1) - record.beginPos + 1;
            else
                record.tLen = record.pNext - positionGapToSeq(contigGaps, std::max(alignedRead.beginPos, alignedRead.endPos) - 1) - 1;
        }
        else
        {
            record.tLen = BamAlignmentRecord::INVALID_LEN;
        }
    }
    else
    {
        record.pNext = BamAlignmentRecord::INVALID_POS;
        record.tLen = BamAlignmentRecord::INVALID_LEN;
    }
}

// --------------------------------------------------------------------------
// Function _writeAlignedReads()
// --------------------------------------------------------------------------

template <typename TStream, typename TSpec, typename TConfig, typename TContext, typename TAlignFunctor>
inline void _writeAlignedReads(
    TStream & target,
    FragmentStore<TSpec, TConfig> & store,
    TContext & context,
    TAlignFunctor & alignFunctor,
    Sam)
{
    typedef FragmentStore<TSpec, TConfig>                           TFragmentStore;

    typedef typename TFragmentStore::TReadStore                     TReadStore;
    typedef typename Value<TReadStore>::Type                        TReadStoreElement;

    typedef typename TFragmentStore::TAlignedReadStore              TAlignedReadStore;
    typedef typename Iterator<TAlignedReadStore, Standard>::Type    TAlignedReadStoreIterator;
    typedef typename Value<TAlignedReadStore>::Type                 TAlignedReadStoreElement;
    typedef typename Id<TAlignedReadStoreElement>::Type             TAlignedReadStoreElementId;

    typedef typename TFragmentStore::TAlignQualityStore             TAlignQualityStore;
    typedef typename Value<TAlignQualityStore>::Type                TAlignQualityStoreElement;

    typedef TAlignQualityStoreElement *                             TAlignQualityStoreElementPtr;

    // Store outer library size for each pair match (indexed by pairMatchId)
    String<TAlignedReadStoreElementId> mateIndices;
    calculateMateIndices(mateIndices, store);

    // Bitset to signal wether a read was aligned at least once.
    String<bool, Packed<> > readAligned;
    resize(readAligned, length(store.readStore), false);

    // Dummy store elements to cope with missing information.
    TAlignQualityStoreElement       noAlignQuality;
    TAlignQualityStoreElementPtr    alignQualityPtr;

    TAlignedReadStoreIterator it    = begin(store.alignedReadStore, Standard());
    TAlignedReadStoreIterator itEnd = end(store.alignedReadStore, Standard());

    BamAlignmentRecord record;

    for (; it != itEnd; ++it)
    {
        TAlignedReadStoreElement &alignedRead = *it;

        // Try to get quality.
        if (alignedRead.id < length(store.alignQualityStore))
            alignQualityPtr = &store.alignQualityStore[alignedRead.id];
        else
            alignQualityPtr = &noAlignQuality;

        // Try to get a mate.

        // We disinguish 3 cases:
        //  1) We have no mate (single-end read)
        //  2) We might have a mate, but it wasn't aligned properly
        //  3) We have a mate which was aligned properly

        // Test for secondary alignment and mark read as seen.
        bool secondary = getValue(readAligned, alignedRead.readId);
        assignValue(readAligned, alignedRead.readId, true);

        // Fill record.
        clear(record);

        // case 1 or 2 is handled here
        setPrimaryMatch(record, store, alignedRead, *alignQualityPtr, alignFunctor, secondary);

        if (alignedRead.pairMatchId != TReadStoreElement::INVALID_ID)
        {
            // case 3
            TAlignedReadStoreElementId mateIndex = mateIndices[2 * alignedRead.pairMatchId + getMateNo(store, alignedRead.readId)];
            if (mateIndex < length(store.alignedReadStore))
                setMateMatch(record, store, alignedRead, store.alignedReadStore[mateIndex]);
        }

        // Append additional tags.
        if (alignedRead.id < length(store.alignedReadTagStore))
            assignTagsSamToBam(record.tags, store.alignedReadTagStore[alignedRead.id]);


        // Write record to target.
        write2(target, record, context, Sam());
    }
}

//////////////////////////////////////////////////////////////////////////////
// write

///.Function.write.param.tag.type:Tag.File Format.tag.Sam

    template<typename TFile, typename TSpec, typename TConfig, typename TAlignFunctor>
    inline void write(TFile & target,
                      FragmentStore<TSpec, TConfig> & store,
                      TAlignFunctor & alignFunctor,
                      Sam)
    {
        typedef FragmentStore<TSpec, TConfig>               TFragmentStore;
        typedef typename TFragmentStore::TContigNameStore   TContigNameStore;
        typedef BamIOContext<TContigNameStore>              TBamIOContext;

        TBamIOContext context(store.contigNameStore, store.contigNameStoreCache);

        // 1. write header
        _writeHeader(target, store, context, Sam());

        // 2. write aligments
        _writeAlignedReads(target, store, context, alignFunctor, Sam());
    }

    template<typename TFile, typename TSpec, typename TConfig>
    inline void write(TFile & target,
                      FragmentStore<TSpec, TConfig> & store,
                      Sam)
    {
        BamAlignFunctorDefault alignFunctor;
        write(target, store, alignFunctor, Sam());
    }

}// namespace SEQAN_NAMESPACE_MAIN

#endif //#ifndef SEQAN_HEADER_...
