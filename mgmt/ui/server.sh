#!/bin/bash

export DB_PATH=/opt/erlotter/db

eval $(perl -Mlocal::lib=/opt/perl/)

exec morbo -m production -l 'http://*:80' /opt/erlotter/mgmt/ui/server.pl
