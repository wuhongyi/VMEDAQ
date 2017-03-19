/* $Id: TArtDataSource.h 4 2009-09-02 15:59:29Z sweetbluemoon $ */
/**
 * @file   TArtDataSource.h
 * @date   Created : Mar 29, 2008 12:29:43 JST<BR>
 *   Last Modified : Dec 04, 2008 21:57:42 JST
 * @author Shinsuke OTA <ota@ribf.riken.jp>
 *  
 *  
 *    Copyright (C)2008
 */
#ifndef TARTDATASOURCE_H
#define TARTDATASOURCE_H

/** enumuration for the status of extraction for data block */
typedef enum {
   kNORMALBLOCK, ///< good block
   kWAIT, ///< wating for new block
   kRUNSTOP, ///< run is stopped (online mode)
   kEOF, ///< reach at end of file (offline mode)
   kERRORBLOCK ///< fatal error occurred
} ArtBlockStat_t;

/** enumeration for the status of extraction for event data */
typedef enum {
  kERROREVENT = 0,///< fatal error occurred
  kNORMALEVENT,  ///< good event
  kCOMMENT, ///< comment
  kEOB ///< reach at end of block
} ArtEventStat_t;

/** enumeration for the file status */
typedef enum {
   kEXIST = 0, ///< file exists
   kNOTEXIST ///< file does not exists (whichever error or not)
} ArtFileStat_t;

typedef enum {
   kFILE = 0, ///< file 
   kSM,       ///< sm
   kStream
} ArtDataSource_t;


/** 
 * This class is a abstract base class for dealing with a data source.
 *
 * Several methods such as TArtDataSource::Open,
 * TArtDataSource::Close, TArtDataSource::Read and
 * TArtDataSource::GetNextBlock should be implemented in each derived
 * class.
 * The derived class itself might be a singleton.
 */
class TArtDataSource {
public:
   static const int kMaxBufferSize;
   virtual ~TArtDataSource();
   virtual bool SMOpen(const int sid = 0){return false;}
   virtual bool FileOpen(const char* sourceName = 0){return false;}
   virtual bool SocketOpen(const char* hostname = 0, const int port = 0, const bool fromlast = false){return false;}
   virtual int GetRunNumber() = 0;
   virtual void Close() = 0;
   virtual int Read(char *buf,const int& size, const int &offset = 0) = 0;
   virtual int Seek(long offset, int origin) = 0;
   // mainly for shared memory data source to check data exists or not
   // and lock semaphore
   virtual int IsPrepared() { return 1; };
   // mainly for shared memory data source to unlock semaphore
   virtual int Wait() { return 1; }

   void SetBlockStatus(ArtBlockStat_t estat){fBlockStatus = estat;}
   void SetFileStatus (ArtFileStat_t estat) {fFileStatus = estat;}
   ArtBlockStat_t GetBlockStatus(){return fBlockStatus;}
   ArtFileStat_t  GetFileStatus() {return fFileStatus;}
   ArtDataSource_t  GetDataSourceType() {return fDataSourceType;}

protected:
   TArtDataSource(int id = 0);
   int fID;
   ArtBlockStat_t  fBlockStatus;
   ArtFileStat_t   fFileStatus;
   ArtDataSource_t fDataSourceType;
private:
   
};
#endif // end of #ifdef TARTDATASOURCE_H
