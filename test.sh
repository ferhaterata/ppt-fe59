#!/bin/bash

printf '%s\n' "--------------------- Initiate tests --------------------------"
printf '%s\n' ""
printf '%s\n' "==============================================================="
printf '%s\n' "Inventory file is successfully repaired..."
printf '%s\n' "==============================================================="

printf "\n (Testing newline delimited records)"
printf "\n$>./repair inventory.in inventory.out\n\n"
valgrind --leak-check=full ./repair tests/inventory.in tests/inventory.out

printf "\n (Testing mixed whitespace delimited records)"
printf "\n$>./repair inventory1.in inventory1.out\n\n"
valgrind --leak-check=full ./repair tests/inventory1.in tests/inventory1.out

printf "\n (Testing inventory file having one record)"
printf "\n$>./repair inventory2.in inventory2.out\n\n"
valgrind --leak-check=full ./repair tests/inventory2.in tests/inventory2.out

printf "\n (Testing inventory file having no record)"
printf "\n$>./repair inventory3.in inventory3.out\n\n"
valgrind --leak-check=full ./repair tests/inventory3.in tests/inventory3.out

printf '%s\n' ""
printf '%s\n' "==============================================================="
printf '%s\n' "Running Corrupted Inventory Files..."
printf '%s\n' "==============================================================="

printf "\n$>./repair inventory4.in inventory4.out\n\n"
valgrind --leak-check=full ./repair tests/inventory4.in tests/inventory4.out

printf "\n$>./repair inventory5.in inventory5.out\n\n"
valgrind --leak-check=full ./repair tests/inventory5.in tests/inventory5.out

printf "\n$>./repair inventory6.in inventory6.out\n\n"
valgrind --leak-check=full ./repair tests/inventory6.in tests/inventory6.out

printf "\n$>./repair inventory7.in inventory7.out\n\n"
valgrind --leak-check=full ./repair tests/inventory7.in tests/inventory7.out

printf '%s\n' ""
printf '%s\n' "--------------------- Finalize tests --------------------------"
printf '%s\n' "==============================================================="
