/*
 * main.c
 *
 *  Created on: Feb 7, 2024
 *      Author: chmnq
 */

#include "main.h"

int main(void)
{
  hwInit();
  apInit();

  apMain();

  return 0;
}
