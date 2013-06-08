/**
 * @file
 * Definition of system clock interface.
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

#ifndef _CLOCK_H
#define _CLOCK_H

#include <stdint.h>
#include <time.h>

namespace ajn {
namespace services {

#ifdef CLOCK_REALTIME
__inline__ uint64_t GetCurrentTimeNanos() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ((uint64_t)ts.tv_sec * 1000000000) + ts.tv_nsec;
}
__inline__ void SleepNanos(uint64_t nanos) {
    struct timespec req;
    struct timespec rem;
    req.tv_sec = nanos / 1000000000;
    req.tv_nsec = nanos % 1000000000;
    nanosleep(&req, &rem);
}
#endif /* CLOCK_REALTIME */

}
}

#endif /* _CLOCK_H */
