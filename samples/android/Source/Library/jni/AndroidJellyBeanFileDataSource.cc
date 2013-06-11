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

#include "AndroidJellyBeanFileDataSource.h"
#include <qcc/platform.h>
#include <qcc/Util.h>
#include "Constants.h"

using namespace std;
using namespace ajn;
using namespace qcc;

#define PREFETCHEVENT_ERROR_CANDIDATE \
        (SL_PREFETCHEVENT_STATUSCHANGE | SL_PREFETCHEVENT_FILLLEVELCHANGE)

 //-----------------------------------------------------------------
/* Callback for "prefetch" events, here used to detect audio resource opening errors */
void PrefetchEventCallback( SLPrefetchStatusItf caller,  void *pContext, SLuint32 event)
{
    SLpermille level = 0;
    SLresult result;
    result = (*caller)->GetFillLevel(caller, &level);
    ////ExitOnError(result);
    SLuint32 status;
    //LOGE("PrefetchEventCallback: received event %u\n", event);
    result = (*caller)->GetPrefetchStatus(caller, &status);
    ////ExitOnError(result);
    if ((PREFETCHEVENT_ERROR_CANDIDATE == (event & PREFETCHEVENT_ERROR_CANDIDATE))
            && (level == 0) && (status == SL_PREFETCHSTATUS_UNDERFLOW)) {
        LOGE("PrefetchEventCallback: Error while prefetching data, exiting\n");
        ((AndroidJellyBeanFileDataSource*)pContext)->prefetchError = true;
        //SignalEos();
    }
}

/* Callback for "playback" events, i.e. event happening during decoding */
void DecProgressCallback(
        SLPlayItf caller,
        void *pContext,
        SLuint32 event)
{
    SLresult result;
    SLmillisecond msec;
    result = (*caller)->GetPosition(caller, &msec);
    ////ExitOnError(result);

    if (SL_PLAYEVENT_HEADATEND & event) {
        LOGE("SL_PLAYEVENT_HEADATEND current position=%u ms\n", msec);
        //SignalEos();
    }

    if (SL_PLAYEVENT_HEADATNEWPOS & event) {
        LOGE("SL_PLAYEVENT_HEADATNEWPOS current position=%u ms\n", msec);
    }

    if (SL_PLAYEVENT_HEADATMARKER & event) {
        LOGE("SL_PLAYEVENT_HEADATMARKER current position=%u ms\n", msec);
    }
}

