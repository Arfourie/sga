///----------------------------------------------
// Copyright 2011 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL
//-----------------------------------------------
//
// HapgenUtil - Utility functions used for 
// working with the haplotype generation
// modules and output
//
#ifndef HAPGENUTIL_H
#define HAPGENUTIL_H

#include "BWT.h"
#include "BWTIntervalCache.h"
#include "SampledSuffixArray.h"

// Simple alignment object representing
// the placement of a string onto a reference genome
struct HapgenAlignment
{

    //
    HapgenAlignment(const int& id, int p, int l, int s, bool rc) : referenceID(id), position(p), length(l), score(s), isRC(rc) {}

    // Sort
    friend bool operator<(const HapgenAlignment& a, const HapgenAlignment& b)
    {
        if(a.referenceID == b.referenceID)
            return a.position < b.position;
        else
            return a.referenceID < b.referenceID;
    }

    // Output
    friend std::ostream& operator<<(std::ostream& o, const HapgenAlignment& align) 
    { 
        o << "R_ID: " << align.referenceID << " P: [" << align.position << ", " << align.position + align.length << "]"; 
        return o; 
    }

    //
    int referenceID;
    int position;
    int length; // alignment length
    int score;
    bool isRC;
};
typedef std::vector<HapgenAlignment> HapgenAlignmentVector;

//
namespace HapgenUtil
{


    // Align the haplotype to the reference genome represented by the BWT/SSA pair
    void alignHaplotypeToReference(const std::string& haplotype,
                                   const BWT* pReferenceBWT,
                                   const SampledSuffixArray* pReferenceSSA,
                                   HapgenAlignmentVector& outAlignments);

    // Coalesce a set of alignments into distinct locations
    void coalesceAlignments(HapgenAlignmentVector& alignments);

    // Extract a substring for a reference defined by the alignment
    std::string extractReference(const HapgenAlignment& aln, const ReadTable* pRefTable, int flanking);

    // Extract substrings of a reference described by an alignment
    // This writes to 3 out parameters, one for the defined position
    // of the reference, one for the upstream flanking bases and one for the downstream flanking
    void extractReferenceSubstrings(const HapgenAlignment& aln, 
                                    const ReadTable* pRefTable, 
                                    int flanking,
                                    std::string& outUpstream, 
                                    std::string& outDefined, 
                                    std::string& outDownstream);
    
    // Make haplotypes for the given reference alignment, including flanking sequences
    // A haplotype for the reference is also generated
    bool makeFlankingHaplotypes(const HapgenAlignment& aln, 
                                const ReadTable* pRefTable, 
                                int flanking,
                                StringVector& inHaplotypes,
                                StringVector& outHaplotypes);


    // Extract reads from an FM-index that have a k-mer match to any given haplotypes
    void extractHaplotypeReads(const StringVector& haplotypes, 
                               const BWT* pBWT, 
                               const BWTIntervalCache* pBWTCache,
                               const SampledSuffixArray* pSSA,
                               int k,
                               bool doReverse,
                               SeqItemVector* pOutReads, 
                               SeqItemVector* pOutMates);

    // Check that all the strings in the vector align to the same coordinates
    // of the passed in sequence
    bool checkAlignmentsAreConsistent(const std::string& refString, const StringVector& queries);

    // Print an alignment to a reference
    void printAlignment(const std::string& query, const HapgenAlignment& aln, const ReadTable* pRefTable);

};

#endif