#!/bin/bash

while true; do
    /usr/bin/python3 /media/bartek/LEXAR/DEV/STM32/multiplex/sendTemp.py
    echo "Script crashed at $(date), restarting in 10s..."
    sleep 10
done
