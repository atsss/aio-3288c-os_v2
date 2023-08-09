LICENSE = "GPLv2"

# Base this image on core-image-sato
include recipes-core/images/core-image-sato.bb

IMAGE_INSTALL:append = " packagegroup-rockchip"
