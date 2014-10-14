#!/bin/sh
filename=config/auth/basic_auth
mkdir -p $(dirname ${filename})
user=$1
htpasswd -c ${filename} ${user}