//-----------------------------------------------------------------
/* Callback for decoding buffer queue events */
void DecPlayCallback(
        SLAndroidSimpleBufferQueueItf queueItf,
        void *pContext)
{

	AndroidJellyBeanFileDataSource *pCntxt = (AndroidJellyBeanFileDataSource*)pContext;

    /* Save the decoded data  */
    //TODO ((AndroidJellyBeanFileDataSource*)pContext)->
//    if (fwrite(pCntxt->pDataBase, 1, BUFFER_SIZE_IN_BYTES, gFp) < BUFFER_SIZE_IN_BYTES) {
//        LOGE("Error writing to output file, signaling EOS\n");
//        //SignalEos();
//        return;
//    }

    /* Increase data pointer by buffer size */
//    pCntxt->pData += BUFFER_SIZE_IN_BYTES;
//
//    if (pCntxt->pData >= pCntxt->pDataBase + (NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES)) {
//        pCntxt->pData = pCntxt->pDataBase;
//    }

    ////ExitOnError(
    //TODO		(*queueItf)->Enqueue(queueItf, pCntxt->pDataBase, BUFFER_SIZE_IN_BYTES) ;//);
    // Note: adding a sleep here or any sync point is a way to slow down the decoding, or
    //  synchronize it with some other event, as the OpenSL ES framework will block until the
    //  buffer queue callback return to proceed with the decoding.

//#if 0
//    /* Example: buffer queue state display */
//    SLAndroidSimpleBufferQueueState decQueueState;
//    //ExitOnError( (*queueItf)->GetState(queueItf, &decQueueState) );
//
//    LOGE( "\DecBufferQueueCallback now has pCntxt->pData=%p queue: "
//            "count=%u playIndex=%u\n",
//            pCntxt->pData, decQueueState.count, decQueueState.index);
//#endif
//
//#if 0
//    /* Example: display duration in callback where we use the callback context for the SLPlayItf*/
//    SLmillisecond durationInMsec = SL_TIME_UNKNOWN;
//    SLresult result = (*pCntxt->playItf)->GetDuration(pCntxt->playItf, &durationInMsec);
//    //ExitOnError(result);
//    if (durationInMsec == SL_TIME_UNKNOWN) {
//        LOGE("Content duration is unknown (in dec callback)\n");
//    } else {
//        LOGE("Content duration is %ums (in dec callback)\n",
//                durationInMsec);
//    }
//#endif
//
//#if 0
//    /* Example: display position in callback where we use the callback context for the SLPlayItf*/
//    SLmillisecond posMsec = SL_TIME_UNKNOWN;
//    SLresult result = (*pCntxt->playItf)->GetPosition(pCntxt->playItf, &posMsec);
//    //ExitOnError(result);
//    if (posMsec == SL_TIME_UNKNOWN) {
//        LOGE("Content position is unknown (in dec callback)\n");
//    } else {
//        LOGE("Content position is %ums (in dec callback)\n",
//                posMsec);
//    }
//#endif

    /* Example: query of the decoded PCM format */
    if (pCntxt->formatQueried) {
        return;
    }
    SLresult res = (*pCntxt->mdExtrItf)->GetValue(pCntxt->mdExtrItf, pCntxt->sampleRateKeyIndex,
            PCM_METADATA_VALUE_SIZE, pCntxt->pcmMetaData);  ////ExitOnError(res);
    // Note: here we could verify the following:
    //         pcmMetaData->encoding == SL_CHARACTERENCODING_BINARY
    //         pcmMetaData->size == sizeof(SLuint32)
    //       but the call was successful for the PCM format keys, so those conditions are implied
    LOGE("sample rate = %dHz, ", *((SLuint32*)pCntxt->pcmMetaData->data));
    res = (*pCntxt->mdExtrItf)->GetValue(pCntxt->mdExtrItf, pCntxt->channelCountKeyIndex,
            PCM_METADATA_VALUE_SIZE, pCntxt->pcmMetaData);  ////ExitOnError(res);
    LOGE(" channel count = %d\n", *((SLuint32*)pCntxt->pcmMetaData->data));
    pCntxt->formatQueried = true;
}

AndroidJellyBeanFileDataSource::AndroidJellyBeanFileDataSource() : mInputFile(NULL), channelCountKeyIndex(-1),
		sampleRateKeyIndex(-1), pcmMetaData(NULL), prefetchError(false), formatQueried(false), pTail(NULL), pHead(NULL)
{
	SLresult    result;
	SLEngineOption EngineOption[] = {
			{(SLuint32) SL_ENGINEOPTION_THREADSAFE, (SLuint32) SL_BOOLEAN_TRUE}
	};

	result = slCreateEngine( &sl, 1, EngineOption, 0, NULL, NULL);
	result = (*sl)->Realize(sl, SL_BOOLEAN_FALSE);

	/* Get the SL Engine Interface which is implicit */
	result = (*sl)->GetInterface(sl, SL_IID_ENGINE, (void*)&EngineItf);
	//ExitOnError(result);

	/* Initialize arrays required[] and iidArray[] */
	for (int i=0 ; i < NUM_EXPLICIT_INTERFACES_FOR_PLAYER ; i++) {
		required[i] = SL_BOOLEAN_FALSE;
		iidArray[i] = SL_IID_NULL;
	}

	/* allocate memory to receive the PCM format metadata */
	if (!pcmMetaData) {
		pcmMetaData = (SLMetadataInfo*) malloc(PCM_METADATA_VALUE_SIZE);
	}

	formatQueried = false;
}

