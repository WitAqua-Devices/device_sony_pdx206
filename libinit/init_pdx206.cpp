/*
 * Copyright (C) 2026 The WitAqua Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>

#include <android-base/logging.h>

#include "sku.h"

/*
 * pdx206 ships as one image for every SKU, so build.prop describes the global
 * XQ-AS72 and the japanese variants only get their model name back.
 *
 * That is not always enough. Anything identifying the device from its build -
 * Osaifu-Keitai provisioning among others - sees a model Sony never sold with
 * the hardware it is looking at, so those devices also want the brand, product
 * name and build fingerprint of their stock firmware. This hook is where that
 * belongs; device/sony/pdx203/libinit does exactly that for SO-51A and SOG01
 * and is the template to follow for SO-52A, SOG02 and A002SO.
 *
 * The values have to be copied verbatim out of a retail dump - ro.build.id,
 * ro.build.version.incremental and ro.product.<partition>.{brand,name,device}
 * - because a made up fingerprint is worse than the global one. Note that
 * putting ro.product.device on the japanese name also means adding that name
 * to TARGET_OTA_ASSERT_DEVICE in BoardConfig.mk and to board-info.txt, or OTA
 * packages stop applying.
 */
void vendor_load_properties() {
    std::string model = sony::DetectLtaModel();
    if (model.empty()) {
        LOG(INFO) << "libinit: no japanese model in LTALabel, keeping the built-in identity";
        return;
    }

    LOG(INFO) << "libinit: detected " << model;
    sony::SetModelProperties(model);
}
