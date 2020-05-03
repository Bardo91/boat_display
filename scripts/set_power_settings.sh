#---------------------------------------------------------------------------------------------------------------------
#  Rosex Hidraulica Sl
#---------------------------------------------------------------------------------------------------------------------
#
#  Copyright 2020 Rosex Hidraulica Sl  - All Rights Reserved
#
#  Unauthorized copying of this file, via any medium is strictly prohibited Proprietary and confidential.
#
#  All information contained herein is, and remains the property of Vertical Engineering Solutions.  The
#  intellectual and technical concepts contained herein are proprietary to Vertical Engineering Solutions
#  and its suppliers and may be covered by UE and Foreign Patents, patents in process, and are protected
#  by trade secret or copyright law. Dissemination of this information or reproduction of this material is
#  strictly forbidden unless prior written permission is obtained from Adobe Systems Incorporated.
#
#---------------------------------------------------------------------------------------------------------------------

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

mv  /etc/rc.local  /etc/rc.local_old

echo "
#!/bin/sh -e
#
# rc.local
#
# This script is executed at the end of each multiuser runlevel.
# Make sure that the script will "exit 0" on success or any other
# value on error.
#
# In order to enable or disable this script just change the execution
# bits.
#
# By default this script does nothing.

# Print the IP address
_IP=$(hostname -I) || true
if [ "$_IP" ]; then
  printf "My IP address is %s\n" "$_IP"
fi

setterm -blank 0 -powerdown 0 -powersave off

exit 0

" > /etc/rc.local
