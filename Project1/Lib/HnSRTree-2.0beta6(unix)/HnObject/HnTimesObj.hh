/*
 * HnTimesObj.hh
 * Copyright (C) 1999 Norio Katayama
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA
 *
 * 02/02/99 Norio KATAYAMA
 * $Id: HnTimesObj.hh,v 1.4 2003/06/11 09:05:58 katayama Exp $
 */

#ifndef _HnTimesObj_hh
#define _HnTimesObj_hh

#ifdef _MSC_VER
#    include <windows.h>	/* for struct timeval */
#else
#    ifdef TIME_WITH_SYS_TIME
#        include <sys/time.h>
#        include <time.h>
#    else
#        ifdef HAVE_SYS_TIME_H
#            include <sys/time.h>
#        else
#            include <time.h>
#        endif
#    endif
#endif
#include "HnSRTree/HnObject.hh"

class HnTimesObj: public HnObject {
private:
    struct timeval rtime;
    struct timeval utime;
    struct timeval stime;

public:
    HnTimesObj(void);

    void setCurrentTimes(void);

    HnTimes add(const HnTimes &times) const;
    HnTimes subtract(const HnTimes &times) const;

    double getRealTime(void) const;
    double getUserTime(void) const;
    double getSystemTime(void) const;
    double getCPUTime(void) const;
};

#endif /* _HnTimesObj_h */
