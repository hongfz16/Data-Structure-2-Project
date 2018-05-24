#!/bin/sh
#
# mswinify.sh
#
# Sep. 9,2002 Norio KATAYAMA
#

topdir="HnSRTree"
progdirs="HnObject HnRect HnSRTree c++-samples c-samples"
tmpfile=temp.$$

#
# make directories
#

if [ ! -d $topdir ]; then
    mkdir $topdir
fi
for dir in $progdirs; do
    if [ ! -d $topdir/$dir ]; then
        mkdir $topdir/$dir
    fi
done
if [ ! -d $topdir/include ]; then
    mkdir $topdir/include
fi
if [ ! -d $topdir/include/HnSRTree ]; then
    mkdir $topdir/include/HnSRTree
fi
if [ ! -d $topdir/lib ]; then
    mkdir $topdir/lib
fi
if [ ! -d $topdir/test ]; then
    mkdir $topdir/test
fi
if [ ! -d $topdir/doc ]; then
    mkdir $topdir/doc
fi

#
# copy program files
#

for dir in $progdirs; do
    for srcfile in `find ../$dir -print`; do
        case $srcfile in
	*.cc|*.hh|*.c|*.h)
            srcbase=`echo $srcfile | sed -e 's|^.*/||' -e 's|\.[^.]*$||'`
            srcext=`echo $srcfile | sed -e 's|^.*\.||'`
            case $srcext in
            cc)
                dstext=cpp
                dstdir=$topdir/$dir
                ;;
            c)
                dstext=c
                dstdir=$topdir/$dir
                ;;
            hh)
                dstext=hh
                dstdir=$topdir/include/HnSRTree
                ;;
            h)
                dstext=h
                dstdir=$topdir/include/HnSRTree
                ;;
            *)
                echo "Error: unexpected extension: $srcext."
                exit 1
                ;;
            esac

	    case $srcbase.$srcext in
	    HnClassArray.cc|HnFTypeArray.cc|HnClassArraySt.c)
	        dstdir=$topdir/include/HnSRTree
		;;
	    RecordFileSt.cc|RecordFileSt.hh)
	        dstdir=$topdir/c++-samples
		;;
	    RecordFileSt.c|RecordFileSt.h)
	        dstdir=$topdir/c-samples
		;;
	    esac

            dstbase=$srcbase
            dstfile=$dstdir/$dstbase.$dstext

            sed -e 's|^#include "\(.*\).cc"|#include "\1.cpp"|' \
                < $srcfile | awk '{ printf("%s\r\n", $0); }' > $dstfile
	    ;;
	esac
    done
done

#
# generate Makefiles
#

for dir in HnObject HnRect HnSRTree; do
    objs=`find $topdir/$dir \( -name '*.c' -o -name '*.cpp' \) -print | \
          sed -e 's|^.*/||' -e 's|.[^.]*$|.obj|' \
              -e '/^.*Test.obj$/d'`

    rm -f $tmpfile
    echo "CFLAGS   =" \
         "/nologo /MD /I../include /D_HNSRTIMP=_declspec(dllexport)" \
         >> $tmpfile
    echo "CPPFLAGS =" \
         "/nologo /MD /I../include /D_HNSRTIMP=_declspec(dllexport)" \
         >> $tmpfile
    echo -n "OBJS    =" >> $tmpfile
    for f in $objs; do
        echo " \\" >> $tmpfile
        echo -n "        $f" >> $tmpfile
    done
    echo >> $tmpfile
    echo "all: \$(OBJS)" >> $tmpfile
    echo >> $tmpfile
    echo >> $tmpfile

    echo "clean:" >> $tmpfile
    echo "	del *.obj" >> $tmpfile

    awk '{ printf("%s\r\n", $0); }' < $tmpfile > $topdir/$dir/Makefile
    rm -f $tmpfile

    for f in $objs; do
        libobjs="$libobjs ../$dir/$f"
    done
done

