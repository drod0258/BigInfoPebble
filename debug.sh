#!/bin/bash

# Target file paths
MAIN_C="src/c/main.c"
INDEX_JS="src/pkjs/index.js"

# Function to display usage details and exit
print_usage() {
    echo "Usage: $0 [e|enable|d|disable]"
    echo "  e, enable   : Enable features and inject mock data"
    echo "  d, disable  : Disable features and revert changes"
    exit 1
}

# Check if no arguments were provided
if [ $# -eq 0 ]; then
    print_usage
fi

# Evaluate the first argument
case "$1" in
    e|enable)
        echo "Status: Enabling settings and injecting mock values..."

        # 1. Update settings in main.c (false -> true)
        sed -i 's/settings\.ShowDate = false;/settings\.ShowDate = true;/g' "$MAIN_C"
        sed -i 's/settings\.ShowDate2 = false;/settings\.ShowDate2 = true;/g' "$MAIN_C"
        sed -i 's/settings\.ShowWeather = false;/settings\.ShowWeather = true;/g' "$MAIN_C"
        sed -i 's/settings\.ShowSteps = false;/settings\.ShowSteps = true;/g' "$MAIN_C"
        sed -i 's/settings\.ShowSun = false;/settings\.ShowSun = true;/g' "$MAIN_C"
        sed -i 's/settings\.ShowMoon = false;/settings\.ShowMoon = true;/g' "$MAIN_C"

        # 2. Inject mock step count and heart rate values into main.c
        # Uses standard sed 'a' command to append a line after a specific pattern
        sed -i '/int step_count = (int)health_service_sum_today(HealthMetricStepCount);/a \ \ \ \ step_count = 23456;' "$MAIN_C"
        sed -i '/int hr = (int)health_service_peek_current_value(HealthMetricHeartRateBPM);/a \ \ \ \ hr = 72;' "$MAIN_C"
        
        echo "Done!"
        ;;
        
    d|disable)
        echo "Status: Disabling settings and reverting mock values..."

        # 1. Revert settings in main.c (true -> false)
        sed -i 's/settings\.ShowDate = true;/settings\.ShowDate = false;/g' "$MAIN_C"
        sed -i 's/settings\.ShowDate2 = true;/settings\.ShowDate2 = false;/g' "$MAIN_C"
        sed -i 's/settings\.ShowWeather = true;/settings\.ShowWeather = false;/g' "$MAIN_C"
        sed -i 's/settings\.ShowSteps = true;/settings\.ShowSteps = false;/g' "$MAIN_C"
        sed -i 's/settings\.ShowSun = true;/settings\.ShowSun = false;/g' "$MAIN_C"
        sed -i 's/settings\.ShowMoon = true;/settings\.ShowMoon = false;/g' "$MAIN_C"

        # 2. Delete the injected mock lines from main.c
        sed -i '/step_count = 23456;/d' "$MAIN_C"
        sed -i '/hr = 72;/d' "$MAIN_C"
        
        echo "Done!"
        ;;
        
    *)
        echo "Error: Invalid argument '$1'"
        print_usage
        ;;
esac