void AndroidJellyBeanFileDataSource::Configure(const char* path)
{
	SLresult    result;
	/* ------------------------------------------------------ */
	/* Configuration of the player  */

	/* Request the AndroidSimpleBufferQueue interface */
	required[0] = SL_BOOLEAN_TRUE;
	iidArray[0] = SL_IID_ANDROIDSIMPLEBUFFERQUEUE;
	/* Request the PrefetchStatus interface */
	required[1] = SL_BOOLEAN_TRUE;
	iidArray[1] = SL_IID_PREFETCHSTATUS;
	/* Request the PrefetchStatus interface */
	required[2] = SL_BOOLEAN_TRUE;
	iidArray[2] = SL_IID_METADATAEXTRACTION;

	/* Setup the data source */
	decUri.locatorType = SL_DATALOCATOR_URI;
	decUri.URI = (SLchar*)path;
	decMime.formatType = SL_DATAFORMAT_MIME;
	/*     this is how ignored mime information is specified, according to OpenSL ES spec
	 *     in 9.1.6 SLDataFormat_MIME and 8.23 SLMetadataTraversalItf GetChildInfo */
	decMime.mimeType      = (SLchar*)NULL;
	decMime.containerType = SL_CONTAINERTYPE_UNSPECIFIED;
	decSource.pLocator = (void *) &decUri;
	decSource.pFormat  = (void *) &decMime;

	/* Setup the data sink */
	decBuffQueue.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
	decBuffQueue.numBuffers = NB_BUFFERS_IN_QUEUE;
	/*    set up the format of the data in the buffer queue */
	pcm.formatType = SL_DATAFORMAT_PCM;
	// FIXME valid value required but currently ignored
	pcm.numChannels = 2;
	pcm.samplesPerSec = SL_SAMPLINGRATE_44_1;
	pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
	pcm.containerSize = 16;
	pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
	pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

	decDest.pLocator = (void *) &decBuffQueue;
	decDest.pFormat = (void * ) &pcm;

	/* Create the audio player */
	result = (*EngineItf)->CreateAudioPlayer(EngineItf, &player, &decSource, &decDest,
			NUM_EXPLICIT_INTERFACES_FOR_PLAYER, iidArray, required);
	//ExitOnError(result);
	LOGE("Player created\n");

	/* Realize the player in synchronous mode. */
	result = (*player)->Realize(player, SL_BOOLEAN_FALSE);
	//ExitOnError(result);
	LOGE("Player realized\n");

	/* Get the play interface which is implicit */
	result = (*player)->GetInterface(player, SL_IID_PLAY, (void*)&playItf);
	//ExitOnError(result);

	/* Set up the player callback to get events during the decoding */
	// FIXME currently ignored
	result = (*playItf)->SetMarkerPosition(playItf, 2000);
	//ExitOnError(result);
	result = (*playItf)->SetPositionUpdatePeriod(playItf, 500);
	//ExitOnError(result);
	result = (*playItf)->SetCallbackEventsMask(playItf,
			SL_PLAYEVENT_HEADATMARKER | SL_PLAYEVENT_HEADATNEWPOS | SL_PLAYEVENT_HEADATEND);
	//ExitOnError(result);
	result = (*playItf)->RegisterCallback(playItf, DecProgressCallback, NULL);
	//ExitOnError(result);
	LOGE("Play callback registered\n");

	/* Get the buffer queue interface which was explicitly requested */
	result = (*player)->GetInterface(player, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
			(void*)&decBuffQueueItf);
	//ExitOnError(result);

	/* Get the prefetch status interface which was explicitly requested */
	result = (*player)->GetInterface(player, SL_IID_PREFETCHSTATUS, (void*)&prefetchItf);
	//ExitOnError(result);

	/* Get the metadata extraction interface which was explicitly requested */
	result = (*player)->GetInterface(player, SL_IID_METADATAEXTRACTION, (void*)&mdExtrItf);
	//ExitOnError(result);

	/* ------------------------------------------------------ */
	/* Initialize the callback and its context for the decoding buffer queue */
	result = (*decBuffQueueItf)->RegisterCallback(decBuffQueueItf, DecPlayCallback, this);
	//ExitOnError(result);

	/* Enqueue buffers to map the region of memory allocated to store the decoded data */
	LOGE("Enqueueing buffer ");
	SLint8* base = (int8_t*)&pcmData;
	for(int i = 0 ; i < NB_BUFFERS_IN_QUEUE ; i++) {
		LOGE("%d ", i);
		result = (*decBuffQueueItf)->Enqueue(decBuffQueueItf, base, BUFFER_SIZE_IN_BYTES);
		//ExitOnError(result);
		base += BUFFER_SIZE_IN_BYTES;
	}
	LOGE("\n");

	/* ------------------------------------------------------ */
	/* Initialize the callback for prefetch errors, if we can't open the resource to decode */
	result = (*prefetchItf)->RegisterCallback(prefetchItf, PrefetchEventCallback, this);
	//ExitOnError(result);
	result = (*prefetchItf)->SetCallbackEventsMask(prefetchItf, PREFETCHEVENT_ERROR_CANDIDATE);
	//ExitOnError(result);

	/* ------------------------------------------------------ */
	/* Prefetch the data so we can get information about the format before starting to decode */
	/*     1/ cause the player to prefetch the data */
	result = (*ajbfds->playItf)->SetPlayState( ajbfds->playItf, SL_PLAYSTATE_PAUSED );
	//ExitOnError(result);
	/*     2/ block until data has been prefetched */
	SLuint32 prefetchStatus = SL_PREFETCHSTATUS_UNDERFLOW;
	SLuint32 timeOutIndex = 50; // time out prefetching after 5s
	while ((prefetchStatus != SL_PREFETCHSTATUS_SUFFICIENTDATA) && (timeOutIndex > 0) &&
			!ajbfds->prefetchError) {
		usleep(10 * 1000);
		(*ajbfds->prefetchItf)->GetPrefetchStatus(ajbfds->prefetchItf, &prefetchStatus);
		timeOutIndex--;
	}
	if (timeOutIndex == 0 || ajbfds->prefetchError) {
		LOGE( "Failure to prefetch data in time, exiting\n");
		//ExitOnError(SL_RESULT_CONTENT_NOT_FOUND);
	}

	/* ------------------------------------------------------ */
	/* Display duration */
	SLmillisecond durationInMsec = SL_TIME_UNKNOWN;
	result = (*ajbfds->playItf)->GetDuration(ajbfds->playItf, &durationInMsec);
	//ExitOnError(result);
	if (durationInMsec == SL_TIME_UNKNOWN) {
		LOGE("Content duration is unknown\n");
	} else {
		LOGE("Content duration is %ums\n", durationInMsec);
	}

	/* ------------------------------------------------------ */
	/* Display the metadata obtained from the decoder */
	//   This is for test / demonstration purposes only where we discover the key and value sizes
	//   of a PCM decoder. An application that would want to directly get access to those values
	//   can make assumptions about the size of the keys and their matching values (all SLuint32)
	SLuint32 itemCount;
	result = (*ajbfds->mdExtrItf)->GetItemCount(ajbfds->mdExtrItf, &itemCount);
	SLuint32 i, keySize, valueSize;
	SLMetadataInfo *keyInfo, *value;
	for(i=0 ; i<itemCount ; i++) {
		keyInfo = NULL; keySize = 0;
		value = NULL;   valueSize = 0;
		result = (*ajbfds->mdExtrItf)->GetKeySize(ajbfds->mdExtrItf, i, &keySize);
		//ExitOnError(result);
		result = (*ajbfds->mdExtrItf)->GetValueSize(ajbfds->mdExtrItf, i, &valueSize);
		//ExitOnError(result);
		keyInfo = (SLMetadataInfo*) malloc(keySize);
		if (NULL != keyInfo) {
			result = (*ajbfds->mdExtrItf)->GetKey(ajbfds->mdExtrItf, i, keySize, keyInfo);
			//ExitOnError(result);
			LOGE("key[%d] size=%d, name=%s \tvalue size=%d \n",
					i, keyInfo->size, keyInfo->data, valueSize);
			/* find out the key index of the metadata we're interested in */
			if (!strcmp((char*)keyInfo->data, ANDROID_KEY_PCMFORMAT_NUMCHANNELS)) {
				ajbfds->channelCountKeyIndex = i;
			} else if (!strcmp((char*)keyInfo->data, ANDROID_KEY_PCMFORMAT_SAMPLERATE)) {
				ajbfds->sampleRateKeyIndex = i;
			}
			free(keyInfo);
		}
	}
	if (ajbfds->channelCountKeyIndex != -1) {
		LOGE("Key %s is at index %d\n",
				ANDROID_KEY_PCMFORMAT_NUMCHANNELS, ajbfds->channelCountKeyIndex);
	} else {
		LOGE( "Unable to find key %s\n", ANDROID_KEY_PCMFORMAT_NUMCHANNELS);
	}
	if (ajbfds->sampleRateKeyIndex != -1) {
		LOGE("Key %s is at index %d\n",
				ANDROID_KEY_PCMFORMAT_SAMPLERATE, ajbfds->sampleRateKeyIndex);
	} else {
		LOGE( "Unable to find key %s\n", ANDROID_KEY_PCMFORMAT_SAMPLERATE);
	}
}