#
# lib/libobjs.txt
#

rm -f $tmpfile
for f in $libobjs; do
    echo "$f" >> $tmpfile
done

awk '{ printf("%s\r\n", $0); }' < $tmpfile > $topdir/lib/libobjs.txt
rm -f $tmpfile

#
# lib/Makefile
#

rm -f $tmpfile
echo -n "OBJS = " >> $tmpfile
for f in $libobjs; do
    echo " \\" >> $tmpfile
    echo -n "        $f" >> $tmpfile
done
echo >> $tmpfile
echo >> $tmpfile

cat >> $tmpfile <<EOF
libHnSRTree.lib: \$(OBJS)
	link /nologo /dll /out:libHnSRTree.dll @libobjs.txt

clean:
	del libHnSRTree.exp
	del libHnSRTree.lib
	del libHnSRTree.dll
EOF

awk '{ printf("%s\r\n", $0); }' < $tmpfile > $topdir/lib/Makefile
rm -f $tmpfile

#
# c++-samples/Makefile
#

cppbases=""
for f in `find $topdir/c++-samples -name '*.cpp' -print | \
          sed -e 's|^.*/||' -e 's|.[^.]*$||'`; do
    cppbases="$cppbases $f"
done

rm -f $tmpfile
echo "CPPFLAGS = /nologo /MD /I../include" >> $tmpfile
echo >> $tmpfile
echo -n "all:" >> $tmpfile
for f in $cppbases; do
    case $f in
    RecordFileSt)
        ;;
    *)
        echo " \\" >> $tmpfile
        echo -n "        $f.exe" >> $tmpfile
	;;
    esac
done
echo >> $tmpfile
echo >> $tmpfile

for f in $cppbases; do
    case $f in
    RecordFileSt)
        ;;
    *)
        echo "$f.exe: $f.cpp RecordFileSt.obj" >> $tmpfile
        echo "	cl \$(CPPFLAGS) $f.cpp \\" >> $tmpfile
	echo "	   RecordFileSt.obj ../lib/libHnSRTree.lib" \
             >> $tmpfile
	;;
    esac
done
echo >> $tmpfile

cat >> $tmpfile <<EOF
clean:
	del *.obj
	del *.exe
EOF

awk '{ printf("%s\r\n", $0); }' < $tmpfile > $topdir/c++-samples/Makefile
rm -f $tmpfile

#
# c-samples/Makefile
#

cbases=""
for f in `find $topdir/c-samples -name '*.c' -print | \
          sed -e 's|^.*/||' -e 's|.[^.]*$||'`; do
    cbases="$cbases $f"
done

rm -f $tmpfile
echo "CFLAGS = /nologo /MD /I../include" >> $tmpfile
echo >> $tmpfile
echo -n "all:" >> $tmpfile
for f in $cbases; do
    case $f in
    RecordFileSt)
        ;;
    *)
        echo " \\" >> $tmpfile
        echo -n "        $f.exe" >> $tmpfile
	;;
    esac
done
echo >> $tmpfile
echo >> $tmpfile

for f in $cbases; do
    case $f in
    RecordFileSt)
        ;;
    *)
        echo "$f.exe: $f.c RecordFileSt.obj" >> $tmpfile
        echo "	cl \$(CFLAGS) $f.c \\" >> $tmpfile
	echo "	   RecordFileSt.obj ../lib/libHnSRTree.lib" \
             >> $tmpfile
	;;
    esac
done
echo >> $tmpfile

cat >> $tmpfile <<EOF
clean:
	del *.obj
	del *.exe
EOF

awk '{ printf("%s\r\n", $0); }' < $tmpfile > $topdir/c-samples/Makefile
rm -f $tmpfile

#
# generate files in the `test' directory
#

for srcpath in ../test/*; do
    filename=`echo $srcpath | sed -e 's|^.*/||'`
    case $filename in
    CVS)
        ;;
    *)
        awk '{ printf("%s\r\n", $0); }' < $srcpath > $topdir/test/$filename
	;;
    esac
