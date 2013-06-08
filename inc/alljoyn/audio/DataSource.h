/**
 * @file
 * Data input sources.
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

#ifndef _DATASOURCE_H_
#define _DATASOURCE_H_

#ifndef __cplusplus
#error Only include DataSource.h in C++ code.
#endif

#include <stddef.h>
#include <stdint.h>

namespace ajn {
namespace services {

/**
 * The base class of data input sources.
 */
class DataSource {
  public:
    virtual ~DataSource() { }

    /**
     * @return the sample rate of the data source.
     */
    virtual double GetSampleRate() = 0;
    /**
     * @return the bytes per frame of the data source.
     */
    virtual uint32_t GetBytesPerFrame() = 0;
    /**
     * @return the channels per frame of the data source.
     */
    virtual uint32_t GetChannelsPerFrame() = 0;
    /**
     * @return the bits per channel of the data source.
     */
    virtual uint32_t GetBitsPerChannel() = 0;
    /**
     * @return the size of the data source in bytes.
     */
    virtual uint32_t GetInputSize() = 0;

    /**
     * Reads data from the source.
     *
     * @param[in] buffer the buffer to read data into.
     * @param[in] offset the byte offset from the beginning of the
     *                   data source to read from.
     * @param[in] length the length of buffer.
     *
     * @return the number of bytes read.
     */
    virtual size_t ReadData(uint8_t* buffer, size_t offset, size_t length) = 0;
};

}
}

#endif //_DATASOURCE_H_