//bool AndroidJellyBeanFileDataSource::setFile(FILE* inputFile)
//{
//	mInputFile = inputFile;
//	if (!getAudioFormat(&mInputFormat)) {
//		LOGE( "file '%s' is not a PCM wave file\n", inputFile);
//		fclose(mInputFile);
//		mInputFile = NULL;
//		return false;
//	}
//
//	return true;
//}

bool AndroidJellyBeanFileDataSource::setFilePath(const char *filePath)
{
//	mInputFile = fopen(filePath, "rb");
//	if (mInputFile == NULL) {
//		LOGE( "can't open file '%s'\n", filePath);
//		return false;
//	}
	Configure(filePath);
	return true;//setFile(mInputFile);
}

AndroidJellyBeanFileDataSource::~AndroidJellyBeanFileDataSource()
{

    /* Destroy the AudioPlayer object */
    (*player)->Destroy(player);

	(*sl)->Destroy(sl);
}

AudioFormat* AndroidJellyBeanFileDataSource::getAudioFormat()
{
	return &mInputFormat;
}

uint32_t AndroidJellyBeanFileDataSource::getInputSize()
{
	return mInputSize;
}

bool AndroidJellyBeanFileDataSource::getAudioFormat(AudioFormat* desc)
{
	desc->mChannelsPerFrame = 2;
	desc->mSampleRate = 441000;
	desc->mBitsPerChannel = 16;
	desc->mBytesPerFrame = 4096;
	return true;
}