done

cat <<EOF | awk '{ printf("%s\r\n", $0); }' > $topdir/test/Makefile 
CFLAGS = /nologo

all: extans.exe cmp.exe libHnSRTree.dll

libHnSRTree.dll: ..\\lib\\libHnSRTree.dll
	copy ..\\lib\\libHnSRTree.dll .

clean:
	del *.obj
	del *.exe
	del libHnSRTree.dll
	del *.tmp
	del *.idx
	del *.log
EOF

#
# test/doTest.bat
#

sed -e '1i\
@echo off' \
    -e 's|^if \[ \$? != 0 \]; .*$|if errorlevel 1 goto failed|' \
    -e '/^[A-Za-z][A-Za-z0-9]*=/s|^|set |' \
    -e 's|\.\./|..\\|g' \
    -e 's|\$commandDir/|%commandDir%\\|g' \
    -e 's|\$\([A-Za-z][A-Za-z0-9]*\)|%\1%|g' \
    -e 's|^echo "\(.*\)"$|echo \1|' \
    -e '/^#/d' \
    -e 's|grep |find |g' \
    -e 's|/dev/null|nul:|g' \
    -e 's|\./extans\.sed|extans|g' \
    -e '$a\
goto exit \
\
:failed \
echo ******** Test failed. ******** \
\
:exit' \
< ../test/doTest | awk '{ printf("%s\r\n", $0); }' > $topdir/test/doTest.bat

#
# generate files in the `doc' directory
#

for srcpath in ../doc/*; do
    filename=`echo $srcpath | sed -e 's|^.*/||'`
    case $filename in
    CVS)
        ;;
    *)
        awk '{ printf("%s\r\n", $0); }' < $srcpath > $topdir/doc/$filename
	;;
    esac
done

#
# copy README and COPYING
#

for f in README COPYING.LIB; do
    awk '{ printf("%s\r\n", $0); }' < ../$f > $topdir/$f
done

#
# Makefile
#
cat <<'EOF' | awk '{ printf("%s\r\n", $0); }' > $topdir/Makefile
all:
	cd HnObject
	$(MAKE) /$(MAKEFLAGS)
	cd ..\HnRect
	$(MAKE) /$(MAKEFLAGS)
	cd ..\HnSRTree
	$(MAKE) /$(MAKEFLAGS)
	cd ..\lib
	$(MAKE) /$(MAKEFLAGS)
	cd ..\c++-samples
	$(MAKE) /$(MAKEFLAGS)
	cd ..\c-samples
	$(MAKE) /$(MAKEFLAGS)
	cd ..\test
	$(MAKE) /$(MAKEFLAGS)

clean:
	cd HnObject
	$(MAKE) /$(MAKEFLAGS) clean
	cd ..\HnRect
	$(MAKE) /$(MAKEFLAGS) clean
	cd ..\HnSRTree
	$(MAKE) /$(MAKEFLAGS) clean
	cd ..\lib
	$(MAKE) /$(MAKEFLAGS) clean
	cd ..\c++-samples
	$(MAKE) /$(MAKEFLAGS) clean
	cd ..\c-samples
	$(MAKE) /$(MAKEFLAGS) clean
	cd ..\test
	$(MAKE) /$(MAKEFLAGS) clean

includes:

EOF

#
# config.h
#

cat <<EOF | awk '{ printf("%s\r\n", $0); }' > $topdir/include/HnSRTree/config.h
#define HAVE_MEMMOVE

#if _MSC_VER < 1500
#define vsnprintf(s, n, format, ap)  vsprintf(s, format, ap)
#endif

#if !defined(_MT) || !defined(_DLL)
#error libHnSRTree.dll requires programs to be compiled with the \
/MD switch so that the global variables (e.g., errno) will be shared \
among the run-time library, the SR-tree library, and user's object files.
#endif

EOF

exit 0
