#!/bin/bash
basepath=config/deployments/ssl

openssl genrsa -des3 -out ${basepath}.key 1024
openssl req -new -key ${basepath}.key -out ${basepath}.csr
cp ${basepath}.key ${basepath}.key.org
openssl rsa -in ${basepath}.key.org -out ${basepath}.key
openssl x509 -req -days 365 -in ${basepath}.csr -signkey ${basepath}.key -out ${basepath}.crt

