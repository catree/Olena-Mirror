#! /bin/sh

[ -d "$1" -a -f "$1/expand.sh" -a -f "$1/ChangeLog" ] || \
  { echo "usage: $0 <swilena srcdir>" >&2; exit 1; }

SWILENA=`cd $1 && pwd`

mkdir -p "$SWILENA/src"
mkdir -p "$SWILENA/python"
mkdir -p "$SWILENA/ruby"

MODULES="$MODULES ntg"
EXTRA_DIST_MODULES=
for dim in 1 2 3; do

  ## C++ header inclusions for basic types.
     cat >"$SWILENA/src/swilena_basics${dim}d.i" <<EOF
// -*- C++ -*-
// Warning: this file was generated by expand.sh
%include swilena_basics.i
decl_basics($dim)
EOF
     # Have Automake include these modules in the distribution, but
     # don't create SWIG wrappers for them.
     EXTRA_DIST_MODULES="$EXTRA_DIST_MODULES basics${dim}d"

  ## {1d,2d,3d} families
  for mod in point structelt w_win imagesize image; do
     cat >"$SWILENA/src/swilena_$mod${dim}d.i" <<EOF
// -*- C++ -*-
// Warning: this file was generated by expand.sh
%module swilena_$mod${dim}d
%include swilena_$mod.i
decl_$mod($dim)
EOF
     MODULES="$MODULES ${mod}${dim}d"
  done

  ## w_window instances
  for t in int float; do
     echo "make_w_win(w_win${dim}d_$t, $dim, $t)" >>"$SWILENA/src/swilena_w_win${dim}d.i"
  done

  ## Image instances
  cat >>"$SWILENA/src/swilena_image${dim}d.i" <<EOF
%include swilena_ntg_macros.i
%import swilena_ntg.i
EOF
  # int_u8 int_u16 int_s8 int_s16 int_s8s int_s16s int_s32s
  # int_u8s int_u16s int_u32s
  # float_s rgb_8 rgb_16 rgb_32
  cat >> "$SWILENA/src/swilena_image${dim}d.i" <<EOF
make_image(image${dim}d_bin, $dim, ntg_bin, ntg_bin_value)
make_image(image${dim}d_u8, $dim, ntg_int_u8, ntg_int_u8_value)
make_image(image${dim}d_u32, $dim, ntg_int_u32, ntg_int_u32_value)
make_image(image${dim}d_s8, $dim, ntg_int_s8, ntg_int_s8_value)
make_image(image${dim}d_s32, $dim, ntg_int_s32, ntg_int_s32_value)
make_image(image${dim}d_float, $dim, ntg_float, ntg_float_value)
EOF

  TYPES="ntg_bin ntg_int_u8 ntg_int_u32 ntg_int_s8 ntg_int_s32 ntg_float"

  ## Morpho algorithms
  for types in $TYPES; do
      MODULES="$MODULES morpho${dim}d_${types}"
  done
  $SWILENA/generate_morpho_instantiations.py $SWILENA/src

  ## Arith
  MODULES="$MODULES arith${dim}d"
  $SWILENA/generate_arith_instantiations.py $SWILENA/src

  ## Conversions
  MODULES="$MODULES conversions${dim}d"
  $SWILENA/generate_conversions_instantiations.py $SWILENA/src
done # for dim ...

#################### Python stuff #######################

PYMODS=''
PYTHON=''

header_python()
{
cat <<EOF
## Include this file into your Makefile.am -*- Makefile -*-
##
## makefile.swig for swilena/python
## NOTE: this file was generated automatically by expand.sh
##

AM_CPPFLAGS = -DOLN_EXCEPTIONS \$(PYTHON_CPPFLAGS) \\
  -I\$(srcdir)/../src -I\$(srcdir)/../meta
AM_CXXFLAGS = \$(CXXFLAGS_OPTIMIZE) \$(DOC_CPPFLAGS)
## We build modules, not plain libs.
AM_LDFLAGS = -avoid-version -module -shared \$(ZLIB_LDFLAGS)
AM_SWIG_FLAGS = -c++ -python \$(AM_CPPFLAGS)
EOF
}

