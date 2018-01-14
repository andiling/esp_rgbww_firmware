#####################################################################
#### Please don't change this file. Use Makefile-user.mk instead ####
#####################################################################
# Including user Makefile.
# Should be used to set project-specific parameters
include ./Makefile-user.mk

#### GIT VERSION Information ##### 
GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)
ifndef GIT_VERSION
$(info no GIT_VERSION available, using unknown)
GIT_VERSION = "unknown"
endif

GIT_DATE := $(firstword $(shell git --no-pager show --date=short --format="%ad" --name-only))
ifndef GIT_DATE
$(info no GIT_DATE available, using unknown)
GIT_DATE = "unknown"
endif

WEBAPP_VERSION := `cat webapp/VERSION`
ifndef WEBAPP_VERSION
$(error can not find webapp/VERSION file - please ensure the source code is complete)
endif

USER_CFLAGS += -DGITVERSION=\"$(GIT_VERSION)\" -DGITDATE=\"$(GIT_DATE)\" -DWEBAPP_VERSION=\"$(WEBAPP_VERSION)\"

# Important parameters check.
# We need to make sure SMING_HOME and ESP_HOME variables are set.
# You can use Makefile-user.mk in each project or use enviromental variables to set it globally.
 
ifndef SMING_HOME
$(error SMING_HOME is not set. Please configure it in Makefile-user.mk)
endif
ifndef ESP_HOME
$(error ESP_HOME is not set. Please configure it in Makefile-user.mk)
endif


# Include main Sming Makefile
ifeq ($(RBOOT_ENABLED), 1)
include $(SMING_HOME)/Makefile-rboot.mk
else
include $(SMING_HOME)/Makefile-project.mk
endif

