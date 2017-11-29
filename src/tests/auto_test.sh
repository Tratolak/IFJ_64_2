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
  name=`sed "${i}q;d" tests/correct/names`
  echo "TEST $i  ($name)"
  echo ""

  # spuštění překladu
  ./ifj17 < tests/correct/test$i > tests/correct/test$i.ifjcode17
  ifj17_exit=$?
  if (($ifj17_exit == 0)); then
    ifj17_out="\e[32m$ifj17_exit\e[0m"
  else
    ifj17_out="\e[31m$ifj17_exit\e[0m -"
    case $ifj17_exit in
      1)
        ifj17_out="$ifj17_out chyba v programu v rámci lexikální analýzy."
        ;;
      2)
        ifj17_out="$ifj17_out chyba v programu v rámci syntaktické analýzy."
        ;;
      3)
        ifj17_out="$ifj17_out sémantická chyba v programu – nedefinovaná funkce/promenná, pokus o redefinici funkce/promenné, atd."
        ;;
      4)
        ifj17_out="$ifj17_out sémantická chyba typové kompatibility v aritmetických, řetezcových a relačních výrazech, příp. špatný počet či typ parametrů u volání funkce."
        ;;
      6)
        ifj17_out="$ifj17_out ostatní sémantické chyby."
        ;;
      99)
        ifj17_out="$ifj17_out interní chyba překladače (např. chyba alokace paměti, atd.)."
        ;;
      *)
        ifj17_out="$ifj17_out neznámá chyba (viz výpis ^ )"
        ;;
    esac
  fi
  echo -e "compiler returns \t\e[1m$ifj17_out\e[0m"

  # spuštění interpretu (pouze, pokud se povedl překlad)
  if (($ifj17_exit == 0)); then
    # požaduje vstup?
  if [ -f "tests/correct/test$i.input" ]
  then
    tests/ic17int tests/correct/test$i.ifjcode17 < tests/correct/test$i.input > tests/correct/test$i.output
  else
    tests/ic17int tests/correct/test$i.ifjcode17 > tests/correct/test$i.output
  fi
  #tests/ic17int tests/correct/test$i.ifjcode17 $input> tests/correct/test$i.output
  ic17int_exit=$?
  if (($ic17int_exit == 0)); then
    ic17int_out="\e[32m$ic17int_exit\e[0m"
  else
    ic17int_out="\e[31m$ic17int_exit\e[0m -"
    case $ic17int_exit in
      51)
        ic17int_out="$ic17int_out chyba při analýze (lexikální, syntaktická) vstupního kódu v IFJcode17."
        ;;
      52)
        ic17int_out="$ic17int_out chyba při sémantických kontrolách vstupního kódu v IFJcode17."
        ;;
      53)
        ic17int_out="$ic17int_out běhová chyba interpretace – špatné typy operandů."
        ;;
      54)
        ic17int_out="$ic17int_out běhová chyba interpretace – přístup k neexistující proměnné (rámec existuje)."
        ;;
      55)
        ic17int_out="$ic17int_out běhová chyba interpretace – rámec neexistuje (např. čtení z prázdného zásobníku rámců)."
        ;;
      56)
        ic17int_out="$ic17int_out běhová chyba interpretace – chybějící hodnota (v proměnné nebo na datovém zásobníku)."
        ;;
      57)
        ic17int_out="$ic17int_out běhová chyba interpretace – dělení nulou."
        ;;
      58)
        ic17int_out="$ic17int_out běhová chyba interpretace – chybná práce s řetezcem."
        ;;
      60)
        ic17int_out="$ic17int_out interní chyba interpretu (např. chyba alokace paměti, atd.)."
        ;;
    esac
  fi
  echo -e "interpreter returns \t\e[1m$ic17int_out\e[0m"
  else
    ic17int_exit=1
  fi

  #spuštění diff (pouze, pokud se povedla interpretace programu)
  if (($ic17int_exit == 0)); then
  diff=$(diff tests/correct/test$i.ok tests/correct/test$i.output)
  diff_exit=$?
  if (($diff_exit == 0)); then
    diff_out="\e[32m$diff_exit\e[0m"
  else
    diff_out="\e[31m$diff_exit\e[0m (diff stored in tests/correct/test$i.diff)"
  fi
  echo -e "diff returns \t\t\e[1m$diff_out\e[0m"
  else
    diff_exit=1
  fi

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
  name=`sed "${i}q;d" tests/incorrect/names`
  echo "TEST $i  ($name)"
  echo ""

  corr_err=`head -n 1 tests/incorrect/test$i.ok`

  ./ifj17 < tests/incorrect/test$i > tests/incorrect/test$i.ifjcode17
  ifj17_exit=$?

  case $ifj17_exit in
    0)
      ifj17_out="bez chyby."
      ;;
    1)
      ifj17_out="chyba v programu v rámci lexikální analýzy."
      ;;
    2)
      ifj17_out="chyba v programu v rámci syntaktické analýzy."
      ;;
    3)
      ifj17_out="sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici funkce/promenné, atd."
      ;;
    4)
      ifj17_out="sémantická chyba typové kompatibility v aritmetických, řetezcových a relačních výrazech, příp. špatný pocet či typ parametrů u volání funkce."
      ;;
    6)
      ifj17_out="ostatní sémantické chyby."
      ;;
    99)
      ifj17_out="interní chyba překladače (např. chyba alokace paměti, atd.)."
      ;;
    *)
      ifj17_out="neznámá chyba (viz výpis ^ )"
      ;;
  esac

  if (($ifj17_exit == $corr_err)); then
    ifj17_out="\e[32m$ifj17_exit\e[0m - $ifj17_out"
  else
    ifj17_out="\e[31m$ifj17_exit\e[0m - $ifj17_out (should be $corr_err)"
  fi
  echo -e "compiler returns \t\e[1m$ifj17_out\e[0m"

  if (($ifj17_exit == $corr_err)); then
    echo -e "\e[1mRESULT: \e[32mOK\e[0m"
  else
    echo -e "\e[1mRESULT: \e[31mFAIL\e[0m"
    incorrect_failed=`expr $incorrect_failed + 1`
  fi
  echo "-----------------------------------------------------------------------"
done

echo ""
echo "***********************************************************************"
echo -e "\e[1mSUMMARY:\e[0m"
echo -e "correct programs: \t$correct_failed failed from $correct_test_count"
echo -e "incorrect programs: \t$incorrect_failed failed from $incorrect_test_count"