dump_python()
{
    echo -n "pyexec_LTLIBRARIES ="
    ilist=0
    for mod in $MODULES; do
      if [ `expr $ilist % 4` = 0 ]; then
         echo " \\"; echo -ne "\t"
      fi
      echo -n " _swilena_$mod.la"
      ilist=`expr $ilist + 1`
    done
    echo; echo
    echo -n "nodist_python_PYTHON ="
    ilist=0
    for mod in $MODULES; do
      if [ `expr $ilist % 4` = 0 ]; then
         echo " \\"; echo -ne "\t"
      fi
      echo -n " swilena_$mod.py"
      ilist=`expr $ilist + 1`
    done
    echo; echo
    for mod in $MODULES; do
      echo "nodist__swilena_${mod}_la_SOURCES = swilena_${mod}_wrap.cc"
    done
    echo; echo
    echo -ne "CLEANFILES= "
    echo -e " swilena_all1d_wrap.cc \\"
    echo -e " swilena_all2d_wrap.cc \\"
    echo -e " swilena_all3d_wrap.cc \\"
    echo -ne " swilena_display_wrap.cc"
    for mod in $MODULES; do
	echo -e "\\"
	echo -ne "\t swilena_$mod.py"
	echo -e "\\"
	echo -ne "\t swilena_$mod.pyc"
	echo -e "\\"
	echo -ne "\t swilena_${mod}_wrap.cc"
    done
    echo; echo

    ilist=0
    for mod in $MODULES; do
      if [ -r "$SWILENA/src/swilena_${mod}.i" ]; then
         sdir=src
      else
         sdir=meta
      fi
      # Rule generating a C++ (*_wrap.cc) and a Python (*.py) wrappers from a
      # SWIG interface.
      #
      # FIXME: The generation of the Python wrapper is separated from
      # the one of the C++ wrapper in this version, and that's not good.
      # We should use Make's trick for multiple outputs (with timestamps)
      # here.
      base="swilena_${mod}"
      src="\$(srcdir)/../$sdir/$base.i"
      dest="${base}_wrap.cc"
      cat <<EOF
$dest: $src
	if \$(SWIG) \$(AM_SWIG_FLAGS) \$(SWIG_FLAGS) -MD -MF \$(DEPDIR)/${base}_wrap.Tcc \$(CPPFLAGS) -o \$@ \$<; \\
	then \\
	  mv -f "\$(DEPDIR)/${base}_wrap.Tcc" "\$(DEPDIR)/${base}_wrap.Pcc"; \\
	else \\
	  rm -f "\$(DEPDIR)/${base}_wrap.Tcc"; exit 1; \\
	fi

@AMDEP_TRUE@@am__include@ @am__quote@./\$(DEPDIR)/${base}_wrap.Pcc@am__quote@

$base.py: $src
	\$(MAKE) \$(AM_MAKEFLAGS) $dest

EOF
    done
}

#################### Ruby stuff #######################

header_ruby()
{
cat <<EOF
## Include this file in your Makefile.am -*- Makefile -*-
##
## makefile.swig for swilena/ruby
## NOTE: this file was generated automatically by expand.sh
##

AM_CPPFLAGS = -DOLN_EXCEPTIONS \$(RUBY_CPPFLAGS) \\
  -I\$(srcdir)/../src -I\$(srcdir)/../meta
AM_CXXFLAGS = \$(CXXFLAGS_OPTIMIZE) \$(DOC_CPPFLAGS)
## We build modules, not plain libs.
AM_LDFLAGS = -avoid-version -module -shared \$(ZLIB_LDFLAGS)
AM_SWIG_FLAGS = -c++ -ruby \$(AM_CPPFLAGS)
EOF
}

