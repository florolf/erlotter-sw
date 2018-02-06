#!/bin/bash

source /opt/erlotter/venv/bin/activate

cd /opt/erlotter/mgmt/daemons/

exec python "$@"
