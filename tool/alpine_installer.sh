#!/bin/bash


apk add gcc
apk add gnupg
apk add zip

gpg template.t.gpg

./install.sh
