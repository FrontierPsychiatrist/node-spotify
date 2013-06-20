#!/bin/bash
if [ ! -d log ]; then
  mkdir log
fi;

#GLOG_log_dir="./log" node node-spotify-backend.js
GLOG_logtostderr=1 node node-spotify-backend.js