bool AndroidJellyBeanFileDataSource::getAudioDataStart(uint32_t* dataPos, uint32_t* dataSize)
{
	*dataPos = 0;
	*dataSize = 100000000;

    return true;
}

size_t AndroidJellyBeanFileDataSource::ReadData(uint8_t* buffer, size_t offset, size_t length)
{
	mInputFileMutex.Lock();

	mInputFileMutex.Unlock();
	return 0;
}

ThreadReturn AndroidJellyBeanFileDataSource::DecodeDataThread(void* arg)
{
	AndroidJellyBeanFileDataSource* ajbfds = (AndroidJellyBeanFileDataSource*)arg;
	SLresult    result;




	    /* ------------------------------------------------------ */
	    /* Start decoding */
	    result = (*ajbfds->playItf)->SetPlayState(ajbfds->playItf, SL_PLAYSTATE_PLAYING);
	    //ExitOnError(result);
	    LOGE("Starting to decode\n");

	    /* Decode until the end of the stream is reached */
	    //TODO
//	    {
//	        android::Mutex::Autolock autoLock(eosLock);
//	        while (!eos) {
//	            eosCondition.wait(eosLock);
//	        }
//	    }
	    LOGE("EOS signaled\n");

	    /* ------------------------------------------------------ */
	    /* End of decoding */

	    /* Stop decoding */
	    result = (*ajbfds->playItf)->SetPlayState(ajbfds->playItf, SL_PLAYSTATE_STOPPED);
	    //ExitOnError(result);
	    LOGE("Stopped decoding\n");

	    return NULL;
}

