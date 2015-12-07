#
# Copyright 2015, RedBear Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of RedBear Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of RedBear Corporation.
#

NAME := I2C_Application

$(NAME)_SOURCES := i2c.c \
                   SeeedGrayOLED.c 
                   
$(NAME)_INCLUDES   := .

VALID_PLATFORMS += RB_DUO \
                   RB_DUO_ext
