#! /usr/bin/perl -w
# Extract all examples from the manual source.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307  USA

use strict;
use IO::File;
use Text::Wrap;
use File::Basename;
use Getopt::Long qw(GetOptions);
use Pod::Usage qw(pod2usage);

my $me = basename ($0);

# Array of hash tables holding the information from the config file
my @config;

# &parse_config ($FILE)
# ---------------------
# Parse the config file.

sub parse_config($)
{
  open (CONFIGFILE, $_[0]) or die "$me: Can't open $_[0]: $!";

  # Number of entries
  my $entries = -1;

  while (<CONFIGFILE>)
    {
      if (/\[ENTRY\]/) { $entries++ }
      else
	{
	SWITCH:
	  {
	    last SWITCH if (/^\s*(\#+.*)*$/);
	    $config[$entries]{"command_prefix"} = $1 and last SWITCH if (/\s*COMMAND_PREFIX\s*=\s*([^\#]*[^\s])\s*(\#+.*)*$/);
	    $config[$entries]{"tag_open"} = $1 and last SWITCH if (/\s*TAG_OPEN\s*=\s*([^\#]*[^\s])\s*(\#+.*)*$/);
	    $config[$entries]{"tag_close"} = $1 and last SWITCH if (/\s*TAG_CLOSE\s*=\s*([^\#]*[^\s])\s*(\#+.*)*$/);
	    $config[$entries]{"captions"} = $1 and last SWITCH if (/\s*CAPTIONS\s*=\s*([^\#]*[^\s])\s*(\#+.*)*$/);
	    $config[$entries]{"out"} = $1 and last SWITCH if (/\s*OUT\s*=\s*([^\#]*[^\s])\s*(\#+.*)*$/);
	    $config[$entries]{"$1"} = $2 and last SWITCH if (/\s*ALIAS\s*([^\s]*)\s*=\s*([^\#]*[^\s])\s*(\#+.*)*$/);
	    $config[$entries]{"comment_style"} = $1 and last SWITCH if (/\s*COMMENT_STYLE\s*=\s*([^\#]*[^\s])\s*(\#+.*)*$/);
	    die "$0: Error: Unmatching entry: $_";
	  }
	}
    }
  close CONFIGFILE;
  die "$me: Invalid config file `$_[0]'" if ($entries == -1); # FIXME: not strict enough.

  for (my $i = 0; $i < @config; $i++)
    {
      $config[$i]{"tag_open"} = $config[$i]{"command_prefix"} . $config[$i]{"tag_open"};
      $config[$i]{"tag_close"} = $config[$i]{"command_prefix"} . $config[$i]{"tag_close"};
    }

  for my $i ( 0 .. $#config )
    {
      for my $role ( keys %{ $config[$i] } )
	{
	  print "elt $i $role is $config[$i]{$role}\n";
	}
    }
}

# Dependencies of KEY as a simple string.
my %dependency;
# Set to 1 if KEY is not to keep as a dependency.
my %ignore;

# &DEPEND ($TARGET, @DEPENDENCY)
# ------------------------------
sub depend ($@)
{
  my ($target, @dependency) = @_;
  @dependency = (grep { !exists $ignore{$_} } @dependency);
  push @{$dependency{$target}}, @dependency
    if @dependency;
}

# List of files already opened.  $FILE{FILE} is where FILE was
# first defined.
my %file;

# &register ($FILE)
# -----------------
# Make sure the $FILE name is used only once (as input or output), to
# avoid clashes.
sub register ($)
{
  my ($file) = @_;
  die "$me: $.: $file already used in $file{$file}"
    if $file{$file};
  $file{$file} = "$ARGV:$.";
}


# example_to_verbatim(CODE)
# -------------------------
# Strip Texinfo markup.
sub example_to_verbatim ($ $)
{
  my ($contents, $style) = @_;

  $contents =~ s/^\@(c |comment|dots|end (ignore|group)|ignore|group).*$//gm;
  # Remove the Texinfo tags.
  $contents =~ s/^\@(error|result)\{\}//gsm;
  $contents =~ s/\@([{@}])/$1/gsm;
  $contents =~ s/^\@comment.*//gm;
  # remove the comment style sequence
  $contents =~ s/^$style//gm;

  return $contents;
}


# &REPORT (@MESSAGE)
# ------------------
# Report some @MESSAGE to the user.
sub message (@)
{
  print STDERR "$me: $ARGV:$.: ", @_, "\n";
}


# &STORE ($FILE, $CONTENTS)
# -------------------------
# Save the $CONTENTS in the $FILE, unless it already contains $WHAT.
sub store ($$)
{
  my ($file, $contents) = @_;
  if (-f $file)
    {
      local $/;			# Turn on slurp-mode.
      my $f = new IO::File "< $file"
	or die "$me: cannot open $file for reading: $!\n";
      my $old_contents = $f->getline;
      if ($old_contents eq $contents)
	{
	  message "$file: unchanged";
	  return ;
	}
    }
  my $out = new IO::File (">$file")
    or die "$me: cannot create $file: $!\n";
  print $out $contents;
  message "$file: new";
}


# The directory where to store the extracted results.
# Parse our options, trying to retain backwards compatibility with pod2man but
# allowing short forms as well.  --lax is currently ignored.
my %option;
Getopt::Long::config ('bundling_override');
GetOptions (\%option,
	    'configfile|c=s',
	    'srcdir|s=s',
	    'output-dir|O=s',
	    'help|h') or exit 1;
pod2usage (0) if $option{help};

my $odir = $option{"output-dir"} || "extract";
if (! -d $odir)
{
  mkdir $odir or die "$me: cannot create $odir: $!\n";
}
my $makefile = new IO::File "> $odir/makefile"
  or die "$me: cannot create $odir/makefile: $!\n";
print $makefile "include $option{srcdir}/../exdoc.mk\n";
print $makefile ".PHONY: all dvi eps html pdf txt\n";
# print $makefile "all: txt\n";
my $example;
my $last_file;

parse_config ($option{"configfile"});

while (<>)
{
  ## @dotcaption{ord, file}
  ## ======================
  if (/^\@dotcaption{([-.\w]+),\s*([^\}]+)}/)
    {
      my $ord = $1;
      my $file = $2;
      register $ord;
      register $file;
      store "$odir/$ord.file", "$file.dot";
      depend "html", "$ord.jpg";
      depend "pdf", "$ord.pdf";
      depend "txt", "$ord.txt";
      depend "dvi", "$ord.eps";
    }

  ## @example/@end example
  ## =====================
  ## Memoize the contents.
  else
    {
      my $i = 0;

      for ($i = 0; ($i < @config)
	   && not ((/^\s*$config[$i]{"comment_style"}\s*$config[$i]{"tag_open"}\s*$/ .. /^\s*$config[$i]{"comment_style"}\s*$config[$i]{"tag_close"}\s*$/)); $i++)
	{
	}

      if ($i < @config)
	{
	  if (/^\s*$config[$i]{"comment_style"}\s*$config[$i]{"tag_open"}\s*$/)
	    {
	      # Nothing.
	      undef $example;
	    }
	  elsif (/^\s*$config[$i]{"comment_style"}\s*$config[$i]{"tag_close"}\s*$/)
	    {
	      if (defined $last_file)
		{
		  # Output as a verbatim file.
		  store "$odir/$last_file", example_to_verbatim ($example, $config[$i]{"comment_style"});
		  undef $last_file;
		}
	    }
	  ## @filecaption{ord, filename}
	  ## Outputting the previous @example as a file.
	  elsif (/^\s*$config[$i]{"comment_style"}\s*$config[$i]{"command_prefix"}filecaption{([-.\w]+),\s*([^\}]+)}/)
	    {
	      my ($ord, $file) = ($1, "$2");
	      register $file;
	      $last_file = $file;
	    }
	  else
	    {
	      $example = $example . $_;
	    }
	}


      ## @c extdoc-ignore: file
      ##       ## A file that we should not depend upon (e.g., it does not exist).
      elsif (/^\@c extdoc-ignore: (.*)$/)
	{
	  message "ignoring $1";
	  $ignore{$1} = 1;
	}


      ## @havmcaption{ord, args}
      ## @mipsycaption{ord, args}
      ## @tccaption{ord, args}
      ## @sedcaption{ord, args}
      ##       ## Running `havm', `mipsy', `tc', or `sed' on the ARGS.
      else
	{
	  my $i = 0;
	  for ($i = 0; ($i < @config) &&
	       not (/^\s*$config[$i]{"comment_style"}\s*$config[$i]{"command_prefix"}($config[$i]{"captions"})caption{([-.\w]+),\s*([^\}]+)}/); $i++)
	    {}
	  if ($i < @config && /^\s*$config[$i]{"comment_style"}\s*$config[$i]{"command_prefix"}($config[$i]{"captions"})caption{([-.\w]+),\s*([^\}]+)}/)
	    {
	      my $prog;


	      if (defined $config[$i]{$1})
		{
		  $prog = $config[$i]{$1};

		  print $prog;
		}
	      else
		{
		  $prog = $1;
		}
	      my $ord = $2;
	      my $args = $3;
	      my $out = "$ord.$config[$i]{out}";
	      register $out;
	      store "$odir/$ord.cmd", "$prog $args";
	      depend "pdf", $out;
	      depend "html", $out;
	      depend "txt", $out;
	      depend "dvi", $out;

	      # Dependencies: args except options and output redirections.
	      depend $out, grep { !/^[->]/ } split (' ', $args);
	    }
	}
    }
}


for my $target (sort keys %dependency)
{
  ## print $makefile ".PHONY: $target\n";
  # Using "map" only to get some functional composition of s///,
  # but there is a single string here.
  print $makefile (map ({ s/$(?!\z)/ \\/gm; $_ }
		   wrap ("$target: ",
			 " " x length ("$target: "),
			 @{$dependency{$target}})),
  "\n");
}

### Setup "GNU" style for perl-mode and cperl-mode.
## Local Variables:
## perl-indent-level: 2
## perl-continued-statement-offset: 2
## perl-continued-brace-offset: 0
## perl-brace-offset: 0
## perl-brace-imaginary-offset: 0
## perl-label-offset: -2
## cperl-indent-level: 2
## cperl-brace-offset: 0
## cperl-continued-brace-offset: 0
## cperl-label-offset: -2
## cperl-extra-newline-before-brace: t
## cperl-merge-trailing-else: nil
## cperl-continued-statement-offset: 2
## End:
