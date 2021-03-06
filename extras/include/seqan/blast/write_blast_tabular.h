// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2014, Knut Reinert, FU Berlin
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
// Author: Hannes Hauswedell <hauswedell@mi.fu-berlin.de>
// ==========================================================================
// This file contains routines to generate BLAST tab-seperated output
// ==========================================================================

#ifndef SEQAN_EXTRAS_BLAST_WRITE_BLAST_TABULAR_H_
#define SEQAN_EXTRAS_BLAST_WRITE_BLAST_TABULAR_H_

namespace seqan {

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

template <typename TVoidSpec = void>
struct BlastMatchField
{
    enum class Enum : uint8_t
    {
        STD,
        Q_SEQ_ID,
        Q_GI,
        Q_ACC,
        Q_ACCVER,
        Q_LEN,
        S_SEQ_ID,
        S_ALL_SEQ_ID,
        S_GI,
        S_ALL_GI,
        S_ACC,
        S_ACCVER,
        S_ALLACC,
        S_LEN,
        Q_START,
        Q_END,
        S_START,
        S_END,
        Q_SEQ,
        S_SEQ,
        E_VALUE,
        BIT_SCORE,
        SCORE,
        LENGTH,
        P_IDENT,
        N_IDENT,
        MISMATCH,
        POSITIVE,
        GAP_OPEN,
        GAPS,
        P_POS,
        FRAMES,
        Q_FRAME,
        S_FRAME,
        BTOP,
        S_TAX_IDS,
        S_SCI_NAMES,
        S_COM_NAMES,
        S_BLAST_NAMES,
        S_S_KINGDOMS,
        S_TITLE,
        S_ALL_TITLES,
        S_STRAND,
        Q_COV_S,
        Q_COV_HSP
    };

    // this is what Enum::STD stands for
    static constexpr std::array<Enum const, 12> defaults
    {
        {
            Enum::Q_SEQ_ID,
            Enum::S_SEQ_ID,
            Enum::P_IDENT,
            Enum::LENGTH,
            Enum::MISMATCH,
            Enum::GAP_OPEN,
            Enum::Q_START,
            Enum::Q_END,
            Enum::S_START,
            Enum::S_END,
            Enum::E_VALUE,
            Enum::BIT_SCORE
        }
    };

    static constexpr char const * const optionLabels [] =
    {
        "std",
        "qseqid",
        "qgi",
        "qacc",
        "qaccver",
        "qlen",
        "sseqid",
        "sallseqid",
        "sgi",
        "sallgi",
        "sacc",
        "saccver",
        "sallacc",
        "slen",
        "qstart",
        "qend",
        "sstart",
        "send",
        "qseq",
        "sseq",
        "evalue",
        "bitscore",
        "score",
        "length",
        "pident",
        "nident",
        "mismatch",
        "positive",
        "gapopen",
        "gaps",
        "ppos",
        "frames",
        "qframe",
        "sframe",
        "btop",
        "staxids",
        "sscinames",
        "scomnames",
        "sblastnames",
        "sskingdoms",
        "stitle",
        "salltitles",
        "sstrand",
        "qcovs",
        "qcovhsp"
    };

    static constexpr char const * const columnLabels [] =
    {
        "query id, subject id, % identity, alignment "
         "length, mismatches, gap opens, q. start, q. end, s. "
         "start, s. end, evalue, bit score",
        "query id",
        "query gi",
        "query acc.",
        "query acc.ver",
        "query length",
        "subject id",
        "subject ids",
        "subject gi",
        "subject gis",
        "subject acc.",
        "subject acc.ver",
        "subject accs.",
        "subject length",
        "q. start",
        "q. end",
        "s. start",
        "s. end",
        "query seq",
        "subject seq",
        "evalue",
        "bit score",
        "score",
        "alignment length",
        "% identity",
        "identical",
        "mismatches",
        "positives",
        "gap opens",
        "gaps",
        "% positives",
        "query/sbjct frames",
        "query frame",
        "sbjct frame",
        "BTOP",
        "subject tax ids",
        "subject sci names",
        "subject com names",
        "subject blast names",
        "subject super kingdoms",
        "subject title",
        "subject titles",
        "subject strand",
        "% subject coverage",
        "% hsp coverage"
    };

    static constexpr char const * const descriptions [] =
    {
        "Default 12 columns (Query Seq-id, Subject Seq-id, Percentage of "
         "identical matches, Alignment length, Number of mismatches, Number of "
         "gap openings, Start of alignment in query, End of alignment in query,"
         " Start of alignment in subject, End of alignment in subject, Expect "
         "value, Bit score",
        "Query Seq-id",
        "Query GI",
        "Query accesion",
        "Query accesion.version",
        "Query sequence length",
        "Subject Seq-id",
        "All subject Seq-id(s), separated by a ';'",
        "Subject GI",
        "All subject GIs",
        "Subject accession",
        "Subject accession.version",
        "All subject accessions",
        "Subject sequence length",
        "Start of alignment in query",
        "End of alignment in query",
        "Start of alignment in subject",
        "End of alignment in subject",
        "Aligned part of query sequence",
        "Aligned part of subject sequence",
        "Expect value",
        "Bit score",
        "Raw score",
        "Alignment length",
        "Percentage of identical matches",
        "Number of identical matches",
        "Number of mismatches",
        "Number of positive-scoring matches",
        "Number of gap openings",
        "Total number of gaps",
        "Percentage of positive-scoring matches",
        "Query and subject frames separated by a '/'",
        "Query frame",
        "Subject frame",
        "Blast traceback operations (BTOP)",
        "unique Subject Taxonomy ID(s), separated by a ';' (in numerical order)",
        "unique Subject Scientific Name(s), separated by a ';'",
        "unique Subject Common Name(s), separated by a ';'",
        "unique Subject Blast Name(s), separated by a ';' (in alphabetical order)",
        "unique Subject Super Kingdom(s), separated by a ';' (in alphabetical order)",
        "Subject Title",
        "All Subject Title(s), separated by a '<>'",
        "Subject Strand",
        "Query Coverage Per Subject",
        "Query Coverage Per HSP"
    };

