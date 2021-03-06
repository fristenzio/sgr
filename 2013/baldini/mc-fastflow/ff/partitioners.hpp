/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

/*! 
 *  \file partitioners.hpp
 *  \brief This file describes the partitioner used in the map skeleton.
 */
 
#ifndef _FF_PART_HPP_
#define _FF_PART_HPP_
/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License version 3 as 
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *
 ****************************************************************************
 */


namespace ff {





struct basePartition {
    basePartition():task(NULL), len(0) {}
    
    inline void   setData(void* t) {task=t;}
    inline void   setLength(size_t l) {len=l;}
    inline void*  getData() { return task;}
    inline size_t getLength() { return len;}
    
    void*  task;
    size_t len;
};

/*!
 *  \ingroup runtime
 *
 *  @{
 */

/*!
 * \class basePartitioner
 *
 * \brief A basic interface for the partitioner used by the Map skeleton.
 */    
class basePartitioner {
public:
    basePartitioner(int parts):parts(parts) {}
    virtual void getPartition(const int threadId, basePartition& P)=0;
    virtual void setTask(void* task)=0;
    
    const int getParts() const { return parts;}
protected:
    int parts;
};

/*!
 *  @}
 */
 
/*!
 *  \ingroup runtime
 *
 *  @{
 */
    
    
// one dimensional array partitioner
/*!
 * \class LinearPartitioner
 *
 * \brief A linear partitioner.
 *
 * A one-dimensional array partitioner for the Map skeleton.
 */
template<typename T>
class LinearPartitioner: public basePartitioner {
public:
    typedef basePartition partition_t;

    /** Default constructor */
    LinearPartitioner(size_t nElements, int nThreads) :
         basePartitioner((nElements>(size_t)nThreads) ? nThreads : nElements), 
         task(NULL), q(nElements / nThreads), r(nElements % nThreads) 
         {
         }
          
    /**
     *  Get a partition
     */
    inline void getPartition(const int threadId, basePartition& P) {
        const size_t start = (threadId * q) + ((r >= (size_t)threadId) ? threadId : r);
        P.setData(task+start);
        P.setLength(((size_t)threadId<r)?(q+1):q);
    }
    
    /// Set the task
    inline void setTask(void* t) { task = (T*)t; }
    
protected:
    T* task;
    const size_t q;  // Num_of_Elements / Num_of_threads
    const size_t r;  // Num_of_Elements % Num_of_threads
};

/*!
 *  @}
 */

// REW
struct basePartitionList: public basePartition {
    basePartitionList():task(NULL), len(0) {}
    
    inline void   setData(void* t) {task=t;}
    inline void   setLength(size_t l) {len=l;}
    inline void*  getData() { return task;}
    inline size_t getLength() { return len;}
    
    void*  task;
    size_t len;
};



#if 0
    // caso base list di N elementi tutti della stessa size e dello stesso tipo

template<typename T>
class LinearPartitionerList: public basePartitioner {
public:
    typedef PartitionList partition_t;

    /** Default constructor */
    LinearPartitionerList(size_t nElements, int nThreads) :
         basePartitioner((nElements>(size_t)nThreads) ? nThreads : nElements), 
         task(NULL), q(nElements / nThreads), r(nElements % nThreads) 
         {
         }

     /**
     *  Get a partition
     */
    inline void getPartition(const int threadId, basePartition& P) {
        const size_t start = (threadId * q) + ((r >= (size_t)threadId) ? threadId : r);

        basePartitionList& pList = *(basePartitionLinst*)&P;

        T::iterator b=T->begin(), e=T->end();
        for(int i=0; b!=e; ++b, ++i) //{
            pList[i].setData((*b) + start);
        //pList[i].
        
        for(size_t i=0;i<listSize;++i) {
            P.setData(task+start);
            P.setLength(((size_t)threadId<r)?(q+1):q);
        }
    
        inline void setTask(void* t) { task = (T*)t; }
    };
    
#endif

    
    
} // namespace ff

#endif /* _FF_PART_HPP_ */
