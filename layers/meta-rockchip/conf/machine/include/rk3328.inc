# Copyright (c) 2020, Fuzhou Rockchip Electronics Co., Ltd
# Released under the MIT license (see COPYING.MIT for the terms)

MACHINEOVERRIDES =. "rk3328:"
SOC_FAMILY ?= "rk3328"
require conf/machine/include/rockchip-arm64-common.inc

MALI_GPU := "utgard-450"
MALI_VERSION ?= "r7p0"