dump_ruby()
{
    echo -n "ruby_LTLIBRARIES ="
    ilist=0
    for mod in $MODULES; do
      if [ `expr $ilist % 4` = 0 ]; then
         echo " \\"; echo -ne "\t"
      fi
      echo -n " swilena_$mod.la"
      ilist=`expr $ilist + 1`
    done
    echo; echo
    for mod in $MODULES; do
      echo "nodist_swilena_${mod}_la_SOURCES = swilena_${mod}_wrap.cc"
    done
    echo
    echo -ne "CLEANFILES= "
    echo -e " swilena_all1d_wrap.cc \\"
    echo -e " swilena_all2d_wrap.cc \\"
    echo -e " swilena_all3d_wrap.cc \\"
    echo -ne " swilena_display_wrap.cc"
    for mod in $MODULES; do
	echo -e "\\"
	echo -ne "\t swilena_${mod}_wrap.cc"
    done
    echo; echo

    echo "ruby_DATA ="
    echo

    ilist=0
    for mod in $MODULES; do
      if [ -r "$SWILENA/src/swilena_${mod}.i" ]; then
         sdir=src
      else
         sdir=meta
      fi
      # Rule generating a C++ (*_wrap.cc) wrapper for Ruby from a SWIG
      # interface.
      base="swilena_${mod}"
      src="\$(srcdir)/../$sdir/$base.i"
      dest="${base}_wrap.cc"
      cat <<EOF
$dest: $src
	if \$(SWIG) \$(AM_SWIG_FLAGS) \$(SWIG_FLAGS) -MD -MF \$(DEPDIR)/${base}_wrap.Tcc \$(CPPFLAGS) -o \$@ \$<; \\
	then \\
	  mv -f "\$(DEPDIR)/${base}_wrap.Tcc" "\$(DEPDIR)/${base}_wrap.Pcc"; \\
	else \\
	  rm -f "\$(DEPDIR)/${base}_wrap.Tcc"; exit 1; \\
	fi

@AMDEP_TRUE@@am__include@ @am__quote@./\$(DEPDIR)/${base}_wrap.Pcc@am__quote@

EOF

# Work around an issue between Libtool and Ruby on Mac OS X.
# Libtool generates dynamic modules ending in `.so', but the Ruby
# interpreter accepts only files ending in `.bundle' (and obviously
# `.rb') as arguments of the `require' method.  Hence we create
# aliases for the .so files to make them appear as .bundle's
# to ruby.
      cat <<EOF
ruby_DATA+ = .libs/$base.bundle
.libs/$base.bundle: .libs/$base.so $base.la
	cd .libs && \$(LN_S) -f $base.so $base.bundle

EOF
    done

    echo -n "CLEANFILES += \$(ruby_DATA)"
}

############ Generic stuff ###########

header_src() {
    cat <<EOF
## Include this file in your Makefile.am
##
## makefile.swig for swilena/src
## NOTE: this file was generated automatically by expand.sh
##
EOF
}

dump_src() {
    echo -n "EXTRA_DIST ="
    ilist=0
    for mod in $MODULES $EXTRA_DIST_MODULES; do
      if [ -r "$SWILENA/src/swilena_${mod}.i" ]; then
         if [ `expr $ilist % 4` = 0 ]; then
	    echo " \\"
            echo -ne "\t"
	 fi
	 echo -n " swilena_${mod}.i"
	 ilist=`expr $ilist + 1`
      fi
    done
    echo
}

############ Output ###############

header_src >"$SWILENA/src/makefile.swig"
dump_src >>"$SWILENA/src/makefile.swig"
header_python >"$SWILENA/python/makefile.swig"
dump_python >>"$SWILENA/python/makefile.swig"
header_ruby >"$SWILENA/ruby/makefile.swig"
dump_ruby >>"$SWILENA/ruby/makefile.swig"
