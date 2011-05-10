######################################################################
# dianvote.pro
#
# project file for all the DianVote applications.
#
# Author: tankery.chen@gmail.com
# Date: 2011-04-17
#
# Copyright (c) Tankery Chen 2011 @ Dian Group
######################################################################

TEMPLATE    = subdirs
CONFIG     += ordered
SUBDIRS    +=   ./DianVoteDrawer \
                ./DianVoteEditor \
                ./hidapi \
                ./DianVoteControl \
                ./DianVotePlayer
