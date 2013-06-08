/**
 * @file
 * The implementation of the org.alljoyn.Stream.Port.MetadataSink interface.
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

#ifndef _METADATASINKOBJECT_H
#define _METADATASINKOBJECT_H

#ifndef __cplusplus
#error Only include MetadataSinkObject.h in C++ code.
#endif

#include "PortObject.h"

namespace ajn {
namespace services {

/**
 * The object that implements the org.alljoyn.Stream.Port.MetadataSink interface.
 */
class MetadataSinkObject : public PortObject {
    friend class StreamObject;

  public:
    /**
     * The constructor.
     *
     * @param[in] bus the bus that this object exists on.
     * @param[in] path the object path for object.
     * @param[in] stream the parent stream object.
     *
     * @remark The supplied bus must not be deleted before this object
     * is deleted. If the caller registers this object on the AllJoyn
     * bus, it must be unregistered from the bus before it is deleted.
     */
    MetadataSinkObject(ajn::BusAttachment* bus, const char* path, StreamObject* stream);

  private:
    QStatus Get(const char* ifcName, const char* propName, ajn::MsgArg& val);

    void DoConnect(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);

    void MetadataSignalHandler(const ajn::InterfaceDescription::Member* member,
                               const char* sourcePath, ajn::Message& msg);
};

}
}

#endif /* _METADATASINKOBJECT_H */
