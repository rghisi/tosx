# Find the Arduino port
ARDUINO_UPLOAD_PORT="$(find /dev/ttyACM* | head -n 1)"

# Reset the Arduino
stty -F "${ARDUINO_UPLOAD_PORT}" 1200
stty -F "${ARDUINO_UPLOAD_PORT}" 1200
stty -F "${ARDUINO_UPLOAD_PORT}" 1200
stty -F "${ARDUINO_UPLOAD_PORT}" 1200

# Wait for it...
while :; do
  sleep 0.5
  [ -c "${ARDUINO_UPLOAD_PORT}" ] && break
done

# ...upload!
avrdude -pm32u4 -cavr109 -P$ARDUINO_UPLOAD_PORT -b57600 -v -Uflash:w:Release/rs485_2.hex:i