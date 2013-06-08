/**
 * @file
 * Handle feeding input into the SinkPlayer
 */

/******************************************************************************
 * Copyright 2013, doubleTwist Corporation and Qualcomm Innovation Center, Inc.
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

#ifndef _WAVDATASOURCE_H_
#define _WAVDATASOURCE_H_

#ifndef __cplusplus
#error Only include WavDataSource.h in C++ code.
#endif

#include <alljoyn/audio/DataSource.h>
#include <qcc/Mutex.h>
#include <stdio.h>

namespace ajn {
namespace services {

/**
 * A WAV file data input source.
 */
class WavDataSource : public DataSource {
  public:
    WavDataSource();
    virtual ~WavDataSource();

    /**
     * Opens the file used to read data from.
     *
     * @param[in] inputFile the file pointer.
     *
     * @return true if open.
     */
    bool Open(FILE* inputFile);
    /**
     * Opens the file used to read data from.
     *
     * @param[in] filePath the file path.
     *
     * @return true if open.
     */
    bool Open(const char* filePath);
    /**
     * Closes the file used to read data from.
     */
    void Close();

    double GetSampleRate() { return mSampleRate; }
    uint32_t GetBytesPerFrame() { return mBytesPerFrame; }
    uint32_t GetChannelsPerFrame() { return mChannelsPerFrame; }
    uint32_t GetBitsPerChannel() { return mBitsPerChannel; }
    uint32_t GetInputSize() { return mInputSize; }

    size_t ReadData(uint8_t* buffer, size_t offset, size_t length);

  private:
    bool ReadHeader();

    double mSampleRate;
    uint32_t mBytesPerFrame;
    uint32_t mChannelsPerFrame;
    uint32_t mBitsPerChannel;
    uint32_t mInputSize;
    uint32_t mInputDataStart;
    qcc::Mutex mInputFileMutex;
    FILE* mInputFile;
};

}
}

#endif //_WAVDATASOURCE_H_