    static constexpr bool const implemented [] =
    {
        true,
        true,
        false,
        false,
        false,
        true,
        true,
        false,
        false,
        false,
        false,
        false,
        false,
        true,
        true,
        true,
        true,
        true,
        false,
        false,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        true,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
        false,
        false
    };
};

template <typename TVoidSpec>
constexpr char const * const
BlastMatchField<TVoidSpec>::optionLabels[45];

template <typename TVoidSpec>
constexpr char const * const
BlastMatchField<TVoidSpec>::columnLabels[45];

template <typename TVoidSpec>
constexpr char const * const
BlastMatchField<TVoidSpec>::descriptions[45];

template <typename TVoidSpec>
constexpr bool const
BlastMatchField<TVoidSpec>::implemented[45];

template <typename TVoidSpec>
constexpr std::array<typename BlastMatchField<TVoidSpec>::Enum const, 12>
BlastMatchField<TVoidSpec>::defaults;

// ============================================================================
// Metafunctions
// ============================================================================

template <BlastFormatProgram p, BlastFormatGeneration g>
constexpr
const char *
_seperatorString(BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                              p,
                              g> const & /*tag*/)
{
    return ", ";
}

template <BlastFormatProgram p, BlastFormatGeneration g>
constexpr
const char *
_seperatorString(BlastFormat<BlastFormatFile::TABULAR,
                              p,
                              g> const & /*tag*/)
{
    return "\t";
}

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// _firstOcc
// ----------------------------------------------------------------------------

template <typename TString>
inline typename Size<TString>::Type
_firstOcc(TString const & str, typename Value<TString>::Type const & val)
{
    typedef typename Size<TString>::Type S;
    for (S s = 0; s < length(str); ++s)
        if (value(str, s) == val)
            return s;
    return length(str);
}

// ----------------------------------------------------------------------------
// Function _writeField() [ match object given]
// ----------------------------------------------------------------------------

template <typename TStream,
          typename TQId,
          typename TSId,
          typename TPos,
          typename TAlign,
          typename TFormat,
          typename TFSpec = void
          >
inline int
_writeField(TStream & s,
            BlastMatch<TQId, TSId, TPos, TAlign> const & match,
            typename BlastMatchField<TFSpec>::Enum const fieldId,
            TFormat const &)
{
    typedef typename BlastMatchField<TFSpec>::Enum ENUM;
    int r = 0;
    switch (fieldId)
    {
        case ENUM::STD:
            r = _writeFields(s, match, BlastMatchField<>::defaults, TFormat());
            break;
        case ENUM::Q_SEQ_ID:
            r = streamPut(s, prefix(match.qId, _firstOcc(match.qId, ' ')));
            break;
//         case ENUM::Q_GI: r = streamPut(s,  * ); break;
//         case ENUM::Q_ACC: r = streamPut(s,  * ); break;
//         case ENUM::Q_ACCVER: r = streamPut(s,  * ); break;
        case ENUM::Q_LEN:
            r = streamPut(s, match.qLength);
            break;
        case ENUM::S_SEQ_ID:
            r = streamPut(s, prefix(match.sId, _firstOcc(match.sId, ' ')));
            break;
//         case ENUM::S_ALL_SEQ_ID: r = streamPut(s,  * ); break;
//         case ENUM::S_GI: r = streamPut(s,  * ); break;
//         case ENUM::S_ALL_GI: r = streamPut(s,  * ); break;
//         case ENUM::S_ACC: r = streamPut(s,  * ); break;
//         case ENUM::S_ACCVER: r = streamPut(s,  * ); break;
//         case ENUM::S_ALLACC: r = streamPut(s,  * ); break;
        case ENUM::S_LEN:
            r = streamPut(s, match.sLength);
            break;
        case ENUM::Q_START:
        {
            TPos effectiveQStart    = match.qStart;
            TPos effectiveQEnd      = match.qEnd;
            _untranslatePositions(effectiveQStart, effectiveQEnd,
                                  match.qFrameShift, match.qLength,
                                  QHasRevComp<TFormat>(),
                                  QIsTranslated<TFormat>());
            r = streamPut(s, effectiveQStart);
        } break;
        case ENUM::Q_END:
        {
            TPos effectiveQStart    = match.qStart;
            TPos effectiveQEnd      = match.qEnd;
            _untranslatePositions(effectiveQStart, effectiveQEnd,
                                  match.qFrameShift, match.qLength,
                                  QHasRevComp<TFormat>(),
                                  QIsTranslated<TFormat>());
            r = streamPut(s, effectiveQEnd);
        } break;
        case ENUM::S_START:
        {
            TPos effectiveSStart    = match.sStart;
            TPos effectiveSEnd      = match.sEnd;

            _untranslatePositions(effectiveSStart, effectiveSEnd,
                                  match.sFrameShift, match.sLength,
                                  SHasRevComp<TFormat>(),
                                  SIsTranslated<TFormat>());
            r = streamPut(s, effectiveSStart);
        } break;
        case ENUM::S_END:
        {
            TPos effectiveSStart    = match.sStart;
            TPos effectiveSEnd      = match.sEnd;

            _untranslatePositions(effectiveSStart, effectiveSEnd,
                                  match.sFrameShift, match.sLength,
                                  SHasRevComp<TFormat>(),
                                  SIsTranslated<TFormat>());
            r = streamPut(s, effectiveSEnd);
        } break;
//         case ENUM::Q_SEQ: r = streamPut(s,  * ); break;
//         case ENUM::S_SEQ: r = streamPut(s,  * ); break;
        case ENUM::E_VALUE:
            r = streamPut(s, match.eValue);
            break;
        case ENUM::BIT_SCORE:
            r = streamPut(s, match.bitScore);
            break;
        case ENUM::SCORE:
            r = streamPut(s, match.score);
            break;
        case ENUM::LENGTH:
            r = streamPut(s, match.aliLength);
            break;
        case ENUM::P_IDENT:
            r = streamPut(s, double(match.identities) * 100 / match.aliLength);
            break;
        case ENUM::N_IDENT:
            r = streamPut(s, match.identities);
            break;
        case ENUM::MISMATCH:
            r = streamPut(s, match.mismatches);
            break;
        case ENUM::POSITIVE:
            r = streamPut(s, match.positives);
            break;
        case ENUM::GAP_OPEN:
            r = streamPut(s, match.gapOpenings);
            break;
        case ENUM::GAPS:
            r = streamPut(s, match.gaps);
            break;
        case ENUM::P_POS:
            r = streamPut(s, double(match.positives) * 100 / match.aliLength);
            break;
        case ENUM::FRAMES:
            if (match.qFrameShift > 0)
                r = streamPut(s, '+');
            r += streamPut(s, match.qFrameShift);
            r += streamPut(s, '/');
            if (match.sFrameShift > 0)
                r += streamPut(s, '+');
            r += streamPut(s, match.sFrameShift);
            break;
        case ENUM::Q_FRAME:
            if (match.qFrameShift > 0)
                r = streamPut(s, '+');
            r += streamPut(s, match.qFrameShift);
            break;
        case ENUM::S_FRAME:
            if (match.sFrameShift > 0)
                r = streamPut(s, '+');
            r += streamPut(s, match.sFrameShift);
            break;
//         case ENUM::BTOP: streamPut( * ); break;
//         case ENUM::S_TAX_IDS: streamPut( * ); break;
//         case ENUM::S_SCI_NAMES: streamPut( * ); break;
//         case ENUM::S_COM_NAMES: streamPut( * ); break;
//         case ENUM::S_BLAST_NAMES: streamPut( * ); break;
//         case ENUM::S_S_KINGDOMS: streamPut( * ); break;
//         case ENUM::S_TITLE: streamPut( * ); break;
//         case ENUM::S_ALL_TITLES: streamPut( * ); break;
//         case ENUM::S_STRAND: streamPut( * ); break;
//         case ENUM::Q_COV_S: streamPut( * ); break;
//         case ENUM::Q_COV_HSP:
        default:
            r = streamPut(s, "n/i"); // not implemented
    };
    return r;
}

// ----------------------------------------------------------------------------
// Function _writeFields() [match object given]
// ----------------------------------------------------------------------------

template <typename TStream,
          typename TQId,
          typename TSId,
          typename TPos,
          typename TAlign,
          typename TFieldList,
          typename TFormat>
inline int
_writeFields(TStream & stream,
             BlastMatch<TQId, TSId, TPos, TAlign> const & match,
             TFieldList const & fields,
             TFormat const &)
{
    int ret = 0;

    for (auto it = seqan::begin(fields), itB = it, itEnd = seqan::end(fields);
         it != itEnd;
         ++it)
    {
        if (it != itB)
        {
            streamPut(stream, _seperatorString(TFormat()));
            if (ret)
                return ret;
        }

        ret = _writeField(stream, match, *it, TFormat());
        if (ret)
            return ret;
    }

    return streamPut(stream, '\n');
}

// ----------------------------------------------------------------------------
// Function _writeFieldLabels()
// ----------------------------------------------------------------------------

template <typename TStream,
          typename TFieldList,
          typename TFormat>
inline int
_writeFieldLabels(TStream & stream,
                  TFieldList const & fields,
                  TFormat const &)
{
    int ret = 0;

    for (auto it = seqan::begin(fields), itB = it, itEnd = seqan::end(fields);
         it != itEnd;
         ++it)
    {
        if (it != itB)
        {
            streamPut(stream, _seperatorString(TFormat()));
            if (ret)
                return ret;
        }

        ret = streamPut(stream, BlastMatchField<>::columnLabels[uint8_t(*it)]);
        if (ret)
            return ret;
    }

    return streamPut(stream, '\n');
}

// ----------------------------------------------------------------------------
// Function _writeFields() [no match object given] // or labels
// ----------------------------------------------------------------------------


template <typename TStream, BlastFormatFile f, BlastFormatProgram p,
          BlastFormatGeneration g>
inline int
_writeFields(TStream & /**/,
             BlastFormat<f, p, g> const & /*tag*/)
{
    return 0;
}

template <typename TStream, typename TField, typename... TFields,
          BlastFormatFile f, BlastFormatProgram p, BlastFormatGeneration g>
inline int
_writeFields(TStream & stream,
             BlastFormat<f, p, g> const & /*tag*/,
             TField const & field1, TFields const & ... fields)
{
    typedef BlastFormat<f, p, g> TFormat;
    int ret = streamPut(stream, _seperatorString(TFormat()));
    if (ret)
        return ret;

    ret = streamPut(stream, field1);
    if (ret)
        return ret;

    return _writeFields(stream, TFormat(), fields... );
}

// ----------------------------------------------------------------------------
// Function writeHeader()
// ----------------------------------------------------------------------------

/*!
 * @fn BlastRecord#writeHeader
 * @headerfile seqan/blast.h
 * @brief write the header of a @link BlastRecord @endlink to file
 * @signature int writeHeader(stream, blastRecord, blastFormatTag)
 * @signature int writeHeader(stream, qryId, dbname, [matchCount,] blastFormatTag[, labels...])
 *
 * This function writes the header of a record if @link BlastFormatFile @endlink
 * is TABULAR_WITH_HEADER (for TABULAR this is a no-op). Custom column labels
 * can be specified with the variadic fields argument (just pass a printable
 * argument for each label); use this if you use custom columns.
 *
 * If you use this function, you also need to print your matches manually with
 * @link BlastMatch#writeMatch @endlink .
 *
 * @param stream    The file to write to (FILE, fstream, @link Stream @endlink ...)
 * @param blastRecord    The @link BlastRecord @endlink whose header you want to print.
 * @param qryId     Alternatively the full ID of the query sequence (e.g. FASTA
 * one-line description)
 * @param dbName    The name of the database (NOT the ID of a subject sequence,
 * but the name of the database, e.g. "NCBI NR")
 * @param matchCount The amount of matches in the record. Mandatory parameter
 * for BlastFormatGeneration::BLAST_PLUS, optional for ::BLAST
 * @param blastFormatTag The @link BlastFormat @endlink specifier.
 * @param labels...   Optional custom column labels
 *
 * @return non-zero on I/O-error
 * @see BlastFormat
 * @see BlastRecord
 * @see BlastRecord#writeRecord
 */

template <typename TStream, typename TString1, typename TString2,
          BlastFormatProgram p, BlastFormatGeneration g>
inline int
_writeHeaderWithoutColumnLabels(TStream & stream,
                                TString1 const & qryId,
                                TString2 const & dbName,
                                BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                                            p,
                                            g> const & /*tag*/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,p,g> TFormat;

    int ret = streamPut(stream, "# ");
    if (ret)
        return ret;
    ret = streamPut(stream, _programTagToString(TFormat()));
    if (ret)
        return ret;
    ret = streamPut(stream, " I/O Module of SeqAn-");
    if (ret)
        return ret;
    ret = streamPut(stream, SEQAN_VERSION_MAJOR);
    if (ret)
        return ret;
    ret = streamPut(stream, '.');
    if (ret)
        return ret;
    ret = streamPut(stream, SEQAN_VERSION_MINOR);
    if (ret)
        return ret;
    ret = streamPut(stream, '.');
    if (ret)
        return ret;
    ret = streamPut(stream, SEQAN_VERSION_PATCH);
    if (ret)
        return ret;
    ret = streamPut(stream, " (http://www.seqan.de)\n# Query: ");
    if (ret)
        return ret;
    ret = streamPut(stream, qryId);
    if (ret)
        return ret;
    ret = streamPut(stream, "\n# Database: ");
    if (ret)
        return ret;
    ret = streamPut(stream, dbName);
    if (ret)
        return ret;
    ret = streamPut(stream, '\n');
    return ret;
}

// default fields
template <typename TStream,
          typename TString,
          typename TString2,
          typename TNumeric,
          BlastFormatProgram p,
          BlastFormatGeneration g>
inline int
writeHeader(TStream & stream,
            TString const & qryId,
            TString2 const & dbName,
            TNumeric const hitCount,
            BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        g> const & /**/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        g> TFormat;

    int ret = _writeHeaderWithoutColumnLabels(stream, qryId, dbName, TFormat());
    if (ret)
        return ret;

    ret = streamPut(stream, "# Fields: ");
    if (ret)
        return ret;

    ret = streamPut(stream, _defaultFields(TFormat()));
    if (ret)
        return ret;

    ret = streamPut(stream, '\n');
    if (ret)
        return ret;

    if (g == BlastFormatGeneration::BLAST_PLUS)
    {
        ret = streamPut(stream, "# ");
        if (ret)
            return ret;
        ret = streamPut(stream, hitCount);

        if (ret)
            return ret;

        ret = streamPut(stream, " hits found\n");
    }
    return ret;
}

// custom fields
template <typename TStream,
          typename TString,
          typename TString2,
          typename TNumeric,
          typename TField,
          typename... TFields,
          BlastFormatProgram p,
          BlastFormatGeneration g>
inline int
writeHeader(TStream & stream,
            TString const & qryId,
            TString2 const & dbName,
            TNumeric const hitCount,
            BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        g> const & /**/,
            TField const & field1,
            TFields const & ... fields)
{
    typedef BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        g> TFormat;

    int ret = _writeHeaderWithoutColumnLabels(stream, qryId, dbName, TFormat());
    if (ret)
        return ret;

    ret = streamPut(stream, "# Fields: ");
    if (ret)
        return ret;

    ret = streamPut(stream, field1);
    if (ret)
        return ret;

    ret = _writeFields(stream, TFormat(), fields...);
    if (ret)
        return ret;

    ret = streamPut(stream, '\n');
    if (ret)
        return ret;

    if (g == BlastFormatGeneration::BLAST_PLUS)
    {
        ret = streamPut(stream, "# ");
        if (ret)
            return ret;
        ret = streamPut(stream, hitCount);

        if (ret)
            return ret;

        ret = streamPut(stream, " hits found\n");
    }
    return ret;
}

// BlastFormatGeneration::BLAST works without hitCount aswell
template <typename TStream,
          typename TString1,
          typename TString2,
          BlastFormatProgram p,
          typename std::enable_if<IsSequence<TString1>::VALUE>::type = 0>
inline int
writeHeader(TStream & stream,
            TString1 const & qryId,
            TString2 const & dbName,
            BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        BlastFormatGeneration::BLAST> const & /**/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        BlastFormatGeneration::BLAST> TFormat;

    return writeHeader(stream,
                       qryId,
                       dbName,
                       0,
                       TFormat());
}

// BlastFormatGeneration::BLAST works without hitCount aswell, custom fields
template <typename TStream,
          typename TString,
          typename TString2,
          typename TField,
          typename... TFields,
          BlastFormatProgram p>
inline int
writeHeader(TStream & stream,
            TString const & qryId,
            TString2 const & dbName,
            BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        BlastFormatGeneration::BLAST> const & /**/,
            TField const & field1,
            TFields const & ... fields)
{
    typedef BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        BlastFormatGeneration::BLAST> TFormat;

    return writeHeader(stream,
                       qryId,
                       dbName,
                       0,
                       TFormat(),
                       field1,
                       fields...);
}

// Record parameter BLAST_PLUS
template <typename TStream,
          typename TRecord,
          typename TSpec,
          BlastFormatProgram p,
          BlastFormatGeneration g>
inline int
writeHeader(TStream & stream,
            TRecord const & record,
            BlastDbSpecs<TSpec> const & dbSpecs,
            BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        g> const & /**/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER, p, g>  TFormat;
    return writeHeader(stream, record.qId, dbSpecs.dbName,
                       record.matches.size(),TFormat());
}

// NO-OPS for TABULAR
template <typename TStream, typename TString1, typename TString2,
          BlastFormatProgram p, BlastFormatGeneration g>
constexpr int
writeHeader(TStream &, TString1 const &, TString2 const &,
            BlastFormat<BlastFormatFile::TABULAR, p, g> const & /*tag*/)
{
    return 0;
}

template <typename TStream,
          typename T,
          typename T2,
          typename... TFields,
          BlastFormatProgram p,
          BlastFormatGeneration g>
constexpr int
writeHeader(TStream & /**/,
            T const & /**/,
            T2 const & /**/,
            BlastFormat<BlastFormatFile::TABULAR,p,g> const & /*tag*/,
            TFields const & ... /**/)
{
    return 0;
}

template <typename TStream,
          typename T,
          typename T2,
          typename TNumeric,
          typename... TFields,
          BlastFormatProgram p,
          BlastFormatGeneration g>
constexpr int
writeHeader(TStream & /**/,
            T const & /**/,
            T2 const & /**/,
            TNumeric const /**/,
            BlastFormat<BlastFormatFile::TABULAR,p,g> const & /*tag*/,
            TFields const & ... /**/)
{
    return 0;
}

// ----------------------------------------------------------------------------
// Function writeMatch()
// ----------------------------------------------------------------------------

/*!
 * @fn BlastMatch#writeMatch
 * @headerfile seqan/blast.h
 * @brief write a @link BlastMatch @endlink to file
 * @signature int writeMatch(stream, blastMatch, [fields,] blastFormatTag)
 * @signature int writeMatch(stream, blastFormatTag, columns...)
 *
 * This function writes a single match if @link BlastFormatFile @endlink
 * is TABULAR_WITH_HEADER or TABULAR. The first signature causes default
 * columns to be printed, which is described <a href="https://www.ncbi.nlm.nih.gov/staff/tao/URLAPI/blastall/blastall_node93.html">
 * here</a>. Please note that BLAST is 1-indexed and considers the last position
 * to be the back, not the end, i.e. last one included in a match/sequence/...,
 * not the one behind it (as SeqAn does); this functions corrects for both
 * these bahaviours, so you don't have to. Additionally, based on your
 * @link BlastFormat @endlink, positions are transformed back to DNA space, if
 * translation has taken place.
 * Please note also that query and subject IDs are truncated at the first space
 * character in NCBI BLAST, this is also done by default here.
 * By passing the fields variable you manually specify which columns you want to
 * print, the same conversions mentioned above will me made. See 
 * @link BlastMatchField<>::Enum @endlink for a list of fields available.
 *
 * The second signature allows an arbitrary amount of and
 * arbitrary typed columns to be printed. If you do this and you use the
 * TABULAR_WITH_HEADER format, you should also print custom column labels
 * with @link BlastRecord#writeHeader @endlink. Please note that in this case
 * the adjustments mentioned above have to be done by yourself (if you use
 * the mentioned fields and you want compatability).
 *
 * Many guides recommend always printing the default columns and using only
 * additional columns with custom data.
 *
 * @param stream    The file to write to (FILE, fstream, @link Stream @endlink ...)
 * @param fields    A @link Sequence @endlink of @link BlastMatchField<>::Enum @endlink
 * @param blastMatch    The @link BlastMatch @endlink you wish to print.
 * @param blastFormatTag The @link BlastFormat @endlink specifier.
 * @param columns...   Custom columns
 *
 * @return non-zero on I/O-error
 * @see BlastFormat
 * @see BlastRecord
 * @see BlastRecord#writeRecord
 * @see BlastRecord#writeHeader
 */

// Function for arbitrary number and typed fields
template <typename TStream, typename TField, typename... TFields,
          BlastFormatProgram p, BlastFormatGeneration g>
inline int
writeMatch(TStream & stream,
           BlastFormat<BlastFormatFile::TABULAR,
                       p,
                       g> const & /*tag*/,
            TField const & field1,
            TFields const & ... fields)
{
    typedef BlastFormat<BlastFormatFile::TABULAR, p, g> TFormat;
    int ret = streamPut(stream, field1);
    if (ret)
        return ret;

    ret = _writeFields(stream, TFormat(), fields...);
    if (ret)
        return ret;
    return streamPut(stream, '\n');
}

// Function for match object and manually given colummns
template <typename TStream, typename TBlastMatch, typename TBlastMatchFields,
          BlastFormatProgram p, BlastFormatGeneration g>
inline int
writeMatch(TStream & stream, TBlastMatch const & match,
           TBlastMatchFields const & fields,
           BlastFormat<BlastFormatFile::TABULAR, p, g> const & /*tag*/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR, p, g> TFormat;
    return _writeFields(stream, match, fields, TFormat());
}

// Function for match object and default colummns
template <typename TStream, typename TBlastMatch,
          BlastFormatProgram p, BlastFormatGeneration g>
inline int
writeMatch(TStream & stream, TBlastMatch const & match,
           BlastFormat<BlastFormatFile::TABULAR, p, g> const & /*tag*/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR, p, g> TFormat;
    return _writeFields(stream, match, BlastMatchField<>::defaults, TFormat());
}

// writeMatch TABULAR_WITH_HEADER equal to TABULAR
template <typename TStream, typename TField, typename... TFields,
          BlastFormatProgram p, BlastFormatGeneration g>
inline int
writeMatch(TStream & stream,
           BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                       p,
                       g> const & /*tag*/,
            TField const & field1,
            TFields const & ... fields)
{
    typedef BlastFormat<BlastFormatFile::TABULAR, p, g> TFormat;
    return writeMatch(stream, TFormat(), field1, fields... );
}

template <typename TStream, typename TBlastMatch, typename TBlastMatchFields,
          BlastFormatProgram p, BlastFormatGeneration g>
inline int
writeMatch(TStream & stream, TBlastMatch const & match,
           TBlastMatchFields const & fields,
           BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER, p, g> const &/**/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR, p, g> TFormat;
    return writeMatch(stream, match, fields, TFormat());
}

template <typename TStream, typename TBlastMatch,
          BlastFormatProgram p, BlastFormatGeneration g>
inline int
writeMatch(TStream & stream, TBlastMatch const & match,
            BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER, p, g> const &/**/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR, p, g> TFormat;
    return writeMatch(stream, match, BlastMatchField<>::defaults, TFormat());
}

// ----------------------------------------------------------------------------
// Function writeTop()
// ----------------------------------------------------------------------------

// template <typename TStream,
//           typename TDbSpecs,
//           BlastFormatProgram p,
//           BlastFormatGeneration g>
// inline int
// writeTop(TStream                    & /**/,
//          TDbSpecs             const & /**/,
//          BlastFormat<BlastFormatFile::TABULAR, p, g> const & /*tag*/)
// {
//     return 0;
// }
// 
// template <typename TStream,
//           typename TDbSpecs,
//           BlastFormatProgram p,
//           BlastFormatGeneration g>
// inline int
// writeTop(TStream                    & /**/,
//          TDbSpecs             const & /**/,
//          BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER, p, g> const & /*tag*/)
// {
//     return 0;
// }

// ----------------------------------------------------------------------------
// Function writeRecord()
// ----------------------------------------------------------------------------

template <typename TStream,
          typename TRecord,
          typename TDbSpecs,
          BlastFormatFile f,
          BlastFormatProgram p,
          BlastFormatGeneration g>
inline int
_writeRecordImplTab(TStream                    & stream,
                    TRecord              const & record,
                    TDbSpecs             const & dbSpecs,
                    BlastFormat<f, p, g> const & /*tag*/)
{
    typedef BlastFormat<f, p, g> TFormat;

    //TODO if debug, do lots of sanity checks on record

    //NOOP for TABULAR
    int ret = writeHeader(stream, record, dbSpecs, TFormat());
    if (ret)
        return ret;
    for (auto it = record.matches.begin(); it != record.matches.end(); ++it)
    {
        //SOME SANITY CHECKS
        SEQAN_ASSERT_EQ(CharString(record.qId), CharString(it->qId));

        ret = writeMatch(stream, *it, TFormat());
        if (ret)
            return ret;
    }
    return 0;
}

template <typename TStream,
          typename TRecord,
          typename TDbSpecs,
          BlastFormatProgram p,
          BlastFormatGeneration g>
inline int
writeRecord(TStream             & stream,
            TRecord       const & record,
            TDbSpecs      const & dbSpecs,
            BlastFormat<BlastFormatFile::TABULAR,
                        p,
                        g> const & /*tag*/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR, p, g> TFormat;
    return _writeRecordImplTab(stream, record, dbSpecs, TFormat());
}

template <typename TStream,
          typename TRecord,
          typename TDbSpecs,
          BlastFormatProgram p,
          BlastFormatGeneration g>
inline int
writeRecord(TStream             & stream,
            TRecord       const & record,
            TDbSpecs      const & dbSpecs,
            BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER,
                        p,
                        g> const & /*tag*/)
{
    typedef BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER, p, g> TFormat;
    return _writeRecordImplTab(stream, record, dbSpecs, TFormat());
}

// ----------------------------------------------------------------------------
// Function writeBottom()
// ----------------------------------------------------------------------------


// template <typename TStream,
//           typename TDbSpecs,
//           typename TBlastScoringAdapater,
//           BlastFormatProgram p,
//           BlastFormatGeneration g>
// inline int
// writeBottom(TStream                           & /**/,
//             TDbSpecs                    const & /**/,
//             TBlastScoringAdapater       const & /**/,
//             BlastFormat<BlastFormatFile::TABULAR, p,g> const & /*tag*/)
// {
//     //TODO check if this is really NO-OP forr this format
//     return 0;
// }
// 
// template <typename TStream,
//           typename TDbSpecs,
//           typename TBlastScoringAdapater,
//           BlastFormatProgram p,
//           BlastFormatGeneration g>
// inline int
// writeBottom(TStream                           & /**/,
//             TDbSpecs                    const & /**/,
//             TBlastScoringAdapater       const & /**/,
//             BlastFormat<BlastFormatFile::TABULAR_WITH_HEADER, p,g> const & /*tag*/)
// {
//     //TODO check if this is really NO-OP forr this format
//     return 0;
// }

} // namespace seqan
#endif // header guard
