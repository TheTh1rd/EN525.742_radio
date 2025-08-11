#!/bin/bash
ip="192.168.1.4" # Default IP

killall -9 "stream_udp_data"

# If called via a POST request, read the data from stdin to get the IP.
if [ "$REQUEST_METHOD" = "POST" ] && [ -n "$CONTENT_LENGTH" ] && [ "$CONTENT_LENGTH" -gt 0 ]; then
    read -n "$CONTENT_LENGTH" POST_DATA
    # Attempt to parse 'ip' from POST data (e.g., "...&ip=1.2.3.4&...").
    parsed_ip=$(echo "$POST_DATA" | sed -n 's/.*ip=\([^&]*\).*/\1/p')
    [ -n "$parsed_ip" ] && ip="$parsed_ip"
fi

echo "Content-type: text/html" # Tells the browser what kind of content to expect
echo "" # An empty line. Mandatory, if it is missed the page content will not load
echo "<p><em>"
echo "Using IP Address: $ip<br>"
echo "loading PL...<br>"
fpgautil -b design_1_wrapper.bit.bin
echo "</p></em><p>"
echo "configuring Codec...<br>"
./configure_codec.sh
echo "</p>"
echo "<p><em>All Done!</em></p>"
echo "</body></html>"
#echo "you would start your UDP Streamer Program Here...<br>"
./stream_udp_data $ip

