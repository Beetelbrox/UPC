#!/bin/bash

ACTION=$1
shift

IN_FOLDER=instances
OUT_FOLDER=out
RESULTS_FILE="results.txt"

if [[ $ACTION = "solve" ]]; then
  for INSTANCE in $(ls instances); do
    FILENAME="${INSTANCE%.*}"
    echo "----------------------------------------------------------"
    echo "Solving instance ${FILENAME}..."
    ./solver < "instances/$INSTANCE" > "${OUT_FOLDER}/${FILENAME}.out"
  done
fi

if [[ $ACTION = "check" ]]; then
  cat ${RESULTS_FILE} | while read LINE; do
    TOKENS=($LINE)
    FILENAME="${TOKENS[0]%.*}"
    OUTPUT=($(cat ${OUT_FOLDER}/${FILENAME}.out))
    if [[ -z $OUTPUT ]]; then
      echo "No solution found for instance ${FILENAME}"
    else
      NUM_INPUTS=${OUTPUT[0]}
      DEPTH_IX=$((1 + $((1 << $NUM_INPUTS))))
      GATES_IX=$((1+$DEPTH_IX))
      DEPTH=${OUTPUT[$DEPTH_IX]}
      GATES=${OUTPUT[$GATES_IX]}
      if [[ "${DEPTH}" == "${TOKENS[1]}" ]] && [[ "${GATES}" == "${TOKENS[2]}" ]]; then
        OPTIMAL_STR="Optimal"
      else
        OPTIMAL_STR="Not Optimal"
      fi
      echo "${FILENAME}: $(./checker < ${OUT_FOLDER}/${FILENAME}.out) - ${OPTIMAL_STR}"
    fi
  done
fi
