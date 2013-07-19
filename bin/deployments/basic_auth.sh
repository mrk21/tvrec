#!/bin/bash
filename=config/deployments/basic_auth
user=$1
htpasswd -c ${filename} ${user}

