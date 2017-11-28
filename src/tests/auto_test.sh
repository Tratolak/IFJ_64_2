#!/bin/bash


# correct programs

echo "***********************************************************************
                        CORRECT PROGRAMS
***********************************************************************"
correct_test_count=`find tests/correct | grep -P "test[0-9]+$" | wc -l`
count=$correct_test_count
correct_failed=0
while [ $count -gt 0 ]
do
  count=`expr $count - 1`
  i=`expr $correct_test_count - $count`
  echo "TEST $i"
  echo ""

  ./ifj17 < tests/correct/test$i > tests/correct/test$i.ifj17code
  ifj17_exit=$?
  if (($ifj17_exit == 0)); then
    ifj17_out="\e[32m$ifj17_exit\e[0m"
  else
    ifj17_out="\e[31m$ifj17_exit\e[0m"
  fi
  echo -e "compiler returns \t\e[1m$ifj17_out\e[0m"

  tests/ic17int tests/correct/test$i.ifj17code > tests/correct/test$i.output
  ic17int_exit=$?
  if (($ic17int_exit == 0)); then
    ic17int_out="\e[32m$ic17int_exit\e[0m"
  else
    ic17int_out="\e[31m$ic17int_exit\e[0m"
  fi
  echo -e "interpreter returns \t\e[1m$ic17int_out\e[0m"

  diff=$(diff tests/correct/test$i.ok tests/correct/test$i.output)
  diff_exit=$?
  if (($diff_exit == 0)); then
    diff_out="\e[32m$diff_exit\e[0m"
  else
    diff_out="\e[31m$diff_exit\e[0m (diff stored in tests/correct/test$i.diff)"
  fi
  echo -e "diff returns \t\t\e[1m$diff_out\e[0m"

  if (($ifj17_exit == 0 && $ic17int_exit == 0 && $diff_exit == 0)); then
    echo -e "\e[1mRESULT: \e[32mOK\e[0m"
  else
    echo "$diff" > tests/correct/test$i.diff
    echo ""
    echo -e "\e[1mRESULT: \e[31mFAIL\e[0m"
    correct_failed=`expr $correct_failed + 1`
  fi
  echo "-----------------------------------------------------------------------"
done

echo ""

# incorrect programs

echo "***********************************************************************
                       INCORRECT PROGRAMS
***********************************************************************"
incorrect_test_count=`find tests/incorrect | grep -P "test[0-9]+$" | wc -l`
count=$incorrect_test_count
incorrect_failed=0
while [ $count -gt 0 ]
do
  count=`expr $count - 1`
  i=`expr $incorrect_test_count - $count`
  echo "TEST $i"
  echo ""

  corr_err=`head -n 1 tests/incorrect/test$i.ok`

  ./ifj17 < tests/incorrect/test$i > tests/incorrect/test$i.ifj17code
  ifj17_exit=$?
  if (($ifj17_exit == $corr_err)); then
    ifj17_out="\e[32m$ifj17_exit\e[0m"
  else
    ifj17_out="\e[31m$ifj17_exit\e[0m (should be $corr_err)"
  fi
  echo -e "compiler returns \t\e[1m$ifj17_out\e[0m"

  if (($ifj17_exit == $corr_err)); then
    echo -e "\e[1mRESULT: \e[32mOK\e[0m"
  else
    echo -e "\e[1mRESULT: \e[31mFAIL\e[0m"
    correct_failed=`expr $correct_failed + 1`
  fi
  echo "-----------------------------------------------------------------------"
done

echo ""
echo "***********************************************************************"
echo -e "\e[1mSUMMARY:\e[0m"
echo -e "correct programs: \t$correct_failed failed from $correct_test_count"
echo -e "incorrect programs: \t$incorrect_failed failed from $incorrect_test_count"
