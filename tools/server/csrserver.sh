### BEGIN INIT INFO
# Provides: csrserver
# Required-Start: $all
# Required-Stop:
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: Start csr gateway server
### END INIT INFO


#! /bin/bash

nohup java -jar /opt/weblabmotor/tools/server/csr.jar &

exit 0
