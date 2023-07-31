# Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd
# Released under the MIT license (see COPYING.MIT for the terms)

require recipes-kernel/linux/linux-yocto.inc
require linux-rockchip.inc

inherit local-git

SRCREV = "${@bb.utils.contains('KERNEL_RELEASE', 'NULL', \
		   '${FIREFLY_SDK_VERSION}', \
		   '${KERNEL_RELEASE}', \
		   d)}"

SRC_URI = " \
	git://gitlab.com/firefly-linux/kernel.git;protocol=https;nobranch=1;branch=${FIREFLY_SDK_BRANCH}; \
	file://${THISDIR}/files/cgroups.cfg \
"

KERNEL_VERSION_SANITY_SKIP = "1"
LINUX_VERSION ?= "4.4"

SRC_URI:append = " ${@bb.utils.contains('IMAGE_FSTYPES', 'ext4', \
		   'file://${THISDIR}/files/ext4.cfg', \
		   '', \
		   d)}"