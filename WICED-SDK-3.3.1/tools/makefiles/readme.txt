
wiced_resources.mk

Comment the conditional build command:
#ifneq ($(filter SFLASH_FILESYSTEM,$(RESOURCES_LOCATION)),)
...
#endif


standard_platform_targets.mk

Adds support for creating single bin file for MSD upload.
 