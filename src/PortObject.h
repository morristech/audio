/**
 * @file
 * The implementation of the org.alljoyn.Stream.Port interface.
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

#ifndef _PORTOBJECT_H
#define _PORTOBJECT_H

#ifndef __cplusplus
#error Only include PortObject.h in C++ code.
#endif

#include "Sink.h"
#include <alljoyn/BusAttachment.h>
#include <alljoyn/BusObject.h>

namespace ajn {
namespace services {

class StreamObject;

/**
 * The object that implements the org.alljoyn.Stream.Port interface.
 */
class PortObject : public ajn::BusObject {
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
    PortObject(ajn::BusAttachment* bus, const char* path, StreamObject* stream);

    virtual ~PortObject();

  protected:
    /**
     * Emits the OwnershipLost signal.
     *
     * This should be called when someone else takes over the port.
     *
     * @param[in] newOwner the new owner of the port.
     *
     * @return ER_OK if emitted.
     */
    QStatus EmitOwnershipLostSignal(const char* newOwner);

    /**
     * Does cleanup that can't be done in destructor.  This must be
     * called before object is deleted.
     */
    virtual void Cleanup();

    /**
     * Handles a bus request to read a property from this object.
     *
     * @param[in] ifcName the interface that the property is defined on.
     * @param[in] propName the property to get.
     * @param[out] val the property value.  The type of this value is
     *                  the actual value type.
     *
     * @return ER_OK if got.
     */
    QStatus Get(const char* ifcName, const char* propName, ajn::MsgArg& val);

    /**
     * Gets the value of a parameter from a capability or configuration.
     *
     * @param[in] parameters an array of parameters.
     * @param[in] numParameters the number of parameters in the array.
     * @param[in] name the name of the parameter.
     *
     * @return the value of the parameter.
     */
    ajn::MsgArg* GetParameterValue(ajn::MsgArg* parameters, size_t numParameters, const char* name);

  private:
    void Connect(const ajn::InterfaceDescription::Member* member, ajn::Message& msg);
    virtual void DoConnect(const ajn::InterfaceDescription::Member* member, ajn::Message& msg) = 0;

    bool IsConfigurable(char* type, size_t numParameters, ajn::MsgArg* parameters);

  protected:
    uint8_t mDirection; /**< Either DIRECTION_SOURCE or DIRECTION_SINK. */
    Capability* mConfiguration;  /**< The current configuration of the port. */
    size_t mNumCapabilities; /**< The number of capabilities of the port. */
    Capability* mCapabilities;  /**< The capabilities of the port. */
    StreamObject* mStream;  /**< The parent stream object. */

  private:
    const ajn::InterfaceDescription::Member* mOwnershipLostMember;
};

}
}

#endif /* _PORTOBJECT_H */
