# Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd
# Released under the MIT license (see COPYING.MIT for the terms)

inherit python3-dir

DEPENDS:append = " openssl-native lz4-native ${PYTHON_PN}-native"

LINUX_VERSION_EXTENSION ?= "-rockchip-${LINUX_KERNEL_TYPE}"

PATCHPATH = "${THISDIR}/${BPN}_${LINUX_VERSION}"
inherit auto-patch

KCONFIG_MODE ?= "--alldefconfig"

# Make sure we use /usr/bin/env ${PYTHON_PN} for scripts
do_patch:append() {
	for s in `grep -rIl python ${S}/scripts`; do
		sed -i -e '1s|^#!.*python[23]*|#!/usr/bin/env ${PYTHON_PN}|' $s
	done

	cp ${STAGING_KERNEL_DIR}/ramdisk.img ${B}/

	#Copy some files needed by the firefly kernel
	if [ -f "${STAGING_KERNEL_DIR}/drivers/spi/spi-wk2xxx_64bit" ];then
		install -d ${B}/drivers/spi/
		install ${STAGING_KERNEL_DIR}/drivers/spi/spi-wk2xxx_64bit ${B}/drivers/spi/
	fi

	if [ -f "${STAGING_KERNEL_DIR}/drivers/spi/spi-wk2xxx_32bit" ];then
		install -d ${B}/drivers/spi/
		install ${STAGING_KERNEL_DIR}/drivers/spi/spi-wk2xxx_32bit ${B}/drivers/spi/
	fi

	if [ -f "${STAGING_KERNEL_DIR}/drivers/char/virtd" ];then
		install -d ${B}/drivers/char/
		install ${STAGING_KERNEL_DIR}/drivers/char/virtd ${B}/drivers/char/
	fi

	if [ -f "${STAGING_KERNEL_DIR}/drivers/mmc/core/mmc_blk_data" ];then
		install -d ${B}/drivers/mmc/core/
		install ${STAGING_KERNEL_DIR}/drivers/mmc/core/mmc_blk_data ${B}/drivers/mmc/core/
	fi

	if [ -d "${STAGING_KERNEL_DIR}/drivers/net/wireless/rockchip_wlan/rtl8821cu" ];then
		install -d ${B}/drivers/net/wireless/rockchip_wlan/
		cp -rf ${STAGING_KERNEL_DIR}/drivers/net/wireless/rockchip_wlan/rtl8821cu ${B}/drivers/net/wireless/rockchip_wlan/
	fi

	if [ -d "${STAGING_KERNEL_DIR}/drivers/net/wireless/rockchip_wlan/rkwifi" ];then
		install -d ${B}/drivers/net/wireless/rockchip_wlan/
		cp -rf ${STAGING_KERNEL_DIR}/drivers/net/wireless/rockchip_wlan/rkwifi ${B}/drivers/net/wireless/rockchip_wlan/
	fi

	# #Copy the header files needed by mail
	# install -d ${B}/drivers/gpu/arm/mali400/mali/
	# cp -rf ${STAGING_KERNEL_DIR}/drivers/gpu/arm/mali400/mali/include ${B}/drivers/gpu/arm/mali400/mali/
}

do_compile:prepend() {
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${STAGING_LIBDIR_NATIVE}
}

do_compile_kernelmodules:prepend() {
        export PKG_CONFIG_DIR="${STAGING_DIR_NATIVE}${libdir_native}/pkgconfig"
        export PKG_CONFIG_PATH="$PKG_CONFIG_DIR:${STAGING_DATADIR_NATIVE}/pkgconfig"
        export PKG_CONFIG_LIBDIR="$PKG_CONFIG_DIR"
        export PKG_CONFIG_SYSROOT_DIR=""
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${STAGING_LIBDIR_NATIVE}
}

# Hack for rockchip style images
ROCKCHIP_KERNEL_IMAGES = "boot.img zboot.img"
KERNEL_IMAGETYPES:append = " ${ROCKCHIP_KERNEL_IMAGES}"
python () {
    if not d.getVar('KERNEL_DEVICETREE'):
        raise bb.parse.SkipPackage('KERNEL_DEVICETREE is not specified!')

    # Use rockchip stype target, which is '<dts(w/o suffix)>.img'
    d.setVar('KERNEL_IMAGETYPE_FOR_MAKE', ' ' + d.getVar('KERNEL_DEVICETREE').replace('rockchip/', '').replace('.dtb', '.img'));
}

# Force regenerating defconfig
do_kernel_metadata:prepend() {
	rm -f ${WORKDIR}/defconfig
}

# Link rockchip style images
do_install:prepend() {
	for image in ${ROCKCHIP_KERNEL_IMAGES};do
		ln -sf ${B}/${image} ${B}/arch/${ARCH}/boot/
	done

	if [ "x${RK_USE_COMPRESSED_KERNEL}" = "x1" ]; then
		ln -sf zboot.img ${B}/arch/${ARCH}/boot/boot.img
	fi
}
