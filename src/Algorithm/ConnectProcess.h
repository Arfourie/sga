///-----------------------------------------------
// Copyright 2010 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL
//-----------------------------------------------
//
// ConnectProcess - Wrapper to resolve the sequence
// connecting two ends of a paired end read
//
#ifndef CONNECTROCESS_H
#define CONNECTPROCESS_H

#include "Util.h"
#include "OverlapAlgorithm.h"
#include "SequenceProcessFramework.h"
#include "SequenceWorkItem.h"
#include "MultiOverlap.h"
#include "Metrics.h"


class ConnectResult
{
    public:
        DNAString resolvedSequence;
};

//
class ConnectProcess
{
    public:
        ConnectProcess(const OverlapAlgorithm* pOverlapper, 
                       int minOverlap);

        ~ConnectProcess();

        ConnectResult process(const SequenceWorkItemPair& workPair);
    
    private:

        const OverlapAlgorithm* m_pOverlapper;
        const int m_minOverlap;
};

// Write the results from the overlap step to an ASQG file
class ConnectPostProcess
{
    public:
        ConnectPostProcess(std::ostream* pWriter);
        void process(const SequenceWorkItemPair& workPair, const ConnectResult& result);

    private:

        std::ostream* m_pWriter;
};

#endif
