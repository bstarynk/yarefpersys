#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-or-later
#
#
# © Copyright (C) 2026, Basile Starynkevitch
yrps_getid="$(git log --format=oneline -q -1 \
		| cut '-d ' -f 1 | head -10c \
		| tr -d '\n')"

printf "%s\n" $yrps_getid
