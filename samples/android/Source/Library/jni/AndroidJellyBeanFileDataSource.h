/**
 * @file
 * Handle feeding input into the SinkPlayer
 */

/******************************************************************************
 * Copyright 2013, doubleTwist Corporation.
 * Copyright 2013, Qualcomm Innovation Center, Inc.
 *
 *    All rights reserved.
 *    This file is licensed under the 3-clause BSD license in the NOTICE.txt
 *    file for this project. A copy of the 3-clause BSD license is found at:
 *
 *        http://opensource.org/licenses/BSD-3-Clause.
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef _PCMFILE_DATASOURCE_H_
#define _PCMFILE_DATASOURCE_H_

#ifndef __cplusplus
#error Only include AudioCodec.h in C++ code.
#endif

#include <stdio.h>
#include <qcc/Thread.h>
#include <qcc/Mutex.h>
#include "DataSource.h"
#include "Constants.h"
#include <jni.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

struct MyAudioDataBuffer {
    MyAudioDataBuffer() : next(NULL) { };
    uint8_t*buffer;
    uint32_t bufferSize;
    MyAudioDataBuffer* next;
};

/* Explicitly requesting SL_IID_ANDROIDSIMPLEBUFFERQUEUE and SL_IID_PREFETCHSTATUS
 * on the AudioPlayer object for decoding, SL_IID_METADATAEXTRACTION for retrieving the
 * format of the decoded audio */
#define NUM_EXPLICIT_INTERFACES_FOR_PLAYER 3

/* Size of the decode buffer queue */
#define NB_BUFFERS_IN_QUEUE 4
/* Size of each buffer in the queue */
#define BUFFER_SIZE_IN_SAMPLES 1152 // number of samples per MP3 frame
#define BUFFER_SIZE_IN_BYTES   (2 * BUFFER_SIZE_IN_SAMPLES)

class AndroidJellyBeanFileDataSource : public DataSource {
  public:
    AndroidJellyBeanFileDataSource();

    bool setFilePath(const char*filePath);

    virtual ~AndroidJellyBeanFileDataSource();

    AudioFormat* getAudioFormat();
    uint32_t getInputSize();
    size_t ReadData(uint8_t* buffer, size_t offset, size_t length);
  private:
    bool getAudioFormat(AudioFormat* desc);
    bool getAudioDataStart(uint32_t* dataPos, uint32_t* dataSize);

    void Configure(const char* path);

    static qcc::ThreadReturn DecodeDataThread(void*arg);

  public:
    AudioFormat mInputFormat;
    uint32_t mInputSize;
    uint32_t mInputDataStart;
    qcc::Mutex mInputFileMutex;
    FILE* mInputFile;

    //Need circular buffer!
    MyAudioDataBuffer*pHead;
    MyAudioDataBuffer*pTail;
    /* Local storage for decoded audio data */
    int8_t pcmData[NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES];

    /* Used to signal prefetching failures */
    bool prefetchError;
    /* metadata key index for the PCM format information we want to retrieve */
    int channelCountKeyIndex;
    int sampleRateKeyIndex;
    /* size of the struct to retrieve the PCM format metadata values: the values we're interested in
     * are SLuint32, but it is saved in the data field of a SLMetadataInfo, hence the larger size.
     * Nate that this size is queried and displayed at l.452 for demonstration/test purposes.
     *  */
        #define PCM_METADATA_VALUE_SIZE 32
    /* used to query metadata values */
    SLMetadataInfo*pcmMetaData;
    /* we only want to query / display the PCM format once */
    bool formatQueried;


    SLObjectItf sl;
    SLEngineItf EngineItf;

    /* Objects this application uses: one audio player */
    SLObjectItf player;

    /* Interfaces for the audio player */
    SLAndroidSimpleBufferQueueItf decBuffQueueItf;
    SLPrefetchStatusItf prefetchItf;
    SLPlayItf playItf;
    SLMetadataExtractionItf mdExtrItf;

    /* Source of audio data for the decoding */
    SLDataSource decSource;
    SLDataLocator_URI decUri;
    SLDataFormat_MIME decMime;

    /* Data sink for decoded audio */
    SLDataSink decDest;
    SLDataLocator_AndroidSimpleBufferQueue decBuffQueue;
    SLDataFormat_PCM pcm;

    SLboolean required[NUM_EXPLICIT_INTERFACES_FOR_PLAYER];
    SLInterfaceID iidArray[NUM_EXPLICIT_INTERFACES_FOR_PLAYER];

};

#endif //_PCMFILE_DATASOURCE_H_
