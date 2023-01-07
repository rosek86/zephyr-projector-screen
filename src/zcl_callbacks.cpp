/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <lib/support/logging/CHIPLogging.h>

#include "app_task.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>

#include "libs/projector_thread.h"
#include "libs/screen_thread.h"

using namespace ::chip;
using namespace ::chip::app::Clusters;
using namespace ::chip::app::Clusters::OnOff;

static void update_projector(uint8_t *value)
{
	ChipLogProgress(Zcl, "Cluster OnOff: attribute OnOff set to %" PRIu8 ", %d", *value);

	*value ? projector_power_on() : projector_power_off();

	AppTask::Instance().GetPWMDevice().InitiateAction(*value ? PWMDevice::ON_ACTION : PWMDevice::OFF_ACTION,
							  static_cast<int32_t>(AppEventType::Lighting), value);
}

static void update_screen(uint8_t *value)
{
	*value ? screen_down() : screen_up();
}

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath &attributePath, uint8_t type,
				       uint16_t size, uint8_t *value)
{
	ClusterId clusterId = attributePath.mClusterId;
	AttributeId attributeId = attributePath.mAttributeId;
	EndpointId endpointId = attributePath.mEndpointId;

	if (clusterId == OnOff::Id && attributeId == OnOff::Attributes::OnOff::Id) {
		switch (endpointId) {
			case 1:
				update_projector(value);
				break;
			case 2:
				update_screen(value);
				break;
		}
	}
}

/** @brief OnOff Cluster Init
 *
 * This function is called when a specific cluster is initialized. It gives the
 * application an opportunity to take care of cluster initialization procedures.
 * It is called exactly once for each endpoint where cluster is present.
 *
 * @param endpoint   Ver.: always
 *
 * TODO Issue #3841
 * emberAfOnOffClusterInitCallback happens before the stack initialize the cluster
 * attributes to the default value.
 * The logic here expects something similar to the deprecated Plugins callback
 * emberAfPluginOnOffClusterServerPostInitCallback.
 *
 */
void emberAfOnOffClusterInitCallback(EndpointId endpoint)
{
	EmberAfStatus status;
	bool storedValue;

	/* Read storedValue on/off value */
	status = Attributes::OnOff::Get(endpoint, &storedValue);
	if (status == EMBER_ZCL_STATUS_SUCCESS) {
		/* Set actual state to the cluster state that was last persisted */
		AppTask::Instance().GetPWMDevice().InitiateAction(
			storedValue ? PWMDevice::ON_ACTION : PWMDevice::OFF_ACTION,
			static_cast<int32_t>(AppEventType::Lighting), reinterpret_cast<uint8_t *>(&storedValue));
	}

	AppTask::Instance().UpdateClusterState();
}
