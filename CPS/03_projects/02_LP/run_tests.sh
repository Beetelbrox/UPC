#!/bin/bash

ACTION=$1
shift

IN_FOLDER=instances
OUT_FOLDER=out
RESULTS_FILE="results.txt"

if [[ $ACTION = "solve" ]]; then
  for FILENAME in $(ls instances); do
    INSTANCE="${FILENAME%.*}"
    echo "----------------------------------------------------------"
    echo "Solving instance ${INSTANCE}..."
    timeout --preserve-status 60 ./solver < instances/$FILENAME > "${OUT_FOLDER}/${INSTANCE}.out"
    if [[ -z "$(cat ${OUT_FOLDER}/${INSTANCE}.out)" ]]; then
      >&2 echo "Instance ${INSTANCE} timed out!"
      rm --force ${OUT_FOLDER}/ ${INSTANCE}.out
    fi
  done
fi

if [[ $ACTION = "check_correctness" ]]; then
  for FILENAME in $(ls ${OUT_FOLDER}); do
    printf "Checking instance ${FILENAME%.*}..."
    cat ${OUT_FOLDER}/${FILENAME} |./checker
  done
fi

# Runs the testbench provided by the instructor
if [[ $ACTION = "run_testbench" ]]; then
  NUM_INSTANCES=($(wc -l ${RESULTS_FILE}))
  cat ${RESULTS_FILE} | while read LINE; do
    TOKENS=($LINE)
    FILENAME=${TOKENS[0]}
    INSTANCE=${TOKENS[0]%.*}
    printf "Solving instance ${INSTANCE}..."
    OUTPUT="$(timeout --preserve-status 60 ./solver < instances/${FILENAME} 2> log.log )"
    if [[ -z ${OUTPUT} ]]; then
      printf "\nNo solution found for instance ${INSTANCE}\n"
    else
      printf "Done\n"
      printf "Checking solution correctness..."
      CORRECT="$(echo ${OUTPUT} | ./checker)"
      if [[ $CORRECT = "OK!" ]]; then
        printf "$CORRECT\n"
        printf "Checking solution Optimality..."
        OUTPUT=($OUTPUT)
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
        printf "${OPTIMAL_STR}\n\n"
      else
        CORRECT="Not correct"
        printf "$CORRECT\n"
      fi
    fi
  done
fi
