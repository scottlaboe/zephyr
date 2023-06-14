#!/bin/bash

#First parameter is the name of the board
BOARD_NAME="$1"
BOARD_PREFIX=${BOARD_NAME%%_*}

OUT_FILE="${ZEPHYR_BASE}/boards/arm/${BOARD_NAME}/support/${BOARD_NAME}.repl"

#Include board specific files
if [ "$BOARD_PREFIX" = "nucleo" ]
then
    BOARD_INC="\
        -I ${ZEPHYR_BASE}/include \
        -I ${ZEPHYR_BASE}/dts \
        -I ${ZEPHYR_BASE}/dts/arm \
        -I ${ZEPHYR_BASE}/dts/common \
        -I ${ZEPHYR_BASE}/../modules/hal/stm32/dts"
fi

#Generate files
FLAT_DTS_FILE=$(mktemp /tmp/genRenodeRepl-flat.dts.XXXXXX)
INCLUDES_FILE=$(mktemp /tmp/genRenodeRepl-includes.txt.XXXXXX)

echo "Generating flattened device tree"
${ZEPHYR_SDK_INSTALL_DIR}/arm-zephyr-eabi/bin/arm-zephyr-eabi-gcc \
    -H -E -P \
    -x assembler-with-cpp \
    ${BOARD_INC} \
    ${ZEPHYR_BASE}/boards/arm/${BOARD_NAME}/${BOARD_NAME}.dts 1>${FLAT_DTS_FILE} 2>${INCLUDES_FILE}

echo "Generating Renode's .repl file"
dts2repl --output ${OUT_FILE} ${FLAT_DTS_FILE}

echo "Cleaning up"
rm ${FLAT_DTS_FILE} ${INCLUDES_FILE}

echo "Done"