/*
 * common.h
 *
 *  Created on: 13.04.2012
 *      Author: igor
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <sys/types.h>

namespace Util
{

char *prog(char *str);

pid_t makedaemon();

void out_message(bool daemon, std::basic_ostream<char>& out, int fac_pri, const char *fmt, ...);

} // namespace Util

#endif /* COMMON_H_ */
