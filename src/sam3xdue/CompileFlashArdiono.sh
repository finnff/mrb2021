#!/bin/bash
set -x # echo on

# Default values
ARCH_MAP=(
  ["Uno"]="arduino:avr:uno"
  ["nano328"]="arduino:avr:nano:cpu=atmega328old"
  ["nano168"]="arduino:avr:nano:cpu=atmega168"
  ["Due"]="arduino:sam:arduino_due_x_dbg"
  ["esp8266"]="esp8266:esp8266:nodemcuv2"
  ["esp32"]="esp32:esp32:esp32"
)
ARCHITECTURE="arduino:avr:uno" # Default to Uno
PORTS=($(ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null))
PORT=""
BAUDRATE=""
BUILD_FLAGS=""

# Parsing arguments
while [[ "$#" -gt 0 ]]; do
  case $1 in
    -F)
      FILENAME="$2"
      shift
      ;;
    -P)
      PORT="$2"
      shift
      ;;
    --serialmon)
      BAUDRATE="$2"
      shift
      ;;
    -A)
      ARCHITECTURE=${ARCH_MAP["$2"]}
      shift
      ;;
    *)
      echo "Unknown parameter passed: $1"
      echo "Usage: CompileFlash -A [Uno|nano328|nano168|Due|esp8266|esp32] -F [filename] -P [port] --serialmon [baudrate]"
      echo "Options:"
      echo "-A: Specify the architecture."
      echo "-F: Specify the filename."
      echo "-P: Specify the port."
      echo "--serialmon: Open serial monitor with specified baud rate."
      exit 1
      ;;
  esac
  shift
done

# Check for port conflicts
if [ ${#PORTS[@]} -eq 0 ]; then
  echo "No valid port found."
  exit 1
elif [ ${#PORTS[@]} -gt 1 ] && [ -z "$PORT" ]; then
  echo "Multiple ports detected. Please specify the port using the -P option."
  exit 1
elif [ ${#PORTS[@]} -eq 1 ] && [ -z "$PORT" ]; then
  PORT=${PORTS[0]}
fi

# Check if FILENAME variable is set and file exists
if [ -z "$FILENAME" ]; then
  echo "Please provide a filename using the -F option."
  exit 1
fi

if [ ! -f "$FILENAME" ]; then
  echo "File $FILENAME does not exist."
  exit 1
fi

# Compile and upload the Arduino script
arduino-cli compile --fqbn $ARCHITECTURE $BUILD_FLAGS $FILENAME
if [ $? -eq 0 ]; then
  arduino-cli upload -p $PORT --fqbn $ARCHITECTURE $FILENAME --verbose
else
  echo "Error during build. Upload skipped."
  exit 1
fi

# Check if baudrate is provided and open serial monitor
if [ ! -z "$BAUDRATE" ]; then
  pyserial-miniterm $PORT $BAUDRATE
fi
